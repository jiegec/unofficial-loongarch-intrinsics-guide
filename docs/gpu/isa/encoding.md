# Instruction Encoding

Every LG200 shader instruction is two little-endian 32-bit words.  The low word
comes first at the lower byte address.

```text
byte offset   +0x00                +0x04
              +-------------------+-------------------+
              |     INST_LO       |     INST_HI       |
              |    bits 31..0     |    bits 31..0     |
              +-------------------+-------------------+
```

Bit ranges are inclusive and written `[msb:lsb]`.  A field that is split
between the two words is still one operand: `GJ11` is
`{INST_HI[2:0], INST_LO[7:0]}`, not two fields.

## Machine formats

A *machine format* fixes where the opcode lives and how the operand bytes are
assigned.  The mnemonic comes from the opcode field inside the row's match mask;
the format is a decoder grouping, not an instruction prefix.

| # | Format | Opcode field | Width | Rows | Operand positions |
|--:|--------|--------------|------:|-----:|-------------------|
| 0 | `sop` | `INST_HI[9:0]` | 10 | 70 | `GJ8` LO[7:0], `RK8` LO[15:8], `RA8` LO[23:16], `RD8` LO[31:24] |
| 1 | `sop-imm-control` | `INST_HI[22:16]` | 7 | 62 | `IJ32`/`IK32` LO[31:0], `RJ8` HI[7:0], `RD8` HI[15:8] |
| 2 | `smem` | `INST_HI[26:16]` | 11 | 10 | `RJ8x2`/`RJ8x4` LO[7:0], `UO20` LO[27:8], `X0_1` LO[30], `GK8` HI[7:0], `RD8` HI[15:8] |
| 3 | `jump-stack` | `INST_HI[26:18]` | 9 | 2 | `RA8` LO[7:0], `X2_23` LO[30:8], `X0_1` LO[31], `X1_8` HI[15:8], `PJ2` HI[17:16] |
| 4 | `vop` | `INST_HI[27:14]` | 14 | 268 | `GJ11` LO[7:0]+HI[2:0], `GK11` LO[15:8]+HI[5:3], `GA11` LO[23:16]+HI[8:6], `VD8`/`RD8` LO[31:24], `M.mod2` HI[10:9], `M.clmp1` HI[11], `PJ2` HI[13:12] |
| 5 | `vop-imm` | `INST_HI[27:18]` | 10 | 107 | `IJ32`/`IK32` LO[31:0], `VJ8` HI[7:0], `VD8`/`RD8` HI[15:8], `PJ2` HI[17:16] |
| 6 | `tex-buffer` | `INST_HI[26:18]` | 9 | 137 | `RA8`/`RA8x4` LO[7:0], `RK8`/`RK8x4`/`RK9x8` LO[15:8]+HI[11], `VJ8x8`/`VQ8` LO[23:16], `VD8` LO[31:24], `X0_4` HI[3:0], `X1_2` HI[10:9], `M.offen1` HI[12], `M.idxen1` HI[13], `M.bl11` HI[14], `M.bl21` HI[15], `PJ2` HI[17:16] |
| 7 | `flat` | `INST_HI[27:18]` | 10 | 40 | `VA8`/`VJ8` LO[7:0], `RK8x2` LO[15:8], `M.bl11` LO[30], `M.bl21` LO[31], `VJ8x2`/`VK8x2` HI[7:0], `VD8` HI[15:8], `PJ2` HI[17:16] |
| 8 | `shared-memory` | `INST_HI[26:18]` | 9 | 52 | `VK8`/`VJ8`/`VA8`/`VD8` LO[31:0], `X0_16` HI[15:0], `PJ2` HI[17:16] |
| 9 | `sio-id` | `INST_HI[26:23]` | 4 | 11 | `VK8`/`VA8` LO[7:0] and LO[15:8], `IA16` LO[31:16], `M.channum12` HI[11:0], `VJ8` HI[19:12], `PJ2` HI[21:20], `M.sioid1` HI[22] |
| 10 | `sio-imm` | `INST_HI[26:23]` | 4 | 6 | `M.chan12` LO[11:0], `IK16`/`IJ16` LO[31:16], remainder as format 9 |
| 11 | `prefix` | `INST_HI[15:14]` | 2 | 3 | `IJ32` LO[31:0] |

Rows do not have to use every position a format offers.  The row's operand
layout is authoritative: a position that a row does not consume is classified by
its masks as fixed, parameter, ignored or reserved.

## Mask discipline

Five masks describe each row.  They are bit masks over the 64-bit instruction:

| Mask | Meaning |
|------|---------|
| `fixed` | bits whose value is part of the row's identity |
| `opcode` | the subset of `fixed` that selects the mnemonic |
| `parameter` | bits occupied by a named operand |
| `ignored` | bits the hardware does not use for this row and that a disassembler does not print |
| `reserved` | bits whose required value is not established |

Three properties hold across the table and a decoder may rely on them:

1. The opcode mask is a subset of the fixed mask in every row: an opcode bit is
   always also a bit whose value is part of the row.  For the rows whose
   encoding is established, the row's base value also satisfies
   `base & fixed == base`.
2. The four masks `fixed`, `parameter`, `ignored` and `reserved` cover all 64
   bits in every row: there is no unclassified bit.
3. `parameter`, `ignored` and `reserved` are pairwise disjoint in every row.

Two exceptions are worth knowing rather than assuming away:

* In 38 rows the `fixed` and `reserved` masks overlap, so a bit is both part of
  the row and of unestablished value.  The overlap occurs in the flat,
  tex-buffer, sio-id and one immediate row, not in the scalar and vector ALU
  formats.
* A few rows carry a seeded base word that is not a subset of its own fixed
  mask, because the stored word was captured with operands already in it rather
  than as a zero-operand template.  A decoder must therefore treat the base
  word as a captured example, not as a canonical template, for those rows.

A row that emits a reserved bit must emit the row's base value for it; a row
that ignores a bit does not use it to infer an operand.

## Encoding a row

The base words are the encodings of a canonical assembly form.  Operand values
are inserted at the row's field positions and every other bit keeps the base
value.  Because the four masks partition the word, an assembler can verify its
output by re-extracting the masks from the produced words and comparing them
with the row.

## Branches

Branches carry a 32-bit signed displacement in `INST_LO[31:0]` whose unit is
one instruction (eight bytes), measured from the instruction that *follows* the
branch:

```text
displacement = (target_address - branch_address - 8) / 8
```

A conditional branch additionally carries its condition register in
`INST_HI[7:0]`.  The condition register class is described in
[Operands](operands.md#conditional-execution).

## UNRESOLVED

* The semantics of the 2-bit `PJ2` field, which appears in most layouts but is
  not used as a predicate by any established producer.
* The meaning of reserved bits: they are emitted at the row's base value, but
  whether the hardware requires that value is not established.
* Whether the opcode fields of formats 9 and 10 (the 4-bit `INST_HI[26:23]`
  selector) carry any structure beyond the mnemonic.
