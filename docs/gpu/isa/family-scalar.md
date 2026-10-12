# Scalar ALU Family

The instruction index for this family is under [scalar-alu](../instructions/scalar-alu.md).

The scalar ALU operates on the scalar register file, which is shared by all lanes
of a wave, and on the condition register.  The register file and the selector
matrix are in [Operands and Execution Registers](operands.md).

## Forms

| Form | Sources | Destination | Notes |
|------|--------:|-------------|-------|
| One source | 1 | scalar | move, bit operations, count and find operations |
| Two source | 2 | scalar | arithmetic, logic, shifts |
| Immediate | 1 plus a literal | scalar | the literal occupies the whole low word |
| Compare | 2 or 1 plus a literal | condition register or scalar | writes the condition |
| Select | 3 | scalar | chooses between two values using the condition |

The immediate forms carry a full 32-bit literal, so a scalar constant never needs
to be materialised before use; the non-immediate forms take their second operand
from the selector table, which includes the inline constant ranges.

## The condition register

Compare rows write the condition register, and several other rows read it:

* Add-with-carry, subtract-with-borrow and their immediate variants consume the
  incoming condition and produce an outgoing one, which is how multi-word
  arithmetic is chained.
* The select row chooses between two scalar values from the condition.
* A conditional branch consumes it as its branch condition.

Two properties of this register matter:

1. **Its value is a flag, not a bit of the result.**  A carry or borrow is
   encoded as "the condition is non-zero", so a producer must treat it as a
   boolean rather than as an arithmetic value derived by shifting.
2. **It is short-lived across compares.**  A consumer must be materialised before
   the next compare is issued; the form of a compare that writes the result to a
   scalar register is not the established one, because back-to-back chains read a
   stale value.  The producer's output confirms this exactly (2026-10-13): every
   scalar compare it emits writes the condition - `s_cmp_lt.i32 scc, s0, s2`,
   `s_cmp_eq.i32 scc, s0, s2` - and the value is then materialised with a select,
   `s_select.b32 s1, s1, i32(1), zero, scc`.  No compare in that program names a
   scalar register as its destination.

## Operations

| Group | Examples |
|-------|----------|
| Arithmetic | add, subtract, add-with-carry, subtract-with-borrow, reverse subtract, multiply, high multiply, absolute difference |
| Logic | and, or, xor, and their complemented and negated forms |
| Shifts and rotates | shift left, arithmetic and logical shift right, rotate, and their reverse immediate forms |
| Bit operations | bit field extract and insert, bit reverse, bit set and clear, bit test |
| Counting | count leading zeros and ones, count trailing zeros and ones, population count |
| Masks | whole-quad mask, quad mask, bit-mask helpers |
| Conversions | sign extension of narrow values, absolute value, negation |
| Movement | move, move immediate, select |

## Whole-quad and quad masks

Two mask rows relate the scalar domain to the lanes of a wave: one computes a
mask covering whole quads, used when helper lanes have to be enabled for
derivative work, and one derives a lane mask from a quad mask.  Both produce lane
masks, so their results are subject to the same lifetime rules as vector compare
results.

## Scalar memory

A small set of scalar loads reads memory into the scalar register file, in the
scalar buffer and scalar flat addressing modes.  They are described with the
memory family in [Memory and Image Family](family-memory.md).  Their results are
tuples of scalar registers, and a wide load is modelled by its leading words.

## UNRESOLVED

* The exact effect of the mask rows beyond their names.
* Whether every arithmetic row defines the condition register, or only the ones
  with an explicit carry role.
* The behaviour of the count and find rows for all-zero input.
* The rounding or saturation behaviour of the high-multiply and absolute
  difference rows for extreme operands.
* The placement limits for wide scalar result tuples in the ordinary register
  window.
