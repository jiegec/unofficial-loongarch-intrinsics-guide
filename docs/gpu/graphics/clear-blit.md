# Clear and Blit

Clears and blits run on BPIPE. They use the BPIPE draw-call payload rather than
the graphics state object. This chapter describes the operations and the
synchronisation a host must add around them. The payload fields are in
[BPIPE](../host/bpipe.md).

## Operations

| Operation | `draw_op` | Source |
|-----------|-----------|--------|
| Fill | 0 | Constant colour or a source texture. |
| Clear | 1 | Constant colour. |
| Blit | 2 | Source texture. |

A fill with `cst_fill` set uses the constant fill colour and needs no source. A
clear writes the constant colour to the box. A blit copies a source rectangle to
a destination rectangle.

## Box

The box is a pair of 16-bit coordinates:

| Field | Meaning |
|-------|---------|
| `box_x0, box_y0` | Origin. |
| `box_x1, box_y1` | End. |

The box is in destination pixels. A rotated blit uses `rot_xy` and the texture
coordinate deltas to rotate the source.

## Source and destination

The source is a texture record: base address, width, height, wrap, filter,
pattern, power-of-two pad, memory type, data format and pitch. The destination
is a surface record: base address, format, type, tiling, swizzle, width, height,
depth, pitch, layers, levels and alignment.

The format and tiling vocabulary is the same as the graphics state object. A
destination whose format or tiling does not match the box is a host error.

## Synchronisation

A clear or blit has no implicit ordering against a graphics draw. A host MUST
order a write-after-write or read-after-write hazard with a fence or a
submission dependency. A resource that a blit writes MUST remain resident until
the blit's fence signals.

A blit whose result must be read by the CPU is submitted standalone and waited on;
the current driver does this for clears that feed a later readback.

## Limits

| Limit | Value |
|-------|-------|
| Coordinate fields | 16 bits |
| Fill pattern | 128 dwords |
| Source pitch | 16 bits |
| Destination pitch | 16 bits |

## Open questions

- Whether a blit can read and write the same surface in one operation.
- The exact tiling conversion rules for a blit between different tilings.
- The behaviour of a box outside the destination surface.
