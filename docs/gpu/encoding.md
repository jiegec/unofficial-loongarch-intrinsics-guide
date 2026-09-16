# Instruction Encoding

## Scope and byte order

This chapter defines the 64-bit LG200 shader instruction word. Host command
packets, full-state records, and BO handles are different formats; their
encoding is defined in [Command and Launch](host-interface.md).

One instruction is two little-endian 32-bit words. The low word is at the lower
byte address and the high word follows it.

```text
byte offset       +0x00                         +0x04
                 +------------------------------+------------------------------+
                 |          INST_LO             |          INST_HI             |
                 |       bits 31 ........ 0     |       bits 31 ........ 0     |
                 +------------------------------+------------------------------+
```

Bit ranges in the diagrams below are inclusive and written `[msb:lsb]`. A
field shown as `GJ11` is the complete selector, even when its bits are split
between the two words.

## Family index

`format_id` is a catalog identifier. It is not an instruction prefix. The
decoder first applies a row's fixed mask, then uses the family opcode field to
select the mnemonic. The row's `operand_layout` remains authoritative when a
form omits one of the fields shown in a family overview.

| Group | ID | Family | Rows | Opcode field | Principal role |
|-------|---:|--------|-----:|--------------|----------------|
| Scalar | 0 | `sop` | 70 | `HI[9:0]` | Scalar ALU and scalar compare |
| Scalar/control | 1 | `sop-imm-control` | 62 | `HI[22:16]` | Scalar immediate, branch, call, trap, and control |
| Scalar memory | 2 | `smem` | 10 | `HI[26:16]` | Scalar descriptor and flat loads |
| Control | 3 | `jump-stack` | 2 | `HI[26:18]` | Jump-stack operations |
| Vector | 4 | `vop` | 268 | `HI[27:14]` | General vector ALU and vector compare |
| Vector immediate | 5 | `vop-imm` | 107 | `HI[27:18]` | Vector operation with a 32-bit literal |
| Resource memory | 6 | `tex-buffer` | 137 | `HI[26:18]` | Buffer, image, sampler, and resource atomics |
| Flat memory | 7 | `flat` | 40 | `HI[27:18]` | Flat/global loads, stores, and atomics |
| Shared memory | 8 | `shared-memory` | 52 | `HI[26:18]` | Workgroup shared memory and interpolation |
| Shader I/O | 9 | `sio-id` | 11 | `HI[26:23]` | SIO register/identifier forms |
| Shader I/O | 10 | `sio-imm` | 6 | `HI[26:23]` | SIO immediate channel forms |
| Prefix | 11 | `prefix` | 3 | `HI[15:14]` | Prefix state operations |

<div class="encoding-legend">
<span class="legend-chip field-fixed">fixed / reserved</span>
<span class="legend-chip field-opcode">opcode</span>
<span class="legend-chip field-r">scalar register</span>
<span class="legend-chip field-v">vector register</span>
<span class="legend-chip field-g">general selector</span>
<span class="legend-chip field-i">immediate</span>
<span class="legend-chip field-p">predicate</span>
<span class="legend-chip field-m">modifier</span>
<span class="legend-chip field-x">row-defined / unresolved</span>
</div>

The next sections give one bit diagram and one operand explanation for every
family. They are family maps, not replacement rows for the machine-readable
TSV: tuple width, destination class, and reserved bits vary by mnemonic.

## Scalar `sop`

The scalar ALU family places up to three 8-bit source/destination positions in
`INST_LO`. The common forms are shown in the same high-to-low order used by
instruction pages. Flex widths are proportional to bit width.

<div class="encoding-compact">
<div class="encoding-row"><div class="encoding-word-tag">INST_HI</div>
<span class="encoding-field field-fixed" style="flex:22 22 0"><b>F</b><small>31:10</small></span>
<span class="encoding-field field-opcode" style="flex:10 10 0"><b>OP</b><small>9:0</small></span>
</div>
<div class="encoding-row"><div class="encoding-word-tag">INST_LO</div>
<span class="encoding-field field-r" style="flex:8 8 0"><b>RD</b><small>31:24</small></span>
<span class="encoding-field field-g" style="flex:8 8 0"><b>RA</b><small>23:16</small></span>
<span class="encoding-field field-g" style="flex:8 8 0"><b>RK</b><small>15:8</small></span>
<span class="encoding-field field-g" style="flex:8 8 0"><b>GJ</b><small>7:0</small></span>
</div>
</div>

`GJ8`, `RK8`, and `RA8` are general-source selectors in the scalar rule
classes. `RD8` is a scalar destination. The one-source and two-source forms
simply leave `RA8` or `RK8` fixed/unused; compare forms may use `SCC` in the
destination according to their row rule.

## Scalar immediate/control `sop-imm-control`

There are three distinct layouts in this family. They must not be collapsed
into one generic “immediate” shape: the ALU form pairs a scalar register
result with a 32-bit literal in the low word, the control form carries a
register/control payload with the literal position row-defined, and the flag
form carries explicit control bits in the low word. Each form uses its own
row shape. The field colors match the instruction pages: green is
scalar/immediate, gray is fixed/opcode, and red is an explicit control bit.

<div class="encoding-compact">
<div class="encoding-row"><div class="encoding-word-tag">ALU HI</div>
<span class="encoding-field field-fixed" style="flex:9 9 0"><b>F</b><small>31:23</small></span>
<span class="encoding-field field-opcode" style="flex:7 7 0"><b>OP</b><small>22:16</small></span>
<span class="encoding-field field-r" style="flex:8 8 0"><b>RD</b><small>15:8</small></span>
<span class="encoding-field field-g" style="flex:8 8 0"><b>RJ</b><small>7:0</small></span>
</div>
<div class="encoding-row"><div class="encoding-word-tag">ALU LO</div>
<span class="encoding-field field-i" style="flex:32 32 0"><b>IJ/IK</b><small>31:0</small></span>
</div>
</div>

<div class="encoding-compact">
<div class="encoding-row"><div class="encoding-word-tag">CTRL HI</div>
<span class="encoding-field field-fixed" style="flex:9 9 0"><b>F</b><small>31:23</small></span>
<span class="encoding-field field-opcode" style="flex:7 7 0"><b>OP</b><small>22:16</small></span>
<span class="encoding-field field-parameter" style="flex:8 8 0"><b>RD/RJ</b><small>15:8</small></span>
<span class="encoding-field field-parameter" style="flex:8 8 0"><b>RJ/GJ</b><small>7:0</small></span>
</div>
<div class="encoding-row"><div class="encoding-word-tag">CTRL LO</div>
<span class="encoding-field field-x" style="flex:32 32 0"><b>X0</b><small>31:0</small></span>
</div>
</div>

<div class="encoding-compact">
<div class="encoding-row"><div class="encoding-word-tag">FLAG HI</div>
<span class="encoding-field field-fixed" style="flex:9 9 0"><b>F</b><small>31:23</small></span>
<span class="encoding-field field-opcode" style="flex:7 7 0"><b>OP</b><small>22:16</small></span>
<span class="encoding-field field-fixed" style="flex:16 16 0"><b>F</b><small>15:0</small></span>
</div>
<div class="encoding-row"><div class="encoding-word-tag">FLAG LO</div>
<span class="encoding-field field-fixed" style="flex:28 28 0"><b>F</b><small>31:4</small></span>
<span class="encoding-field field-x" style="flex:2 2 0"><b>X1</b><small>3:2</small></span>
<span class="encoding-field field-x" style="flex:1 1 0"><b>VM</b><small>1</small></span>
<span class="encoding-field field-x" style="flex:1 1 0"><b>X0</b><small>0</small></span>
</div>
</div>

All bits not listed in a row are fixed, reserved, or ignored according to its
catalog mask. This avoids implying that `X0_1` and `X0_32` coexist in one word.

`X0_40` in a logical signature does not mean that every control row has a
40-bit displacement. In the current `jirl`/`trap` layouts, `X0_32` occupies
`INST_LO[31:0]` and `RJ8x2` occupies `INST_HI[7:0]`; those eight high bits are a
register selector, not displacement bits.

## Scalar memory `smem`

The scalar-memory forms share one word shape.  The descriptor width and result
tuple width are selected by the instruction row; they do not change the bit
positions.

<div class="encoding-compact">
<div class="encoding-row"><div class="encoding-word-tag">INST_HI</div>
<span class="encoding-field field-fixed" style="flex:5 5 0"><b>F</b><small>31:27</small></span>
<span class="encoding-field field-opcode" style="flex:11 11 0"><b>OP</b><small>26:16</small></span>
<span class="encoding-field field-r" style="flex:8 8 0"><b>RD</b><small>15:8</small></span>
<span class="encoding-field field-g" style="flex:8 8 0"><b>GK</b><small>7:0</small></span>
</div>
<div class="encoding-row"><div class="encoding-word-tag">INST_LO</div>
<span class="encoding-field field-fixed" style="flex:1 1 0"><b>F</b><small>31</small></span>
<span class="encoding-field field-x" style="flex:1 1 0"><b>X0</b><small>30</small></span>
<span class="encoding-field field-fixed" style="flex:2 2 0"><b>F</b><small>29:28</small></span>
<span class="encoding-field field-u" style="flex:20 20 0"><b>UO20</b><small>27:8</small></span>
<span class="encoding-field field-r" style="flex:8 8 0"><b>RJ</b><small>7:0</small></span>
</div>
</div>

`RJ8x2` is a 64-bit scalar address; `RJ8x4` is a 16-byte buffer descriptor.
`GK8` uses the 8-bit general-source table, and `RD8x1/2/4/8/16` selects the
number of scalar result registers.

## Jump-stack `jump-stack`

The jump-stack rows use a displacement/control payload in `INST_LO` and a
predicate/control payload in `INST_HI`.

<div class="encoding-compact">
<div class="encoding-row"><div class="encoding-word-tag">INST_HI</div>
<span class="encoding-field field-fixed" style="flex:5 5 0"><b>F</b><small>31:27</small></span>
<span class="encoding-field field-opcode" style="flex:9 9 0"><b>OP</b><small>26:18</small></span>
<span class="encoding-field field-p" style="flex:2 2 0"><b>PJ</b><small>17:16</small></span>
<span class="encoding-field field-x" style="flex:8 8 0"><b>X1</b><small>15:8</small></span>
<span class="encoding-field field-fixed" style="flex:8 8 0"><b>F</b><small>7:0</small></span>
</div>
<div class="encoding-row"><div class="encoding-word-tag">INST_LO</div>
<span class="encoding-field field-x" style="flex:1 1 0"><b>X0</b><small>31</small></span>
<span class="encoding-field field-x" style="flex:23 23 0"><b>X2</b><small>30:8</small></span>
<span class="encoding-field field-r" style="flex:8 8 0"><b>RA</b><small>7:0</small></span>
</div>
</div>

`RA8` is the scalar stack/control source selected by the row. `PJ2` controls
lane participation; the displacement is a 23-bit unsigned field in the
machine encoding and is interpreted by the individual jump-stack operation.

## Vector ALU `vop`

The three-source form has three split 11-bit general selectors.  The high
three bits of each selector are visible in `INST_HI`; they are not padding.

<div class="encoding-compact">
<div class="encoding-row"><div class="encoding-word-tag">INST_HI</div>
<span class="encoding-field field-fixed" style="flex:4 4 0"><b>F</b><small>31:28</small></span>
<span class="encoding-field field-opcode" style="flex:14 14 0"><b>OP</b><small>27:14</small></span>
<span class="encoding-field field-p" style="flex:2 2 0"><b>PJ</b><small>13:12</small></span>
<span class="encoding-field field-m" style="flex:1 1 0"><b>CL</b><small>11</small></span>
<span class="encoding-field field-m" style="flex:2 2 0"><b>MOD</b><small>10:9</small></span>
<span class="encoding-field field-g" style="flex:3 3 0"><b>GA[10:8]</b><small>8:6</small></span>
<span class="encoding-field field-g" style="flex:3 3 0"><b>GK[10:8]</b><small>5:3</small></span>
<span class="encoding-field field-g" style="flex:3 3 0"><b>GJ[10:8]</b><small>2:0</small></span>
</div>
<div class="encoding-row"><div class="encoding-word-tag">INST_LO</div>
<span class="encoding-field field-parameter" style="flex:8 8 0"><b>VD/RD</b><small>31:24</small></span>
<span class="encoding-field field-g" style="flex:8 8 0"><b>GA[7:0]</b><small>23:16</small></span>
<span class="encoding-field field-g" style="flex:8 8 0"><b>GK[7:0]</b><small>15:8</small></span>
<span class="encoding-field field-g" style="flex:8 8 0"><b>GJ[7:0]</b><small>7:0</small></span>
</div>
</div>

Rows with fewer sources fix or reuse the corresponding selector positions;
rows with a scalar result use `RD8` in the destination position.  A carry or
tuple row may use `VD8x2`, `GK11x2`, or `GA11x2` in the same byte positions.
The row's `operand_layout` is authoritative.

One- and two-source rows omit `GA11` or `GK11`; carry forms replace a source
position with a VCC field; compare and vector-to-scalar rows use `RD8` as a
scalar/VCC result position. `M.mod2` and `M.clmp1` are independent operation
modifiers, not part of the G selector.

## Vector immediate `vop-imm`

The 32-bit literal occupies the complete low word.  The high word contains the
operation and the vector source/destination selectors.

<div class="encoding-compact">
<div class="encoding-row"><div class="encoding-word-tag">INST_HI</div>
<span class="encoding-field field-fixed" style="flex:4 4 0"><b>F</b><small>31:28</small></span>
<span class="encoding-field field-opcode" style="flex:10 10 0"><b>OP</b><small>27:18</small></span>
<span class="encoding-field field-p" style="flex:2 2 0"><b>PJ</b><small>17:16</small></span>
<span class="encoding-field field-parameter" style="flex:8 8 0"><b>VD/RD</b><small>15:8</small></span>
<span class="encoding-field field-v" style="flex:8 8 0"><b>VJ</b><small>7:0</small></span>
</div>
<div class="encoding-row"><div class="encoding-word-tag">INST_LO</div>
<span class="encoding-field field-i" style="flex:32 32 0"><b>IJ/IK32</b><small>31:0</small></span>
</div>
</div>

`VJ8` is a vector source; `VD8` is a vector destination and `RD8` is the
scalar-result alternative. The literal is a raw 32-bit bit pattern, not a
sign-extended selector.

## Resource memory `tex-buffer`

This family contains two materially different packet shapes. Buffer forms use
an offset/index tuple; image and sampler forms use descriptor tuples.

### Buffer form

<div class="encoding-compact">
<div class="encoding-row"><div class="encoding-word-tag">INST_HI</div>
<span class="encoding-field field-fixed" style="flex:5 5 0"><b>F</b><small>31:27</small></span>
<span class="encoding-field field-opcode" style="flex:9 9 0"><b>OP</b><small>26:18</small></span>
<span class="encoding-field field-p" style="flex:2 2 0"><b>PJ</b><small>17:16</small></span>
<span class="encoding-field field-m" style="flex:1 1 0"><b>BL2</b><small>15</small></span>
<span class="encoding-field field-m" style="flex:1 1 0"><b>BL1</b><small>14</small></span>
<span class="encoding-field field-m" style="flex:1 1 0"><b>IDX</b><small>13</small></span>
<span class="encoding-field field-m" style="flex:1 1 0"><b>OFF</b><small>12</small></span>
<span class="encoding-field field-u" style="flex:12 12 0"><b>UO12</b><small>11:0</small></span>
</div>
<div class="encoding-row"><div class="encoding-word-tag">INST_LO</div>
<span class="encoding-field field-v" style="flex:8 8 0"><b>VD/N</b><small>31:24</small></span>
<span class="encoding-field field-v" style="flex:8 8 0"><b>VQ</b><small>23:16</small></span>
<span class="encoding-field field-r" style="flex:8 8 0"><b>RJ/RK</b><small>15:8</small></span>
<span class="encoding-field field-r" style="flex:8 8 0"><b>RK/RA</b><small>7:0</small></span>
</div>
</div>

The descriptor is `RJ8x4` for scalar-buffer rows and `RK8x4` for vector-buffer
rows.  `VQ8` is present only in indexed rows; the same byte is row-defined when
the index operand is absent.

### Image/sampler form

<div class="encoding-compact">
<div class="encoding-row"><div class="encoding-word-tag">INST_HI</div>
<span class="encoding-field field-fixed" style="flex:5 5 0"><b>F</b><small>31:27</small></span>
<span class="encoding-field field-opcode" style="flex:9 9 0"><b>OP</b><small>26:18</small></span>
<span class="encoding-field field-p" style="flex:2 2 0"><b>PJ</b><small>17:16</small></span>
<span class="encoding-field field-m" style="flex:1 1 0"><b>BL2</b><small>15</small></span>
<span class="encoding-field field-m" style="flex:1 1 0"><b>BL1</b><small>14</small></span>
<span class="encoding-field field-fixed" style="flex:2 2 0"><b>F</b><small>13:12</small></span>
<span class="encoding-field field-m" style="flex:1 1 0"><b>RSZ/RK[8]</b><small>11</small></span>
<span class="encoding-field field-m" style="flex:1 1 0"><b>UN</b><small>10</small></span>
<span class="encoding-field field-m" style="flex:1 1 0"><b>NA</b><small>9</small></span>
<span class="encoding-field field-fixed" style="flex:5 5 0"><b>F</b><small>8:4</small></span>
<span class="encoding-field field-m" style="flex:4 4 0"><b>DMASK</b><small>3:0</small></span>
</div>
<div class="encoding-row"><div class="encoding-word-tag">INST_LO</div>
<span class="encoding-field field-v" style="flex:8 8 0"><b>VD/VJ</b><small>31:24</small></span>
<span class="encoding-field field-v" style="flex:8 8 0"><b>VJ/VK</b><small>23:16</small></span>
<span class="encoding-field field-r" style="flex:8 8 0"><b>RK/RA</b><small>15:8</small></span>
<span class="encoding-field field-r" style="flex:8 8 0"><b>RA/F</b><small>7:0</small></span>
</div>
</div>

Some image rows use `INST_HI[11]` as the ninth bit of an `RK9x8` image tuple
instead of `M.rsz1`. The row's `operand_layout` is authoritative for that
variant; the bit is not simultaneously a mode bit and a tuple bit.

`RJ8x4/RK8x4/RA8x4` names a four-SGPR descriptor. `RK8x8/RA8x8` names an
eight-SGPR image or sampler tuple. `VQ8` is the vector address/index position;
`UO12`, `offen`, and `idxen` jointly define buffer offset formation. `dmask`
selects returned channels; it is not a destination-register count.

## Flat memory `flat`

Load, store, and atomic rows reuse the same fixed positions but assign the
operand bytes differently.  The family map below shows every bit, including
the fixed low-word gap and the high-word prefix.

<div class="encoding-compact">
<div class="encoding-row"><div class="encoding-word-tag">INST_HI</div>
<span class="encoding-field field-fixed" style="flex:4 4 0"><b>F</b><small>31:28</small></span>
<span class="encoding-field field-opcode" style="flex:10 10 0"><b>OP</b><small>27:18</small></span>
<span class="encoding-field field-p" style="flex:2 2 0"><b>PJ</b><small>17:16</small></span>
<span class="encoding-field field-parameter" style="flex:8 8 0"><b>VD/F</b><small>15:8</small></span>
<span class="encoding-field field-v" style="flex:8 8 0"><b>VJ/VK</b><small>7:0</small></span>
</div>
<div class="encoding-row"><div class="encoding-word-tag">INST_LO</div>
<span class="encoding-field field-m" style="flex:1 1 0"><b>BL2</b><small>31</small></span>
<span class="encoding-field field-m" style="flex:1 1 0"><b>BL1</b><small>30</small></span>
<span class="encoding-field field-fixed" style="flex:14 14 0"><b>F</b><small>29:16</small></span>
<span class="encoding-field field-r" style="flex:8 8 0"><b>RK/RA</b><small>15:8</small></span>
<span class="encoding-field field-v" style="flex:8 8 0"><b>VA/VJ</b><small>7:0</small></span>
</div>
</div>

Load forms use `VJ8x2` as the flat address and `RK8x2` as the scalar address
pair. Store forms use `VJ8` data, `VK8x2` vector address, and `RA8x2` scalar
address. Atomic forms add `VA8` as the compare or RMW-data position. `bl1` and
`bl2` are memory behavior controls and remain in the word even when their
canonical value is zero.

## Shared memory `shared-memory`

The shared-memory family has a memory tuple form and a scalar interpolation
form.  Rows that do not consume a byte shown as an operand encode that byte as
the row's fixed value.

### Memory tuple form

<div class="encoding-compact">
<div class="encoding-row"><div class="encoding-word-tag">INST_HI</div>
<span class="encoding-field field-fixed" style="flex:5 5 0"><b>F</b><small>31:27</small></span>
<span class="encoding-field field-opcode" style="flex:9 9 0"><b>OP</b><small>26:18</small></span>
<span class="encoding-field field-p" style="flex:2 2 0"><b>PJ</b><small>17:16</small></span>
<span class="encoding-field field-x" style="flex:16 16 0"><b>X0_16/F</b><small>15:0</small></span>
</div>
<div class="encoding-row"><div class="encoding-word-tag">INST_LO</div>
<span class="encoding-field field-v" style="flex:8 8 0"><b>VD</b><small>31:24</small></span>
<span class="encoding-field field-v" style="flex:8 8 0"><b>VJ</b><small>23:16</small></span>
<span class="encoding-field field-v" style="flex:8 8 0"><b>VK/F</b><small>15:8</small></span>
<span class="encoding-field field-v" style="flex:8 8 0"><b>VA/VK/F</b><small>7:0</small></span>
</div>
</div>

### Interpolation form

<div class="encoding-compact">
<div class="encoding-row"><div class="encoding-word-tag">INST_HI</div>
<span class="encoding-field field-fixed" style="flex:5 5 0"><b>F</b><small>31:27</small></span>
<span class="encoding-field field-opcode" style="flex:9 9 0"><b>OP</b><small>26:18</small></span>
<span class="encoding-field field-p" style="flex:2 2 0"><b>PJ</b><small>17:16</small></span>
<span class="encoding-field field-fixed" style="flex:8 8 0"><b>F</b><small>15:8</small></span>
<span class="encoding-field field-u" style="flex:8 8 0"><b>UO8</b><small>7:0</small></span>
</div>
<div class="encoding-row"><div class="encoding-word-tag">INST_LO</div>
<span class="encoding-field field-v" style="flex:8 8 0"><b>VD</b><small>31:24</small></span>
<span class="encoding-field field-r" style="flex:8 8 0"><b>RJ</b><small>23:16</small></span>
<span class="encoding-field field-fixed" style="flex:16 16 0"><b>F</b><small>15:0</small></span>
</div>
</div>

`X0_16` is a shared-memory byte/element offset in load/store/atomic rows. The
interpolation form instead uses `RJ8`, `VD8`, and `UO8`; matrix forms have only
`PJ2` in the open row layout and do not acquire invented operands.

## Shader I/O `sio-id`

The identifier form carries either a 16-bit SIO identifier/immediate or a
16-bit row-defined field in the upper low-word half.  The low byte is a second
vector selector only for rows that need it.

<div class="encoding-compact">
<div class="encoding-row"><div class="encoding-word-tag">INST_HI</div>
<span class="encoding-field field-fixed" style="flex:5 5 0"><b>F</b><small>31:27</small></span>
<span class="encoding-field field-opcode" style="flex:4 4 0"><b>OP</b><small>26:23</small></span>
<span class="encoding-field field-x" style="flex:1 1 0"><b>X1</b><small>22</small></span>
<span class="encoding-field field-p" style="flex:2 2 0"><b>PJ</b><small>21:20</small></span>
<span class="encoding-field field-v" style="flex:8 8 0"><b>VJ/VD</b><small>19:12</small></span>
<span class="encoding-field field-x" style="flex:12 12 0"><b>X0_12</b><small>11:0</small></span>
</div>
<div class="encoding-row"><div class="encoding-word-tag">INST_LO</div>
<span class="encoding-field field-i" style="flex:16 16 0"><b>IA/IK/X2</b><small>31:16</small></span>
<span class="encoding-field field-v" style="flex:8 8 0"><b>VA/VK/F</b><small>15:8</small></span>
<span class="encoding-field field-v" style="flex:8 8 0"><b>VK/VJ/F</b><small>7:0</small></span>
</div>
</div>

`X0_12` is the channel count/number field, `IA16/IK16/X2_16` is the SIO
identifier or offset field, and `X1_1` selects the row's SIO mode. `VJ8x4` is a
four-register payload tuple; its stage-specific placement is defined in the
[Shader ABI](shader-abi.md).

## Shader I/O `sio-imm`

The immediate form reserves `INST_LO[15:12]`; the channel field occupies only
`INST_LO[11:0]`.  The high word follows the same opcode/predicate/payload
partition as `sio-id`.

<div class="encoding-compact">
<div class="encoding-row"><div class="encoding-word-tag">INST_HI</div>
<span class="encoding-field field-fixed" style="flex:5 5 0"><b>F</b><small>31:27</small></span>
<span class="encoding-field field-opcode" style="flex:4 4 0"><b>OP</b><small>26:23</small></span>
<span class="encoding-field field-x" style="flex:1 1 0"><b>X1/SIO</b><small>22</small></span>
<span class="encoding-field field-p" style="flex:2 2 0"><b>PJ</b><small>21:20</small></span>
<span class="encoding-field field-v" style="flex:8 8 0"><b>VJ/VD</b><small>19:12</small></span>
<span class="encoding-field field-x" style="flex:12 12 0"><b>X0_12/CHAN</b><small>11:0</small></span>
</div>
<div class="encoding-row"><div class="encoding-word-tag">INST_LO</div>
<span class="encoding-field field-i" style="flex:16 16 0"><b>IK/IJ16</b><small>31:16</small></span>
<span class="encoding-field field-fixed" style="flex:4 4 0"><b>F</b><small>15:12</small></span>
<span class="encoding-field field-x" style="flex:12 12 0"><b>X2/CHAN</b><small>11:0</small></span>
</div>
</div>

Immediate SIO rows use `IK16` or `IJ16` for the data/offset literal and
`X2_12`/`M.chan12` for the channel selector. The mnemonic chooses whether the
source or destination field is `VJ8` or `VD8`.

## Prefix `prefix`

The prefix family has one literal shape.  Only `INST_HI[15:14]` selects the
prefix operation; every other high-word bit is fixed or reserved by the row.

<div class="encoding-compact">
<div class="encoding-row"><div class="encoding-word-tag">INST_HI</div>
<span class="encoding-field field-fixed" style="flex:16 16 0"><b>F</b><small>31:16</small></span>
<span class="encoding-field field-opcode" style="flex:2 2 0"><b>OP</b><small>15:14</small></span>
<span class="encoding-field field-fixed" style="flex:14 14 0"><b>F</b><small>13:0</small></span>
</div>
<div class="encoding-row"><div class="encoding-word-tag">INST_LO</div>
<span class="encoding-field field-i" style="flex:32 32 0"><b>IJ32</b><small>31:0</small></span>
</div>
</div>

The prefix literal is consumed by the following prefix-defined operation. It
does not turn a host packet or a second 64-bit word into a shader instruction.

## Selector fields: GJ8 and GJ11

### The complete 8-bit base selector

`GJ8`, `GK8`, and the other `G*8` source fields carry only the low eight bits
of the selector.  In the current LG200 codec, these fields name an SGPR,
`zero`, or one of the defined inline constants.  They do not select a VGPR;
VGPR sources use the high-bit form described below.

| Selector | Meaning |
|----------|---------|
| `0x00..0x65` | `s0..s101` (ordinary scalar registers) |
| `0x66` | `zero` |
| `0x67` | special island; reads `0` |
| `0x68..0x77` | special island; reads `0x00060000` |
| `0x78..0x7a` | special island; reads `pi` / `e` / `1/(2*pi)` |
| `0x7b` | special island; reads `0` |
| `0x7c..0x80` | special island; state-dependent reads (`0x7c` = `0x0000ffff`, `0x80` = `1`) |
| `0x81..0x9f` | special island; reads `0` |
| `0xa0..0xdf` | `i(1)..i(64)`; encoding is `0x9f + n` |
| `0xe0..0xef` | `i(-1)..i(-16)`; encoding is `0xdf + n` |
| `0xf0..0xfa` | `f(0.5)`, `f(-0.5)`, `f(1.0)`, `f(-1.0)`, `f(2.0)`, `f(-2.0)`, `f(4.0)`, `f(-4.0)`, `pi`, `e`, `1/(2*pi)` |
| `0xfb..0xff` | special island; reads `0` |

The table is a field contract, not a promise that every raw byte has a
printable register spelling.  A decoder must preserve a reserved value as a
diagnostic; an assembler must reject it.  Predicate selectors are encoded by
`PJ2`, not by `GJ8`.

The special islands (`0x67..0x80`, `0x81..0x9f`, `0xfb..0xff`) are not ordinary
scalar registers: dependency chains through them complete with the wrong value,
so they must not enter an ordinary SGPR allocation pool. Their exact read
values are not stable across contexts; roles, writability, and stage lifetime
are UNRESOLVED.

### The 11-bit selector and its high three bits

`GJ11`, `GK11`, and `GA11` are formed as:

```text
selector[10:0] = { namespace[2:0], base_selector[7:0] }
field bits      = { INST_HI[2:0], INST_LO[7:0] }   (GJ example)
```

The corresponding high-bit slices are `INST_HI[5:3]` for `GK11` and
`INST_HI[8:6]` for `GA11`.

The high three bits select the source namespace.  They are part of the operand
encoding and must be emitted when a VGPR is selected.

| Bit | Meaning |
|----:|---------|
| `base[7:0]` | the G*8 base selector from the table above |
| `bit 8` (`0x100`) | `1` selects a VGPR: the source is `v[base]` |
| `bit 9` (`0x200`) | raw-bit ABS: clear the sign bit of the source value |
| `bit 10` (`0x400`) | raw-bit NEG: flip the sign bit of the source value |

The combinations are `abs`, `neg`, and `abs|neg` (which is `-abs`).

`GJ8` has no vgpr/modifier bits and therefore cannot name a VGPR.  The vector
ALU `M.mod2` and `M.clmp1` bits are operation modifiers on the instruction
result, distinct from the per-source raw-bit abs/neg of the selector.  A tuple
such as `GK11x2` applies the same 11-bit selector to both consecutive VGPR or
SGPR positions.

### V fields and R fields are different namespaces

`V*8` fields directly name a VGPR index (or a row-defined zero selector).
`G*` fields use the selector matrix above. `R*8` fields are scalar positions,
descriptor bases, VCC bases, or offsets according to their semantic rule.
The same raw byte therefore has different legal spellings in different fields;
the mnemonic's operand rule, not the first letter alone, determines the class.

| Field family | Typical legal values | Important distinction |
|--------------|----------------------|-----------------------|
| `VD8`, `VJ8`, `VK8`, `VA8`, `VQ8` | `v0..v255`, sometimes `zero` | Direct vector-register index; no G selector high flags |
| `GJ8`, `GK8` | 8-bit base-selector table | SGPR/special/inline only; no VGPR namespace bit |
| `GJ11`, `GK11`, `GA11` | 8-bit base plus vgpr/abs/neg bits | bit 8 selects a VGPR; bits 9..10 are raw-bit abs/neg modifiers |
| `RD8` | Scalar destination, SCC, VCC, or predicate in a row-specific form | The destination rule controls special values |
| `RJ8x2/x4`, `RK8x4/x8`, `RA8x4/x8` | Consecutive SGPR descriptors | Tuple width describes descriptor width, not selector class |

## Immediate, modifier, and mask rules

| Form | Bits | Meaning |
|------|-----:|---------|
| `IJ32`, `IK32` | 32 | Literal bit pattern in `INST_LO[31:0]` |
| `IA16`, `IJ16`, `IK16`, `X2_16` | 16 | SIO identifier, channel, or offset literal |
| `UO8`, `UO12`, `UO20` | 8/12/20 | Unsigned offset selected by the instruction |
| `X0_1`, `X1_1`, `X1_2`, `X1_8`, `X2_12`, `X0_12`, `X0_16`, `X2_23` | as named | Format-specific control fields shown in the family diagram |
| `M.mod2`, `M.clmp1` | 2/1 | Vector operation mode and clamp |
| `M.offen1`, `M.idxen1`, `M.bl11`, `M.bl21` | 1 | Buffer address/memory controls |
| `M.un1`, `M.na1`, `M.rsz1`, `M.dmask4` | 1/1/1/4 | Image addressing, array, result-size, and channel mask controls |

The catalog row defines which masks are fixed, parameter, ignored, or reserved.
`fixed_mask` is the total decoder match mask; `opcode_mask` identifies the
operation subset within it. A reserved bit must be emitted with the row's base
value, and an ignored bit must not be used to infer an operand.

## Canonical assembly boundary

The committed codec table `code/lg200/gpu/codec/generated/lg200_codec.inc`
(generated from the LG200 catalog; the `generated::kCanonicalVariants` rows)
binds the currently supported typed assembly forms to exact word pairs. The
architectural selector tables above describe the machine namespace; a
particular assembler revision may expose only the typed forms represented by
its `FieldRule` table. It must reject a selector that is not legal for that
field rule, a malformed tuple, an out-of-range immediate, or a reserved family
value.
