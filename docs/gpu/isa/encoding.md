# Instruction Encoding

An LG200 shader is a sequence of 8-byte instructions. Each instruction is two
32-bit words and is encoded from one machine row. This chapter fixes the word
layout, the twelve machine formats, the five masks, the base word and the
branch arithmetic. Operand selectors are in [Operands](operands.md); the rows
are in the [instruction reference](reference.md).

## Instruction word

| Byte offset | Word | Bits |
|------------:|------|-----:|
| 0-3 | `INST_LO` | 31..0 |
| 4-7 | `INST_HI` | 31..0 |

The image stores 32-bit words in little-endian order: the word at the lower
address is `INST_LO`. An instruction occupies one 8-byte slot, and consecutive
instructions occupy consecutive slots. A branch displacement counts slots, so
a branch target is a slot address.

Bit ranges are inclusive and written `[msb:lsb]`. A field can cross the word
boundary, and the `INST_LO` range holds its low bits: `GJ11` is
`INST_HI[2:0]:INST_LO[7:0]`, with `INST_LO[7:0]` as bits 7..0 and
`INST_HI[2:0]` as bits 10..8. A layout lists the ranges in ascending order of
the operand's bits, and the first range is the least significant.

## Machine formats

A format fixes the opcode field's position and width, and the slots its rows
use. Each row belongs to one of twelve formats; no field in the word is a
format tag.

| # | Format | Opcode field | Width | Rows | Principal fields |
|--:|--------|--------------|------:|-----:|------------------|
| 0 | `sop` | `INST_HI[9:0]` | 10 | 70 | `GJ8` LO[7:0], `RK8` LO[15:8], `RA8` LO[23:16], `RD8` LO[31:24] |
| 1 | `sop-imm-control` | `INST_HI[22:16]` | 7 | 62 | `IK32` or `IJ32` LO[31:0], `RJ8` or `GJ8` HI[7:0], `RD8` HI[15:8], `IO32` LO[31:0], `X0_1` LO[0], `M.vm1` LO[1], `X1_2` LO[3:2] |
| 2 | `smem` | `INST_HI[26:16]` | 11 | 10 | `RJ8` LO[7:0], `UO20` LO[27:8], `X0_1` LO[30], `GK8` HI[7:0], `RD8` HI[15:8] |
| 3 | `jump-stack` | `INST_HI[26:18]` | 9 | 2 | `RA8` LO[7:0], `X2_23` LO[30:8], `X0_1` LO[31], `X1_8` HI[15:8], `PJ2` HI[17:16] |
| 4 | `vop` | `INST_HI[27:14]` | 14 | 268 | `GJ11` LO[7:0]+HI[2:0], `GK11` LO[15:8]+HI[5:3], `GA11` LO[23:16]+HI[8:6], `VD8` or `RD8` LO[31:24], `M.mod2` HI[10:9], `M.clmp1` HI[11], `PJ2` HI[13:12] |
| 5 | `vop-imm` | `INST_HI[27:18]` | 10 | 107 | `IK32` or `IJ32` LO[31:0], `VJ8` HI[7:0], `VD8` or `RD8` HI[15:8], `PJ2` HI[17:16] |
| 6 | `tex-buffer` | `INST_HI[26:18]` | 9 | 137 | `RA8` or `RK8` LO[7:0], `RJ8` or `RK8` LO[15:8], `RK9` or `RA8` LO[15:8]+HI[11], `VJ8`, `VQ8` or `VK8` LO[23:16], `VD8` or `VJ8` LO[31:24], `UO12` HI[11:0], `X0_4` HI[3:0], `X1_2` HI[10:9], `M.offen1` HI[12], `M.idxen1` HI[13], `M.bl11` HI[14], `M.bl21` HI[15], `PJ2` HI[17:16] |
| 7 | `flat` | `INST_HI[27:18]` | 10 | 40 | `VA8` or `VJ8` LO[7:0], `RK8` or `RA8` LO[15:8], `M.bl11` LO[30], `M.bl21` LO[31], `VJ8` or `VK8` HI[7:0], `VD8` HI[15:8], `PJ2` HI[17:16] |
| 8 | `shared-memory` | `INST_HI[26:18]` | 9 | 52 | `VK8` LO[7:0], `VJ8` LO[23:16], `VD8` LO[31:24], `X0_16` HI[15:0], `PJ2` HI[17:16] |
| 9 | `sio-id` | `INST_HI[26:23]` | 4 | 11 | `VK8` LO[7:0] or LO[15:8], `VA8` LO[7:0], `IA16` or `IK16` LO[31:16], `X0_12` (`M.channum12`) HI[11:0], `VJ8` HI[19:12], `PJ2` HI[21:20], `X1_1` (`M.sioid1`) HI[22] |
| 10 | `sio-imm` | `INST_HI[26:23]` | 4 | 6 | `X2_12` (`M.chan12`) LO[11:0], `IK16` or `IJ16` LO[31:16], then as `sio-id`: `X0_12` HI[11:0], `VJ8` HI[19:12], `PJ2` HI[21:20], `X1_1` HI[22] |
| 11 | `prefix` | `INST_HI[15:14]` | 2 | 3 | `IJ32` LO[31:0] |

The table gives a format's shape, not every row's layout: a row carries only
the fields its layout names, and a slot can appear in a few rows. A decoder
MUST read the row's layout before naming a field. The table mixes encoding slot
names with instruction-table operand names for the same bits: the branch
displacement is `X0_32` and `IO32`, the SIO window size `X0_12` and
`M.channum12`, the slot immediate `X2_12` and `M.chan12`, and the stream
selector `X1_1` and `M.sioid1`.

## Masks

Every row is one base word plus five 64-bit masks.

| Mask | Meaning | Encoding spelling |
|------|---------|-------------------|
| `fixed` | Bits whose value is part of the row's identity. | `fixed-mask` |
| `opcode` | Bits that carry the row's opcode. | `opcode-mask` |
| `parameter` | Bits the row encodes as operands. | `parameter-mask` |
| `ignored` | Bits in a field slot that are not operands of the row. | `print-ignored-mask` |
| `reserved` | Bits the row does not define; `fixed` wins if both mark a bit. | `undefined-mask` |
| base | The row's encoding with every operand zero. | `value` |

These properties hold:

1. The four masks cover all 64 bits.
2. `parameter`, `ignored` and `reserved` are pairwise disjoint.
3. `fixed` is disjoint from `parameter` and `ignored`.
4. A row can mark a bit both `fixed` and `reserved`; `fixed` then decides
   the match.
5. The base is confined to `fixed` and carries no `parameter` bit; an
   `ignored` bit of the base is zero.
6. Every `parameter` bit belongs to a named field. The converse fails: an
   atomic buffer row can name `VQ8` at `INST_LO[23:16]` while its `parameter`
   mask leaves those bits out.

A word matches a row when `(word & fixed) == base` in each 32-bit word. The
match uses the whole `fixed` mask, not the opcode field alone: ten of the 62
`sop-imm-control` rows fix `INST_HI[29]` to zero; the other 52 leave it free.
No two rows of one format match the same word, so (format, word) names at most
one mnemonic.

The format is not recoverable from the word alone: a base word can satisfy the
fixed mask of a row in another format. The zero-operand `v_ceil_f32` word
(`INST_HI = 0x10164000`, `INST_LO = 0`) also satisfies `v_mul_imm_u32`. A
disassembler not told the format MUST report the ambiguity. How the machine
resolves it is UNRESOLVED.

`ignored` and `reserved` bits take no part in the match, except where the row
also marks the bit `fixed`. The base holds zero in an `ignored` bit and in a
`reserved` bit that is not also `fixed`; but zero is not established as a
requirement, and a seed can carry a nonzero value. What the machine reads from
an `ignored` bit, and whether a `reserved` bit must be zero, is UNRESOLVED. An
encoder SHOULD write the base value.

## Base word

The base word is the row's encoding with every operand zero. Start from the
base, insert each operand in its field's `parameter` bits, keep every `fixed`
bit at the base value, and leave every bit that is neither `fixed` nor
`parameter` at zero. The result matches the row by construction. An encoder
MUST NOT change a fixed bit and MUST reject an operand that does not fit its
field.

`nop` has no operands, so its word is its base: `INST_HI = 0x40300000`,
`INST_LO = 0`. `cb` has base `INST_HI = 0x40270000`; selector 5 (`s5`) in
`INST_HI[7:0]` and displacement 2 in `INST_LO[31:0]` give
`INST_HI = 0x40270005`, `INST_LO = 0x00000002`. `b` has base
`INST_HI = 0x40260000`, `INST_LO = 0`.

A row's seed is a base with one legal operand set; its fixed bits equal the
base, and its other bits are an example.

## Branches

Three rows define the arithmetic: `b`, `cb` and `czb`, all in
`sop-imm-control`. The displacement is a signed count of 8-byte slots in
`INST_LO[31:0]`; a conditional row carries its condition selector in
`INST_HI[7:0]`.

| Row | Taken when | Displacement | Condition |
|-----|------------|--------------|-----------|
| `b` | always | `INST_LO[31:0]`, signed | none |
| `cb` | `INST_HI[7:0]` nonzero | `INST_LO[31:0]`, signed | `INST_HI[7:0]` |
| `czb` | `INST_HI[7:0]` zero | `INST_LO[31:0]`, signed | `INST_HI[7:0]` |

```text
target = branch_address + 8 + 8 * displacement
displacement = (target - branch_address - 8) / 8
```

The displacement is measured from the slot after the branch, so zero continues
at the next instruction. The field holds -2^31 through 2^31-1; a farther target
needs an intermediate branch.

The condition is an 8-bit general-source selector: `0x00..0x65` are
`s0..s101`, `0x7c..0x7f` are `p0..p3`, and `0x80` is `SCC`. A lane-mask
condition tests whether any lane is active, because an empty mask reads as
zero. [Operands](operands.md) has the rest.

Other control transfers are not relative branches:

| Row | Format | Fields | Transfer |
|-----|--------|--------|----------|
| `jirl` | `sop-imm-control` | source pair `INST_HI[7:0]`, return pair `INST_HI[15:8]` | pair plus the 32-bit field |
| `trap` | `sop-imm-control` | two pair selectors `INST_HI[15:0]` | to the encoded target |
| `rfe` | `sop-imm-control` | state pair `INST_HI[7:0]` | to the encoded target |
| `jpush`, `jpop` | `jump-stack` | `RA8` LO[7:0], `X2_23` LO[30:8], `X0_1` LO[31], `X1_8` HI[15:8], `PJ2` HI[17:16] | record and position |

The 32-bit field of `jirl`, `trap` and `rfe` is a signed displacement in the
row text, but its unit is not established for the first two: `jirl` adds it
to a register pair, and a `trap` target must be instruction-aligned. The
`rfe` text says it follows the branch rule. For `jpush` and `jpop`, `X2_23`
is the encoded position and `X0_1` and `X1_8` complete the record; the scaling
is not established. A producer MUST NOT reuse the `b` rule by assumption.

## Open questions

- How the machine selects a format when a word matches rows in more than one.
  The word carries no tag and the formats state no priority.
- What the machine reads from an `ignored` bit, and whether a `reserved` bit
  must be zero.
- The unit of the `jirl` and `trap` displacement, and the scaling of the
  `jpush` and `jpop` position.
- Whether an opcode field encodes anything below the row level; none is
  established for the 4-bit `sio-id` and `sio-imm` fields or the 2-bit
  `prefix` field.
- Whether `prefix` mode state reaches one instruction, persists, or composes.
  The three `prefix` rows differ only in the opcode field and share the
  32-bit literal.
