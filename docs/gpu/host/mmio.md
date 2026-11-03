# MMIO and Command Words

The device exposes a command block: 32-bit registers at fixed byte offsets that
the host uses to load firmware, bind a ring, arm the interrupt ring and reset a
pipe. A client never writes it. This chapter fixes the offsets, the
immediate-command protocol, the packet words and the order in which a ring is
provisioned, because the state of those registers decides what a submission can
rely on and when a ring may be used.

## Who may write the register file

On LG200 the block is BAR 0; on LG210 the same layout begins 1 MiB into BAR 2.
Every register is one 32-bit dword and every access is a single 32-bit load or
store, so a 64-bit value is a pair of registers with the low dword first.

| Path | Holder | Reach |
|------|--------|-------|
| Command block | kernel | every offset in the map below, read and write |
| Register-read query | any client | read-only, at most 128 consecutive dwords from one offset |
| Doorbell aperture | compute client, LG210 only | 64-bit slots in a 4 MiB window at BAR + 0x800000 |
| MCS ring and index block | compute client | ordinary mapped memory, not a register |

The register-read query is the only client access to the block. Its offset field
is named for dwords but is applied as a byte offset into the same aperture, the
engine selector must be the broadcast value 0xffff, and the flag word must be
zero. Only the state register at 0x44 is answered; every other offset comes back
as zero. A client MUST NOT use the query to sample a ring cursor, the status
word or an interrupt pointer: a zero there is the absence of an answer, not a
reading.

The doorbell aperture is a table of 64-bit slots, one per queue. The kernel
enables it by writing the table base into a control register in the unshifted
part of the same BAR, in 4 MiB units, and by issuing the enable operation of the
doorbell block. LG200 has no aperture, so a queue position there is published
only through the doorbell packet.

## Register map

Offsets are byte offsets from the start of the command block.

| Offset | Register | Access | Meaning |
|-------:|----------|--------|---------|
| 0x00 | Command | write | The immediate command word. |
| 0x04 | Status | read/write | Completion word; written with zero before each command. |
| 0x08 | Argument 0 | write | First input dword, or the low dword of a 64-bit input. |
| 0x0c | Argument 1 | write | Second input dword, or the high dword of a 64-bit input. |
| 0x10 | Return 0 | read | Result, low dword. |
| 0x14 | Return 1 | read | Result, high dword. |
| 0x18, 0x1c | GPIPE cursors | read/write | Write cursor, then read cursor. The read cursor is zeroed at bind and read only for diagnostics. |
| 0x20, 0x24 | XDMA cursors | read/write | The same pair as GPIPE. |
| 0x30, 0x34 | BPIPE cursors | read/write | Write cursor, then read cursor; here the read cursor is the consumption record. |
| 0x38, 0x3c | CPIPE cursors | read/write | Write cursor, then read cursor, of the management ring. |
| 0x40 | Firmware version | read | Major [7:0], minor [15:8], revision [23:16]. |
| 0x44 | State | read | Sampled by the power path as a busy/load indicator. |
| 0x48, 0x4c | DPIPE cursors | read/write | The same pair; LG210 only. |
| 0x50, 0x54 | EPIPE cursors | read/write | The same pair; LG210 only. |
| 0x80 | Execution control | read/write | Bit 0 running; clearing it halts the command processor. |
| 0x84 | Command trigger | write | Written with 1 once the arguments are in place; this starts the command. |
| 0x88 | Host interrupt | read/write | Interrupt-ring pointers and control. |
| 0x8c | Hardware information | read | Family in [11:8], hardware version in [11:0]. |
| 0x90, 0x94 | Time counter | read | Low and high dword of the device counter. |
| 0x9c | Frequency scale | read | Current power level as one of eight odd codes. |
| 0xe8 | Power-management state | read | Bit 31 set when the firmware exposes power levels. |
| 0xf0 | Firmware write port | write | One dword per store during upload. |
| 0xf4 | Firmware write pointer | read | Bytes accepted; compared with the image size. |
| 0xf8 | Firmware checksum | - | Defined; no reader. |

Every register in this map belongs to the kernel; a client's queue state lives
in memory, and the only BAR address a client writes is a doorbell slot, on
LG210 only.
The same offsets mean something else on LG100, where 0x18 is a ring base
register rather than a cursor, so a driver MUST NOT carry an offset from one
generation to another.

A ring cursor is a dword index, not a byte offset, and the host masks it with
the ring's dword count minus one. The read cursor is not one object: BPIPE and
CPIPE read it from 0x34 and 0x3c, while GPIPE and XDMA read a host memory slot
and touch 0x1c and 0x24 only to zero them at bind or to dump them when a
submission is stuck. For those two pipes the register is not the consumption
record.

## Immediate command word

A command is one 32-bit word:

```text
   31              16 15               8 7                0
  +------------------+------------------+------------------+
  |  configuration   |    operation     |      block       |
  +------------------+------------------+------------------+

  word = (block & 0xff) | ((operation & 0xff) << 8)
                         | ((configuration & 0xffff) << 16)
```

The block selects an operation group, the operation selects one action within
it, and the configuration is a 16-bit qualifier that the operation either uses
or ignores.

A command is issued in a fixed sequence:

1. wait until the status word reads done;
2. write zero to 0x04;
3. write the command word to 0x00;
4. write argument 0 to 0x08 and argument 1 to 0x0c;
5. write 1 to 0x84, which starts the command;
6. wait until the status word reads done again;
7. read 0x10 and 0x14 to obtain the 64-bit result.

Step 5 is the trigger: writing the command word alone starts nothing, so a
driver that omits it waits for a completion that cannot arrive. Step 1 is also
the liveness check, because until the firmware has reached done no command can
be issued at all.

Only four status values are defined:

| Value | Name | Role |
|------:|------|------|
| 0x0000_0000 | idle | Written by the host before a command to clear the previous result; not a value to poll for. |
| 0xB007_B007 | boot | Firmware has finished booting. Defined; no host check reads it. |
| 0xD02E_D02E | done | The only value that completes a command. |
| 0xFFFF_0000 | running | High half set while a command executes; the low 16 bits are documented as a command count. Defined; no host check reads it. |

Completion is the exact word 0xD02E_D02E: a driver MUST compare the whole word
and MUST NOT infer completion from the running value changing, because that
value carries a count in its low half.

One command may be outstanding at a time. A second issuer waits for the command
port, and an issuer that cannot sleep, such as an interrupt handler, is refused
rather than allowed to spin. A command that never completes occupies the port
until the wait expires, and the result of an abandoned command is the all-ones
64-bit value, which is indistinguishable from a command that legitimately
returned all ones; no operation MAY define that value as meaningful.

## Blocks and operations

| Block | MOP | Operations (SOP) | Used for |
|-------|----:|------------------|----------|
| GPIPE | 0x01 | BGQ 1, GQSZ 2, UBGQ 3, RESET 4 | Bind, size, unbind and reset the graphics ring. |
| BPIPE | 0x02 | BBQ 1, BQSZ 2, UBBQ 3 | The same for the blit ring. |
| CPIPE | 0x03 | BCQ 1, CQSZ 2, UBCQ 3, BCDB 4, UBCDB 5 | Bind, size and unbind a compute queue, and set its doorbell offset. |
| XDMA | 0x04 | BDQ 1, DQSZ 2, UBDQ 3 | The same for the transfer ring. |
| SYNC | 0x05 | GSYNC 1, BSYNC 2, CSYNC 3 | Drain a pipe before its ring is re-armed. |
| MMU | 0x06 | MMUEN 1, UDIR 2, USAFE 3, UPG 4, UEXP 5, FTLB 6, UVMADDR 7, UVMSZ 8, RETRY 9 | Directory base and geometry per domain, safe address, fault reporting, TLB flush, fault retry. |
| ZIP | 0x07 | ZEN 1, ZDIS 2, UTAGADDR 3, UTAGMASK 4, CNT 6, CNTEN 7, CNTDIS 8 | Tag-table base and mask; enable and disable. |
| FREQ | 0x08 | UFRQ 0, POWER_LEVEL_UFRQ 1 | Request a power level. |
| EXC | 0x09 | BEQ 1, EQSZ 2, UBEQ 3, HBEQ 4 | Bind, size and unbind the host interrupt ring. |
| DOORBELL | 0x0a | DB_ZEN 1, DB_ZDIS 2 | Enable and disable the doorbell table. |
| CWSR | 0x0b | CWSR_ZEN 1, CWSR_ZDIS 2 | Enable and disable context save and restore. |
| DPIPE | 0x0c | BBQ 1, BQSZ 2, UBBQ 3 | Display ring, LG210 only. |
| EPIPE | 0x0d | BBQ 1, BQSZ 2, UBBQ 3 | Encode ring, LG210 only. |
| VRAM | 0x0e | BINDADDR 1, READ 2, WRITE 3 | Relocate a byte window and read or write one dword of video memory. |

The argument conventions these operations share: a 64-bit address is argument 0
as the low dword and argument 1 as the high dword, and a ring size is argument 0
in dwords, that is bytes divided by four, with argument 1 zero. GPIPE RESET
takes no argument. MMU UDIR takes a directory base, UPG the domain index in the
configuration and a geometry code in argument 0, MMUEN an enable bit plus a
fault-mode field, and the fault-reporting operation UEXP a mask where 1 reports
and 0 suppresses. EXC HBEQ takes the ring base in the two arguments and its
control word in the configuration. VRAM BINDADDR relocates to a byte address,
READ returns one dword for a byte count, and WRITE takes the count and the
value.

> UNRESOLVED: several operations that take one input pass 0xFFFF_FFFE in the
> second argument, and nothing defines a meaning for it. A driver cannot tell
> whether it is a required sentinel.

An operation that changes a pipe answers done whether or not the pipe accepts
the change, so a bind that answers proves the command was received, not that the
pipe fetches from the new ring.

## Stream packets

Ring words, indirect-buffer words and compute-queue words share one shape: bits
[7:0] are the opcode and bits [31:8] are the configuration. A packet's length is
implied by its opcode; only the indirect-buffer packet carries a dword count of
its own.

| Packet | Opcode | Dwords |
|--------|-------:|-------:|
| Stream No Operation | 0x00 | 1 |
| State Config | 0x01 | 1 + N |
| Stream Submit | 0x03 | 1 |
| Stream VMID | 0x0f | 1 |
| Stream Writeback 32 | 0x10 | 4 |
| Stream Writeback 64 | 0x11 | 5 |
| Stream Event | 0x12 | 3 |
| Stream Interrupt | 0x14 | 1 |
| Stream DBAR | 0x16 | 1 |
| Stream Indirect Buffer | 0x80 | 4 |
| Stream Write Register | 0x81 | 3 |
| Stream Poll | 0x83 | 6 |
| Stream VM Bind | 0x84 | 4 |
| Stream Map Process | 0xe0 | 10 |
| Stream Map Queue | 0xe2 | 15 |
| Stream Unmap Queue | 0xe3 | 3 |
| Stream Doorbell | 0xe4 | 3 |

The packets from 0x0f to 0x84 are what a pipe ring carries. The three below them
are command-stream words: the ring uses no-operation as its padding word, and
state config and submit appear inside indirect buffers. The four at 0xe0, 0xe2,
0xe3 and 0xe4 are the compute management ring's queue-management vocabulary,
produced only by the host, and are how a client request becomes firmware action.
Opcode 0x84 is defined and has no producer in this interface.

A compute stream is a third vocabulary and its numbers are not interchangeable
with a ring's. There, 0x16 closes the stream instead of ordering a ring, 0x89 to
0x8e name dispatch and configuration words that no ring packet defines, and 0x03
is not one of its words. A reader MUST resolve an opcode against the stream it
was found in. [Compute submission](compute-submission.md) owns that vocabulary,
and [indirect buffers](indirect-buffers.md) owns the record words that open and
close a graphics command stream.

Two rules bound a client stream. The host validates an indirect buffer's range
and residency but does not parse its contents, so nothing stands between a
malformed word and the pipe: a client MUST use only the packets its pipe
documents. And the queue-management packets MUST NOT appear in one, because they
reprogram queue state the firmware tracks, which is the host's authority.

## Ring provisioning

A pipe needs three things before it fetches: the ring's base address, its size
in dwords, and a zeroed cursor pair. On this generation the base and the size
are immediate-command arguments; there are no base or size registers at all,
which is the visible difference from the previous generation's map.

| Pipe | Bind | Size | Unbind |
|------|------|------|--------|
| GPIPE | BGQ | GQSZ | UBGQ |
| BPIPE | BBQ | BQSZ | UBBQ |
| XDMA | BDQ | DQSZ | UBDQ |
| CPIPE management | BCQ | CQSZ | UBCQ |
| CPIPE client queue | MAP_QUEUE | length field of the queue descriptor | UNMAP_QUEUE |

A kernel ring is provisioned in one order: fill the ring memory with
no-operation words, write the write cursor and the read cursor to zero, wait,
then bind and size. GPIPE unbinds first with UBGQ, because a command processor
that still considers the queue bound can ignore the re-bind; the unbind may go
unanswered and the bind decides whether the ring is usable. BPIPE and XDMA bind
without an unbind and check neither answer.

The host sizes the ring to a power-of-two dword count, because it masks the
cursor with the count minus one, and takes the base from a page-aligned
allocation. Both are host policies; a pipe is told only the resulting dword
count.

> UNRESOLVED: whether a pipe requires a power-of-two ring, or only the host's
> mask arithmetic does.

A client compute queue is provisioned differently. The host sends Stream Map
Queue with a descriptor that carries the ring base, the length in dwords, the
PASID, the queue identifier, and two addresses: the write index at the ring base
minus 64 bytes and the read index 8 bytes after it. The client writes the write
index into its own index block and then requests a doorbell; the firmware reads
the index. The ring base is the first packet, so the stream wraps across the
whole ring and never enters the index block.

## Interrupts

Interrupt delivery is not armed by a register write. The host interrupt ring is
bound by EXC HBEQ, with the ring base in the two arguments and a control word in
the configuration field:

| Field | Bits of the control word | Meaning |
|-------|--------------------------|---------|
| Enable | 0 | 1 arms the ring. |
| Address space | 8 | 0 for a GPU address, 1 for a bus address. |
| Domain | 9 | Domain the ring belongs to; zero here. |
| Size code | 7:4 | Ring depth, see below. |

| Size code | LG200 vectors | LG210 vectors |
|----------:|--------------:|--------------:|
| 0x8 | 256 | 256 |
| 0x9 | not emitted | 1024 |
| 0xa | 1024 | 2048 |
| 0xb | 2048 | not emitted |

One code changes meaning between the parts, so a driver MUST select the code
for the part it runs on. This generation uses 2048 vectors of 16 bytes, a 32 KiB
ring.

The ring's two pointers live in the host interrupt register at 0x88:

| Bits | Field | Meaning |
|-----:|-------|---------|
| [11:0] | Write pointer | Next vector the device will write, as a dword index; multiply by 4 for a byte offset. |
| [27:16] | Read pointer | Next vector the host will consume, as a dword index; multiply by 4 for a byte offset. |
| 29 | Full | The ring has overflowed. |
| 30 | Acknowledge | Defined; no host path uses it. |
| 31 | Clear | Written with 1 to reset both pointers; the host polls until the bit reads 0. |

A host MUST publish the read pointer back after consuming vectors, leaving the
other fields intact. A pointer that is never advanced fills the ring and sets
the full bit, and the host's only response to that is a log line.

Each vector is 16 bytes:

| Bits of the vector | Field |
|--------------------|-------|
| Word 0 [7:0] | Source identifier. |
| Word 0 [23:8] | PASID of the faulting or completing context. |
| Word 0 [31:28] | Domain the vector came from. |
| Word 1 | Undocumented; carried but not interpreted. |
| Word 2, word 3 | Source-specific payload, for example the fault address. |

| Source identifier | Meaning |
|------------------:|---------|
| 0x01 | GPIPE end of pipe. |
| 0x02 | BPIPE end of pipe. |
| 0x03 | DPIPE end of pipe, LG210 only. |
| 0x04 | EPIPE end of pipe, LG210 only. |
| 0x20 | MMU page fault. |
| 0x21 | MMU port fault. |
| 0x22 | CPIPE queue interrupt. |
| 0x23 | XDMA trap. |
| 0x24 | Shader interrupt message. |

This interface has no per-pipe interrupt mask: its enable and disable path for
an end-of-pipe source is empty and it never writes a register for one. A pipe
raises the interrupt when it executes the interrupt packet, so the only way not
to receive one is not to emit that packet.

> UNRESOLVED: the pointer fields are read as 12-bit dword indices, but the ring
> is 2048 vectors, which is 8192 dwords. Whether the register fields are wider
> than the mask the host applies, or the ring is smaller in practice, is not
> established.

## Time

The counter at 0x90 and 0x94 is read as two dwords, low first, with no latch
between them. The timestamp query returns the pair on LG200 and returns zero on
LG210, because only the LG200 path reads the pair, so a client that calibrates
against that query gets a clock on one part and nothing on the other.

> UNRESOLVED: the unit of the counter, its wrap behaviour, and whether the two
> halves can tear across a carry. Nothing calibrates it against a host clock, and
> a driver MUST NOT present it as wall-clock time.

## Reset

Two mechanisms carry the name. The GPIPE block has a RESET operation that resets
the graphics pipe; no other pipe has a reset operation of its own, and a full
device reset is a suspend, reset and resume of every block, driven by the host.

The RESET command does not leave the command processor usable. Afterwards the
microcontroller comes back empty: it fetches nothing from the graphics ring,
while the bind, size and unbind commands still answer, so "the commands answer"
is not evidence that the pipe fetches. Restoration is ordered: push the firmware
through 0xf0 and verify 0xf4 against the image size, set bit 0 of 0x80 to run
the command processor, and only then bind the ring again.

Clearing bit 0 of 0x80 stops the fetch of every ring already bound, not just the
graphics ring. A host that halts the command processor MUST re-bind every ring
afterwards, and a reset path with no resume phase MUST NOT halt it.

| Reset path | What it is defined to have stopped |
|------------|------------------------------------|
| Graphics pipe reset | GPIPE and CPIPE |
| Full device reset | Every engine; the display controller keeps scanning out |
| Per-block soft reset | Nothing the host can name |

A client never writes the command block. Its submissions are refused with a
retryable error while a reset is in progress.

> UNRESOLVED: which state a graphics-pipe reset preserves for the other pipes,
> and whether an in-flight client queue survives it. The host has no
> compute-only reset primitive, so a compute recovery is always a graphics or a
> full device reset.

## Open questions

- The meaning of the ZIP counter operations beyond their names.
- Whether the state register at 0x44 reports one pipe or the whole device.
- The encoding of the geometry code that MMU UPG takes.
