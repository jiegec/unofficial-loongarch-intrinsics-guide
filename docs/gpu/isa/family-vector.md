# Vector ALU Family

The instruction index for this family is under [vector-alu](../instructions/vector-alu.md).

The vector ALU is the largest family: arithmetic, logic, shifts, conversions,
comparisons and data movement over the vector register file.  The operand classes
and the selector matrix are defined in
[Operands and Execution Registers](operands.md); this chapter covers what is
common to the family.

## Forms

| Form | Sources | Destination | Literal |
|------|--------:|-------------|---------|
| One source | 1 | vector | none |
| Two source | 2 | vector | none |
| Three source | 3 | vector | none |
| Immediate | 2 | vector or scalar | 32-bit literal in the low word |

A form that uses fewer sources than the layout provides leaves the unused
selector at its fixed value.  Rows with a scalar or condition result use the
destination byte as a scalar position instead of a vector index.

Operands are 11-bit selectors: the low eight bits select from the base table and
the top three add the vector namespace and the per-source sign modifiers.  The
same byte therefore means a scalar register in one row and a vector register in
another; the row's operand rule decides.

## Destination tuples

Many rows write more than one register:

* A 64-bit result writes an aligned pair, and a 128-bit result an aligned quad.
* An add or subtract with carry writes the sum and the carry out, in two
  consecutive registers.
* A comparison writes a lane mask rather than a value.

Tuple alignment follows the register-file rule: a pair starts at an even
register and a quad at a multiple of four.

## Per-source modifiers versus operation modifiers

Two different mechanisms look similar and must not be confused:

| Mechanism | Where | Effect |
|-----------|-------|--------|
| Selector bits 9 and 10 (`0x200` and `0x400` of the selector value) | on each source | clears or flips the sign bit of that source's value |
| `M.mod2` and `M.clmp1` | on the instruction | select an operation mode and clamp the result |

The selector modifiers are raw-bit operations: they act on the value's sign bit,
not on its numeric interpretation, and they apply before the operation.  The two bits
the page above calls `0x200` and `0x400` are exactly bits 9 and 10 of the selector
value, and a general source field is 11 bits wide (`LO[7:0]` plus `HI[2:0]`), so the
number, the clear bit and the flip bit fill it exactly.  Verified against the operand
encodings 2026-10-13.  The
instruction modifiers apply to the result.  Their exact encodings beyond the names are UNRESOLVED - but the reason is now known
to be structural rather than incidental.  In two whole linked programs (~30,000 lines
each, runtime libraries included) the disassembler prints none of `mod2`, `clmp1`,
`dmask` or `un`, so the producer emits them at ZERO everywhere; `mod2` and `clmp1` do
not appear anywhere in its intrinsic layer either.  `bl1`/`bl2` are the exception -
both are reachable as the buffer forms' two boolean parameters - and even there only
`bl1` is used, on private spill traffic.

## Conditions and lane masks

Comparison rows produce a lane mask: one bit per lane, written to a destination
position rather than to a data register.  The result is used in three ways:

* As the predicate of a vector select, which chooses between two values per
  lane.
* As the input of a branch, which acts on the mask as a value.
* As the source of a scalar reduction that materialises a boolean.

Two properties limit how long a mask is useful: a mask does not survive the next
vector instruction, and vector memory and sampling instructions clear masks.
A producer keeps a mask in the data domain or consumes it immediately.

Carry and borrow arithmetic reads its incoming condition from the condition
register and writes its outgoing condition back, so a multi-word addition is a
chain in which the carry is live between instructions.

## Floating point

The floating-point rows run with a fixed hardware behaviour rather than a
per-instruction policy:

* Results are rounded to nearest, and the rounding-direction and flush controls
  the encoding carries are inert.
* Denormal inputs and results are flushed.
* A derived NaN uses the positive quiet pattern `0x7FC00000`; a propagated NaN
  keeps the source sign and payload.
* The minimum and maximum rows propagate NaN rather than suppressing it.
* Reciprocal, square-root and logarithm rows are approximations and are
  documented with their observed accuracy.

These behaviours belong to the exact variants that were measured; a different
operand form of the same mnemonic is a separate question and is marked
`UNRESOLVED` where it has not been established.

## Conversions

Conversion rows convert between integer widths, between integer and floating
point, and between floating-point widths.  A conversion that narrows saturates
or wraps according to the row, and a conversion that widens sign-extends or
zero-extends according to the row; the per-row behaviour is documented with the
instruction.  Several conversion encodings are inert on this generation — they
decode and execute without changing the destination — which is documented per
row.

## Matrix multiply-accumulate

Eight rows multiply a packed 8-bit matrix by a packed 8-bit matrix and add a
32-bit accumulator: `v_mma_8816_*` is an 8x16 by 16x8 product accumulated into
8x8, and `v_mma_323216_*` is 32x16 by 16x32 into 32x32. Each shape is a separate
operation code, and the signedness of the two sources is a further field
(`INST_HI[15:8]`), so shape and signedness are both decoded rather than carried
by an operand.

The fragments are a property of the wave, not of the operand syntax.  For the
8816 shape the mapping is measured over the whole operand range: with `byte` the
byte index inside a lane, `GJ11` holds an 8x16 operand with `row = lane/4` and
`k = 4*(lane%4) + byte`, `GK11` holds 16x8 with the same `k` and `j = lane/4`,
and the destination holds 8x8 i32 with `row = lane/4` and
`j = 2*(lane%4) + word`.  For the 323216 shape the per-lane fragment counts
follow from the vendor's declared operand types; its lane mapping has not been
measured separately.

These rows are wave-wide: the vendor declares them convergent, and the
accumulator is an operand rather than the destination's incoming value, so a
contraction is a chain of ordered steps from a defined start.  A lowering is not
obliged to use them: because the fragments are fully determined, a
cooperative-matrix operand can equally be assembled from ordinary per-lane loads
and stores, and such a lowering has its own lane assignment.  The mapping above
describes the instruction, not any particular producer.

## Matrix reads

`load_sm_matrix_x1`, `load_sm_matrix_x2` and `load_sm_matrix_x4` read a matrix
plane from the workgroup allocation into a register tuple.

Measured 2026-10-30: the tuple is contiguous and its width is 1/2/4; plane 0 is
`base + 4*(lane&3) + s*(lane>>2)`, plane 1 is 8*s further.  The fill grows with the
number of active lanes rather than switching on at one width: `x1` is complete at 4
lanes, `x2` at 16 and `x4` at 32, while 20/24/28 lanes fill `x4` only partially (plane 2
for lanes 0-15 at 20; all three planes at 24; plane 3 for lanes 0-19 at 28) and the
missing cells are words from outside the launch, not window data.  The rule behind the
partial fill is UNRESOLVED; the widths above are the guaranteed ones - see
[the memory family](family-memory.md#shared-memory).

## Family map

| Group | Examples |
|-------|----------|
| Integer arithmetic | add, subtract, multiply, multiply-accumulate, add-with-carry |
| Integer logic | and, or, xor and their complemented forms, bit select |
| Shifts and rotates | shift left, logical and arithmetic shift right, rotate, and their immediate forms |
| Bit operations | bit field extract and insert, count leading zeros and ones, bit reverse, find first set |
| Packing and unpacking | pack and unpack between widths, saturating variants |
| Floating point | add, subtract, multiply, multiply-add, divide, reciprocal, square root |
| Conversions | integer to floating point and back, between floating-point widths, with rounding variants |
| Comparisons | equal, not-equal, less, greater, ordered and unordered, over integers and floating point |
| Data movement | move, swap halves, permute, lane broadcast, quad and byte permute |
| Matrix | packed 8-bit multiply-accumulate, matrix plane read |
| Special values | cube-face selection, exponent extraction, mantissa manipulation |

## UNRESOLVED

* The encodings and effects of `M.mod2` and `M.clmp1`.
* The exact meaning of clamp and saturation on rows where the modifier is inert.
* The numeric accuracy of the approximation rows beyond the cases that were
  measured.
* The behaviour of the arithmetic rows for operand forms that were not measured.
* Whether a lane-mask result is readable as an ordinary scalar value.
* The interaction between the per-source sign modifiers and integer saturation.
* Which of the two sources of a mixed-signedness matrix row carries the signed
  reading, since the mnemonic's letter order and the operand order are not
  established to agree.
* The lane mapping of the 323216 matrix shape, which was measured only for 8816.
