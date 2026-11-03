# Conventions

This chapter fixes the vocabulary and notation the rest of the manual uses: how a
requirement is stated, how an unestablished property is marked, how numbers, bit
ranges and tables are written, and how instruction words, registers and links are
named. A chapter that departs from a form defined here says so where it departs.

## Normative terms

| Term | Meaning |
|------|---------|
| **MUST** | Required for conformance: a driver, compiler, shader, command stream or data record that violates it is non-conforming. |
| **MUST NOT** | Prohibited, under the same test. |
| **SHOULD** | Recommended; a deviation needs a reason, stated where the deviation is made. |
| **SHOULD NOT** | Not recommended, for the same reason as SHOULD. |
| **MAY** | Permitted, with no requirement either way. |

A sentence without one of these words states a fact rather than a requirement.
The words constrain what the host produces — a driver, a compiler, a shader, a
command stream or the contents of a buffer — never the device itself. A chapter
uses MUST only where the behaviour is established; an unestablished property is
marked `UNRESOLVED` instead of becoming a requirement.

A statement introduced by **Host policy** records a driver or compiler choice that
the device does not require, so another host may choose differently. Host policy
is not part of the device contract, and a chapter that relies on one says so where
it relies on it.

## Unresolved behaviour

`UNRESOLVED` marks a property that the manual does not establish. It states the
question, never a guess, where the property matters: as a word in a field's
meaning, or as a blockquote when the question needs explanation.

> UNRESOLVED: whether an odd-base pair is legal. A compiler aligns every tuple
> to its width.

An unresolved property is not permission to assume either outcome. A host that
has to act picks one behaviour and records it as host policy, stating which
outcome it assumes. The [UNRESOLVED register](appendices/unresolved.md) lists
the open questions by area, each with its consequence; an item leaves the
register when the property is established, and the manual keeps no history of
closed items.

## Numbers

| Form or term | Meaning |
|------|---------|
| `0x1f0` | A hexadecimal value. Digits may be upper or lower case; `_` may group a long constant, as in `0x0001_0000_0000_0000`. |
| `1024` | A decimal value: a count, a width, an index or a register number. |
| dword, word | A 32-bit word: the unit of the state object, of command packets and of each half of an instruction. |
| B, KiB, MiB, GiB | A byte, and 1024-based multiples of it. |
| little-endian | The least-significant byte of a word is at the lowest byte address. |

A length given in prose and an address are in bytes unless the sentence gives
another unit; an offset given in prose is a byte offset unless the sentence says
otherwise, and an offset that counts dwords is labelled as one. A register number
is decimal (`s12`, `v3`); an encoded selector is hexadecimal (`0x7c`).

## Bit ranges

Bit 0 is the least-significant bit of the unit a range is counted in. A range is
inclusive and is written most-significant bit first: `[11:8]` is four bits,
`[0]` is one bit, and no space separates the parts.

The unit is the one the range names or the table's unit column gives. A bare
range in a table with no unit column counts across the whole record: bit 0 is the
least-significant bit of its first dword, and `[63:32]` is its second dword. A
range that names a unit (`dw0[7:0]`) or sits under a `Dword`, `Word` or
`Register` column counts within that unit, and a cell may hold one range per
field or per word of the row. A table that lists control bits one per row uses a
`Bit` column with a decimal bit number.

Where a table counts across the record, a row that describes one field holds one
bare range in its Bits cell and nothing else; a unit or a condition belongs in
the column that explains the field.

An instruction is eight bytes: two 32-bit words, `INST_LO` in bytes 0-3 and
`INST_HI` in bytes 4-7, both little-endian. A field's range names its word,
`INST_LO[7:0]` or `INST_HI[17:16]`; a field that occupies both words is written
with its most significant part first, `INST_HI[2:0]:INST_LO[7:0]`.

## Registers and masks

| Name | Meaning |
|------|---------|
| `sN` | Scalar register N of the wave. |
| `vN` | Vector register N of a lane. |
| `pN` | Lane-mask register N, one bit per lane. |
| `SCC` | The wave's condition register. |
| `s[a:b]` | A tuple: the consecutive registers `a` through `b`. |

N counts from 0 in each of the three name spaces. `p0`..`p3` are scalar
registers at four distinguished selectors, not a separate register file; a gated
instruction names the mask it reads through its `PJ2` field, and the four can
also be read as general-source operands. `SCC` has a selector of its own. A group of consecutive
registers is also written by its ends, as `s4:s5` for a pair and `s11..s13` for
a longer run; all three forms include both ends. The file sizes and selectors are
in [Registers and Allocation](shaders/registers.md); the tuple base alignment is
in [Operands](isa/operands.md).

## Tables

A field table has these columns.

| Column | Contents |
|--------|---------|
| Bits | The field's range. |
| Field | The name the manual uses for the field. |
| Meaning | What the field does, with its units and any condition. |

The third heading names what the column adds; `Meaning` is the usual one. A table
of values has no Bits column; it pairs each encoded `Value` with its name. A cell
with no value in that context holds `—`, and bits that no row defines are stated
in the sentence after the table, not as a row.

## Cross-references

A fact — a field's position, a limit, an encoding — is defined in one chapter;
another chapter links to the definition instead of restating it, and a repeated
value carries the link beside it. A link to a heading is a page path relative to
the current page, then `#` and the heading's anchor: its text lowercased, with
each run of spaces or underscores turned into one hyphen and every other
character except letters, digits and hyphens removed. `## Pipe Enables` is
`#pipe-enables`; a reference on the same page omits the path.
