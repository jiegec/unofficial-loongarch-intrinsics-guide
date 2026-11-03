# Operands

An instruction is two 32-bit words, and inside them a fixed set of operand
fields. This chapter fixes what those fields select: the space a scalar
register, a vector register or an inline constant is named in, the width and
unit of each immediate, the modifier bits, and the alignment a multi-register
operand must satisfy. The formats that carry the fields are in
[Encoding](encoding.md); what the selected registers hold is in
[Registers and Allocation](../shaders/registers.md).

## Operand fields

A field name has three parts: a role letter, the encoded width in bits, and,
for an operand that occupies more than one register, `x` and the count.
`GJ11` is a general source in the J slot, eleven bits wide; `RK8x4` is an R
field naming four consecutive scalar registers from an eight-bit base. `J`,
`K` and `A` name source slots and `D` a destination; which slot an
instruction reads first is fixed by the row, not by the letter.

| Letter | Selects | Fields |
|--------|---------|--------|
| `G` | A general source: a register or an inline constant. | `GJ8`, `GK8`, `GJ11`, `GK11`, `GA11` |
| `R` | A register whose role the row defines: destination, descriptor base or offset source. No inline constant. | `RD8`, `RJ8`, `RK8`, `RA8`, `RK9`, `RA9` |
| `V` | A vector register number, 0 to 255. | `VD8`, `VJ8`, `VK8`, `VA8`, `VQ8` |
| `I` | A literal. | `IJ32`, `IK32`, `IA16`, `IJ16`, `IK16` |
| `U` | An unsigned offset. | `UO8`, `UO12`, `UO20` |
| `M.` | A modifier bit or bit field. | `M.mod2`, `M.clmp1` |
| `X` | A row-defined literal with no class of its own. | `X0_32`, `X0_16`, `X0_4`, `X1_2` |

The letter says what kind of thing a field names, not how a row uses it. `RK8`
is a general source in the scalar two-source forms, where an inline constant is
allowed, and an offset source in the buffer forms, where only a register or the
zero selector is. `RD8` is a destination in most rows and a compare
destination in the scalar compare rows, where it may also name `SCC`.

Most fields sit inside one byte of one word. Five do not, because what they
name does not fit a byte: `GJ11` is `INST_HI[2:0]:INST_LO[7:0]`, `GK11` is
`INST_HI[5:3]:INST_LO[15:8]`, `GA11` is `INST_HI[8:6]:INST_LO[23:16]`, and
the nine-bit `RK9` and `RA9` are `INST_HI[11]:INST_LO[15:8]`.

`PJ2` is two bits at `INST_HI[13:12]` in the vector ALU forms,
`INST_HI[17:16]` in the other predicated formats and `INST_HI[21:20]` in the
SIO forms. It is not an operand: it names the lane-mask register the
instruction runs under, so a row without the field cannot be gated, and the
scalar formats carry no `PJ2`.

## General-source selector

An eight-bit general-source field carries one byte from a single table. The
same table serves the eleven-bit fields, where the byte is the low part.

| Value | Selects | Scope |
|-------|---------|-------|
| `0x00..0x65` | Scalar registers `s0`..`s101`, 102 × 32 bits. | One per wave |
| `0x66` | Constant zero. | — |
| `0x67` | A second constant encoding beside `0x66`. What it supplies, and why two exist, is UNRESOLVED. | — |
| `0x68..0x7b` | Unassigned. | — |
| `0x7c..0x7f` | The lane-mask registers `p0`..`p3`, 4 × 32 bits. | One per wave |
| `0x80` | The condition register `SCC`. | One per wave |
| `0x81..0x9f` | Unassigned. | — |
| `0xa0..0xdf` | The integers 1 to 64, encoded as `0x9f + n`. | — |
| `0xe0..0xef` | The integers −1 to −16, encoded as `0xdf + m` for the magnitude `m`. | — |
| `0xf0..0xfa` | 0.5, −0.5, 1.0, −1.0, 2.0, −2.0, 4.0, −4.0, π, e, 1/(2π). | — |
| `0xfb..0xff` | Unassigned. | — |

An unassigned value is not a register and MUST NOT enter a register allocation
pool. A reader MUST NOT treat one as zero either: nothing in the encoding
establishes what an unassigned selector supplies, so the value it contributes
to an operand is UNRESOLVED.

## 11-bit selector

An eleven-bit general-source field widens the same byte with three bits above
it:

| Bits | Value | Effect |
|-----:|------:|--------|
| `[7:0]` | | The base selector from the table above. |
| `[8]` | `0x100` | The base is a vector register number in `v0`..`v255`; the constant table does not apply. |
| `[9]` | `0x200` | Absolute value of the source. |
| `[10]` | `0x400` | Negation of the source. |

Bits 9 and 10 act on the value, not on the register file: they are the source
modifiers, and an eight-bit field has no room for them, so a source that is
negated or made absolute needs an eleven-bit field. With `0x100` set the base
is a whole register number, which is why `v102` (`0x166`) and zero (`0x66`)
are different sources.

A source with both modifier bits set is encodable. Whether the negation applies
to the absolute value or the absolute value to the negation is UNRESOLVED, so a
producer that needs the negation of an absolute value SHOULD build it in two
instructions instead of relying on the combination.

## Register files

The vector file is reachable two ways. A `V` field holds the register number
directly, in eight bits, and an eleven-bit `G` field reaches the same file
through the `0x100` namespace. An eight-bit `G` field cannot: the namespace
bit does not fit.

The scalar file has 102 registers, so `0x68..0x7b` and `0x81..0x9f` name no
register. The four lane masks are scalar addresses rather than a fifth file,
and `PJ2` names which of them gates an instruction. A lane mask holds one bit
per lane and a wave is 32 lanes wide, so one register covers exactly one wave.

## Immediate forms

| Field | Bits | Position | Unit |
|-------|-----:|----------|------|
| `IJ32`, `IK32` | 32 | `INST_LO[31:0]` | A literal bit pattern. |
| `X0_32` | 32 | `INST_LO[31:0]` | A row-defined literal. |
| `IA16`, `IJ16`, `IK16` | 16 | `INST_LO[31:16]` | An SIO id, channel slot or window offset. |
| `UO8` | 8 | `INST_HI[7:0]` | An unsigned value, 0 to 255. |
| `UO12` | 12 | `INST_HI[11:0]` | An unsigned byte offset, 0 to 4095. |
| `UO20` | 20 | `INST_LO[27:8]` | An unsigned offset, 0 to 1048575. |
| `X0_16` | 16 | `INST_HI[15:0]` | A row-defined literal. |

A 32-bit literal takes the whole low word, so a row that carries one has no
room for a second register operand there and the format routes the literal with
fixed bits instead of with a field. The value is an uninterpreted bit pattern:
whether it is read as signed or unsigned is a property of the mnemonic, not of
the field.

Only the branch displacement is signed at the encoding level. The branch rows
carry it in the 32-bit literal at `INST_LO[31:0]` as a signed count of
eight-byte units added to the address of the instruction after the branch, and
the field holds the two's-complement low 32 bits; a producer MUST range-check
the value before truncating it. Every other immediate is packed as an unsigned
value with the range in the table, so a negative offset has no encoding in a
`UO` field.

Two row-defined fields carry image controls. `X0_4` at `INST_HI[3:0]` is the
channel mask, and a producer MUST NOT encode a zero mask. The pair `X1_2` at
`INST_HI[10:9]` holds the unnormalized-coordinate bit at bit 10; the role of
bit 9 is UNRESOLVED.

## Modifiers

A modifier is a bit whose effect is on another operand or on the row itself.
Each occupies one position per format.

| Field | Bits | Position | Effect |
|-------|-----:|----------|--------|
| `M.mod2` | 2 | `INST_HI[10:9]` | Selects one of four operation variants of the row. The mapping from value to operation is UNRESOLVED. |
| `M.clmp1` | 1 | `INST_HI[11]` | Clamps the result. The clamp's range and its behaviour on NaN are UNRESOLVED. |
| `M.offen1` | 1 | `INST_HI[12]` | The `UO12` constant offset enters the address. |
| `M.idxen1` | 1 | `INST_HI[13]` | The `VQ8` per-lane index enters the address; the gating is UNRESOLVED. |
| `M.bl11` | 1 | `INST_HI[14]` in the buffer forms, `INST_LO[30]` in the flat forms | A form bit of the memory row. The buffer loads and stores carry it set. |
| `M.bl21` | 1 | `INST_HI[15]` in the buffer forms, `INST_LO[31]` in the flat forms | A second form bit, also set on the streamout store. |
| `M.vm1` | 1 | `INST_LO[1]` | Bit 1 of the ordering domain of `check`. |

`M.offen1` and `M.idxen1` are enables: clearing one removes a term from the
address rather than supplying a zero for it, and a buffer row with both clear
addresses the descriptor base alone. `M.bl11` and `M.bl21` have no
established meaning to the memory system beyond the combinations the rows use,
and whether a buffer form with `M.bl11` clear works is UNRESOLVED.

`M.vm1` is not a flag of the row but one bit of a four-bit domain mask that
`check` carries at `INST_LO[3:0]`: bit 0 orders the scalar and kernel
domain, bit 1 the vector memory domain, bit 2 the vector store domain and bit 3
the shared-memory domain. A set bit includes its domain in the ordering, and a
producer with no narrower requirement SHOULD set all four. What the ordering
guarantees belongs to the control family chapter.

## Tuple alignment

A multi-register operand stores one base in the field; the width comes from the
row's `x` suffix. The registers are consecutive, and the base MUST satisfy the
row's alignment.

| Width | Alignment | Where it occurs |
|------:|----------:|-----------------|
| 1 | None | Every single-register operand. |
| 2 | Even | Vector pairs, absolute address pairs, the zero scalar pair of the flat forms. |
| 3 | None | Three-register buffer results. |
| 4 | Multiple of 4 | Vector quads, buffer and sampler descriptors, scalar-memory results. |
| 8 | Multiple of 8 | Image descriptors, eight-register scalar groups. |
| 16 | Multiple of 16 | The widest scalar-memory result. |

The alignment is a property of the row, not a formula over the width. It equals
the width for most tuples, is absent for a width of three, and is four for the
sampler forms that take a gradient window even when that window is three or
five registers wide. A texture coordinate window is the other exception: it
must be consecutive but need not be aligned at all.

A descriptor base is a scalar register number and the descriptor is a
consecutive run from it: four registers for a buffer or a sampler, eight for an
image. The image forms widen the base to nine bits, the ninth at
`INST_HI[11]`, so that base does not come from an eight-bit field.

An unaligned run is rejected rather than split. Whether the machine would mask
an unaligned base instead is UNRESOLVED, and so is whether an odd-base pair is
legal in an encoding that never carries one.
