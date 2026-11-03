# Formats and Layout

This chapter defines the resource formats, the number formats and the memory
layouts that the state object and the descriptors refer to. The texture path is
in [Texture and sampling](texture.md); the state fields are in
[State object](state-object.md).

## Number formats

| Value | Format |
|------:|--------|
| 0 | UNORM |
| 1 | SNORM |
| 2 | USCALED |
| 3 | SSCALED |
| 4 | UINT |
| 5 | SINT |
| 7 | FLOAT |
| 9 | sRGB |

The number format is interpreted together with the data format. A format without
a suffix uses the number format to distinguish UNORM, SNORM and integer
interpretations on the same hardware encoding.

## Colour formats

| Value | Format | Value | Format |
|------:|--------|------:|--------|
| 0 | RGBA8 | 2 | RGBA16 float |
| 3 | RGBA32 float | 4 | RGBA8 SINT |
| 5 | RGBA16 SINT | 6 | RGBA32 SINT |
| 7 | RGBA8 UINT | 8 | RGBA16 UINT |
| 9 | RGBA32 UINT | 10 | RG8 |
| 12 | RG16 float | 13 | RG32 float |
| 14 | RG8 SINT | 15 | RG16 SINT |
| 16 | RG32 SINT | 17 | RG8 UINT |
| 18 | RG16 UINT | 19 | RG32 UINT |
| 20 | R8 | 22 | R16 float |
| 23 | R32 float | 24 | R8 SINT |
| 25 | R16 SINT | 26 | R32 SINT |
| 27 | R8 UINT | 28 | R16 UINT |
| 29 | R32 UINT | 32 | R11G11B10 float |
| 35 | RGBA8 sRGB | 44 | RGB10A2 |

Unassigned values are not usable. UNORM, SNORM and integer elements may share a
hardware encoding when the format is unsuffixed; the number format selects the
interpretation.

## Depth formats

| Value | Format |
|------:|--------|
| 37 | Z24 |
| 38 | Z32 float |
| 39 | Z24 plus stencil 8 |
| 40 | Z32 float plus stencil 8 |

A Z16 format is stored on the Z32 float format.

## Image data formats

| Value | Format | Value | Format |
|------:|--------|------:|--------|
| 1 | R8 | 2 | R16 |
| 3 | RG8 | 4 | R32 |
| 5 | RG16 | 6 | R11G11B10 |
| 9 | RGB10A2 | 10 | RGBA8 |
| 11 | RG32 | 12 | RGBA16 |
| 13 | RGB32 | 14 | RGBA32 |
| 21 | Depth | 34 | RGB9E5 |
| 38 | RGTC1 | 39 | RGTC2 |

## Buffer data formats

| Value | Format | Value | Format |
|------:|--------|------:|--------|
| 1 | R8 | 2 | R16 |
| 3 | RG8 | 4 | R32 |
| 5 | RG16 | 10 | RGBA8 |
| 11 | RG32 | 12 | RGBA16 |
| 13 | RGB32 | 14 | RGBA32 |

There is no RGB8 or RGB16 buffer tuple; a caller pads to RGBA.

## Image types

| Value | Type | Value | Type |
|------:|------|------:|------|
| 8 | 1D | 9 | 2D |
| 10 | 3D | 11 | Cube |
| 12 | 1D array | 13 | 2D array |
| 14 | 2D multisample | 15 | 2D multisample array |

## Surface types

| Value | Type |
|------:|------|
| 0 | 2D |
| 2 | 2D mip level |
| 4 | Layered |
| 6 | Layered mip level |
| 8 | 2D multisample |
| 12 | 2D multisample array |

There is no separate 1D, 3D or cube surface type.

## Tiling

Two layouts exist: linear (tiling index 0) and tile2 (tiling index 2). No other
value is used by the current path.

Tile2 arranges a level as 4x4 blocks in Morton order:

```text
row_elements = row_stride / block_size
tile         = (y >> 2) * (row_elements >> 2) + (x >> 2)
intra        = ((y & 2) << 2) | ((x & 2) << 1) | ((y & 1) << 1) | (x & 1)
offset       = (tile * 16 + intra) * block_size
```

## Alignment and pitch

- A level's row pitch is the next power of two at or above the level width, with
  a minimum row of 256 bytes for a tiled render target and for a 3D volume.
- The physical width is padded to eight elements and the height to four rows for
  a multi-level chain; level storage is 256-byte aligned.
- An array or cube image keeps the chain in one allocation; each level is
  256-byte aligned and the level sizes accumulate into the array stride.
- A block-compressed image is always linear, with one padded stride carried
  across the chain.
- A linear 2D image uses a power-of-two row pitch with a 64-element floor.
- The descriptor carries no slice stride: a 3D volume's slice step is the level
  pitch times the logical height.

A cube allocation is six times the logical layout. The alignment code of a
surface record is the base-2 logarithm of the level pitch for a mip-mapped cube
face, and 6 for a 4x multisample attachment and for every depth attachment. The
general meaning of the alignment code is UNRESOLVED.

## Compression

S3TC textures are decompressed by the hardware into the RGBA8 slots. The image
descriptor's compression-enable bit is not used by the current path. RGTC1 and
RGTC2 are explicit image data formats.

## Multisample layout

A 4x colour surface uses the tile2 layout with a sample dimension. A 4x depth
surface uses an interleaved sample map: the layer span is
`ceil(height / 4) * (pitch / 4) * 64 * block_size` bytes, and the per-sample
word of a texel `(x, y)` is

```text
h     = height - 1 - y
group = (pitch / 4) * 0x100
off   = (h / 4) * group
      + ((h >> 1) & 1) * 0x80
      + (h & 1) * 0x20
      + (x / 4) * 0x100
      + ((x % 4) >= 2 ? 0x40 : 0)
      + ((x & 1) ? 0x10 : 0)
      + sample * 4
```

The rows are inverted. This map is specific to the 4x depth layout; a 2x
layout is UNRESOLVED.

## Open questions

- The 12-bit colour swizzle subfields, beyond the per-format constants.
- The meaning of the performance-mode field.
- Tiling values other than 0 and 2.
- Surface types 2 and 6.
- The 2x multisample layout.
