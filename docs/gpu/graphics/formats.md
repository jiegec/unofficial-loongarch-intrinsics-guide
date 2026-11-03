# Formats and Layout

A texel is described by two words: a **data format**, which fixes the component
count and element width, and a **number format**, which fixes how the bits are
read — normalized, scaled, integer, float or sRGB. A colour or depth attachment is
the exception: its surface word carries one combined *hardware format* value
instead. This chapter gives both vocabularies, the units of the pitch, tiling and
alignment fields, and the address maps for linear, tile-2, block-compressed and
multisampled surfaces. Texturing is in [Texture and sampling](texture.md); the
state words that carry these fields are in [State object](state-object.md).

## Where each format lives

| Word | Field | Bits | Enum |
|------|-------|------|------|
| Colour buffer | Hardware Format | `[71:64]` | colour hardware format |
| Depth buffer | Hardware Format | `[71:64]` | depth hardware format |
| Image descriptor | Data Format | `[57:52]` | image data format |
| Image descriptor | Number Format | `[61:58]` | image number format |
| Buffer descriptor | Data Format | `[114:111]` | buffer data format |
| Buffer descriptor | Number Format | `[110:108]` | buffer number format |

A render target therefore has one eight-bit field where a sampler has a six-bit
data format plus a four-bit number format. The 8-bit layouts have their own UNORM
entries (RGBA8 is 0, RG8 is 10, R8 is 20) and the 16-bit UNORM form rides the UINT
entry; there is no SNORM entry at all, so an SNORM target rides the SINT entry of
the same layout and the fragment export quantizes. Blending is unavailable for SNORM,
UNORM16 and packed-integer RGB10A2 exports, because the blend unit would work on
the raw integer rather than on a normalized value. RGB10A2 is the one layout with a
dedicated normalized entry (44); its integer form rides an R32 entry and exports
packed integers.

## Colour hardware formats

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

Unassigned values are not usable. A 16-bit packed RGB5A1 target has no tuple of
its own: it is stored in entry 0 with an identity channel order, so its texels
occupy four bytes and every layout computation for it must use that width.

## Depth formats

| Value | Format |
|------:|--------|
| 37 | Z24 |
| 38 | Z32 float |
| 39 | Z24 plus stencil 8 |
| 40 | Z32 float plus stencil 8 |

There is no Z16 entry. A 16-bit depth attachment is a Z32 float surface (entry 38)
holding the 16-bit UNORM value widened to float, and the depth unit accepts such a
surface only in that float entry. The fixed-point depth formats are decoded through
data format 21, which reads fixed point only and returns zero for float words, so a
float depth surface is sampled as R32 float and an eight-byte depth/stencil surface
as RG32 float, whose element width matches the texel stride.

## Number formats

| Value | Interpretation | Buffer | Image |
|------:|----------------|--------|-------|
| 0 | UNORM | yes | yes |
| 1 | SNORM | yes | yes |
| 2 | USCALED | yes | no |
| 3 | SSCALED | yes | no |
| 4 | UINT | yes | yes |
| 5 | SINT | yes | yes |
| 7 | float | yes | yes |
| 9 | sRGB | no | yes |

A scaled interpretation is available only to a buffer fetch, which in practice
means the vertex-fetch path, and sRGB only to a sampled image: a colour target
expresses its sRGB form through its own hardware format instead.

## Buffer data formats

| Value | Format | Value | Format |
|------:|--------|------:|--------|
| 1 | R8 | 2 | R16 |
| 3 | RG8 | 4 | R32 |
| 5 | RG16 | 10 | RGBA8 |
| 11 | RG32 | 12 | RGBA16 |
| 13 | RGB32 | 14 | RGBA32 |

There is no RGB8 and no RGB16 tuple: a tight three-component buffer must be padded
by the caller and described as RGBA8 or RGBA16 with the fourth channel select
reading one. RGB32 exists and needs no padding.

## Image data formats

| Value | Data format | Value | Data format |
|------:|-------------|------:|-------------|
| 1 | R8 | 2 | R16 |
| 3 | RG8 | 4 | R32 |
| 5 | RG16 | 6 | R11G11B10 |
| 9 | RGB10A2 | 10 | RGBA8 |
| 11 | RG32 | 12 | RGBA16 |
| 13 | RGB32 | 14 | RGBA32 |
| 21 | depth | 34 | RGB9E5 |
| 38 | RGTC1 | 39 | RGTC2 |

RGB9E5 (34) is the only shared-exponent format and is sampled with number format 7.
Formats 38 and 39 are the only block-compressed entries; every other compressed
family must be decoded before it reaches the sampler.

## Image types

| Value | Type | Value | Type |
|------:|------|------:|------|
| 8 | 1D | 9 | 2D |
| 10 | 3D | 11 | cube |
| 12 | 1D array | 13 | 2D array |
| 14 | 2D multisample | 15 | 2D multisample array |

The type is a property of the image descriptor, not of the allocation: one 2D-array
allocation can be described as an array or as a single layer by narrowing the array
range, and a 3D allocation can be described as 2D or 2D array only when the
descriptor names exactly one mip level, because a slice of a 3D image has no mip
chain of its own.

## Surface types

| Value | Type |
|------:|------|
| 0 | 2D |
| 2 | 2D mip level |
| 4 | layered |
| 6 | layered mip level |
| 8 | 2D multisample |
| 12 | 2D multisample array |

There is no separate 1D, 3D or cube surface type: a layered attachment — an array, a
volume spanning several slices, a cube — serializes as type 4, and a cube with a mip
chain as type 6. A single slice of an array or volume is described by rebasing the
surface base onto that slice and keeping type 0, but a cube face keeps the layered
type and names its one face in the layer range. Type 2 is never emitted.

**The sample count is not in the surface word.** Type 8 covers both 2x and 4x, and
nothing elsewhere in the command stream distinguishes them: a driver that needs the
count must keep it in its own resource state. 4x is the only count that can be
allocated, so 2x and 8x are refused rather than encoded.

## Renderable and filterable

Only a colour hardware format can be a colour attachment and only a depth hardware
format can be a depth/stencil attachment; the two vocabularies do not overlap. The
sampled data formats include surfaces that no attachment can name — RGB9E5, RGTC1,
RGTC2, the depth decoder — and the colour set includes one format the sampler
cannot read at all: a 16-bit packed RGB5A1 target is a render target only, so it
cannot be sampled as itself.

Filtering follows the number format rather than the layout. A UINT or SINT image
supports point sampling only, because integer texels have no defined
interpolation; float, UNORM, SNORM and sRGB images can be filtered linearly.

## Tiling

Two tiling indices are used: 0 for a linear surface and 2 for tile-2. The image
descriptor's Tiling Index field is five bits wide, but no other value is emitted and
the meaning of the rest is UNRESOLVED. A colour or depth surface carries the same
two values in its Tiling Mode field, and the sampler and the render target read the
index from the same allocation, so an image is uploaded and read back through the
map its index names.

Tile-2 stores each level as independent 4x4 blocks in Morton order. With `stride`
the level's row stride in bytes and `block` the storage block size:

```text
pitch  = stride / block                (elements per row)
tile   = (y >> 2) * (pitch >> 2) + (x >> 2)
intra  = ((y & 2) << 2) | ((x & 2) << 1) | ((y & 1) << 1) | (x & 1)
offset = (tile * 16 + intra) * block
```

The map stays valid for a level narrower than four elements: with `pitch` below
four there is one tile column and every texel lives in tile 0, so mip levels may
shrink below the tile width without breaking it.

## Pitch, level height and level size

Pitch counts **elements**, never bytes. An element is one texel of the *storage*
format, so a 16-bit-exposed D16 surface on a four-byte Z32 float allocation counts
four-byte elements, and a block-compressed format counts blocks times the block
width, that is four texels per 8- or 16-byte block. A factor-of-two error here is
the difference between a correct image and one stretched along *y*.

For the shared plane geometry — plain 2D, 3D, array and cube — with `block` the
storage block size, `w` the level-0 width and `levels` the number of mip levels:

| Quantity | Rule |
|----------|------|
| level-0 pitch | next power of two at or above `w` |
| minimum pitch | 256 bytes / `block` for a tiled colour render target, and for a 3D volume |
| layered pitch floor | at least `2^(levels-1)`, so the deepest level keeps one element |
| level pitch | halved per level: floored at four elements on a plain chain, unfloored on a layered one |
| level height | level-0 height rounded up to a power of two, halved per level, then rounded up to four rows |
| level size | level pitch times level height, rounded up to 256 bytes |
| array stride | sum of the level sizes |

The descriptor carries one pitch — the level-0 pitch — plus base and last level, and
the sampler derives every deeper level by halving it. Rounding each level's own pitch
up to eight elements instead makes the planes
wider than the hardware's from the first level below eight elements, after which
every level offset and every array slice drifts. Rows are padded in whole four-row
groups because the tile is four rows tall, so a height that is not a multiple of
four costs up to three rows per level, and a clear that writes only `h` rows leaves
the tail of the last group untouched.

Three surfaces need their own arithmetic:

- **Cube.** The allocation holds six logical faces, six times the level chain, while
  the surface word keeps one base address and names the face range in its layer
  fields.
- **3D volume.** There is no slice-stride field; the hardware derives the slice step
  from the level pitch and the level height rounded to the tile height. A
  single-level volume is packed by its logical height, while a mipmapped one
  declares a power-of-two chain through the descriptor's power-of-two pad bit and is
  addressed as `pow2_up(depth) >> level` slices.
- **Block-compressed image.** Always linear, never tile-2, even with a mip chain.
  The level-0 pitch is the next power of two at or above the width in blocks with a
  64-block floor, each level halves it down to one block, and the 256-byte floor
  applies to the level size rather than to the pitch.

A single-level 2D colour image may instead be described with the same plane geometry
and tiling index 0, which drops only the swizzle: the rows are then back to back.
Every surface that takes
the plain row-major fallback — 1D, rectangle, colour formats the tiled path does not
take — uses a power-of-two pitch with a 64-element floor, pads a colour height to
four rows and a depth height to a power of two.

## The alignment code

The colour and depth surface words carry a four-bit Alignment field. Three values
are emitted: 0, 6, and the base-2 logarithm of the level pitch. Six is used for a 4x
multisample attachment and for every depth attachment; the logarithm is used for a
mipmapped cube face. What the hardware does with the field, and what the other
thirteen values mean, is UNRESOLVED. A driver must reproduce the three values it can
derive and must not invent a fourth.

## Compression

Texel-compressed sampling exists for exactly two block layouts, data formats 38 and
39. Everything else — S3TC/BC1–3, BC6H, BC7, ETC2, EAC — has no decoder on the
sampling path: the LG200 texture unit is byte-addressed and decodes no compressed
block. Such an image must be decompressed before it is sampled, and the sampleable
surface is then an ordinary RGBA8 image, or an RGBA16 float image for the HDR block
format. The blocks may be kept in a shadow range so that a transfer reads back what
was written, but the surface the sampler sees is the decoded one.

A compressed image is therefore never a render target and never tile-2, and a
layered compressed image cannot be described natively at all: the tile-2 layer
arithmetic addresses texels rather than 4x4 blocks, so a compressed cube or array
must also be decoded. The image descriptor's compression-enable bit is not set by
any path: a surface is compressed by virtue of its data format or not at all, and
whether the bit enables a hardware scheme for some other surface type is
UNRESOLVED.

## Multisample layout

4x is the only sample count that can be allocated. A 4x allocation declares tiling
index 2 like a tiled surface, but it is not addressed by the tile-2 map, and the
surface word does not carry the sample count, so the geometry comes from the
resource state and the descriptor's level fields.

A 4x layer is allocated over whole four-row groups of four sample positions:

```text
rows  = ceil(height / 4) * 4          (whole four-row groups)
layer = stride * rows * 4             (four samples per texel)
```

For a 4x depth/stencil surface the position of each sample inside that span is
established, and the depth clear, the readback and a scissored clear all compute it.
With `block` the storage block size and `pitch` in elements:

```text
tile   = 64 * block
group  = (pitch / 4) * tile
offset = (y / 4) * group
       + ((y >> 1) & 1) * 32 * block
       + (y & 1) * 8 * block
       + (x / 4) * tile
       + ((x % 4) >= 2 ? 16 * block : 0)
       + ((x & 1) ? 4 * block : 0)
       + s * block
```

Every term scales with the storage block size, so a one-byte and an eight-byte
depth/stencil format share the map. The `y` is the row inside the surface, counted
from the first stored row; converting a window-space row is the caller's job, not
this map's. A scissored clear of a 4x surface walks the same map, colour included,
but a second consumer is not an independent confirmation of it.

Whether a 4x *colour* region is placed by the same map is UNRESOLVED. Two readings
fit the sources: this interleaved map, and a segmented placement that divides a
layer into fixed-size segments, leaves part of the span unused and rotates the
channels of the exported samples. The sources establish only that a 4x colour
surface is not addressed by the tile-2 map, that it is allocated over the same
four-row-group span, and that its descriptor must declare a fixed level shape (last
level 2) instead of the resource's mip range. A driver MUST NOT compute a colour
sample address from the depth map; a colour sample is read back through the resolve.

A 2x layout is UNRESOLVED as well, and no 2x resource can be allocated.

## Open questions

- The meaning of alignment codes other than 0, 6 and the pitch logarithm.
- The performance-mode field: three bits, always written as 4, meaning unknown.
- Tiling indices other than 0 and 2.
- Surface type 2 ("2D mip level"): a mipmapped 2D attachment is a type-0 surface
  with its base rebased onto the level, so nothing exercises the field.
- The internal encoding of the 12-bit colour swizzle. The per-format constants
  (0x688 for RGBA order, 0x60a for BGRA) are fixed, but the set of legal per-channel
  select values is not established.
- The placement of a 4x colour region inside its layer, and the 2x layout.