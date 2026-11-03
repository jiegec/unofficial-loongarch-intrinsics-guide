# Instruction Encoding

An LG200 shader instruction is two little-endian 32-bit words. The low word
(`INST_LO`) is at the lower byte address.

## Instruction word

| Byte offset | Word | Bits |
|------------:|------|------|
| `+0x00` | `INST_LO` | 31..0 |
| `+0x04` | `INST_HI` | 31..0 |

Bit ranges are inclusive and written `[msb:lsb]`. A field split between the two
words is one operand: `GJ11` is `{INST_HI[2:0], INST_LO[7:0]}`, not two fields.

## Machine formats

A machine format fixes where the opcode lives and how the operand bits are
assigned. The mnemonic comes from the opcode field inside the row's match mask;
the format is a decoder grouping, not an instruction prefix.

| # | Format | Opcode field | Width | Rows | Principal operand positions |
|--:|--------|--------------|------:|-----:|----------------------------|
| 0 | `sop` | `INST_HI[9:0]` | 10 | 70 | `GJ8` LO[7:0], `RK8` LO[15:8], `RA8` LO[23:16], `RD8` LO[31:24] |
| 1 | `sop-imm-control` | `INST_HI[22:16]` | 7 | 62 | `IJ32` and `IK32` LO[31:0], `RJ8` HI[7:0], `RD8` HI[15:8] |
| 2 | `smem` | `INST_HI[26:16]` | 11 | 10 | `RJ8x2` or `RJ8x4` LO[7:0], `UO20` LO[27:8], `X0_1` LO[30], `GK8` HI[7:0], `RD8` HI[15:8] |
| 3 | `jump-stack` | `INST_HI[26:18]` | 9 | 2 | `RA8` LO[7:0], `X2_23` LO[30:8], `X0_1` LO[31], `X1_8` HI[15:8], `PJ2` HI[17:16] |
| 4 | `vop` | `INST_HI[27:14]` | 14 | 268 | `GJ11` LO[7:0] and HI[2:0], `GK11` LO[15:8] and HI[5:3], `GA11` LO[23:16] and HI[8:6], `VD8` or `RD8` LO[31:24], `M.mod2` HI[10:9], `M.clmp1` HI[11], `PJ2` HI[13:12] |
| 5 | `vop-imm` | `INST_HI[27:18]` | 10 | 107 | `IJ32` or `IK32` LO[31:0], `VJ8` HI[7:0], `VD8` or `RD8` HI[15:8], `PJ2` HI[17:16] |
| 6 | `tex-buffer` | `INST_HI[26:18]` | 9 | 137 | `RA8` or `RA8x4` LO[7:0], `RK8` or `RK8x4` LO[15:8] and HI[11], `VJ8x8` or `VQ8` LO[23:16], `VD8` LO[31:24], `X0_4` HI[3:0], `X1_2` HI[10:9], `M.offen1` HI[12], `M.idxen1` HI[13], `M.bl11` HI[14], `M.bl21` HI[15], `PJ2` HI[17:16] |
| 7 | `flat` | `INST_HI[27:18]` | 10 | 40 | `VA8` or `VJ8` LO[7:0], `RK8x2` LO[15:8], `M.bl11` LO[30], `M.bl21` LO[31], `VJ8x2` or `VK8x2` HI[7:0], `VD8` HI[15:8], `PJ2` HI[17:16] |
| 8 | `shared-memory` | `INST_HI[26:18]` | 9 | 52 | `VK8`, `VJ8`, `VA8` or `VD8` LO[31:0], `X0_16` HI[15:0], `PJ2` HI[17:16] |
| 9 | `sio-id` | `INST_HI[26:23]` | 4 | 11 | `VK8` or `VA8` LO[7:0] and LO[15:8], `IA16` LO[31:16], `M.channum12` HI[11:0], `VJ8` HI[19:12], `PJ2` HI[21:20], `M.sioid1` HI[22] |
| 10 | `sio-imm` | `INST_HI[26:23]` | 4 | 6 | `M.chan12` LO[11:0], `IK16` or `IJ16` LO[31:16], remainder as format 9 |
| 11 | `prefix` | `INST_HI[15:14]` | 2 | 3 | `IJ32` LO[31:0] |

The lists above are the format's principal fields, not an exhaustive inventory.
A row's own layout is the authority for which operands it encodes; the format
table gives the shape.

## Masks

Five masks describe each row. They are bit masks over the 64-bit instruction:

| Mask | Meaning |
|------|---------|
| `fixed` | Bits whose value is part of the row's identity. |
| `opcode` | The subset of `fixed` that selects the mnemonic. |
| `parameter` | Bits occupied by a named operand. |
| `ignored` | Bits the hardware does not use for this row. |
| `reserved` | Bits whose required value is not established. |

Four properties hold across the table:

1. The opcode mask is a subset of the fixed mask in every row.
2. Every row's base value satisfies `base & fixed == base`, and `base & parameter`
   is zero. The base word is a template, not a captured example.
3. The four masks `fixed`, `parameter`, `ignored` and `reserved` cover all 64
   bits.
4. `parameter`, `ignored` and `reserved` are pairwise disjoint.

A row that emits a reserved bit emits the row's base value for it. A row that
ignores a bit does not use it to infer an operand.

## Base word

Each row's base word is its encoding with every operand field zero. Operand
values are inserted at the row's field positions, and every other bit keeps the
base value. Because the four masks partition the word, an assembler can verify
its output by re-extracting the masks and comparing them with the row.

## Branches

A branch carries a 32-bit signed displacement in `INST_LO[31:0]`. Its unit is one
instruction (eight bytes), measured from the instruction that follows the branch:

```text
displacement = (target_address - branch_address - 8) / 8
```

A conditional branch carries its condition register in `INST_HI[7:0]`. The
condition is a lane-mask register or `SCC`.

## Open questions

- Whether the opcode fields of formats 9 and 10 carry structure beyond the
  mnemonic.
- The meaning of a reserved bit that a row's base value does not fix.
