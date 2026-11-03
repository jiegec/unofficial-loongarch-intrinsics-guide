# Depth and Stencil

Depth and stencil state is per draw and lives in the state object. This chapter
defines the fields, the comparison functions and the operations. The state map
is in [State object](state-object.md).

## Depth Control

Depth Control is 16 bytes at state offset `0x324`.

| Bits | Field | Meaning |
|------|-------|---------|
| `[31:0]` | Offset units | Float pattern. |
| `[63:32]` | Offset factor | Float pattern. |
| `[95:64]` | Offset clamp | Float pattern. |
| `[96]` | Depth enable | Perform the depth test. |
| `[97]` | Depth write | Write the resulting depth. |
| `[98]` | Clamp near | Clamp to the near depth bound. |
| `[99]` | Clamp far | Clamp to the far depth bound. |
| `[102:100]` | Depth function | Compare function. |
| `[119:112]` | Front stencil reference | Reference value for the front face. |
| `[127:120]` | Back stencil reference | Reference value for the back face. |

The depth **range** is in the viewport record, not in this block: the Z scale and
offset place the incoming depth, and the record's depth minimum and maximum
bound it.

## Stencil face record

Each face has a 4-byte record: front at `0x334`, back at `0x338`.

| Bits | Field | Meaning |
|------|-------|---------|
| `[0]` | Enable | Run the stencil test for this face. |
| `[3:1]` | Function | Compare function. |
| `[6:4]` | Fail operation | Applied when the stencil test fails. |
| `[9:7]` | Depth-pass operation | Applied when both tests pass. |
| `[12:10]` | Depth-fail operation | Applied when stencil passes and depth fails. |
| `[23:16]` | Value mask | Bits compared against the reference. |
| `[31:24]` | Write mask | Bits written back. |

The reference value is **not** in the face record. It is in Depth Control:
front at `[119:112]` and back at `[127:120]`. A two-sided stencil draw therefore
has two independent references.

## Compare functions

| Value | Function |
|------:|----------|
| 0 | Never |
| 1 | Less |
| 2 | Equal |
| 3 | Less or equal |
| 4 | Greater |
| 5 | Not equal |
| 6 | Greater or equal |
| 7 | Always |

## Stencil operations

| Value | Operation |
|------:|-----------|
| 0 | Keep |
| 1 | Zero |
| 2 | Replace |
| 3 | Increment and clamp |
| 4 | Decrement and clamp |
| 5 | Increment and wrap |
| 6 | Decrement and wrap |
| 7 | Invert |

## Rules

- The depth test runs only when Depth enable is set. A disabled test passes and
  does not update the depth value.
- The stencil test runs per face. The face is selected by the primitive's
  facing, which is derived from the winding rule in
  [Rasterization](rasterization.md).
- A depth write updates the depth surface only when the test passes and Depth
  write is set.
- Polygon offset is applied before the depth comparison. Its unit and factor
  are scaled by the bound depth format.

## Open questions

- Whether near and far clamp apply to floating-point depth formats.
- The exact wrap and clamp boundaries for increment and decrement.
- How depth and stencil interact when only one of the two surfaces is bound.
- The legal combinations of depth and stencil formats.
