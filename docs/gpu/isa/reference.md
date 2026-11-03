# Instruction Reference

The instruction set has 768 machine rows in seven documentation families. This
chapter explains how to read a row and indexes the families. The encoding is in
[Encoding](encoding.md); the operands are in [Operands](operands.md).

## Reading a row

Each row page follows the same structure:

| Section | Contents |
|---------|----------|
| Synopsis | Mnemonic, assembly form, format, class and base encoding. |
| Description | What the exact variant computes. |
| Encoding and operands | The bit fields the row encodes, with a base word. |
| Operation | Reference pseudocode for the row. |
| UNRESOLVED | Behaviour that the row does not establish. |

A row describes its **exact variant**. Another operand form, width, modifier or
tuple of the same mnemonic is a different question. The family chapter describes
what the rows share; the row page is the authority for its own fields.

## Families

| Family | Rows | Machine formats | Chapter | Index |
|--------|-----:|-----------------|---------|-------|
| Vector ALU | 374 | `vop`, `vop-imm`, `shared-memory` | [Vector](families/vector.md) | [Index](../instructions/vector-alu.md) |
| Scalar ALU | 119 | `sop`, `sop-imm-control` | [Scalar](families/scalar.md) | [Index](../instructions/scalar-alu.md) |
| Memory and image | 138 | `smem`, `tex-buffer`, `flat`, `shared-memory` | [Memory](families/memory.md) | [Index](../instructions/memory-image.md) |
| Atomic | 97 | `tex-buffer`, `flat`, `shared-memory` | [Atomic](families/atomic.md) | [Index](../instructions/atomic.md) |
| Control and ordering | 20 | `sop-imm-control`, `jump-stack`, `shared-memory`, `prefix` | [Control](families/control.md) | [Index](../instructions/control-prefix.md) |
| SIO and export | 17 | `sio-id`, `sio-imm` | [SIO](families/sio.md) | [Index](../instructions/sio-export.md) |
| Interpolation | 3 | `vop` | [Interpolation](families/interpolation.md) | [Index](../instructions/interpolation.md) |

A family is a documentation grouping, not an encoding property. A family may
draw on several machine formats, and a format may serve several families.

## Boundary rows

Four rows sit across the family boundary and are documented in the chapter that
describes their mechanism:

| Row | Mechanism |
|-----|-----------|
| `v_bperm_sm_b32` | Permute over the workgroup allocation. |
| `v_perm_sm_b32` | Permute over the workgroup allocation. |
| `copy_sm_from_global` | Block copy into shared memory. |
| `copy_sm_to_global` | Block copy out of shared memory. |

## Search

A row can be found by its mnemonic in the family index or by the page's search
function. The mnemonic is the exact spelling used by the assembler; a suffix
such as `_imm`, `_c4` or `_b32` selects a distinct row.

## Open questions

- The meaning of the opcode fields of formats 9 and 10 beyond the mnemonic.
- The complete set of rows that share a single encoding with a modifier.
