# SIO

SIO is the channel-addressed interface between a shader and the fixed-function
stages around it.  It carries vertex output, geometry input, geometry output and
fragment colour output without going through memory.  A transfer names a stream,
a channel slot and a window size; there is no address and no descriptor.

## Streams

| Stream | Carries |
|--------|---------|
| SIO0 | vertex output; the geometry stage reads its input from the same ring |
| SIO1 | geometry output and fragment colour output |
| SIO2 | no established producer |

## Channels

A stream is a ring of 32-bit channels.  A producer writes channels and commits
them with an export; a consumer reads them back.  The window is described by the
instruction itself, so every write and every read carries the size of the window
it addresses:

* the geometry input window is `(varying_count + 2) * 4` channels, at least 12,
  and the read instruction carries that count;
* a geometry output window holds the position record, one record per varying and
  a header slot, and the last slot is rewritten with the emitted-vertex counter
  before the export.

The window size is the record the producer commits; a consumer may declare it at
least that large, and the two must agree on the value.

* The closed GL driver's geometry stage for a shader that forwards one clip
  distance and one varying (the `gs-forward-clip` case of the LG200 geometry
  contract probe) carries `channum(132)` on every input read, while this
  repository's compiler carries `channum(12)` for the same GLSL source and that
  pipeline renders correctly.  A window far wider than
  `(varying_count + 2) * 4` is therefore legal.
* The size is not decorative.  Re-running one Vulkan geometry pipeline with a
  forced window of 16 instead of 12 moved every read onto a different ring
  record: the frame became deterministic but wrong, so a producer and its
  consumer have to agree on the value.
* The field is twelve bits wide.  Values 12, 16, 20, 24, 64, 128 and 132 were
  each encoded and decoded back unchanged in bits 8..19 of the instruction word
  (`3500400c`, `35004010`, `35004014`, `35004018`, `35004040`,
  `35004080`, `35004084`).  A reader must not treat it as a four-bit field.

Geometry input channel layout:

| Channels | Content |
|----------|---------|
| 0-3 | position; one four-channel read at channel 0 returns `xyzw` |
| `4 + 4b + c` | component `c` of the varying whose output base is `b` |

A read that does not move forward in the ring stalls every later export of the
stage, so reads are issued in ascending channel order even when the program
loads its inputs in another order.

## Instructions

| Group | Instructions |
|-------|--------------|
| write a channel | `siowrite`, `siowrite_imm`, `siowrrot`, `siowrrot_c4`, `siowrrot_id`, `siowrrot_id_c4`, `siowrrot_imm`, `siowrrot_imm_c4` |
| read a channel | `sioread`, `sioread_imm`, `siordrot`, `siordrot_c4`, `siordrot_imm`, `siordrot_imm_c4`, `siordrot_id`, `siordrot_id_c4` |
| commit an export | `sioexport` |
| emit a geometry vertex | `v_emit2`, `v_emit3` |

The `_c4` forms move four channels at once and take an aligned four-register
tuple; every other form moves one channel through one register.

## Operand roles

| Field | Role |
|-------|------|
| `VJ8` | data register: the value written, or the destination of a single-channel read |
| `VK8` | channel slot, held in a register |
| `M.chan12` | channel slot, as an immediate |
| `M.channum12` | window size in channels (`channum`) |
| `IA16` / `IK16` | window offset: which output plane of the stream the transfer addresses |
| `M.sioid1` | stream selector; the printed form is `sio:0` or `sio:1` |
| `VA8` | ring channel index, held in a register (identifier read form) |
| `IA16` | ring channel index, as an immediate (identifier read form) |
| `PJ2` | mask selector over `p0..p3`; see [Operands](../isa/operands.md#conditional-execution) |

## Writing channels

A write takes its data from the register named by `VJ8` and puts it in the
channel slot named by `VK8` (register form) or `M.chan12` (immediate form), inside
the window selected by the offset field.  The write is a staging step: the
channel becomes visible to the consumer at the ordering operation that follows,
and the stage output owns it after the export.

The slot is not only an index.  Before a write, the register whose number equals
the slot must hold the record descriptor for that channel, because the hardware
reads the descriptor from that register rather than from an operand.  A producer
stages the descriptor with a register move, writes the data, and orders; a write
whose slot register holds data leaves the channel with the wrong record type.

## Reading channels

The identifier read form `siordrot_id_c4` is the geometry stage's input read.  It
takes the ring channel index from `VA8`, the input-vertex selector from `VK8`, and
the ring window size from `M.channum12`.

Its destination is not the encoded destination field.  A four-channel read
writes the four VGPRs that begin at the next entry of a fixed sequence,
`4, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 8`, in read order, and
the destination field is ignored.  The sequence holds fifteen reads and then
wraps, so a consumer that needs more than fifteen open reads must copy each read
out of the window immediately.  This differs from the convention of a comparable
architecture, where the read instruction selects its own destination.

## Emitting

`v_emit3` and `v_emit2` emit the vertex staged in the output window.  Both take
the emit index in `GK11`, an argument in `GA11`, and the number of payload chunks
of that emit in `VJ8`.

The two opcodes select the stream shape, not the primitive cut: `v_emit2` belongs
to point and line streams and `v_emit3` to triangle streams.  Emitting a line
stream with `v_emit3` loses the first segment of every strip, because the
hardware assembles triangle records and only the cut resets them.  The cut is a
separate step: after an emit the producer writes a zero separator, rotates the
active lane mask, advances the vertex counter, and orders before the next emit.

## Export

`sioexport` commits a channel count from one stream to the stage output.  The
count must match what the stage produced: a vertex stage exports position
channels plus varying channels, a fragment stage exports its colour channels plus
one if it wrote depth, and the geometry stage rewrites its header slot with the
vertex counter before the export.  An export whose count does not match the
staged data leaves staged channels unexported or commits channel positions that
were never written, and an export whose header slot was not updated commits an
empty stream that never reaches the rasterizer.

Two shapes are special.  A vertex stage fused into the geometry stage does not
export at all, because the geometry stage reads the ring directly.  A fragment
stage that writes nothing still exports one channel.

## Ordering

SIO is order-sensitive; these are interface rules, not implementation advice:

1. an ordering operation follows every write, and precedes every export;
2. reads are issued in ascending channel order;
3. an emit is followed by an ordering operation before the next emit stages its
   payload;
4. a vertex must not reuse a slot register for its own payload;
5. the header slot is updated with the emitted count before the final export;
6. exports are issued in ascending output order;
7. a stage ends with writes, then the export, then the ordering operation, then
   the terminator.

## UNRESOLVED

* `PJ2` is the instruction's mask selector in every row of the family and the
  selector semantics are measured ([Operands](../isa/operands.md#conditional-execution));
  what is untested here is only whether an SIO transfer is gated like the ALU and
  flat-memory forms.
* The write forms other than `siowrrot` and `siowrrot_imm`, and the read forms
  other than `siordrot_id_c4`, are not emitted by any established producer, so
  their operand roles come from the field layout rather than from observation.
* What distinguishes the `write` and `rot` spellings of the write and read forms.
* The offset field's other classes, beyond the ordinary window and the special
  payload class marked by the `0x20` bit.
* SIO2.
* Whether the stream interface needs state to settle before a read observes a
  write, and how a write-to-read round trip is timed.
