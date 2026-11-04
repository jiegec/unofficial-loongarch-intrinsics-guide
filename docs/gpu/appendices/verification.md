# Verification Corpus

Every machine row carries three things: a reference body that states what the row
does, a CPU oracle that computes the same result independently, and case vectors
that name an input and the result it must produce. This chapter states what that
corpus covers, because a vector set is only as good as the classes it stages.

## Coverage

A row is asked for a set of boundary classes derived from its own type: signed
zeros, the two infinities, both quiet and signalling NaN, the largest finite
magnitude, denormals, the integer extremes and the carry, borrow and wrap points.
A class counts as covered only when a vector stages it in a column the row's model
actually reads; an address or selector column is held inside its window, because a
boundary value there would test the window rather than the instruction.

| Measure | Value |
|---|---|
| machine rows | 768 |
| rows whose boundaries are expressible as an input value | 738 |
| rows carrying every required boundary class | **723** |
| rows still with gaps | **0** |
| rows with no result to compare | 15 |
| rows that need a launch shape rather than a case | 30 |
| rows the boundary grid is built for | 308 |
| vectors | **88,197** |
| generated vectors | 83,542 |
| hand-written cases | 4,655 |

The 15 rows without a result are the barrier, check, copy, exit, flag, nop, prefix,
return and trap forms: their oracle takes no inputs and returns none, so there is
nothing for a vector to assert. The 30 launch-shape rows read state the launch
supplies - the execution environment, the SIO channel file, the interpolator
coefficient window, the matrix file - and need a launch, not a datum.

## What the corpus does not ask

Three gaps in the question itself, stated here so that a zero above is not read as
more than it is.

The class space holds input literals only: it has no "tie at the rounding
boundary" and no "result a flush takes to zero". A row can therefore never be
reported missing either.

The judgement is per row, not per operand position. For a tagged row the audit
unions the inputs of the whole row, so a pair of rows that differ only in the order
of their operands counts as covered once.

The audit does not know which side of an instruction the float sits on, so a
conversion whose float is the RESULT is asked for float input classes.

## Evidence

A vector that has been compared against the oracle on both pipes is a measurement.
A vector the runner could not stage is recorded as an observation: the value the
hardware produced is kept, but no independent comparison backs it. The two are
counted separately, and an observation never fails.

## Unresolved clauses

The rows state **503** of the 768 with at least one UNRESOLVED clause, in **664**
distinct sentences and 903 occurrences in total. 460 of those rows name, in every
open clause, the layer that cannot settle it, which is what separates "we do not
know yet" from "we have not asked".
