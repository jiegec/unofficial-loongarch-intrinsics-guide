# Vector ALU Family

The vector ALU family is the lane-wise compute core: 374 rows that read the
vector register file, optionally combine it with a scalar register, a lane mask
or an inline constant, and write a vector register or a mask. This chapter
covers what the rows share - the two machine formats, the source selector,
destination tuples, the modifier fields, mask gating, condition results, the
floating-point policy, the conversion rules and the matrix rows. The rows
themselves are in the [vector index](../../instructions/vector-alu.md).

## Forms

Two machine formats carry 372 of the 374 rows. The other two, `v_bperm_sm_b32`
and `v_perm_sm_b32`, take the shared-memory format and form their byte selection
through the workgroup path rather than from the operand words alone.

| Form | Format | Opcode bits | Sources | Destination | Literal | Rows |
|------|--------|-------------|---------|-------------|---------|-----:|
| Register | `vop` | `INST_HI[27:14]` | Zero to three selectors | `VD8` or `RD8` | None | 265 |
| Immediate | `vop-imm` | `INST_HI[27:18]` | One selector | `INST_HI[15:8]` | `INST_LO[31:0]` | 107 |

| Bits | Field | Role |
|------|-------|------|
| `INST_LO[7:0]`, `INST_HI[2:0]` | `GJ11` | First source selector, 11 bits |
| `INST_LO[15:8]`, `INST_HI[5:3]` | `GK11` | Second source selector, 11 bits |
| `INST_LO[23:16]`, `INST_HI[8:6]` | `GA11` | Third source selector, 11 bits |
| `INST_LO[31:24]` | `VD8` | Vector destination |
| `INST_LO[31:24]` | `RD8` | Scalar or lane-mask destination |
| `INST_LO[23:16]` | `RD8` | Second result: carry, borrow or condition |
| `INST_HI[10:9]` | `M.mod2` | Instruction modifier |
| `INST_HI[11]` | `M.clmp1` | Instruction modifier |
| `INST_HI[13:12]` | `PJ2` | Mask selector |
| `INST_LO[31:0]` | `IK32`, `IJ32` | 32-bit literal, immediate form only |
| `INST_HI[7:0]` | `VJ8` | Vector source, immediate form only |
| `INST_HI[15:8]` | `VD8` or `RD8` | Destination, immediate form only |
| `INST_HI[17:16]` | `PJ2` | Mask selector, immediate form only |

The immediate form moves the destination into the high word because the low
word holds the literal, and it carries no `M.mod2` or `M.clmp1`: its arithmetic
policy is the row's and not selectable. A row that uses fewer source selectors
than its format provides names no operand in the unused slot, so those bits are
neither fixed nor an operand and a producer MUST write zero there.

## Source selectors

An 11-bit selector names one register or one inline constant and carries two
per-source value modifiers:

| Bits | Meaning |
|------|---------|
| `[7:0]` | Register number, or the base encoding of an inline constant |
| `[8]` | Set: vector register. Clear: scalar register or inline constant. |
| `[9]` | Magnitude of the source: the sign bit is cleared |
| `[10]` | Negation of the source: the sign bit is flipped |

With bit 8 clear, the low byte selects from the scalar file or the constant
table: `0x00`-`0x65` are `s0`-`s101`; `0x66` is integer zero and `0x67` the
zero of a 64-bit source's high half; `0x7c`-`0x7f` are `p0`-`p3`; `0x80` is
`SCC`; `0xa0`-`0xdf` are the integers 1 to 64 and `0xe0`-`0xef` the integers -1
to -16; and `0xf0`-`0xfa` are 0.5, -0.5, 1.0, -1.0, 2.0, -2.0, 4.0, -4.0, pi,
e and 1/(2 pi). The remaining bases, `0x68`-`0x7b`, `0x81`-`0x9f` and
`0xfb`-`0xff`, have no assigned meaning and a producer MUST NOT use them.

The two modifier bits apply to an inline constant as well as to a register, so
a negated literal is still one selector. They act on the value before the row's
operation and are visible only where the row exposes the sign. `v_max_u32`
documents the pair directly: `0x200` clears the sign bit, turning `0x80000000`
into `0x00000000`, and `0x400` flips it, moving the same pattern to the other
end of the unsigned order. `v_mov_b32` documents them as a magnitude or a
negation of any 32-bit pattern, floating point and NaN included. Where the
operation discards the bit the modifier acts on, the modifier is inert: on
`v_mul24_i32` and its siblings the reduction drops bit 31, so neither bit can
negate the result.

A source whose layout says `x2` is 64 bits wide: an even-aligned vector pair
whose base occupies the selector a single register would use, or one inline
constant replicated into both halves. The suffix adds no encoding bit of its
own.

## Destination tuples

`VD8` names a vector register. A destination whose layout says `x2` writes two
consecutive registers and its base MUST be even; the suffix states the width a
reader must honour, not an extra field, and the field always carries the base.

`RD8` names a register in the scalar file: `0x00`-`0x65` an ordinary scalar
register, `0x7c`-`0x7f` a mask register. A comparison uses that field in either
role. Naming a mask register selects the predicate form, which gates a later
instruction per lane. Naming an ordinary scalar register writes a mask word:
`0x0000ffff` when the comparison holds and `0x00000000` when it does not,
whatever the number of active lanes. That word is fixed, not one bit per lane,
so a producer MUST NOT read it as a ballot over thirty-two lanes.

Ten rows carry a second result at `INST_LO[23:16]`, the byte a third source
would otherwise use:

| Rows | Second result |
|------|---------------|
| `v_add_u32`, `v_add_i32` | Per-lane carry out |
| `v_sub_u32`, `v_subrev_u32` | Per-lane borrow out |
| `v_sub_i32` | Assigned; its content is UNRESOLVED |
| `v_addc_u32`, `v_subb_u32`, `v_subbrev_u32` | Carry or borrow state, read and written |
| `v_absdiff_i32`, `v_absdiff_u32` | Per-lane condition; content UNRESOLVED |

`v_firstv2s_b32` is the other shape of scalar destination: it copies one active
lane's word to a scalar register unchanged and leaves no vector result.

## Modifiers

Every register-form row carries `M.mod2` at `INST_HI[10:9]` and `M.clmp1` at
`INST_HI[11]`; no immediate-form row carries either. Both are parameter bits,
not fixed ones, so a stream can write any value - but no row establishes an
effect for either, and the rows that name them state the effect as UNRESOLVED.
A producer MUST write zero in both unless it is deliberately testing them,
because no value is established to be inert.

## Conditions and masks

The carry state is a per-lane field in the low 16 bits of the scalar word the
second result names, and the following register is not written. `v_addc_u32`
reads bit *i* as lane *i*'s carry-in and writes the carry-out bits back to the
same positions, so a multi-word addition is a chain of ordered steps through one
scalar register. What the sixteen bits hold for lanes above the field's width,
and whether a carry is produced for a lane whose destination is not written, are
UNRESOLVED.

A mask selector names one of `p0`-`p3` and gates the instruction; the field is
`INST_HI[13:12]` in the register form and `INST_HI[17:16]` in the immediate
form. The selected mask is an absolute lane set, not a predicate the instruction
evaluates per lane, and the instruction does not modify it. What a gated-off
lane's destination holds is UNRESOLVED: the sources disagree, one treating it as
cleared and the other as unchanged. A producer MUST NOT build a merge from two
complementary gated writes, and instead selects per lane with the mask as an
operand. The selector table is in [Execution](../execution.md).

A comparison is the family's mask producer, in the two destination forms
described above. It reads its operands as the row's width and signedness; a NaN
operand makes an ordered predicate false, and the ordered and unordered
spellings are separate rows.

## Floating point

The policy is fixed by the row, not selected by a field: there is no rounding
mode, no flush control and no denormal-mode bit anywhere in the family.

| Behaviour | Value |
|-----------|-------|
| Rounding | Nearest, ties to even |
| Denormal input | Treated as zero |
| Denormal result | Flushed to a signed zero |
| Derived NaN | `0x7fc00000` |
| Propagated NaN | Keeps the source sign and payload, quieted if signalling |
| Overflow | Infinity, never a clamped finite maximum |

A NaN arriving on an operand is propagated rather than replaced; only a NaN the
operation itself creates takes the canonical pattern. `v_min_f32` and
`v_max_f32` propagate a NaN from either position and return it, so the ordering
is not total: a NaN candidate wins against a numeric one. Equal-magnitude zeros
resolve to the negative zero for the lesser selection and to the positive zero
for the greater.

The remaining special cases are per row. Infinity added to negative infinity and
zero multiplied by infinity both give `0x7fc00000`. `v_sqrt_f32` returns
`0xffc00014` for a negative source that is neither zero nor NaN, and negative
zero returns negative zero. `v_fma_f32` keeps the product exact and rounds once,
so a sum that cancels the leading bits of the product keeps the residual; its
NaN precedence is the addend, then the first multiplicand.

Reciprocal, reciprocal square root, square root, logarithm, exponential, sine
and cosine are approximations. The accuracy away from the points their rows fix
is UNRESOLVED, so a correctly rounded result needs a refinement step.

## Conversions

Conversion rows move between integer widths, between integer and floating point,
and between floating-point widths. The direction decides the failure mode:

- Integer to float rounds to nearest, ties to even, and cannot overflow.
  `v_fcvt_f32_i32` sends `0x7fffffff` to `0x4f000000`, the encoding of 2^31,
  and converts `0x80000000` exactly.
- Float to integer rounds to nearest, ties to even, and clamps at both ends of
  the destination range. `v_fcvt_i32_f32` leaves `0x7fffffff` and `0x80000000`;
  `v_fcvt_u32_f32` leaves `0xffffffff` for a source at or above 2^32 and zero
  for a negative source.
- Narrowing floats round once. `v_fcvt_f16_f32` keeps a result below the
  half-precision normal range as a subnormal instead of flushing it, and an
  out-of-range result becomes an infinity rather than a saturated maximum.
- Widening `v_fcvt_f32_f16` is exact for every binary16 value that survives the
  flush, but the flush decision is taken on the source format, so the binary16
  subnormal `0x0001` becomes zero although it is an ordinary binary32 value.

The pack rows are not conversions. `v_pack_i16_i32` and `v_pack_u16_u32` place
the low 16 bits of each source side by side with no range check, so a source
wider than the field silently loses its top bits; the immediate forms of the
same names clamp the literal instead, so `0x10000` contributes `0x7fff` to the
signed form and `0xffff` to the unsigned one. `v_pack_i8_i32` clamps the first
source to the signed 8-bit range and inserts it into a byte of the third source
chosen by the second source modulo four. `v_pack_u8_u32` inserts the smaller of
the raw word and `0xff`, a pattern comparison and not a numeric clamp.
`v_sext_i32_i8` and its siblings extend the low element exactly, with no
rounding, saturation or flush.

## Matrix rows

Eight rows multiply a packed 8-bit matrix by a packed 8-bit matrix and add a
32-bit accumulator. Shape and signedness are opcode bits rather than operand
fields, so every member is a separate row:

| Shape | Product | Operand tuples | Rows |
|-------|---------|----------------|-----:|
| `v_mma_8816_*` | 8x16 by 16x8, accumulated into 8x8 | `GJ11`, `GK11` one register each; `GA11`, `VD8` two-register tuples | 4 |
| `v_mma_323216_*` | 32x16 by 16x32, accumulated into 32x32 | `GJ11`, `GK11` four-register tuples; `GA11`, `VD8` thirty-two-register tuples | 4 |

The signedness pair occupies two adjacent opcode bits inside `INST_HI[27:14]`:
`u8_u8` leaves both clear, `i8_i8` sets both, and the mixed members set exactly
one. Which of the two bits belongs to which source is UNRESOLVED for the mixed
members, because the reference model for those rows applies one signedness to
both operands and no case separates them from the pure forms.

For the 8816 shape, with `byte` the byte index inside a lane:

```text
GJ11: 8x16, row = lane / 4, k = 4 * (lane % 4) + byte
GK11: 16x8, k as above,     j = lane / 4
VD8:  8x8 i32, row = lane / 4, j = 2 * (lane % 4) + word
```

The 323216 shape establishes only the per-lane element counts - sixteen bytes of
each source and thirty-two accumulator words - so its lane mapping is
UNRESOLVED.

The accumulator is an operand, so a contraction is a chain of ordered steps from
a defined start, and the chain wraps modulo 2^32: it neither saturates nor
faults, so an overflowing accumulation is silent. Both sources are read as bytes
of the declared signedness, so a packed unsigned field in a signed member needs
sign-extension or re-packing first.

Cooperative matrices do not travel through these rows. The supported path holds
a cooperative matrix as eight eight-component vectors, one per row, and expands
the multiply into ordinary lane-wise operations over a thirty-two-lane subgroup.
A producer that wants the matrix rows MUST emit them directly.

## Matrix reads

`load_sm_matrix_x1`, `load_sm_matrix_x2` and `load_sm_matrix_x4` read a matrix
plane from the launch-persistent matrix file into a contiguous register tuple.
`VJ8` holds the byte address as a single vector register even in the wide forms,
`VD8` is the tuple start, and no shared-memory base register is encoded because
the launch supplies the segment.

For the single-plane form, with `s` the per-lane address stride in words:

```text
word = base + (lane & 3) + s * (lane >> 2)
```

Lane *n* receives element *n* of a four-column matrix whose row pitch is the
caller's stride. The mapping follows the stride and not each lane's own address:
a constant added to every lane's address shifts the whole result and changes
nothing else. The 16-bit field at `INST_HI[15:0]` is inert - writing small
values into it leaves the result bit-identical - and the opcode field
`INST_HI[26:18]` carries `0x031`, `0x032` and `0x033` for the three widths.

How the second and later planes follow the first is UNRESOLVED: an increment of
the plane index, of the address and of a lane coordinate are not separated by
any case. The `x2` form writes exactly the two registers its tuple names, low
plane first. The `x4` form writes more than four - seven consecutive registers
change, in an element order of 0, 8, 16, 24, 20, 28, 32 - so how many registers
it writes and how the four planes map onto them are UNRESOLVED, as is the
intra-plane layout across the lanes of a wave. A selector that leaves the matrix
file, and the value of a plane the launch has not written, are UNRESOLVED.

## Family map

| Group | Behaviour |
|-------|-----------|
| Comparisons | 160 rows over 16-, 32- and 64-bit integers and floats, ordered and unordered. Each writes a mask word. |
| Conversions | 23 `v_fcvt_*` rows between integer and float widths, and between float widths. |
| Multiply | `v_mul_*`, `v_mul24_*`, `v_mul_hi_*`, `v_mac_*` and the four `v_dp4a_*` dot products. |
| Add, subtract, difference | `v_add_*`, `v_sub_*`, `v_subrev_*`, `v_absdiff_*`, and the carry and borrow forms. |
| Minimum and maximum | `v_min_*`, `v_max_*` and the three-way `v_min3_*`, `v_med3_*`, `v_max3_*`. |
| Logic and bit field | And, or, xor, not, bit-field extract and insert, count, scan, reverse. |
| Shifts and rotates | Shift, rotate, reversed and immediate forms; the count is masked. |
| Packing | Pack and unpack between widths, saturating and normalized variants. |
| Data movement | Move, permute, byte permute, lane broadcast, quad and shared-memory permute. |
| Matrix | Packed 8-bit multiply-accumulate, matrix plane read. |
| Special values | Cube-face selection, exponent and mantissa manipulation, derivatives. |

## Open questions

- The effect of `M.mod2` and `M.clmp1` on any row, and whether a nonzero value
  is inert.
- Whether the two per-source modifier bits compose as a negated magnitude when
  both are set.
- The destination of a lane outside the selected mask.
- The mask width: the scalar destination of a comparison is a fixed sixteen-bit
  word, and what a row holds for lanes above bit 15 is not established.
- Whether a carry is produced for a lane whose destination is not written, and
  how the carry field interacts with a lane mask.
- The second result of `v_sub_i32`, and the condition result of the absolute
  difference rows.
- The accuracy of the approximation rows beyond the points their cases fix.
- The signedness of each source in the two mixed-sign matrix members.
- The lane mapping of the 323216 shape, and the plane addressing of the matrix
  reads including the register count of `x4`.
- Which lane `v_firstv2s_b32` selects when several are active.
