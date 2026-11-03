# GPIPE

GPIPE is the command processor of the graphics pipe. It consumes the kernel-owned
graphics ring, executes the indirect buffers the ring launches, and applies the state
records they carry. A buffer holds no shader instructions: a record names each stage's
entry address and register footprint ([Shader ABI](../shaders/shader-abi.md)).

## Ring and ownership

The write cursor is a dword index in register `0x18`; the read cursor is a writeback
word the pipe writes back, so it lags ([Rings](rings.md)). Completion interrupts carry
source id `0x01`. Eight VMIDs exist: `0` kernel, `1`–`3` client graphics, `4`–`7`
compute. The ring is bound by immediate commands rather than base-address registers
([MMIO](mmio.md)). A frame is published by writing the new dword cursor to `0x18`. A
client never performs that write: it submits buffers and receives a sequence number.

## Command vocabulary

Every word splits into an 8-bit opcode in bits `[7:0]` and a 24-bit config in bits
`[31:8]`; the config means whatever the opcode says. The last column says where the form
appears.

| Opcode | Name | Dwords | Payload | Appears |
|-------:|------|-------:|---------|---------|
| `0x00` | no operation | 1 | none | both, as padding |
| `0x01` | state window | 1 + n | n state dwords | buffer, as a record's body |
| `0x03` | submit | 1 | none | buffer, closing a record |
| `0x0f` | VM select | 1 | VMID in the config | both |
| `0x10` | writeback 32 | 4 | address 64, data 32 | both |
| `0x11` | writeback 64 | 5 | address 64, data 64 | declared; no fence uses it |
| `0x12` | event | 3 | selector, address 64 | ring |
| `0x14` | interrupt | 1 | none | ring |
| `0x16` | barrier | 1 | none | ring |
| `0x80` | indirect buffer | 4 | address 64, count and VMID | both |
| `0x81` | register write | 3 | register offset, value | ring |
| `0x83` | poll | 6 | see [Poll](#poll) | ring |
| `0x84` | VM bind | 4 | pipe type, VMID, PASID | ring |

## Indirect buffer

A launch is five dwords: a VM select word and the 4-dword buffer packet behind it.

| Dword | Content |
|------:|---------|
| 0 | `(vmid << 8) OR 0x0f` — VM select |
| 1 | `0x80` — buffer opcode, config zero |
| 2–3 | buffer address, bits `[31:0]` then `[63:32]` |
| 4 | `(vmid << 24) OR count` — count `[23:0]`, VMID `[31:24]` |

The VMID appears twice in a launch; a driver MUST make the two agree, and which field the
processor obeys when they differ is UNRESOLVED. The select is per launch, not sticky: every
frame ends with a VMID-0 fence, so no earlier selection survives it.

A buffer is admitted, not trusted: a nonzero count, a nonzero 4-byte-aligned start, at most
`0x00ff_ffff` dwords, one present mapping over the whole range, the backing object in the
residency list, a page directory that translates the start. A page-table entry carries a
present and a writeable bit and no execute bit, so presence is the whole permission. Of a
chunk's flag bits, only the copy-engine and preamble ones are accepted. All buffers of one
submission MUST name the same ring, and their number is bounded by ring space:
`LOONGGPU_INFO_MAX_IBS` reports `floor((frame budget - per-frame reserve) / 5)` per
engine — 39 for GPIPE with the shipped 256-dword budget, a host policy.

## What a buffer may carry

The record that opens a submission MUST be the full form, because the processor is not
known to hold the state block after a flush; later records MAY state only what changed. The
grammar and the window descriptor are in [Indirect buffers](indirect-buffers.md). The
border-table preamble `0x0000_00a1` MUST lead the buffer — anywhere else it makes the
processor re-parse the stream as a new buffer and walk past the end — and the end command
`0x0000_0f05` MUST close the buffer, with the declared count reaching it. A buffer MAY
carry a 32-bit writeback of its own — one per record stores a progress ordinal — and MAY
launch a further buffer with a VM select and a buffer packet; whether a client buffer may do
so is UNRESOLVED. The remaining forms — register write,
poll, interrupt, barrier, event, VM bind — are used on the ring only. Whether the processor
would accept one inside a buffer is UNRESOLVED, so a stream that places one there is
unverified rather than merely unusual, and a driver MUST NOT. Query, timestamp and
streamout packets belong to a buffer instead; their forms are in
[Indirect buffers](indirect-buffers.md).

## Fence

| Dword | Content |
|------:|---------|
| 0 | `0x0f` with VMID `0` |
| 1–4 | `0x10`, address `[31:0]`, address `[63:32]`, sequence |
| 5 | `0x16` — barrier |
| 6 | `0x14` — interrupt, when one is requested |

Every fence selects VMID `0`, because both fence addresses are kernel-aperture addresses.
The barrier orders the processor's own writes, so the store lands before the interrupt
behind it; it drains nothing. The interrupt names no fence: it asks the host to read the
word. One fence follows the last buffer and covers every buffer; a submission that names a
fence buffer gets a second writeback in front of the kernel's. A fence proves the processor
consumed the stream to that point, not that the graphics pipe finished the draws. A client
that needs a submission's results waits on that submission's fence — `WAIT_CS` or
`WAIT_FENCES` — or asks the retirement query; a buffer's idle status is a read-level wait
and does not stand in for it ([Buffer objects](buffer-objects.md)).

## VM switch

| Register | Bits | Content |
|----------|------|---------|
| `0x10020 + 8 * vmid` | `[31:0]` | page-directory base, low |
| `0x10024 + 8 * vmid` | `[31:0]` | page-directory base, high |
| `0x10018` | `[0]`, `[1]`, `[15:8]` | flush enable, flush every VMID, VMID |

Three register writes, each a 3-dword register-write packet, in this order: base low, base
high, invalidate, and only then the VM select and the buffer that need them — the buffer is
fetched through the directory the same frame installs. A base write with no invalidate
behind it is never correct; the reverse is routine, because the invalidate belongs to the
submission and the base write to the change. The base write is not packet-ordered against
work already in the pipeline, so a handover also waits for the previous owner's work to
retire and for the processor to report idle, bounded by a host timeout of 20 ms.

## Poll

| Dword | Bits | Content |
|------:|------|---------|
| 0 | `[7:0]`, `[10:8]`, `[12]` | `0x83`, condition, memory select |
| 1–2 | `[63:0]` | address |
| 3 | `[31:0]` | reference |
| 4 | `[31:0]` | mask |
| 5 | `[15:0]`, `[31:16]` | interval, retry count |

The condition is `0` true, `1` less, `2` less or equal, `3` equal, `4` not equal,
`5` greater or equal, `6` greater; memory select `1` polls memory, `0` a register.
A pipeline sync would be a poll of the ring's own fence word for equality with the last
sequence, under an all-ones mask, and the submission path does not emit it: that word
reports processor progress and not pipe completion, so the wait would answer the wrong
question.

> UNRESOLVED: the unit of the interval and retry fields, what the pipe does when the retry
> count expires, and whether the mask applies to the reference too. Only equality under an
> all-ones mask is exercised, so a driver SHOULD use no other combination.

## Ordering and errors

Inside one frame the order is fixed: address-space switch, buffers in submission order, the
client fence writeback when one was asked for, the kernel fence. The processor consumes the
ring in order, so buffers run back to back; submissions on one ring are serialised, and
those on different rings are ordered only by their dependencies.

A submission can fail before anything reaches the ring: a zero or over-long count, a
misaligned start, a range no present mapping covers, a buffer outside the residency list, a
directory that cannot translate the start, an unknown flag bit, or a second ring. It is
refused as a unit.

- **A fault** stops the pipe, so the stream behind it never runs and the hardware never
  signals the fence. The driver separates a range never mapped from one cleared under an
  access already in flight, drops a fault it can prove stale, and otherwise quarantines the
  pipe and resets the engine.
- **A timeout** ends the ring's outstanding fences: the timed-out submission with an error,
  the work behind it as cancelled, the fence word left untouched so a later real completion
  cannot be read as progress that happened. The reset follows. A client learns the outcome
  from the failed fence and the reset counter the context query reports.

> UNRESOLVED: how much of a buffer a timed-out submission executed. Ring order fixes the
> sequence, not the stopping point, and no per-buffer record exists; the only evidence is a
> marker a client writes into its own stream.
>
> UNRESOLVED: whether the processor reports a malformed packet inside a buffer differently
> from a stuck pipe. From the interface the two are indistinguishable, so a bad buffer costs
> the same recovery as a hang.
