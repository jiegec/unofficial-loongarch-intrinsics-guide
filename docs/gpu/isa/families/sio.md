# SIO and Export Family

This family moves data through the SIO rings and commits exports. The stream
interface is in [SIO](../../shaders/io.md); this chapter describes what the rows
share. The rows are in the [SIO index](../../instructions/sio-export.md).

## Forms

| Rows | Direction | Channel slot | Width |
|------|-----------|--------------|------:|
| `sioread`, `sioread_imm` | Stream to register | Form's own slot | 1 |
| `siordrot`, `siordrot_imm` | Stream to register | `VK8` or immediate | 1 |
| `siordrot_c4`, `siordrot_imm_c4` | Stream to register | `VK8` or immediate | 4 |
| `siowrite`, `siowrite_imm` | Register to stream | Form's own slot | 1 |
| `siowrrot`, `siowrrot_imm` | Register to stream | `VK8` or immediate | 1 |
| `siowrrot_c4`, `siowrrot_imm_c4` | Register to stream | `VK8` or immediate | 4 |
| `sioexport` | Commit a channel count | — | — |

The `_imm` rows carry the slot in `M.chan12`; the other rows carry it in `VK8`.
Every `_c4` row moves four channels through an aligned four-register tuple.

## Operands

| Field | Role |
|-------|------|
| `VJ8` | Data register: write source or single-channel read destination. |
| `VD8` | Destination of the `sioread` forms. |
| `VK8` | Channel slot, held in a register. |
| `M.chan12` | Channel slot, as an immediate. |
| `M.channum12` | Window size in channels. |
| `IA16`, `IK16`, `IJ16` | Window offset. |
| `M.sioid1` | Stream selector. |
| `VA8` | Ring channel index. |
| `PJ2` | Mask selector. |

## Export

`sioexport` commits a channel count from one stream to the stage output. The
count need not equal the window width. The rules are in
[SIO](../../shaders/io.md#export).

## Destination

The `_c4` identifier read is the one form whose destination is not the encoded
destination field: it writes the fixed sequence listed in
[SIO](../../shaders/io.md#reading). Every other `_c4` form writes the aligned
four-register tuple its destination field names.

## Open questions

- Whether an SIO transfer is gated by the mask selector.
- The rows other than `siowrrot`, `siowrrot_imm` and `siordrot_id_c4` are not
  exercised by the current compiler; their operand roles come from the field
  layout.
- The offset field's special payload class.
- SIO2.
- Why `sioread` and `sioread_imm` carry no register channel slot.
