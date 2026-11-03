# Interpolation Family

This family combines a coefficient vector with a barycentric value. The
fragment inputs and the coefficient table are in
[Interpolation](../../shaders/interpolation.md). The rows are in the
[interpolation index](../../instructions/interpolation.md).

## Forms

| Row | Role |
|-----|------|
| `v_interp_mov_b32` | Move one coefficient payload word into a vector register, unchanged. |
| `v_interp_1_f32` | First phase: derive the intermediate value. |
| `v_interp_2_f32` | Second phase: combine the intermediate with the coefficient and write the varying. |

The two f32 phases are paired, and the destination is an in-place source. The
phases may not be reordered against a write to that register.

## Operands

The rows use the vector ALU layout:

| Field | Role |
|-------|------|
| `GJ11` | First source: the barycentric value. |
| `GK11` | Second source: the coefficient. |
| `VD8` | Destination and in-place source. |
| `M.mod2`, `M.clmp1` | Vector operation modifiers. |
| `PJ2` | Mask selector. |

The third general-source position is not read.

The first phase computes:

```text
phase1 = second_source * (first_source + 1)
```

## Coverage

The arithmetic of this family is the least established part of the instruction
set. `v_interp_mov_b32` performs no arithmetic. The first phase is documented for
the values it was exercised with; the second phase's arithmetic when its state
is not neutral is UNRESOLVED.

## Open questions

- The second phase's arithmetic, rounding and denormal handling.
- The width of the state the second phase reads.
- Whether both phases must write the same destination.
- Whether the first phase's sum is rounded once or twice.
- Whether an out-of-range coefficient slot is clamped or undefined.
- Whether an inactive lane participates.
