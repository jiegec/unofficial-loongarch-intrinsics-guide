# SIO and Export Family

SIO is a channel interface: one 32-bit value per channel moves between a shader
register and a stage record. A transfer names a stream, a window and a slot, and
carries no address, so nothing checks the two ends. This chapter fixes the
family's seventeen rows, the slots they share, the arithmetic that turns a base
and a slot into a channel, the register a read writes, and what an export
commits. The record layout and the per-stage export shapes are in
[SIO](../../shaders/io.md); the machine rows are in the
[SIO index](../../instructions/sio-export.md).

## Rows

Seventeen rows in two formats. `sio-id` holds eleven and takes the channel slot
from a register; `sio-imm` holds six and takes it from a 12-bit immediate.
Sixteen rows move data; the seventeenth commits a count.

| Row | Format | Direction | Slot source | Channels |
|-----|--------|-----------|-------------|---------:|
| `sioread` | `sio-id` | read | register, `LO[7:0]` | 1 |
| `siordrot` | `sio-id` | read | register, `LO[7:0]` | 1 |
| `siordrot_c4` | `sio-id` | read | register, `LO[7:0]` | 4 |
| `siordrot_id` | `sio-id` | read | ring index, `VA8` | 1 |
| `siordrot_id_c4` | `sio-id` | read | ring index, `VA8` | 4 |
| `sioread_imm` | `sio-imm` | read | immediate, `LO[11:0]` | 1 |
| `siordrot_imm` | `sio-imm` | read | immediate, `LO[11:0]` | 1 |
| `siordrot_imm_c4` | `sio-imm` | read | immediate, `LO[11:0]` | 4 |
| `siowrite` | `sio-id` | write | register, `LO[7:0]` | 1 |
| `siowrrot` | `sio-id` | write | register, `LO[7:0]` | 1 |
| `siowrrot_c4` | `sio-id` | write | register, `LO[7:0]` | 4 |
| `siowrrot_id` | `sio-id` | write | ring index, `VA8` | 1 |
| `siowrrot_id_c4` | `sio-id` | write | ring index, `VA8` | 4 |
| `siowrite_imm` | `sio-imm` | write | immediate, `LO[11:0]` | 1 |
| `siowrrot_imm` | `sio-imm` | write | immediate, `LO[11:0]` | 1 |
| `siowrrot_imm_c4` | `sio-imm` | write | immediate, `LO[11:0]` | 4 |
| `sioexport` | `sio-id` | commit | none | count |

Only the single-channel shapes come in both spellings: `sioread`/`siordrot`,
`sioread_imm`/`siordrot_imm`, and on the write side
`siowrite`/`siowrrot`, `siowrite_imm`/`siowrrot_imm`. A pair shares one
field layout and differs only in the fixed part of the word. No source
establishes a different effect for the two spellings, so a producer MUST NOT
expect a `rot` row to rotate anything the plain row leaves alone: UNRESOLVED.

A `_c4` row moves four consecutive channels through one aligned register quad.
The `id` rows add a second register, the input-record selector, and a 16-bit
field that the established path leaves zero.

## Encoding

A transfer is two dwords. Both SIO formats place the stream, the mask selector,
the data register and the window in the high word:

| Bits | Field | Role |
|------|-------|------|
| `HI[26:23]` | opcode | Four-bit row selector. A row is matched by its whole fixed part, so the opcode field alone does not name it. |
| `HI[22]` | `X1_1`, `M.sioid1` | Stream: 0 selects SIO0, 1 selects SIO1. |
| `HI[21:20]` | `PJ2` | One of the four mask registers `p0`..`p3`. |
| `HI[19:12]` | `VJ8` or `VD8` | The data register: source on a write, destination on a read. |
| `HI[11:0]` | `X0_12`, `M.channum12` | Window width in channels; on `sioexport`, the committed count. |

The low word differs by row group:

| Group | `LO[11:0]` | `LO[15:8]` | `LO[31:16]` |
|-------|-----------|-----------|------------|
| Register slot: `siowrite`, `siowrrot`, `siowrrot_c4`, `siordrot`, `siordrot_c4`, `sioread` | slot register (`VK8`; `VJ8` on `sioread`) | not an operand | window base (`IA16`; `IK16` on `sioread`) |
| Ring index: `siowrrot_id`, `siowrrot_id_c4`, `siordrot_id`, `siordrot_id_c4` | ring channel register (`VA8`) | input-record selector (`VK8`) | 16-bit identifier (`X2_16`) |
| Immediate: `siowrite_imm`, `siowrrot_imm`, `siowrrot_imm_c4`, `siordrot_imm`, `siordrot_imm_c4`, `sioread_imm` | slot immediate (`X2_12`) | not an operand | window base (`IK16`; `IJ16` on `sioread_imm`) |

Field names belong to rows, not to bit ranges: `LO[7:0]` is `VK8` on
`siowrrot`, `VJ8` on `sioread` and `VA8` on `siowrrot_id`, and the
destination is `VD8` on the `read` spellings and `VJ8` on the `rot`
spellings. A decoder MUST take every field name from the row it matched.

`sioexport` has no data register: its `HI[19:12]` bits are not an operand,
and its `X0_12` field is a count, not a window.

## Addressing

A transfer addresses one channel as a base plus a slot, with the base counted in
units of `0x80`:

```text
channel = (base >> 7) + slot
```

The base is the 16-bit low-word field and the slot is the 8-bit register number
or the 12-bit immediate. Base 0 with slot 0 is channel 0; base `0x80` with slot
0 is channel 1. The low seven bits of the base are not a channel: what a base
that is not a multiple of `0x80` selects is UNRESOLVED.

The window is the record width in channels. `X0_12` holds 0 to 4095, and a
window of zero is UNRESOLVED. The window bounds the slots a transfer may name; a
slot that lands outside it is UNRESOLVED.

The stream selector is one bit, so exactly two streams are encodable. SIO0
carries the vertex stage's output: the input ring a geometry stage reads, or the
exported record when no geometry stage follows. SIO1 carries the geometry
stage's record and the fragment stage's depth, coverage and colour.

## Reads and destination selects

All eight read rows name their destination in `HI[19:12]`. A `_c4` read names
the base of four consecutive registers, so the base MUST be a multiple of four
and at most 252; the row encodes one register number and the three above it are
implied. Whether the hardware checks that alignment is UNRESOLVED.

`siordrot_id_c4` is the exception: its destination is not the encoded field.
The four channels land in the quad whose base follows a fixed fifteen-entry
sequence, one entry per read:

```text
4, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 8
```

The sequence advances per read, not per channel, so issue order decides which
quad holds which channel. A producer MUST issue ring reads in ascending channel
order and MUST copy each quad out of its window before the next read. It MUST
also encode the window base the sequence selects in the destination field,
because whether the field is ignored is UNRESOLVED. The sixteenth read is
UNRESOLVED: the sequence may restart at 4 or continue to 64, and the two choices
disagree about every read past the fifteenth.

A ring read is a side effect: it advances the destination window, so two reads
cannot be exchanged and a compiler MUST NOT reorder one across another or across
a write to the same ring. A read of a channel that was never written is
UNRESOLVED: zero, stale data and undefined data are all unexcluded.

## Writes

A write's destination is the stream, and a write row has no destination
register. The data comes from `HI[19:12]` and the slot names where it lands.

A register-addressed write carries an implicit operand. The slot is a register
number, and the register whose number equals the slot MUST hold the channel's
record descriptor when the write issues, because the hardware reads the
descriptor from that register rather than from an operand. The payload therefore
comes from another register. A write whose slot register still holds payload
stamps the wrong record type into the channel, and reusing the slot register for
the value being written is invalid.

The immediate rows name the slot in the instruction and borrow no register, so
they carry no descriptor. How the record type reaches the interface on those
rows is UNRESOLVED.

A write stages a channel; it does not publish it. Whether two lanes writing one
channel merge or replace is UNRESOLVED, as is the effect of a slot outside the
window.

## Export and ordering

`sioexport` commits a channel count from one stream. It carries the stream, the
count and the mask selector, and no data: the channels are the ones the
preceding writes staged.

The count is the record's channel total, not a window. A write states the window
it writes into; the export states how many channels the consumer may read, and
the two need not be equal. A fragment stage that puts the coverage value at base
0 with a one-channel window and two colours at base `0x80` with a two-channel
window exports three channels, not one or two.

A control channel counts as one channel. The geometry stage's emitted-vertex
count and the fragment stage's depth or coverage value occupy channel 0, and the
record starts above it at base `0x80`; the count covers both.

A producer MUST write every channel it exports before the export, and MUST NOT
export a stream it did not write. Only its own order holds a write against an
export: no row in the family waits, drains or reports completion, and whether
some other row orders a stream write against a later read or export is
UNRESOLVED. A compiler MUST treat a stream write as a store, MUST NOT move a load
across one, and MUST NOT move an export above the writes it commits.

Exporting a stream the stage does not consume, exporting twice, and exporting a
count that disagrees with the channels written are UNRESOLVED in effect.

## Masking

Every row encodes `PJ2`, and the field names one of four mask registers. Whether
it gates the transfer is UNRESOLVED. What is established is the execution mask:
a write, an export and the ring read transfer one channel per active lane and
run under the mask installed in `p0` at the row. A producer that needs a subset
of lanes MUST narrow the execution mask before the row and MUST NOT rely on
`PJ2`.

## Open questions

- What a `rot` spelling does that its plain spelling does not. The two share a
  layout and differ only in fixed bits, and no case separates them.
- Whether `PJ2` gates a transfer, and what a non-`p0` selector would do.
- The meaning of the 16-bit field on the `id` rows; the established path passes
  zero.
- Whether `siordrot_id_c4` reads its destination field or ignores it.
- The destination of the sixteenth and later ring reads.
- The descriptor source on the immediate write rows, which have no slot register.
- Whether a write becomes visible to a consumer without a drain, and which row
  would provide one.
- The value a read returns from a channel that was never written.
