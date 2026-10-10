# Draw and Rasterization

A draw is described by Draw Info for the geometry and by Pipe Enables for the
rasterization controls.  Field tables are in
[The State Object](state-object.md); this chapter describes what the controls do
and how they interact.

## Draw description

Draw Info names the primitive type, the element or vertex count, the instance
count and the index buffer.  Three details matter when translating an API call:

* **Index size.**  Zero means there is no index buffer and vertices are fetched
  sequentially from the vertex offset; 2 and 4 select a 16-bit or 32-bit index
  buffer.
* **Vertex offset.**  The field carries the API's first-vertex or index-bias
  value.  The vertex stage receives `v0` with this bias already applied.
* **Vertices per patch.**  Written as 3 for the ordinary vertex role; the field
  belongs to the tessellation path, which does not exist here.

Primitive restart is controlled by a dedicated bit and by a restart index; a
host may also split a restart draw on the CPU.

## Primitive types

The primitive enumeration is this family's own:

| Value | Primitive | Value | Primitive |
|------:|-----------|------:|-----------|
| 0 | point list | 1 | line list |
| 2 | line loop | 3 | line strip |
| 4 | triangle list | 5 | triangle strip |
| 6 | triangle fan | 7 | quad list |
| 8 | quad strip | 9 | polygon |
| 10 | line list adjacency | 11 | line strip adjacency |
| 12 | triangle list adjacency | 13 | triangle strip adjacency |

A primitive outside this table is rejected before state is built.

## Rasterization controls

**Culling and winding.**  Front and back faces are selected independently, and
the effective front direction is the `Front CCW` bit combined with the sign of
the viewport's Y scale and the `Flip Coordinate Y` control:

```text
effective_front_ccw = front_ccw XOR (Y_scale < 0) XOR flip_coordinate_y
```

A host must therefore derive the winding bit from the API convention together
with the final viewport orientation, rather than from a fixed constant.

**Fill mode.**  Front and back fill modes are selected separately as fill, line
or point.  A polygon drawn in point mode still uses the polygon primitive type.

**Point and line size.**  Point size and line width are stored in the Line
Raster block in units of quarter pixels.  A fixed point size can be enabled so
that the size comes from state rather than from the shader.

**Line stipple.**  A stipple factor and a 16-bit pattern are available for lines.

**Polygon offset.**  Units, factor and clamp are supplied as floats, with
separate enables for point, line and fill modes.

**Multisampling.**  One bit enables 4x sampling for the draw.  The sample count
is not stored anywhere else, so a 4x render target and this bit must agree.

**Integer coordinates.**  A control selects pixel-centre integer coordinates.

**Scissor.**  Each viewport carries a scissor rectangle.  A degenerate or empty
scissor is how a draw is made to produce no fragments: it is not an error and
does not need a different command.

## Viewport transform

A viewport record holds X, Y and Z scale and offset as float patterns, plus the
scissor rectangle and the depth range:

```text
x_window = x_clip * X_scale + X_offset
y_window = y_clip * Y_scale + Y_offset
z_window = z_clip * Z_scale + Z_offset      clamped to [depth_min, depth_max]
```

A negative Y scale flips the coordinate system and, as noted above, also flips
the effective front-face direction.  The depth range lives in the viewport
record, not in the depth block.

Sixteen viewport records exist; the established path consumes viewport 0, and a
viewport index can be selected per draw through the Pipe Enables layer and
viewport fields.

## Guardband

Four float patterns give the minimum and maximum X and Y of the guardband.  A
primitive outside it is clipped before rasterization.  How the guardband
interacts with the clip and cull plane masks, and the precision of clipping, is
UNRESOLVED.

## Layer selection

The layer value and the viewport index are per-draw state, each with an enable
bit for the corresponding system output.  Layered rendering therefore does not
need a separate command per layer.

## UNRESOLVED

* The rasterization rules themselves: fill rules for shared edges, subpixel
  precision and the sample positions used for multisampling.
* Guardband clipping details and its interaction with the clip and cull plane
  masks.
* How primitive restart interacts with the restart bits when a strip is not
  split on the host.
* The behaviour of adjacency primitives, and of the polygon primitive type when
  it is drawn in point or line fill mode.
* Whether a scissor rectangle includes or excludes its edge pixels.
* The meaning of the sample-rate, smooth-mode and raster-scan selectors beyond
  the values a single producer uses.
