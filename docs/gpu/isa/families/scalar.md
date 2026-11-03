# Scalar ALU Family

The scalar ALU family computes on the scalar register file and on the condition
register. A scalar register holds one value for the whole wave, so these rows
produce the addresses, counters and record words the per-lane rows consume, and
this is the only family that reads and writes the condition register as an
operand. The family's 119 rows are in the [scalar index](../../instructions/scalar-alu.md);
this chapter describes what they share.

## Forms

Every row is one of two machine formats and one of seven operand shapes.

| Shape | Rows | Fields | Result |
|-------|-----:|--------|--------|
| One source | 18 | `GJ8`, `RD8` | Complement, negate, extend, count, reverse, mask conversion. |
| Two sources | 44 | `GJ8`, `RK8`, `RD8` | Arithmetic, logic, shift, min, max, bit field, compare, bit test. |
| Three sources | 6 | `GJ8`, `RK8`, `RA8`, `RD8` | Add with carry, subtract with borrow, bit-field extract, select, index read. |
| 64-bit second source | 2 | `GJ8`, `RK8x2`, `RD8` | Equality and inequality of register pairs. |
| Literal to destination | 1 | `IJ32`, `RD8` | Move of a 32-bit literal. |
| Register and literal | 35 | `IK32`, `RJ8`, `RD8` | Literal form of the two-source rows. |
| Compare with literal | 13 | `IK32`, `RJ8` | Comparison whose only destination is the condition register. |

The register shapes use the `sop` format and the literal shapes the
`sop-imm-control` format, which is shared with the control family. A word in
that format is therefore not necessarily a scalar row.

| Field | `sop` position | `sop-imm-control` position |
|-------|----------------|----------------------------|
| Opcode | `INST_HI[9:0]` | `INST_HI[22:16]` |
| `GJ8` | `INST_LO[7:0]` | — |
| `RK8` | `INST_LO[15:8]` | — |
| `RA8` | `INST_LO[23:16]` | — |
| `RJ8` | — | `INST_HI[7:0]` |
| `RD8` | `INST_LO[31:24]` | `INST_HI[15:8]` |
| `IK32`, `IJ32` | — | `INST_LO[31:0]` |

A register row holds every operand in `INST_LO` and its opcode in the high
word; a literal row puts the whole 32-bit literal in `INST_LO` and both of its
operands in `INST_HI`. Assembly order is destination first and literal last,
the reverse of the encoded order: `s_add_imm_u32 s0, s0, 0x0` encodes the
literal in `INST_LO[31:0]`, the source in `INST_HI[7:0]` and the destination
in `INST_HI[15:8]`.

The formats number their rows independently. The literal comparisons reuse
opcodes `0x038`–`0x043` of the register comparisons, but the arithmetic,
logic and shift literal rows count from `0x000` and skip the operations with
no literal form, so an opcode number is not a name across formats: `0x010` is
`s_absdiff_u32` in one and `s_and_imm_b32` in the other. Opcodes
`0x025`–`0x037` of the literal format are not scalar rows, and
`s_cmp_imm` at `0x024` defines no operand — none of its bits is marked as an
operand and it has no assembly form — so a producer MUST NOT emit it.

## Selectors

Every scalar operand field, destination included, carries the same 8-bit
selector: `0x00`–`0x65` are `s0`–`s101`, `0x66` is the constant zero,
`0x7c`–`0x7f` are the lane masks `p0`–`p3`, and `0x80` is the condition
register. Inline constants are usable wherever a value is read; the whole table
is in [Operands](../operands.md).

The two-source rows read their first source from the general position `GJ8`
and the later ones from register positions, but the space is common. That is
what lets a carry or a condition be named rather than copied: `s_addc_u32`
reads its carry from `RA8`, so `0x66` supplies a zero carry, `0x80`
supplies the condition register, and any other selector supplies one whenever
that register is non-zero.

Two rows are two-address. `s_bitset0_b32` and `s_bitset1_b32` take the bit
index from `GJ8` and read, modify and write the word in `RD8`; there is no
separate data source and no separate result. The documented model reduces the
index modulo 32.

## Condition register

`SCC` is one bit per wave. 88 of the 119 rows update it as a side effect of
the write they already perform; the 31 that do not are the 27 comparisons and
the 4 bit tests.

A comparison writes its outcome to the destination field. Selector `0x80`
makes that destination `SCC`; any other selector writes `0xffff` for true and
`0` for false into an ordinary scalar register, and does not also set the
condition. `0xffff` is the encoding a set condition materialises, which is why
a consumer tests its source for non-zero instead of reading bit 0. The 13
literal comparisons have no destination field at all: their outcome goes to
`SCC` and nowhere else. The four bit tests — `s_bitcmp0_b32`,
`s_bitcmp1_b32`, `s_bitcmprev0_imm_b32`, `s_bitcmprev1_imm_b32` —
materialise `0xffff` or `0` into their destination and leave `SCC` alone.

Three mechanisms consume it:

- Add with carry and subtract with borrow read it from the third source of the
  register forms, and implicitly in `s_addc_imm_u32`, `s_subb_imm_u32` and
  `s_subbrev_imm_u32`, whose layout has no third source. The literal sits on
  opposite sides: `s_subb_imm_u32` subtracts the literal and the borrow from
  the register, `s_subbrev_imm_u32` subtracts the register and the borrow from
  the literal.
- `s_select_b32` reads its third source as the condition and returns the first
  source when it is non-zero, the second otherwise.
- The conditional branch rows test the same selector space, with `0x80`
  selecting the condition register. [Control family](control.md) covers them.

A carry chain names `0x80` in the carry source of every step after the first.
The flag is short-lived: a row that writes `SCC` supersedes the previous value,
and a read issued immediately after a write is documented as possibly returning
the earlier one. UNRESOLVED: whether the value a carry row leaves in `SCC` is
its carry out, whether that write is ordered after the row's own read of the
incoming carry, and how much distance a read needs after a write.

## Whole-quad and quad masks

A wave is 32 lanes and a lane mask holds one bit per lane, so the 32 bits of a
mask divide into eight groups of four. One group is one quad. Two rows convert
between the two granularities.

| Row | Opcode | Operation | Example |
|-----|-------:|-----------|---------|
| `s_wqm_b32` | `0x030` | A non-zero group of four bits becomes four ones; an empty group stays zero. | `0x00000055` → `0x000000ff` |
| `s_quadmask_b32` | `0x031` | A group of four bits becomes one bit, set when the group is non-empty, group 0 to bit 0. | `0x12345678` → `0x000000ff` |

`s_wqm_b32` widens: every partly active quad becomes fully active, so a quad
of the result is either empty or full. `s_quadmask_b32` narrows: it marks
which quads held a set bit, so the result needs eight bits. Neither is
reversible; the position of a bit inside its group is lost. Both take one
general source and one scalar destination, and both update `SCC`.

UNRESOLVED: whether `s_quadmask_b32` writes the whole destination word or only
its low eight bits; whether either row may name a mask register `p0`–`p3` as
its destination, which placing the result within reach of an instruction's mask
selector would require; and which of the source or the result the row leaves in
`SCC`.

## Scalar memory

Ten rows load memory into the scalar file. The family has no scalar store.

| Rows | Destination width | Address |
|------|------------------:|---------|
| `load_s_buff_w1`, `_w2`, `_w4`, `_w8`, `_w16` | 1, 2, 4, 8, 16 words | A four-register buffer record plus a 20-bit offset. |
| `load_s_flat_w1`, `_w2`, `_w4`, `_w8`, `_w16` | 1, 2, 4, 8, 16 words | An absolute address pair plus a 20-bit offset. |

| Field | Bits | Meaning |
|-------|------|---------|
| `RJ8x4` | `INST_LO[7:0]` | Base of the buffer record, four consecutive registers. |
| `RJ8x2` | `INST_LO[7:0]` | Base of the address pair, two consecutive registers, even-aligned. |
| `UO20` | `INST_LO[27:8]` | Unsigned constant offset, `0` to `0xfffff`. |
| `X0_1` | `INST_LO[30]` | One parameter bit whose role on these rows is UNRESOLVED. |
| `GK8` | `INST_HI[7:0]` | General source; every form writes the zero selector `0x66`. |
| `RD8xN` | `INST_HI[15:8]` | Base of the destination tuple, `N` consecutive registers. |
| Opcode | `INST_HI[26:16]` | Row selector. |

The destination tuple is aligned to its width: a two-word load starts at an even
register, a four-word load at a multiple of four, a sixteen-word load at a
multiple of sixteen. The tuple must lie inside `s0`–`s101`. A load returns
32-bit words and does not extend the sign bit, so `0x80000000` arrives as
`0x80000000`.

- The offset's unit, bytes or elements, is UNRESOLVED, so the field cannot be
  scaled to a byte address until it is settled.
- Whether a load needs natural alignment, how an access crossing a page or a
  cache line is divided, and what an address outside the addressed object does
  are UNRESOLVED.
- The buffer record supplies base, size and format; whether the load consults
  the format fields is UNRESOLVED. The flat form has no record, bound or format.
- Whether `GK8` is read at all is UNRESOLVED.

## Family map

| Group | Rows | Result |
|-------|------|--------|
| Addition and subtraction | `s_add_*`, `s_sub_*`, `s_subrev_imm_*`, `s_addc_*`, `s_subb_*`, `s_subbrev_imm_u32` | Low 32 bits; wraps, never saturates. |
| Multiplication | `s_mul_*`, `s_mul_hi_*` | Low or high half of a 64-bit product. |
| Magnitude and order | `s_abs_i32`, `s_ineg_i32`, `s_absdiff_*`, `s_min_*`, `s_max_*` | Absolute value, negation, absolute difference, ordered choice. |
| Logic | `s_and*`, `s_or*`, `s_xor*`, `s_nand*`, `s_nor*`, `s_xnor*`, `s_not_b32` | Bitwise; `andn1`, `andn2`, `orn1`, `orn2` complement one source first. |
| Shifts and rotates | `s_sll*`, `s_srl*`, `s_sra*`, `s_rotr*` | Shift or rotate by a count taken modulo 32. |
| Bit field and single bits | `s_bfe_*`, `s_bfm_b32`, `s_bitcmp*`, `s_bitset*` | Extract or build a field, test one bit, set or clear one bit. |
| Counts and reverse | `s_bcnt_*`, `s_clz_b32`, `s_ctz_b32`, `s_clo_b32`, `s_cto_b32`, `s_clb_b32`, `s_bfrev_b32` | Population count, leading and trailing runs, sign-bit count, reversal. |
| Movement and conversion | `s_mov_b32`, `s_mov_imm_b32`, `s_sext_*`, `s_select_b32`, `s_movindex_b32` | Copy, sign-extend, select on the condition, read an index word. |
| Comparison | `s_cmp*` | Equality and ordering, signed and unsigned, 32-bit and, for two rows, 64-bit. |

## Open questions

- Which predicate each of the 88 side-effecting rows writes to `SCC`.
- Whether `s_cmp` at `0x037` is a distinct operation: its text repeats
  `s_cmp_eq_i32`, it has no literal counterpart, and nothing else separates it.
- The value `s_movindex_b32` reads: which operand carries the index, what the
  window holds, and what an out-of-range index selects.
