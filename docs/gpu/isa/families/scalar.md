# Scalar ALU Family

The scalar ALU operates on the scalar register file, which is shared by all
lanes of a wave, and on the condition register. This chapter describes what the
family's rows share. The rows are in the [scalar index](../../instructions/scalar-alu.md).

## Forms

| Form | Sources | Destination | Notes |
|------|--------:|-------------|-------|
| One source | 1 | Scalar | Move, bit operations, count and find. |
| Two source | 2 | Scalar | Arithmetic, logic, shifts. |
| Immediate | 1 plus a literal | Scalar | The literal occupies the low word. |
| Compare | 2 or 1 plus a literal | Condition or scalar | Writes the condition. |
| Select | 3 | Scalar | Chooses between two values using the condition. |

An immediate form carries a full 32-bit literal. A non-immediate form takes its
second operand from the selector table, which includes the inline constants.

## Condition register

Compare rows write `SCC`. Add-with-carry, subtract-with-borrow, select and
conditional branches read it.

- The value is a flag, not a bit of the arithmetic result.
- The register is short-lived across compares; a consumer must be materialised
  before the next compare.
- A compare that writes a scalar register is not the established form.

## Operations

| Group | Examples |
|-------|----------|
| Arithmetic | Add, subtract, add-with-carry, subtract-with-borrow, reverse subtract, multiply, high multiply, absolute difference. |
| Logic | And, or, xor, and their complemented and negated forms. |
| Shifts and rotates | Shift, rotate and reverse immediate forms. |
| Bit operations | Bit-field extract and insert, reverse, set, clear, test. |
| Counting | Count leading and trailing zeros and ones, population count. |
| Masks | Whole-quad mask, quad mask, bit-mask helpers. |
| Conversions | Sign extension, absolute value, negation. |
| Movement | Move, move immediate, select. |

## Whole-quad and quad masks

Two mask rows relate the scalar domain to the lanes of a wave: one computes a
mask covering whole quads, used when helper lanes are enabled for derivative
work; the other derives a lane mask from a quad mask. Both produce lane masks,
which are ordinary registers.

## Scalar memory

A small set of scalar loads reads memory into the scalar register file, in the
scalar buffer and scalar flat modes. Their results are tuples of scalar
registers, and a wide load is modelled by its leading words. See
[Memory family](memory.md#scalar-memory).

## Open questions

- The exact effect of the mask rows beyond their names.
- Whether every arithmetic row defines the condition register or only the rows
  with an explicit carry role.
- The behaviour of count and find rows for all-zero input.
- The rounding and saturation of high-multiply and absolute difference.
- The placement limits for wide scalar result tuples.
