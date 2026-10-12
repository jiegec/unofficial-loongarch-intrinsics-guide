# SIO and Export Family

The instruction index for this family is under [sio-export](../instructions/sio-export.md).

The stream interface itself — the streams, the channel layout, the descriptor a
write stages, and the ordering rules — is described in [SIO](../shader/sio.md).
This chapter describes what the rows of the family have in common.

## Forms

| Rows | Direction | Channel slot | Width |
|------|-----------|--------------|------:|
| `sioread`, `sioread_imm` | stream to register | none | 1 |
| `siordrot`, `siordrot_imm` | stream to register | `VK8` or immediate | 1 |
| `siordrot_c4`, `siordrot_imm_c4` | stream to register | `VK8` or immediate | 4 |
| `siowrite`, `siowrite_imm` | register to stream | `VK8` or immediate | 1 |
| `siowrrot`, `siowrrot_imm` | register to stream | `VK8` or immediate | 1 |
| `siowrrot_c4`, `siowrrot_imm_c4` | register to stream | `VK8` or immediate | 4 |
| `sioexport` | commit a channel count | — | — |

The `_imm` rows carry the channel slot in the `M.chan12` immediate field; the
other rows carry it in the `VK8` register.  Every `_c4` row moves four channels
through an aligned four-register tuple, and every other row moves one channel
through one register.

The direction is a property of the operation code, not of an operand: `siowrite`,
`siordrot` and `siowrrot` have identical operand signatures and differ only in
the opcode.  What separates the `read` and `rot` spellings is therefore
UNRESOLVED; only `siowrrot`, `siowrrot_imm` and `siordrot_id_c4` are emitted by
an established producer.

## Operands

| Field | Role |
|-------|------|
| `VJ8` | data register: the source of a write, or the destination of a single-channel read |
| `VD8` | destination of the `sioread` forms, which carry no register channel slot |
| `VK8` | channel slot, held in a register |
| `M.chan12` | channel slot, as a 12-bit immediate |
| `M.channum12` | window size in channels, printed `channum(n)`; twelve bits wide |
| `IA16`, `IK16`, `IJ16` | window offset: which output plane of the stream the transfer addresses |
| `M.sioid1` | stream selector, printed `sio:0` or `sio:1`; both are real streams, so a zero here is a choice and not an absence |
| `VA8` | ring channel index, held in a register (identifier form) |
| `PJ2` | predicate position; see UNRESOLVED |

Every row carries `M.channum12`, `M.sioid1` and `PJ2`; every row except
`sioexport` carries a window offset field.

## Export

`sioexport` commits a channel count from one stream to the stage output.  The
count must match what the stage produced, the header slot must be updated first
for a geometry stage, and a vertex stage fused into the geometry stage does not
export at all.  The rules and the two special shapes are in
[SIO](../shader/sio.md#export).

## Destination

The `_c4` identifier read is the one form whose destination is not the encoded
destination field: it writes the fixed sequence of registers that
[SIO](../shader/sio.md#reading-channels) lists, and the destination field is
ignored.  Every other `_c4` form writes the aligned four-register tuple its
destination field names.

## UNRESOLVED

* `PJ2` is named as a predicate in every row of the family, but no established
  producer predicates with it; per-lane gating is done with a lane mask.
* The rows other than `siowrrot`, `siowrrot_imm` and `siordrot_id_c4` are not
  emitted by any established producer, so their operand roles come from the
  field layout rather than from observation.
* What distinguishes the `read` and `rot` spellings of the read and write forms.
* Why `sioread` and `sioread_imm` carry no register channel slot while the other
  forms do, and which ring position they read.
* The offset field's classes beyond the ordinary window and the special payload
  class marked by the `0x20` bit.
* SIO2.
