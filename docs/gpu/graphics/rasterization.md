# Rasterization

The rasterizer turns assembled primitives into fragments and decides which of
them reach the depth and colour units. Its controls are the 12-byte pipe-enable
word at `0x030`, sixteen viewport slots at `0x048 + 0x02c·n` for `n < 16`,
a 16-byte guardband at `0x308` and a 12-byte line-raster record at `0x318`;
the polygon offset sits in Depth Control at `0x324`. This chapter gives the
layout of each, the arithmetic a driver applies when filling it, and the
behaviour the encoding leaves open.

Offsets are bytes into the 512-dword state object; [State
object](state-object.md) maps the rest of the block and tabulates every bit of
the pipe-enable word.

## Viewport

A viewport record holds one window transform, one scissor rectangle and one
depth range. The record is 40 bytes at `0x048 + 0x02c·n`; the last slot ends at
`0x307`, where the guardband begins, and the four bytes after each 40-byte
record carry no defined field.

| Byte | Size | Field | Encoding |
|-----:|-----:|-------|----------|
| `+0x00` | 4 | X Scale | float |
| `+0x04` | 4 | X Offset | float |
| `+0x08` | 4 | Y Scale | float |
| `+0x0c` | 4 | Y Offset | float |
| `+0x10` | 4 | Z Scale | float |
| `+0x14` | 4 | Z Offset | float |
| `+0x18` | 2 | Scissor Min X | unsigned |
| `+0x1a` | 2 | Scissor Max X | unsigned |
| `+0x1c` | 2 | Scissor Min Y | unsigned |
| `+0x1e` | 2 | Scissor Max Y | unsigned |
| `+0x20` | 4 | Depth Min | float |
| `+0x24` | 4 | Depth Max | float |

The transform is affine, and its coefficients are IEEE single precision rather
than fixed point: `x_window = x_ndc * X Scale + X Offset`, and likewise for Y
and Z. A driver writes half the rectangle extent as the scale and the rectangle
centre as the offset, so `X Scale = width / 2` and `X Offset = x + width / 2`;
a negative Y scale mirrors the primitive vertically. The Z pair carries the
API's normalized depth onto Depth Min and Depth Max, and its coefficients follow
that API's normalized depth range: half the range with the midpoint as the
offset where normalized z runs from -1 to +1, the whole range with the near
value as the offset where it runs from 0 to 1. Whether the rasterizer clamps to
those two bounds or only compares against them is UNRESOLVED; the near and far
clamp bits in Depth Control are separate controls.

A four-bit Viewport Index and a Viewport Index Enable bit in the pipe-enable
word name one slot for a whole draw. Whether the
rasterizer acts on them is UNRESOLVED: the geometry-stage shader info has a
matching output flag, but no established path carries a per-primitive index to
the rasterizer.

## Guardband

The guardband is 16 bytes at `0x308` and is one record per draw, not one per
viewport, so a multi-viewport draw fills it once. It is four floats — minimum X,
maximum X, minimum Y, maximum Y — in normalized device coordinates.

A driver derives a half-extent per axis, `extent = max((16383 - abs(offset)) /
abs(scale), 0)`, and writes it into both fields of that axis, so the record
describes a symmetric range about the origin. The constant 16383 is the mapped
window coordinate the extent is derived from: a vertex whose `ndc * scale +
offset` stays within ±16383 stays inside the extent. The extent is the boundary
between geometry the rasterizer processes directly and geometry that must be
clipped first; the scissor then limits the fragments of a primitive that
survives. A zero scale gives a zero extent.

UNRESOLVED: what happens at and beyond the extent — whether a primitive that
crosses it is clipped against the guardband or against the surface, or discarded
— and whether the four fields are independent bounds, so that an asymmetric
guardband is expressible; only two magnitudes are written into them. Nor does
the encoding say what the rasterizer does with a primitive that maps past 16383
when the record is set wider than the formula.

## Scissor

Each viewport record carries its own scissor rectangle, so the scissor is per
slot rather than per draw. The four fields are 16-bit unsigned window
coordinates. A driver encodes the maximum as `offset + extent` and clamps both
ends into the surface, which makes the encoded rectangle half-open. The scissor
removes fragments after they are positioned; it does not move a vertex, change
the viewport transform, or relax the guardband.

`min == max` on either axis is encodable and describes an empty rectangle; a
rectangle whose minimum exceeds its maximum is rejected when the record is
built. What an empty rectangle covers is UNRESOLVED, and so is whether the
rasterizer includes or excludes the maximum column and row. The pipe-enable bit
that a driver might otherwise use to discard such a draw counts primitives for
query and streamout draws; its earlier reading as a raster discard is refuted.

## Culling and winding

Front Face Cull (bit 4) and Back Face Cull (bit 5) are independent. Raising both
discards every primitive that has a facing.

Front CCW (bit 8) selects the front winding. The value is the inverse of a
front-face-counter-clockwise flag: clear when counter-clockwise is front, set
when clockwise is front. A driver that copies its API flag straight into this
bit culls the opposite faces from the ones the application asked for.

UNRESOLVED: the frame in which the rasterizer measures winding. The field is
named for counter-clockwise, but nothing in the encoding says whether the
comparison happens before or after the viewport's Y scale is applied, so a
mirrored and an unmirrored viewport may need opposite values.

## Fill mode

Fill Front (bits 12–13) and Fill Back (bits 14–15) choose fill (0), line (1) or
point (2) for the two facings independently; an API with a single polygon mode
writes the same value twice. Fill mode is not a primitive type: the primitive
comes from the Draw Info record and keeps its type at every fill mode, and the
mode only decides how the primitive's area is sampled. UNRESOLVED: what a point
or line fill mode covers on a quad or polygon primitive.

## Point and line

Line Raster, at `0x318`, holds the stipple and the two widths.

| Byte | Size | Field | Encoding |
|-----:|-----:|-------|----------|
| `+0x00` | 4 | Stipple Factor | unsigned |
| `+0x04` | 2 | Stipple Pattern | 16 bits, one bit per fragment |
| `+0x06` | 2 | — | no defined field |
| `+0x08` | 2 | Point Size | quarter pixels |
| `+0x0a` | 2 | Line Width | quarter pixels |

The rasterizer advances the 16-bit pattern one bit per fragment along the line
and ignores Stipple Factor: every factor produces the unit dash length, and a
large one switches stippling off. A driver that must honour a repeat factor
expands the pattern into a 16-bit-periodic equivalent, or applies the pattern in
the fragment shader. Line Stipple (bit 3) gates the pattern; the pattern index
is per fragment, not per screen column. Both widths are quarter-pixel counts in
16 bits, so the largest is 16383.75 pixels; a driver converts a width in pixels
by multiplying by four and clamping into `1 .. 65535`.

Fixed Point Size Enable (bit 26) takes the point diameter from Point Size
instead of the value the vertex or geometry stage exports per point. UNRESOLVED:
whether the field is honoured, and at what value; a large point requested
through it draws a single pixel.

## Polygon offset

The three offset floats are the first three dwords of Depth Control at
`0x324`: Units at `+0x00`, Factor at `+0x04`, Clamp at `+0x08`. Three
pipe-enable bits apply them per primitive class — Offset Point (20), Offset
Line (21), Offset Fill (22) — so a draw that mixes classes enables each of them
separately.

Units is the constant term: the rasterizer multiplies it by the smallest depth
difference the bound format can resolve. The unit is depth-range-relative, not
absolute: an API that defines it as `2^-n` for an `n`-bit fixed-point range
must multiply by `2^(25 - n)` — 512 for 16-bit depth, 2 for 24-bit — which
places the record's unit at `2^-25` of the range. Storing the API number
unchanged shrinks the offset by that factor. Factor scales the primitive's
maximum depth slope, and Clamp bounds the sum.

UNRESOLVED: whether the sum is evaluated per primitive or per fragment; whether
a positive unit moves the primitive towards the viewer or away from it; and how
a float depth attachment, which has no `2^-n` unit, maps onto the record's
unit.

## Clip and cull planes

Byte 4 of the pipe-enable word is the clip-plane mask and byte 5 the cull-plane
mask, eight bits each, one per plane. A clip plane cuts a primitive and creates
vertices on the cut; a cull plane discards a primitive that lies outside it,
creating none. Both read distances exported by the vertex or geometry stage.

The cull mask does not index a plane array in the record: its bit position
selects a channel of the position payload after the clip distances. Where clip
and cull distances share one array, the first cull plane follows the last clip
plane; where the cull distances have their own varying group, the base is the
clip-plane count rounded up to a multiple of four. A driver that starts the cull
mask at bit 0 tests the clip distances instead. A driver that folds a synthetic
near plane into the clip array clears that plane's bit when the depth state
clamps instead of clipping.

## Early depth and hi-Z

Early Z (bit 6) runs the depth test before the fragment stage. It is a property
of the shader as much as of the draw: a shader that discards, writes depth or
performs an ordered write can overturn the depth result after an early test, so
the bit belongs only to shaders that declare an early test. Hi Z (bit 7) enables
the hierarchical depth path; the encoding declares the bit and nothing else.

UNRESOLVED: which fragment-stage effects force the test late, whether a
depth-writing shader clears the early path by itself, and the whole behaviour of
Hi Z — whether it needs a resolved depth buffer and what it does when the buffer
is not one.

## Coordinates and sampling

Integer Coordinates (bit 60) selects a pixel-centre integer mode for fragment
coordinates. UNRESOLVED: what the mode changes — the sample points or only the
coordinate the shader reads.

The state block carries no sample count, and neither does the surface word: a
two-sample and a four-sample attachment serialize the same state, the same
surface word and the same buffer size. MSAA Enable (bit 48) marks a multisampled
draw, and Sample Rate (bits 56–57) selects the shading rate — fragment-shading
passes per pixel minus one — which is honoured only while MSAA Enable is set.
Which field, if any, names the number of samples of the target is UNRESOLVED;
see [Multisampling](multisampling.md).

Flip Coordinate Y (bit 17) makes the rasterizer emit a top-down fragment
coordinate and a top-down interpolated Y. It mirrors the fragment coordinate,
not the geometry, so setting it while the API already applies its own Y
transform mirrors the coordinate twice. Flip Y (16) and Invert Vertex Position Y
(18) are two further vertical controls; which of the three mirrors geometry and
which mirrors the coordinate is UNRESOLVED. Sprite Origin (19) selects the
upper-left origin for point-sprite texture coordinates. Raster Order (bits
10–11) and Raster Scan (bits 52–55) select the fragment walk and the scan
pattern; which value selects which pattern, and whether the choice is visible in
the result, is UNRESOLVED.

## Open questions

- Subpixel precision, and the fill rule at a shared edge. Bottom Edge Rule
  (bit 9) selects between two tie-breaks; which level is top-left and which is
  bottom-right is not established.
- How a mirrored viewport combines with Front CCW, and how the guardband
  interacts with the clip and cull masks.
- The behaviour of adjacency primitives, and of the polygon primitive in point
  or line fill mode.
- Whether Point Size is read at all.
