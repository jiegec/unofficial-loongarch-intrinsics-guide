# SIO

SIO is the channel interface between a shader and the fixed-function stages
around it. A transfer names a stream, a channel and a window, and moves one
32-bit value between a shader register and a stage record. The interface has no
address: the window is the only bound, and both ends of a stream must size it
alike.

## SIO and LDS

A shared-memory instruction takes an address whose high bits select the group
aperture, `0x1 << 48`; the hardware programs bits 47:32 of that base, a 4 GiB
granule, and the low bits are a byte offset inside it. SIO has no address at all:
a transfer selects a stream in one bit, a channel in a register or an immediate,
and a window in a 12-bit count; no aperture is programmed per dispatch, and there
is no byte offset or alignment rule. Use shared memory when invocations exchange
values; use SIO only to hand a stage's output to the fixed function that consumes
it. Nothing checks the two ends, so a window that disagrees with the producer's
record size is a silent layout error.

## Streams

The selector is one bit, so exactly two streams exist and no third is encodable.

| Stream | Written by | Read by |
|--------|------------|---------|
| SIO0 | Vertex stage: position block and varyings. | Geometry stage input reads. |
| SIO1 | Geometry stage record; fragment stage depth, coverage and colour. | The rasterizer and the attachment pipeline. |

A vertex stage that feeds a geometry stage writes that input ring on SIO0 and
does not export at all; a geometry stage reads that ring and writes its own
records on SIO1.

## Rows and fields

Seventeen rows belong to the family, in two formats: `sio-id` puts the channel
slot in an 8-bit field holding a register number, `sio-imm` in a 12-bit
immediate.

| Field | Bits | Role |
|-------|------|------|
| `X1_1` | high[22] | Stream selector, 0 or 1. |
| `PJ2` | high[21:20] | Mask selector; names one of four mask registers, not an operand. |
| `VJ8` | high[19:12] | Data source on a write; destination on a read. |
| `X0_12` | high[11:0] | Window width, in channels. |
| `VK8` | low[7:0] | Channel slot as a register number, on a `sio-id` write. |
| `X2_12` | low[11:0] | Channel slot immediate, on `sio-imm`. |
| `IA16`, `IK16` | low[31:16] | Window base, in 0x80 units. |
| `VA8`, `VK8`, `X2_16` | low[7:0], low[15:8], low[31:16] | Ring channel, input-record selector and a zeroed field, on an `id` read. |

The `X` fields are per-format immediates sharing one encoding slot, so a field
name means nothing without the row that carries it. A `_c4` row puts an aligned
four-register tuple in the `VJ8` position, which the `read` spelling calls `VD8`.

| Direction | Slot in a register | Slot immediate |
|-----------|--------------------|----------------|
| Write one channel | `siowrite`, `siowrrot` | `siowrite_imm`, `siowrrot_imm` |
| Write four channels | `siowrrot_c4` | `siowrrot_imm_c4` |
| Write with a ring index | `siowrrot_id`, `siowrrot_id_c4` | none |
| Read one channel | `sioread`, `siordrot` | `sioread_imm`, `siordrot_imm` |
| Read four channels | `siordrot_c4` | `siordrot_imm_c4` |
| Read with a ring index | `siordrot_id`, `siordrot_id_c4` | none |
| Commit a window | `sioexport` | --- |

## Channels

A record is a window of 32-bit channels. The width is `X0_12`, so a record holds
at most 4095 channels even though the SIO1 ring is 4096 channels wide.

| Channels | Vertex stage content |
|----------|----------------------|
| 0--3 | Position, one channel per component. |
| 4 | Miscellaneous vector: point size or edge flag when written; otherwise the first clip-distance group. |
| 4--7 or 8--15 | Clip and cull distances, position-adjacent. Base 4 while at most one distance group is present and no miscellaneous vector is written, base 8 otherwise. |
| `position_channels + 4b + c` | Component `c` of the `b`-th generic varying, in varying order. |

The position block is 4 channels wide with no distances and no miscellaneous
vector, 8 with one, and otherwise the distance base plus four channels per
distance group. The record width is the largest of `position_channels + 4v` for
`v` varyings, `(v + 2) * 4`, and 12; above 32 it rounds up to a multiple of 32,
so one vertex's input quads cannot alias the next.

On SIO1, channel 0 is the control channel: the geometry stage's emitted-vertex
count, or the fragment stage's depth or coverage mask. Above it lie the geometry
record channels or the fragment colour targets in target order. A colour target
occupies one channel when it packs to 8 bits, two as an `f16` pair, and four as
`f32` components.

## Addressing

The **slot** names a channel inside the window: a register number in `sio-id`, a
12-bit immediate in `sio-imm`. The **window** is the width the producer wrote; a
consumer that reads fewer channels does not shrink it, because the record stride
is the producer's, so sizing it from the consumer's inputs moves every boundary.

The **base** is not a channel index. It counts 0x80 per channel: the vertex stage
writes its position block at 0 and its varyings at `position_channels * 0x80`,
the geometry stage writes its record at `0x80` because channel 0 holds the control
value, and a fragment colour target after `n` lower channels writes `n << 7`. A
base that is not a multiple of 0x80 addresses the middle of a channel.

## Writing

A write takes its data from `VJ8` and places it in the slot, inside the window
selected by the base. The destination is the stream; a write row has no
destination register.

A register-addressed write carries an implicit operand. The slot is a register
number, and that register must hold the channel's record descriptor: stage the
descriptor into the register whose number equals the slot, then write the data
from another register. Payload left in the slot register lands where the
descriptor belongs. The immediate forms borrow no register and carry no
descriptor; how the record type reaches the interface there is UNRESOLVED.

## Reading

A channel read moves one channel into `VD8` or `VJ8`, and a `_c4` read moves four
into an aligned quad. Reads take the same window and base fields as writes.

The identifier read, `siordrot_id_c4`, is the form a geometry stage uses for its
input ring: the ring channel comes from `VA8`, the input-record selector from
`VK8`, a sixteen-bit field from `X2_16`, and the window from `X0_12`. The
selector is one five-bit field of the input vertex id, so a single register
addresses one varying across the records of a primitive.

The destination is not free. The four channels land in a quad whose base follows
a fixed fifteen-entry sequence:

```text
4, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 8
```

Each read advances the sequence and the sixteenth returns to the start. It
advances per read, not per channel, so the issue order decides which quad holds
which channel: a consumer needing more than fifteen live reads must copy each quad
out of its window before the next read, and must issue reads in ascending channel
order even when the shader names its inputs in another order.

## Export

`sioexport` commits the first channels of one stream, from channel 0 up to the
encoded count; a channel at or above the count is not committed, so a producer
that writes a high channel and exports a short count loses it.

The count is the committed prefix, not the window, and the two are separate
decisions:

| Stage | Stream | Count |
|-------|--------|-------|
| Vertex | SIO0 | Position block plus varyings, distance channels included. |
| Vertex feeding a geometry stage | --- | No export at all. |
| Geometry | SIO1 | The record's channel total, after the control channel is written. |
| Fragment | SIO1 | Colour channels, plus one when depth or coverage holds channel 0. |
| Fragment with no output | SIO1 | One, with no writes. |

The geometry stage must write the emitted-vertex count to SIO1 channel 0 before
the export; the rasterizer walks the record with that count, so an export whose
control channel still holds an earlier value commits a stream it cannot delimit.
The fragment export starts at channel 0 unless the shader also writes depth or a
coverage mask; either owns channel 0 and moves the colours to `0x80`, and the
count then covers both.

## Emitting

`v_emit2` and `v_emit3` are vector rows that write the same stream. They publish
the vertex staged in the output window, taking the emit ordinal through `GK11` as
a vector source selector, the record argument through `GA11` in a scalar
register, and an 8-bit raw field in `VJ8`: the number of a register holding the
separator, or a chunk immediate. `M.mod2` and `M.clmp1` are cleared.

The two rows differ in the cut: `v_emit2` closes the current primitive and
`v_emit3` does not. A triangle strip is the stream that assembles without a cut,
so it takes `v_emit3`; point streams and every other output primitive take
`v_emit2`. The cut-less row on a stream that needs a cut joins vertices from
different primitives.

Both rows are gated per lane by the mask selector, so an inactive lane emits
nothing. After each emit the window is maintained: the separator is written to
SIO1 above the record base, the emit counter advances by the active lanes, and the
execution mask is restored. An emit that reuses the window without that
maintenance publishes a record the separator still treats as open.

## Ordering

The interface carries no address and no queue, so order is all that keeps the two
ends coherent. A producer MUST:

1. Write a channel before it exports that channel.
2. Write the geometry control channel before the geometry export.
3. Copy a `_c4` read out of its window before issuing the next read.
4. Issue reads in ascending channel order.
5. Keep the payload out of the slot register on a register-addressed write.
6. Stage the emit payload before the emit, and maintain the window before the next.
7. Export the stream it wrote, with a count covering every channel it wrote.

Stream writes order like stores, and a compiler MUST NOT move a load across them
even though they reach no memory. Nothing in the family drains the stream and no
row in it waits for one; whether some other row orders a write against a later
read or export is UNRESOLVED.

## Open questions

- What distinguishes the `write` spelling from the `rot` spelling: the rows share
  a format and a field layout, and only the opcode differs.
- The descriptor source on the immediate write forms, which have no slot register.
- The meaning of `X2_16` on the identifier read; the supported path passes zero.
- Whether a write becomes visible to a consumer without a drain, and which row
  would provide one.
- Whether `PJ2` gates these rows: they carry the field, but a per-lane write is
  produced by installing the mask in `p0` instead.
