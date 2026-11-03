# Rasterization

The rasterizer turns assembled primitives into fragments. This chapter defines
the viewport, the guardband, culling, fill rules and the per-draw controls. The
state fields are in [State object](state-object.md); the draw description is in
[Draw](draw.md).

## Viewport

A viewport record is 40 bytes at state offset `0x048 + 0x2c * n`.

| Offset | Size | Field |
|-------:|-----:|-------|
| `+0x00` | 4 | X scale. |
| `+0x04` | 4 | X offset. |
| `+0x08` | 4 | Y scale. |
| `+0x0c` | 4 | Y offset. |
| `+0x10` | 4 | Z scale. |
| `+0x14` | 4 | Z offset. |
| `+0x18` | 2 | Scissor minimum X. |
| `+0x1a` | 2 | Scissor maximum X. |
| `+0x1c` | 2 | Scissor minimum Y. |
| `+0x1e` | 2 | Scissor maximum Y. |
| `+0x20` | 4 | Depth minimum. |
| `+0x24` | 4 | Depth maximum. |

The transform is:

```text
x_window = x_clip * X_scale + X_offset
y_window = y_clip * Y_scale + Y_offset
z_window = z_clip * Z_scale + Z_offset
```

The Z result is clamped to the record's depth minimum and maximum.

The viewport count is 16. The current driver writes viewport 0 and leaves the
others zero; a viewport index is not selected per draw by the established path.

## Guardband

The guardband is a 16-byte record at `0x308`: minimum X, maximum X, minimum Y
and maximum Y as float patterns. A primitive outside the guardband is clipped
before rasterization.

## Scissor

Each viewport carries a scissor rectangle. A degenerate scissor — an empty range
— produces no fragments. It is a legal state, not an error.

> UNRESOLVED: whether the scissor rectangle includes or excludes its edge
> pixels.

## Culling and winding

Front and back faces are culled independently by the Pipe Enables bits. The
effective front-face direction combines the `Front CCW` bit with the sign of the
viewport's Y scale:

```text
effective_front_ccw =
    front_ccw_bit XOR (Y_scale < 0) XOR (Y_scale >= 0)
```

The last two terms are complementary, so the stored bit is inverted relative to
the API convention. The `Flip Coordinate Y` control is not part of this
expression; the established path keeps it zero and lets the state tracker apply
its own Y transform.

## Fill mode

Front and back fill modes are selected independently: fill, line or point. A
polygon drawn in point mode still uses the polygon primitive type.

## Point and line

The Line Raster block at `0x318` holds:

| Offset | Size | Field |
|-------:|-----:|-------|
| `+0x00` | 4 | Stipple factor. |
| `+0x04` | 2 | Stipple pattern, 16 bits. |
| `+0x06` | 2 | Point size, in quarter pixels. |
| `+0x08` | 2 | Line width, in quarter pixels. |
| `+0x0a` | 2 | Reserved. |

A fixed point size is enabled by a Pipe Enables bit. The stipple factor register
is inert on the current hardware path; a stipple with a factor above one is
emulated in the fragment shader.

> UNRESOLVED: the exact point-size and line-width rasterization rules, and
> whether the stipple pattern is applied in screen or window coordinates.

## Polygon offset

Polygon offset has separate enables for point, line and fill modes. The units,
factor and clamp are float patterns in Depth Control. The unit is scaled by
`2^(25 - depth_bits)` for the bound depth format.

## Integer coordinates

A Pipe Enables bit selects pixel-centre integer coordinates. The bit changes the
fragment coordinate convention, not the vertex transform.

## Multisampling

A Pipe Enables bit enables multisampling for the draw. The sample count is not
stored in the state object; the surface type and the resource description carry
it. See [Multisampling](multisampling.md).

## Early depth and hi-Z

Two Pipe Enables bits select early depth and hi-Z behaviour. The exact
conditions under which a depth test is performed before the fragment stage are
not established:

> UNRESOLVED: the early-depth and hi-Z acceptance rules, and whether a shader
> that writes depth disables early depth for the draw.

## Clip and cull planes

The Pipe Enables word carries an 8-bit clip-plane mask and an 8-bit cull-plane
mask. A primitive outside a clip plane is clipped; a primitive outside a cull
plane is discarded.

## Open questions

- The subpixel precision and the fill rule for shared edges.
- The sample positions used for multisampling.
- How the guardband interacts with the clip and cull plane masks.
- The behaviour of adjacency primitives and of the polygon primitive in point
  or line fill mode.
