# Instruction Reference

The instruction set is 768 machine rows. A row is one mnemonic with one
encoding; another width, modifier or tuple is another row, and a suffix such as
`_imm` or `_c4` marks one. Seven family indexes list the rows, each row has its
own page there, and both key on the mnemonic. This chapter states what such a
page prints and what it leaves open.
[Encoding](encoding.md) covers the words and their masks, [Operands](operands.md)
the selector each field encodes, and each family chapter what its rows share.

## Reading a row

| Section | What it carries | Present |
|---------|-----------------|---------|
| Heading | The mnemonic as the assembler spells it, as a second-level heading of the index page. | always |
| `Synopsis` | `Instruction`, the assembly form; `Format`, the machine-format name such as `vop`; `Encoding`, the base word as `INST_LO` and `INST_HI`. | always |
| `Description` | Normative prose for the exact variant. | always |
| `Encoding and operands` | The bit strip of both words, its legend, the base encoding, the encoded fields. | always |
| `Operation` | Reference pseudocode. | when the row has pseudocode |
| `Latency` | Relative cost of one operation, not a cycle count. | only where the row has a cost entry |

Those three lines are all the `Synopsis` prints: a row's launch class and its
semantic-model label belong to the machine table and are not printed.

`Description` is normative, and an unsettled behaviour appears there as a
sentence containing `UNRESOLVED` rather than as a section of its own, so a row
without such a sentence has not thereby settled its boundaries. A sentence that
names a date, a path or one of a fixed vocabulary of provenance words is dropped
before the page is printed; if it carried an open question, the page prints
`UNRESOLVED: this property is not established.` in its place.

## Encoding and operands

Each word is drawn from bit 31 at the left, `INST_HI` above `INST_LO`. A span
is one field or one run of bits of a single class, with its bit range under the
label:

| Label | Meaning |
|-------|---------|
| field name | An encoded operand. The label is the name's leading capital letters, so `VD8` and `VD8x2` print `VD` and `GJ11` prints `GJ`; an `X` name keeps its index, so `X0_16` prints `X0`. A modifier drops the `M.` and abbreviates the stem: `M.dmask4` prints `DM`, `M.bl11` prints `B1`. |
| `F` | Fixed: the base word carries the required value. |
| `OP` | Opcode bit. |
| `R` | Reserved bit. |
| `PARAM` | A parameter position with no declared field. |
| `·` | Ignored bit. |

A bit is drawn in one class only. A named field wins over every mask, then
opcode, ignored, reserved, parameter and fixed; the fixed and opcode masks that
[Encoding](encoding.md) defines overlap in every row, so a bit that both claim
is drawn as `OP`.

Under the strip, `Base encoding` prints both words in hexadecimal, and the field
values are read from that word. Each active field then appears as a chip and in
a `Field / Encoded bits / Width` table, both carrying the field name, its bit
range and its width; the chip adds a role sentence keyed on the name prefix:
`PJ` a predicate selector, `V` a vector operand with `VD` as its destination,
`G` a general source, `R` a scalar operand or descriptor base with `RD` as a
result, `I` an immediate, `UO` an unsigned offset, `X` a field the row defines,
and `M.` a modifier. A name ending in `xN` prints as the base of an
`N`-register tuple.

The base word is not an identifier. `siordrot` and `v_pack_imm_snorm16_f32`
belong to different formats and print the same pair, `0x00000000` /
`0x01000000`; `siowrrot_id` and `v_cmp_imm_f_f16` print `0x00000000` /
`0x04800000`. Identify a row by its mnemonic and decode it through its format.

A declared field whose bits the row does not classify as parameter positions is
printed as inactive instead, under a note naming it. That note says the row
classifies the bits as ignored, and the statement is not always true: the
`VQ8` fields of the buffered atomics do lie in the ignored mask, while the
`M.bl11` field of the flat atomics lies on bits the same rows mark fixed and
opcode. Take the class of an inactive field from the row's masks, not from the
note. Three rows declare no field at all — `barrier`, `exit`, `nop` — and
print `No active operand fields in this row.`

## Families

| Family | Rows | Machine formats | Chapter | Index |
|--------|-----:|-----------------|---------|-------|
| Vector ALU | 374 | `vop` 265, `vop-imm` 107, `shared-memory` 2 | [Vector](families/vector.md) | [Index](../instructions/vector-alu.md) |
| Scalar ALU | 119 | `sop` 70, `sop-imm-control` 49 | [Scalar](families/scalar.md) | [Index](../instructions/scalar-alu.md) |
| Memory and image | 138 | `tex-buffer` 98, `shared-memory` 16, `flat` 14, `smem` 10 | [Memory](families/memory.md) | [Index](../instructions/memory-image.md) |
| Atomic | 97 | `tex-buffer` 39, `shared-memory` 32, `flat` 26 | [Atomic](families/atomic.md) | [Index](../instructions/atomic.md) |
| Control and ordering | 20 | `sop-imm-control` 13, `prefix` 3, `shared-memory` 2, `jump-stack` 2 | [Control](families/control.md) | [Index](../instructions/control-prefix.md) |
| SIO and export | 17 | `sio-id` 11, `sio-imm` 6 | [SIO](families/sio.md) | [Index](../instructions/sio-export.md) |
| Interpolation | 3 | `vop` 3 | [Interpolation](families/interpolation.md) | [Index](../instructions/interpolation.md) |

The seven indexes are disjoint and their entries sum to 768: every mnemonic
appears exactly once, in alphabetical order. A family is a documentation
grouping, not an encoding property; one format serves several families and one
family draws on several formats.

## Boundary rows

Four rows are the only rows of their families that use the `shared-memory`
format. The other 48 rows of that format serve the memory and atomic families:

| Row | Family | Encoded operands | Mechanism |
|-----|--------|------------------|-----------|
| `v_bperm_sm_b32` | Vector ALU | `VK8`, `VJ8`, `VD8`, `PJ2` | Byte selection formed through the workgroup shared-memory path. |
| `v_perm_sm_b32` | Vector ALU | `VK8`, `VJ8`, `VD8`, `PJ2` | Permutation through workgroup shared memory. |
| `copy_sm_from_global` | Control and ordering | `PJ2` | Block copy into workgroup shared memory. |
| `copy_sm_to_global` | Control and ordering | `PJ2` | Block copy out to global memory. |

The two permutes keep a vector-ALU page; the shared-memory path they use is
described in [Memory and image](families/memory.md). The two copies name no
address and no size: the extent, the addresses and the completion point come
from the launch, and `PJ2` is their only operand field. Their pages sit with
[Control and ordering](families/control.md). None of the four is listed under
the memory family.

## Open questions

- The fixed and opcode masks overlap in every row: 808 of the 1536 words carry
  a bit in both. Whether such a bit holds the value the base word shows or
  belongs to the opcode field is UNRESOLVED, so a decoder MUST NOT reject an
  encoding because that bit differs from the base word.
- 57 declared fields in 56 rows are printed as inactive, 40 of them the
  `M.bl11` field of a flat atomics row. Whether those bits are decoded at all
  is UNRESOLVED, and a decoder MUST NOT read the declared field as an encoded
  operand.
