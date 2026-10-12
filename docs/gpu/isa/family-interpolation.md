# Interpolation Family

The instruction index for this family is under
[interpolation](../instructions/interpolation.md).

How a fragment stage reads the coefficient table and combines it with the
barycentric coordinates is described in
[Stage Interfaces](../shader/stages.md); the registers the launch supplies at
entry are in the [shader ABI](../shader/abi.md).  This chapter describes what the
three rows have in common.

## Forms

Interpolation is a two-phase operation over an f32 varying, plus the payload move
that feeds it:

| Row | Role |
|-----|------|
| `v_interp_mov_b32` | moves one coefficient payload word from a lane source into a vector register, unchanged |
| `v_interp_1_f32` | first phase: derives the intermediate interpolation value for the lane |
| `v_interp_2_f32` | second phase: combines the intermediate with the coefficient and writes the varying |

The two f32 phases are paired, and their destination is an in-place source: the
register the result lands in is also one of the operands, so the dependency
between the phases is real and neither row may be reordered against a write to
that register.  The register a shader supplies is not free either, because the
coefficient slot and the payload layout come from the fragment-stage ABI rather
than from the operand fields.

## Operands

The rows use the vector ALU layout, not a family-specific one:

| Field | Role |
|-------|------|
| `GJ11` | general-source selector; carries a lane source |
| `GK11` | general-source selector; carries a lane source |
| `VD8` | destination, and an in-place source in the reviewed form |
| `M.mod2`, `M.clmp1` | the vector rows' operation modifiers |
| `PJ2` | predicate position |

The third general-source position of the vector layout is unread by all three
rows, and every established producer writes it as `zero`.  Which of the two read
selectors addresses the coefficient and which the barycentric value is
UNRESOLVED.

## Coverage

The arithmetic of this family is the least established part of the instruction
set, and the descriptions say so per row.  The first phase is measured only for
positive coverage with a zero second source.  The second phase returned its first
source unchanged on the single measured vector, so it behaved as a pass-through
rather than completing an interpolation, and the state it reads was apparently
neutral.  `v_interp_mov_b32` performs no arithmetic at all, which makes it the
control for the two phases and the only member whose behaviour is established
over the values that were exercised.

A producer that needs a correct varying should therefore treat the phase
arithmetic as unverified and take the fragment inputs the
[stage interfaces](../shader/stages.md) chapter describes, rather than relying on
these rows to reproduce a reference interpolation.

## UNRESOLVED

* Nothing: the assignment is established (2026-10-13) by two independent lines that
  agree.  The rows themselves give the arithmetic - the result is the SECOND source
  times the sum of the FIRST source plus one - and the driver-side disassembly of the
  same instructions reads `v_interp_1_f32 result, v2, coefficient`, putting the
  **coefficient in the second source** (`GK11`) and the barycentric value in the first
  (`GJ11`).  Coefficient x (barycentric + 1) follows.
* The arithmetic of `v_interp_2_f32` when the state it reads is not neutral,
  including its rounding, its denormal flushing and the identity by which it
  becomes a pass-through.
* The width of that state.
* Whether both phases must write the same destination, and whether they may
  interleave with other instructions.
* Whether the sum with one in the first phase is rounded once or twice before the
  multiply.
* Whether an out-of-range coefficient slot in `v_interp_mov_b32` is clamped or
  yields an undefined word, and whether `VD8` is written in full or in part.
* Whether an inactive lane participates.
