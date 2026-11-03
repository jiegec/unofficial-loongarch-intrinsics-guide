# Interpolation

The fragment stage receives interpolated varyings. The rasterizer supplies
barycentric coordinates and a coefficient table; the shader combines them with
the interpolation instructions or reads flat values directly. This chapter
defines the inputs and the three interpolation rows.

## Inputs

| Register | Contents |
|----------|----------|
| `v2:v3` | Barycentric pair, smooth interpolation. |
| `v8:v9` | Barycentric pair, noperspective interpolation. |
| `v0:v1` | Barycentric pair when a sample mask is used. |
| `v12..v15` | Fragment coordinate. |
| `v6` | Coverage or sample mask. |
| `v12` or `v16` | Front-facing flag. |
| `s9` | Fragment input window base. |
| `s2` | Interpolation selector. |

The fragment input config's input address selects where the input window starts;
its enable mask selects which input groups are delivered: smooth or flat,
noperspective, fragment coordinate and face.

## Coefficient table

The interpolation coefficients live in the constant root. The coefficient
count is in the fragment input config. A coefficient vector is read with the
shared-memory interpolation load.

> UNRESOLVED: the exact slot formula. The current compiler reads slot
> `base * 4 + channel + component`, where `base` is the coefficient-table base
> and `channel` and `component` select the varying and the component. The
> hardware's address unit for this table is not established.

## Instructions

| Row | Role |
|-----|------|
| `v_interp_mov_b32` | Move one coefficient payload word from a lane source into a vector register, unchanged. |
| `v_interp_1_f32` | First phase: derive the intermediate value for the lane. |
| `v_interp_2_f32` | Second phase: combine the intermediate with the coefficient and write the varying. |

The two f32 phases are paired, and the destination is an in-place source: the
register that receives the result is also an operand. The phases may not be
reordered against a write to that register.

The first phase computes the second source times the sum of the first source
plus one:

```text
phase1 = second_source * (first_source + 1)
```

The first source is the barycentric value (`GJ11`); the second source is the
coefficient (`GK11`).

> UNRESOLVED: the second phase's arithmetic when the state it reads is not
> neutral, including its rounding and denormal handling. On the only measured
> vector it returned the first source unchanged.

## Interpolation modes

| Mode | Source |
|------|--------|
| Smooth | Barycentric pair in `v2:v3`. |
| Noperspective | Barycentric pair in `v8:v9`. |
| Flat | The provoking vertex's value; no interpolation. |
| Sample | The sample-mask path in `v0:v1`. |

Flat interpolation takes the provoking-vertex value directly. The provoking
vertex is selected by the flat-shade-first control in Pipe Enables.

## Attribute map

The fragment attribute map has 32 two-byte entries. Each entry maps the
fragment stage's varying-input ordinal to the producer's output ordinal, with a
generated flag and a sprite flag. The whole map is rewritten per draw so that
stale entries cannot survive.

## Open questions

- The width of the state that the second phase reads.
- Whether both phases must write the same destination.
- Whether the first phase's sum is rounded once or twice before the multiply.
- Whether an out-of-range coefficient slot is clamped or returns an undefined
  word.
- Whether an inactive lane participates.
