# Compute Submission

A compute dispatch on LG200 is a micro-code stream in a ring the client owns. The
CPIPE runs that ring as a user queue: the client builds the stream, publishes a
byte write pointer, and rings a doorbell. This chapter defines the queue geometry,
the stream a dispatch is built from, the kernel descriptor and kernarg buffer that
stream names, the workgroup and LDS encoding, the doorbell, and the completion
protocol. The CPIPE's register interface is in [CPIPE](cpipe.md); the shader's
view of the launch is in [Compute](../shaders/compute.md).

## The dispatch record never reaches the ring

The 64-byte AQL kernel-dispatch record is the vocabulary of a launch, not a packet
on the queue. A well-formed AQL record placed at the write pointer is ignored: the
CPIPE consumes MCS (micro-code stream) commands, and a dispatch is a sequence of
them. Every field of the AQL record survives as an argument to one of those
commands.

| AQL field | Where it lands |
|-----------|----------------|
| Workgroup size X/Y/Z | Aperture command, first payload dword (packed); descriptor `+0x04` |
| Grid size X/Y/Z | Group command payload, divided by the workgroup size; descriptor `+0x0c` |
| Private segment size | Private-size configuration command; descriptor `+0x18` |
| Group segment size | Descriptor `+0x1c`, then folded into the resource word at `+0x30` |
| Kernel object | Kernel-object configuration command; the address of the descriptor |
| Kernarg address | Kernarg configuration command; descriptor `+0x28` |
| Completion signal | Submit command payload |

The grid size here is the **global** size in work-items, not a workgroup count. A
caller that holds a workgroup count multiplies by the workgroup size before
building the stream. Passing the count through divides by the local size a second
time and dispatches nothing whenever the count is smaller than the local size.

## User queue

A queue is a 64-byte index block followed by the ring.

| Property | Requirement |
|----------|-------------|
| Ring size | Power of two, 4096 B to 1 MiB |
| Ring address | 64-byte aligned GPU virtual address, at least 64 bytes above the bottom of its mapping; the index block and the whole ring inside one client mapping |
| Index block | 64 bytes immediately before the ring; MUST stay mapped |
| Write index | 64-bit byte position at `ring - 64`, written by the client |
| Read index | 64-bit byte position at `ring - 56`, written by the device |
| Length | Ring size in **dwords**: the ring size divided by four |
| Queues per PASID | One |

The queue's management block holds the **addresses** of the two counters, not
offsets inside the ring, so the ring runs from byte 0 and a stream may wrap across
the whole ring without crossing the index block. Both counters are byte positions:
the client publishes the end of what it has written, the device publishes what it
has consumed. The length field is not a byte count; it is the ring size in dwords.

Both counters start at zero. A client MUST zero the index block and the ring
before creating the queue: the allocation is system memory and is not guaranteed
zeroed, and an arbitrary read index can make the firmware believe the queue is
already drained. A zeroed ring byte is a NOP, so an unused tail reads as padding.

Creation maps the queue into the firmware's tables. The firmware needs the process
page directory before it can translate the queue's virtual addresses, so a
page-directory record is sent for the PASID, then the queue record carrying the
management block. A queue is identified to the firmware by the pair (pasid,
queue_id); the queue id is a per-PASID slot. Teardown is filtered by PASID, not by
queue id, so a client MUST NOT create a second queue on the same PASID: releasing
either one releases both.

## MCS command stream

One command is a little-endian dword with the opcode in bits `7:0`, followed by an
opcode-specific payload. A stream is self-delimiting: the firmware walks it command
by command.

| Opcode | Name | Header beyond the opcode | Payload |
|-------:|------|--------------------------|---------|
| `0x00` | NOP | none | none |
| `0x10`, `0x11` | Write-back 32, write-back 64 | none | address, then the data |
| `0x16` | Write-back and barrier (WBDBAR) | none | none |
| `0x83` | Poll | condition `10:8`, memory flag `12` | address, reference, mask, interval, retries |
| `0x87` | Group | X/Y/Z enable `10:8` | three 32-bit workgroup counts |
| `0x89` | State configuration (STSCFG) | offset in dwords `19:8`, payload dwords `31:20` | `num` dwords |
| `0x8a` | Submit (SBMT) | skip `8` | 64-bit signal address |
| `0x8b` | Cache invalidate (CINV) | cache tag `11:8`, end-of-pipe `16` | none |
| `0x8d` | Wait | semaphore id `19:16`, zero for a dispatch | none |
| `0x8e` | Dispatch | none | 64-bit operand; not used on LG200 |

Signal (`0x86`), barrier value (`0x88`) and sync (`0x8c`) are defined opcodes whose
payloads no dispatch stream uses; their encoding is not established. These numbers
belong to the compute stream alone: a pipe ring and an indirect buffer assign
different meanings to several of them, as [MMIO and command words](mmio.md) records.

## The stream a dispatch is built from

The command order is fixed. A stream carrying these commands in another order is
not a dispatch. STSCFG offsets are dword offsets into the configuration block.

| # | Command | Encoded as | Payload |
|--:|---------|-----------|---------|
| 1 | Apertures | STSCFG, offset `1`, 3 dwords | Packed workgroup size; group aperture `0x0001_0000`; private aperture `0x0002_0000` |
| 2 | Implicit block | STSCFG, offset `8`, 4 dwords | Entry low, entry high, resource word 0, resource word 1 |
| 3 | Configuration | STSCFG, offsets from `16` | Flag-driven, see below |
| 4 | Head invalidate | CINV tag `0xf`, end-of-pipe 0 | Optional; makes the caches clean before the waves fetch |
| 5 | Group | GROUP, enables from the flag word `11:9` | Grid size divided by workgroup size, per dimension |
| 6 | Wait | WAIT | None |
| 7 | Invalidate | CINV tag `0xf`, end-of-pipe 0 | None |
| 8 | Submit | SBMT, skip 0 | 64-bit completion-signal address |
| 9 | Barrier, closes the stream | WBDBAR | None |

The stream ends with NOP padding to a multiple of 64 bytes, because the published
write pointer must be 64-byte aligned. A whole stream MUST fit in 256 bytes.

The configuration block is gated by the descriptor's flag word:

| Flag bit | Dwords | Block content |
|---------:|-------:|---------------|
| 2 | 4 | Scratch resource descriptor |
| 3 | 2 | Kernel-object address |
| 4 | 2 | Queue pointer (group aperture) |
| 5 | 2 | Kernarg base address |
| 6 | 2 | Dispatch id, zero |
| 7 | 2 | Scratch backing address |
| 8 | 1 | Private segment size |

The offset advances only for commands that are emitted, so the offset of a later
command depends on which earlier ones fired. A compute dispatch's flag word is
`0x0fac`: bits 2, 3, 5, 7 and 8 select the configuration commands, and bits 11:9,
also set, enable the three group-id SGPRs and supply the group command's three count
fields. With that word the sequence is the scratch resource descriptor at `16`, the
kernel object at `20`, the kernarg address at `22`, the scratch backing at `24` and
the private size at `26`. A flag word carrying any other bit MUST be refused: an
unknown bit silently changes which commands a stream contains.

An LG200 stream emits no dispatch command. Opcode `0x8e` and its 64-bit operand
are not part of an LG200 dispatch and MUST NOT appear in one.

## Kernel descriptor

The kernel object a dispatch names is a 96-byte record; the shader entry is a field
inside it, not the object itself. Most of the record is rewritten on every
dispatch, because the firmware reads these words when it launches the waves and a
record still holding its creation-time zeros describes a different launch.

| Offset | Size | Field | Written per dispatch |
|-------:|-----:|-------|----------------------|
| `0x00` | 4 | Flags | No: `0x0fac` |
| `0x04` | 2 each | Workgroup size X/Y/Z, at `0x04`, `0x06`, `0x08` | Yes |
| `0x0a` | 2 | Reserved | Zero |
| `0x0c` | 4 each | Grid size X/Y/Z, at `0x0c`, `0x10`, `0x14`, in global work-items | Yes |
| `0x18` | 4 | Private segment size, bytes | Yes; zero without scratch |
| `0x1c` | 4 | Shared (LDS) size, bytes | Yes |
| `0x20` | 8 | Entry address | Yes |
| `0x28` | 8 | Kernarg base address | Yes |
| `0x30` | 4 | Resource word 0: register counts and LDS granules | Yes |
| `0x34` | 4 | Resource word 1: SGPR counts and floating-point mode | Yes |

The fields end at `+0x38`. What the remaining bytes of the 96-byte record are read
as is not established. The two resource words are packed:

| Word | Bits | Field | Encoding |
|------|------|-------|----------|
| 0 | `5:0` | VGPR count | `ceil(vgprs / 4)` |
| 0 | `9:6` | SGPR count | `ceil(sgprs / 8)` |
| 0 | `20:10` | Shared size | `ceil(bytes / 128)`, a 128-byte granule |
| 1 | `4:1` | Kernel SGPR count | 11 |
| 1 | `7:5` | Group SGPR count | 3 |
| 1 | `21:14` | Floating-point mode | Four 2-bit fields, see below |
| 1 | `31:27` | Waves per workgroup | `ceil(invocations / 32)`, five bits |

Resource word 1 sets bit 13 as a constant of the launch word. The two register
counts are refused rather than wrapped when they do not fit: at most 252 VGPRs and
120 SGPRs, the largest values the fields can name.

The kernel SGPR count places the group-id window. The compiler reads
`group_id_x` from `s[kernel_sgpr_cnt]`, so the count a launch declares and the
register the shader reads are the same number; a descriptor that declares a
different count faults the GPU. The group ids occupy `s11` to `s13`. The kernarg
pair is separate and sits below that window: the hardware supplies the kernarg base
in `s[6:7]`, which the flag word enables with bit 5.

## Implicit arguments

The kernarg base is reached through `s[6:7]` and is also the root of the
descriptor tables, because the compiler reads both from that pair. The buffer
therefore starts with the set-pointer table:

| Offset | Size | Content |
|-------:|-----:|---------|
| `0x00` | `0x50` | Five 16-byte descriptor set pointers |
| `0x50` | 64 | Implicit-argument block |
| `0x90` | 256 | Push constants |
| `0x190` | 16 | Driver-hidden window: scratch segment base |
| `0x1a0` | 8 | Completion signal |
| `0x1c0` | rest of the page | Packed descriptor slots, 64 bytes each |

The block's distance from the kernarg base is a compile-time constant the shader is
built with; the block layout itself is fixed. Placing the block at offset 0 would
hand the shader two geometry words to dereference as a 64-bit address.

| Offset | Size | Field |
|-------:|-----:|-------|
| `0`, `4`, `8` | 4 each | Workgroup count X, Y, Z: the grid size in workgroups |
| `12`, `14`, `16` | 2 each | Workgroup size X, Y, Z |
| `18`, `20`, `22` | 2 each | Remainder X, Y, Z |
| `24` | 2 | Grid dimensions |
| `26` | 2 | Padding |
| `32`, `40`, `48` | 8 each | Global offset X, Y, Z |

The workgroup counts are the values `get_num_groups()` returns. The local
invocation id is in `v0/v1/v2`; the group id arrives in the SGPR window above. Only
the workgroup counts and the workgroup sizes are read back by the shader. The
global offsets carry the base group of a base-group dispatch, which is not folded
into the workgroup counts.

## Workgroup and LDS encoding

The workgroup size is written twice: in the descriptor at `+0x04` and packed into
the first payload dword of the aperture command, three 10-bit fields, one per
dimension, each holding the size minus one.

| Bits | Field |
|------|-------|
| `9:0` | Workgroup size X minus one |
| `19:10` | Workgroup size Y minus one |
| `29:20` | Workgroup size Z minus one |

The packed word is what derives the wave shape. A stream that omits it runs one
wave per workgroup with a single lane, whatever local size the shader declared.

The second and third payload dwords of the same command are the two segment
apertures, each a 4 GiB window named by bits 47:32 of its base:

| Segment | Base | Programmed word |
|---------|------|-----------------|
| LDS (group) | `0x0001_0000_0000_0000` | `0x0001_0000` |
| Scratch (private) | `0x0002_0000_0000_0000` | `0x0002_0000` |

A dispatch that declares shared memory MUST carry a non-zero group aperture.
Without one the firmware is asked to place a shared-memory launch in no region at
all, and the first shared access faults inside the aperture. A client buffer MUST
NOT be mapped inside either aperture. The launch limits are:

| Limit | Value | Kind |
|-------|------:|------|
| Invocations per workgroup | 256 | Policy |
| Workgroup size Z | 64 | Policy |
| Shared memory per workgroup | 16384 B | Policy, inside the descriptor's reach |
| Workgroup count per dimension | 65535 | Launch geometry |

The invocation limit is a policy value; the workgroup-size fields can name 1024 per
dimension. The product of the three sizes is what a launch is really bounded by,
and it is compared in 64 bits so a three-dimensional size cannot wrap. A count
above 65535 in any dimension wraps the launch geometry and can turn a valid
dispatch into a fault.

The shared size is a byte count in the descriptor and a granule count in the
resource word. The granule is 128 bytes, so the largest representable window is
262016 bytes; a larger declaration MUST be refused rather than folded down, because
folding it produces a window smaller than the shader addresses.

## Local-size and dispatch records

Two fixed records in the command vocabulary describe a launch as a local size and a
grid extent. Neither appears in a dispatch stream on LG200, and whether the CPIPE
consumes them is unresolved; they are documented because their layout is fixed.

A **Compute Local Size** record is 8 bytes:

| Bits | Field | Meaning |
|------|-------|---------|
| [31:0] | Command | `0x0010_0189` |
| [41:32] | Local Size X | Workgroup extent, 10 bits |
| [51:42] | Local Size Y | Workgroup extent, 10 bits |
| [61:52] | Local Size Z | Workgroup extent, 10 bits |

A **Compute Dispatch** record is 16 bytes:

| Bits | Field | Meaning |
|------|-------|---------|
| [31:0] | Command | `0x0000_0787` |
| [63:32] | Dimension 0 | Grid extent |
| [95:64] | Dimension 1 | Grid extent |
| [127:96] | Dimension 2 | Grid extent |

The order of the three dimension fields is not implied by the command word; each is
programmed independently.

## Doorbell

The doorbell names the queue; it carries no position. Publishing the write pointer
is the whole submit step, because the stream is already in client memory and the
firmware reads it after the doorbell.

1. Write the stream into the ring at the current slot, padded to 64 bytes.
2. Write the new write index into the index block, as a byte position.
3. Issue the doorbell for (pasid, queue_id).
4. Wait for the completion signal.

The doorbell ioctl also takes a write pointer, which it validates: 64-byte aligned
and strictly below the ring size. That argument is a sanity check only; the value
the device acts on is the counter in the index block. A client MUST write both and
keep them equal. The ioctl may refuse a doorbell it did not publish, with a
retryable error; a client SHOULD bound its retries rather than spin.

A published position of exactly the ring size is rejected, and a stream is never
split across the end of the ring. A client that cannot fit its worst-case stream
before the end of the ring starts the next stream at the beginning instead;
skipping the tail is legal because the stream is self-delimiting and the device's
read index is a byte counter.

## Completion

The submit command carries the address of a 64-bit completion signal. When the
stream retires, the device **decrements** that word by one. It is a countdown, not
a flag: the client stores the number of streams it has outstanding before the
doorbell and waits for the word to reach zero. A comparison below the stored count is wrong, because an
unsigned decrement past zero wraps to a large value and the wait would succeed the
instant the stream retired. The signal lives in host-readable memory, and the read
MUST be a volatile load so it cannot be hoisted above the device's write.

Completion is not a fence. The doorbell returns once the doorbell packet has been
queued to the firmware; it does not wait for the stream, and its reply reports no
retirement. A client that needs a dispatch's results MUST wait for the signal
before reading them, and MUST NOT treat a successful doorbell as retirement.

Three properties bound what completion tells a client:

- A signal retires only for a stream that contains a group command. A stream built
  only from a wait, an invalidate, a submit and a barrier never retires its signal,
  and the queue consumes no stream that has no dispatch in it.
- The submit command precedes the stream's write-back barrier, so the signal may
  retire before every write of the dispatch is visible. Whether the signal covers
  those writes is [unresolved](#open-questions).
- The read index is the other progress indicator: the device advances it as it
  consumes a stream. An unchanged read index means the stream was never seen.

A client that serialises its streams stores 1 before each doorbell. Carrying a
running total instead is wrong: the second stream would start above zero, retire
once, and never reach zero again. A ring slot MUST NOT be reused before the signal
of the stream in it has retired.

Releasing a queue is the one operation in this path backed by a fence. Teardown
sends an unmap filtered by PASID and waits for a status fence before the ring
memory may be released, so a client MUST NOT free or unmap the ring or its index
block before teardown returns.

## Floating-point mode

Resource word 1 carries a per-dispatch floating-point mode in four 2-bit fields. It
is not inherited from the descriptor's other bits: the launch word supplies all
four fields, so a mode left out is a mode chosen.

| Bits | Field |
|------|-------|
| `15:14` | Rounding for 32-bit operations |
| `17:16` | Rounding for 16- and 64-bit operations |
| `19:18` | Denormal handling for 32-bit operations |
| `21:20` | Denormal handling for 16- and 64-bit operations |

| Denormal value | Meaning |
|---------------:|---------|
| 0 | Flush source and result |
| 1 | Flush result only |
| 2 | Flush source only |
| 3 | Do not flush |

Zero is a real mode, not "unset". A dispatch whose shader declares no float
controls gets mode 3 in both denormal fields, which is do-not-flush, and
round-to-nearest-even in both rounding fields. A shader that asks for flush-to-zero
on input and output gets mode 0 in both. The graphics pipe has no such field and
flushes denormals natively; the compute pipe reaches the mode only through this
word, which is why the mode is per dispatch rather than per pipeline.

## Open questions

- Whether a retired completion signal covers the writes of the dispatch, or whether
  the trailing write-back barrier must also complete. The submit command is emitted
  before the barrier, so the two are distinct events.
- What the bytes `+0x38` to `+0x5f` of the 96-byte kernel descriptor are read as.
- The per-workgroup LDS capacity beyond the advertised window, and the maximum
  number of waves one workgroup may occupy.
- Whether a user queue may have more than one stream outstanding. The streams share
  one staging buffer and one signal, so a client serialises them; the firmware's
  behaviour with two published and unconsumed streams is not established.
- Whether the 65535 workgroup limit per dimension is a field width or a smaller
  launch-geometry bound.
