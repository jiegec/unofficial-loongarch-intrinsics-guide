# Multisampling

LG200 supports multisampled rendering. The state encodes 2x and 4x surfaces, and
the current driver exposes 4x. This chapter defines the state, the resource
layout and the resolve paths.

## Sample count

The sample count is not stored in the 512-dword state object, in the indirect
buffer or in the buffer size. It is encoded in the resource:

- the image or surface type is a multisample type;
- the image descriptor's last level is `log2(samples)`: 1 for 2x, 2 for 4x;
- the surface type is 8 for a 2D multisample surface, or 12 for a 2D
  multisample array.

A Pipe Enables bit enables multisampling for the draw. The bit does not carry
the sample count. A draw MUST use a surface whose type and last level agree with
the requested count.

## Supported counts

| Count | State encoding | Current driver |
|------:|----------------|----------------|
| 2x | Encodable | Not exposed |
| 4x | Encodable | Exposed |

> UNRESOLVED: whether the hardware supports 2x rendering with the same resolve
> paths as 4x, and whether a mixed 2x/4x attachment is legal.

## Colour layout

A 4x colour surface uses the tile2 layout with a sample dimension. The surface
record's alignment code is 6. A multisample colour attachment is cleared through
a CPU mapping; a 4x colour resolve runs as a GPU pass that reads the sample
plane.

## Depth and stencil layout

A 4x depth surface uses the interleaved sample map in
[Formats and layout](formats.md#multisample-layout). The rows are inverted, and
the layer span is a function of the pitch and the block size. Depth and stencil
resolve run through CPU transfers that read the sample plane and write a single
plane.

## Resolve

| Resource | Resolve path |
|----------|--------------|
| Colour | GPU pass that reads the sample plane. |
| Depth | CPU transfer. |
| Stencil | CPU transfer. |

A resolve is not a blend. A colour resolve copies the sample plane to a
single-sample surface; it does not use the blend equation. A host MUST NOT fold
a depth resolve into the colour pass.

## Alpha-to-coverage

Alpha-to-coverage requires 4x multisampling. A draw that requests it with any
other sample count MUST be rejected.

## Open questions

- The sample positions used by the rasterizer.
- Whether a 2x surface can be resolved through the same GPU path.
- The exact sample order for a 2x depth surface.
- Whether a multisample surface can be sampled by a shader.
