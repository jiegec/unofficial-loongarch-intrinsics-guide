# Vector ALU Family

The vector ALU family performs arithmetic, logic, shifts, conversions,
comparisons and data movement over the vector register file. This chapter
describes what the family's rows share. The rows themselves are in the
[vector index](../../instructions/vector-alu.md).

## Forms

| Form | Sources | Destination | Literal |
|------|--------:|-------------|---------|
| One source | 1 | Vector | None. |
| Two source | 2 | Vector | None. |
| Three source | 3 | Vector | None. |
| Immediate | 2 | Vector or scalar | 32-bit literal in the low word. |

A form that uses fewer sources than its layout provides leaves the unused
selector at its fixed value. A row with a scalar or condition result uses the
destination byte as a scalar position instead of a vector index.

## Destination tuples

- A 64-bit result writes an aligned pair.
- A 128-bit result writes an aligned quad.
- An add or subtract with carry writes the sum and the carry out in two
  consecutive registers.
- A comparison writes a lane mask rather than a data value.

## Per-source and operation modifiers

| Mechanism | Where | Effect |
|-----------|-------|--------|
| Selector bits 9 and 10 | On each source | Clear or flip the sign bit of that source. |
| `M.mod2`, `M.clmp1` | On the instruction | Select an operation mode and clamp the result. |

The selector modifiers act on the value's sign bit, before the operation. The
instruction modifiers apply to the result. Their exact encodings beyond the
names are UNRESOLVED.

## Conditions and masks

A comparison produces a lane mask. The result can be used as the predicate of a
vector select, as the input of a branch, or as the source of a scalar reduction.

A mask is an absolute lane set and survives the instruction. It is not cleared
by a vector memory or sampling instruction. The mask model is in
[Execution](../execution.md).

Carry and borrow arithmetic reads its incoming condition from `SCC` and writes
its outgoing condition back, so a multi-word addition is a chain.

## Floating point

- Results are rounded to nearest.
- Denormal inputs and results are flushed on the graphics pipe.
- A derived NaN uses `0x7fc00000`; a propagated NaN keeps the source sign and
  payload.
- Minimum and maximum propagate NaN.
- Reciprocal, square-root and logarithm rows are approximations; their accuracy
  is documented with the row.

These behaviours apply to the exact variant documented on the row. A different
operand form of the same mnemonic is a separate question.

## Conversions

Conversion rows convert between integer widths, between integer and floating
point, and between floating-point widths. A narrowing conversion saturates or
wraps according to the row; a widening conversion sign-extends or zero-extends
according to the row. A row that decodes and executes without changing the
destination is documented as inert.

## Matrix multiply-accumulate

Eight rows multiply a packed 8-bit matrix by a packed 8-bit matrix and add a
32-bit accumulator:

| Shape | Product |
|-------|---------|
| `v_mma_8816_*` | 8x16 by 16x8, accumulated into 8x8. |
| `v_mma_323216_*` | 32x16 by 16x32, accumulated into 32x32. |

The shape is a separate operation code, and the signedness of the two sources is
a field in `INST_HI[15:8]`.

For the 8816 shape, with `byte` the byte index inside a lane:

```text
GJ11: 8x16, row = lane / 4, k = 4 * (lane % 4) + byte
GK11: 16x8, k as above,     j = lane / 4
VD8:  8x8 i32, row = lane / 4, j = 2 * (lane % 4) + word
```

The 323216 lane mapping is UNRESOLVED. The rows are wave-wide: the accumulator
is an operand, not the destination's incoming value, so a contraction is a chain
of ordered steps from a defined start.

## Matrix reads

`load_sm_matrix_x1`, `load_sm_matrix_x2` and `load_sm_matrix_x4` read a matrix
plane from the workgroup allocation into a contiguous register tuple. The
addressing is in [Memory family](memory.md#shared-memory).

## Family map

| Group | Examples |
|-------|----------|
| Integer arithmetic | Add, subtract, multiply, multiply-accumulate, add-with-carry. |
| Logic | And, or, xor, bit select. |
| Shifts and rotates | Shift, rotate and their immediate forms. |
| Bit operations | Bit-field extract and insert, count, reverse, find first set. |
| Packing | Pack and unpack between widths, saturating variants. |
| Floating point | Add, subtract, multiply, multiply-add, divide, reciprocal, square root. |
| Conversions | Integer to float and back, between float widths. |
| Comparisons | Equal, not-equal, less, greater, ordered and unordered. |
| Data movement | Move, permute, broadcast, quad and byte permute. |
| Matrix | Packed 8-bit multiply-accumulate, matrix plane read. |
| Special values | Cube-face selection, exponent and mantissa manipulation. |

## Open questions

- The encodings and effects of `M.mod2` and `M.clmp1`.
- The exact meaning of clamp and saturation on rows where the modifier is inert.
- The accuracy of the approximation rows beyond the documented cases.
- The behaviour of rows for operand forms that are not documented.
- Whether a lane-mask result is readable as an ordinary scalar value.
- The interaction between the sign modifiers and integer saturation.
- The lane mapping of the 323216 matrix shape.
