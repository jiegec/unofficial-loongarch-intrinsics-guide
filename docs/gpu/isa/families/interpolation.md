# Interpolation Family

A fragment varying is not delivered to the shader; the shader builds it from a
barycentric pair and one coefficient word per component. This family is that
build: two phases that apply the pair to the coefficient, and a move that
delivers an input without modulation. The rows are in the
[interpolation index](../../instructions/interpolation.md).

## Forms

| Row | Opcode, `INST_HI[27:14]` | Operation |
|-----|--------------------------:|-----------|
| `v_interp_1_f32` | `0x55` | First phase: combines one barycentric component with the coefficient word. Vector destination, two vector sources. |
| `v_interp_2_f32` | `0x56` | Second phase: completes the varying from the other component and the same coefficient. Accumulates in place: it writes the register the first phase produced. |
| `v_interp_mov_b32` | `0x63` | Flat input: writes the input without modulation. Destination and coefficient name the same register in the emitted form; the first source is a scalar selector. |

In the emitted form the phases are a pair on one register: the second writes the
register the first produced and still reads the coefficient. Whether the device
requires that pairing is UNRESOLVED. When both phases are emitted, the
coefficient MUST stay intact until the second reads it; naming it as the first
phase's destination destroys it.

## Encoding

All three rows use format 4 (`vop`) and the same layout; only the opcode field
separates them.

| Bits | Field | Role in this family |
|------|-------|---------------------|
| `INST_LO[7:0]`, `INST_HI[2:0]` | `GJ11` | First general source: one barycentric component in the phases, a scalar selector in the emitted move. |
| `INST_LO[15:8]`, `INST_HI[5:3]` | `GK11` | Second general source: the coefficient word. |
| `INST_LO[31:24]` | `VD8` | The vector destination. |
| `INST_HI[10:9]` | `M.mod2` | Carried by the layout; its effect here is UNRESOLVED. |
| `INST_HI[11]` | `M.clmp1` | Carried by the layout; its effect here is UNRESOLVED. |
| `INST_HI[13:12]` | `PJ2` | The lane-mask register `p0`..`p3` the row runs under. |

An eleven-bit general source holds its base selector in the low byte; the three
bits above it are the register-namespace bit `0x100` and the source modifiers
`0x200` and `0x400` ([Operands](../operands.md)). With `0x100` the base is a vector
register; without it the selector names a scalar register or an inline constant.
The emitted forms clear both modifiers. So
`v_interp_1_f32 p0, v0, zero, v0` has `GJ11` = `0x66` (zero) and `GK11` = `0x100`
(`v0`), and encodes `INST_LO = 0x00000066`, `INST_HI = 0x10154008`. The format's
third general-source slot is not an operand here; the seed words leave its bits
clear, a producer SHOULD do the same, and what a non-zero value there does is
UNRESOLVED.

## Coefficients

Each varying component owns one 32-bit coefficient word in the fragment
interpolation window. The rows read that word from a vector register and never
touch the window themselves: the read is the separate `load_sm_interp` row,
whose scalar base is launch state and whose eight-bit slot the stage derives as
`4 * window + channel + component`
([Interpolation](../../shaders/interpolation.md)). The slot is settled before the
shader runs, which is why no bit of these three words selects an attribute.

## Compiler use

For each component a fragment compiler loads the coefficient, waits for the
load, and selects by interpolation mode:

- flat input: `v_interp_mov_b32 vC, s2, vC`
- smooth input: `v_interp_1_f32 vD, vB0, vC` then `v_interp_2_f32 vD, vB1, vC`

`vC` is the coefficient, `vB0:vB1` the barycentric pair, `vD` the varying, and
`s2` the scalar selector the flat path passes. The move is emitted in place,
with the coefficient as its own destination; the phases write a destination of
their own, and the second overwrites what the first left. The emitted contract
is stricter than the encoding: both general slots of a phase hold vector
registers in `v0`..`v255`, the move's first slot holds a scalar register and its
destination is the coefficient register, and the wait for the coefficient load
precedes the first consumer. A shape that breaks one of these is a compile
error, not an encoding to approximate. The family is per-lane and runs under the
mask `PJ2` names, so that mask gates which lanes receive the component.

## Coverage

| Row | Established | UNRESOLVED |
|-----|-------------|------------|
| `v_interp_1_f32` | `dst = GK11 × (GJ11 + 1)` in binary32 for non-negative operands: (1.0, 2.0) gives 4.0, (0.5, 2.0) gives 3.0, (2.0, 2.0) gives 6.0, (3.0, 3.0) gives 12.0; a zero `GK11` collapses the result. | Whether the sum with one rounds once or twice; negative operands; fractions other than one half; overflow; NaN, infinity and denormal operands. |
| `v_interp_2_f32` | With a neutral state the first source reaches the destination unchanged. | The arithmetic when that state is not neutral, its width and identity, the rounding of the final combine, special values. |
| `v_interp_mov_b32` | It writes the destination without arithmetic. | Which operand supplies the word, whether the destination is written whole, the value for an out-of-range coefficient slot. |

The move performs no arithmetic, but its data source is not established: the
row's model forwards the first source, a word placed there has not appeared in
the destination, and the compiler's form passes a scalar selector in that slot
with the coefficient in place. A producer MUST NOT use the move to forward a
computed value. Family-wide, it is also
UNRESOLVED whether both phases must write the same destination, whether an
inactive lane's destination is written, and what `M.mod2` and `M.clmp1` do here.