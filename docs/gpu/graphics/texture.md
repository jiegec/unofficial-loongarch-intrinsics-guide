# Texture and Sampling

The texture unit turns a coordinate tuple into filtered texel values. Two
records drive it: a 32-byte **image descriptor** describing the surface and a
16-byte **sampler descriptor** describing the level window, filters, wrap modes
and comparison. They share one 64-byte slot, and a shader names a slot, never an
API binding number. This chapter defines the slot and both records field by
field, the level-of-detail rule, the filter, wrap, border and compare state, the
coordinate tuples the sampling instructions read, and the formats the unit
decodes itself.

![Texture sampling path](../assets/fig-texture.svg)

*The sampling path. A three-to-ten register coordinate tuple and one 0x40-byte
slot drive it: the image record supplies extent, layout and format, the sampler
record the level window, filters, wrap and comparison, and the result is filtered
and converted before it is written.*

## Binding slot and descriptor table

A slot is 0x40 bytes: the 8-dword image descriptor at `+0x00`, a 16-byte gap at
`+0x20` that belongs to neither record and is written zero, and the 4-dword
sampler descriptor at `+0x30`. A sampling instruction takes the image dwords as
one scalar operand group and the sampler dwords as another; they are separate
sources even though they share a slot. The fragment stage reaches the table
through the 64-bit resource root in `s6:s7`, slot *n* at `root + n * 0x40`.
Each stage owns a table of its own, so a vertex or geometry texture operation
cannot alias the fragment table; the chain above it is in
[shaders/descriptors.md](../shaders/descriptors.md).

The slot number is a **physical** slot, not a logical texture unit. The units a
stage's shader samples are ranked by ascending unit number and packed into slots
0 through n-1: with units 1, 3 and 4 sampled and units 0 and 2 not, unit 1 takes
slot 0, unit 3 slot 1 and unit 4 slot 2. Physical slot *k* holds the *k*-th
lowest sampled unit, and the handle a shader is compiled with is `k * 0x40`,
the slot's byte offset from the resource root. A unit the shader never names
consumes no slot. The image and the sampler of one unit are the same slot, so one
byte offset is handed to the instruction as both sources, and a texel-fetch form
that reads no sampler state still needs the image record.

The table holds 16 sampler units plus one further slot, 17 slots or 0x440 bytes
per stage. That count is a host layout convention, not a documented hardware
bound: the unit addresses a slot by a byte offset the shader computes, so the
real limit is the table a driver allocates and the reach of the shader's root.

Every slot a sampling shader can reach MUST hold a well-formed record even when
nothing is bound there. A descriptor whose base address is 0 faults on the first
fetch, because VA 0 is unmapped in every client VM; a driver MUST point an
unbound unit at mapped memory, with a zero extent or a one-texel dummy.

## Image descriptor

The image descriptor is 32 bytes.

| Bits | Field | Meaning |
|------|-------|---------|
| `[39:0]` | Base Address | GPU virtual address of the image, in bytes. 40-bit client VA. |
| `[51:40]` | Minimum LOD | 12-bit. Not written; meaning UNRESOLVED. |
| `[57:52]` | Data Format | Texel layout, see below. |
| `[61:58]` | Number Format | Component interpretation, see below. |
| `[63:62]` | Memory Type Low | 2-bit. Not written; meaning UNRESOLVED. |
| `[77:64]` | Width | Base-level width in texels, 14-bit. |
| `[91:78]` | Height | Base-level height in texels, 14-bit. |
| `[94:92]` | Performance Mode | 3-bit. Written as 4; meaning UNRESOLVED. |
| `[95]` | Interlaced | Not written; meaning UNRESOLVED. |
| `[98:96]` | Destination Select X | 3-bit channel select for the written X component. |
| `[101:99]` | Destination Select Y | 3-bit channel select for the written Y component. |
| `[104:102]` | Destination Select Z | 3-bit channel select for the written Z component. |
| `[107:105]` | Destination Select W | 3-bit channel select for the written W component. |
| `[111:108]` | Base Level | First mip level the record describes, 4-bit. |
| `[115:112]` | Last Level | Last mip level, 4-bit; on a 4x surface, the sample layout's level count. |
| `[120:116]` | Tiling Index | 0 linear, 2 tile2. The only two values written. |
| `[121]` | Power Of Two Pad | Set for a padded row layout. |
| `[122]` | Memory Type High | 1-bit. Not written; meaning UNRESOLVED. |
| `[123]` | Texture Flip Y | Not written; meaning UNRESOLVED. |
| `[127:124]` | Type | 8 1D, 9 2D, 10 3D, 11 cube, 12 1D array, 13 2D array, 14 2D multisample, 15 2D multisample array. |
| `[140:128]` | Depth | 13-bit. Level depth for a 3D image; for an array, the layer count used as the third fetch bound. |
| `[154:141]` | Pitch | Row stride in texels of the storage layout, 14-bit. |
| `[172:160]` | Base Array | First array layer, 13-bit. |
| `[185:173]` | Last Array | Last array layer, 13-bit; a record above 0x1fff is rejected. |
| `[203:192]` | Minimum LOD Warning | 12-bit. Not written; meaning UNRESOLVED. |
| `[211:204]` | Counter Bank ID | 8-bit. Not written; meaning UNRESOLVED. |
| `[212]` | LOD hardware count | Enable for a hardware LOD counter. Not written; meaning UNRESOLVED. |
| `[213]` | Compression Enable | Not written. No compressed-texel path exists, see below. |
| `[214]` | Alpha is MSB | Not written; meaning UNRESOLVED. |
| `[215]` | Color Transform | Not written; meaning UNRESOLVED. |
| `[219:216]` | Lost Alpha Bits | Not written; meaning UNRESOLVED. |
| `[223:220]` | Lost colour bits | Not written; meaning UNRESOLVED. |

**Data format** names the texel layout: 1 R8, 2 R16, 3 RG8, 4 R32, 5 RG16,
6 R11G11B10, 9 RGB10A2, 10 RGBA8, 11 RG32, 12 RGBA16, 13 RGB32, 14 RGBA32,
21 depth, 34 RGB9_E5, 38 RGTC1, 39 RGTC2. There is no value for S3TC, BPTC or
ETC. **Number format** is 0 UNORM, 1 SNORM, 4 UINT, 5 SINT, 7 float, 9 sRGB. A
pair outside these sets is not a request the unit can service; a driver MUST NOT
write one. [Formats and layout](formats.md) maps interface formats onto the pair.

**Level addressing** has two shapes, and the shape decides what the level fields
mean. A 1D, 2D, 3D or rectangle view is **rebased onto its first level**: the
base address already includes that level's offset, width and height are that
level's extents, the base level is 0 and the last level is the view's level count
minus one; adding a level offset again would sample outside the image. An array
or cube view keeps the resource's geometry: the base address is level 0, width
and height are level-0 extents, the base level is the view's first level, the
last level is the **resource's** last mip rather than the view's, and the layer
range is base through last array. The unit derives the level chain from the
level-0 pitch plus the level fields, so a last level below the resource's end
mis-addresses every sampled level; the view's own bound is enforced by the
sampler's maximum LOD instead. A record whose base level exceeds its last level,
whose base array exceeds its last array, or whose pitch is zero is malformed.

**Pitch** counts texels of the storage layout along a row: bytes per row divided
by the bytes per stored element, times the storage format's block width.
**Depth** names the third dimension: on an array type the layer stride comes from
base and last array and the value is read as the third fetch bound, so it MUST be
at least the layer count or an out-of-range layer computes an address past the
mapping; on a cube type its meaning — 1, the face count or the layer count — is
UNRESOLVED, since the candidate encodings sample identically at level 0.

A **multisample** image uses type 14 or 15, and a 4x surface writes 2 into the
last level. Whether that is a log2 of the sample count or the level count of the
sample layout is UNRESOLVED; both readings yield 2 and only a non-4x surface
would separate them. Stencil has no data format of its own — the one depth format
returns depth bits — so a stencil-only view is described as an integer R8 or
RGBA8 image over the stencil bytes.

**Destination select** is four independent 3-bit fields, one per written
component, not one 12-bit subfield: 0 constant zero, 1 constant one, 4 texel X,
5 texel Y, 6 texel Z, 7 texel W. The select applies to the fetched texel after
the stored layout has been decoded, and to a border texel exactly as to a fetched
one, so a driver folds a storage channel order into the select and not into the
stored bytes: an R,G,B,A request over a B,G,R,A surface is Z, Y, X, W. Values 2
and 3 are not channel selects; what they select is UNRESOLVED and a driver MUST
NOT emit them.

## Sampler descriptor

The sampler descriptor is 16 bytes.

| Bits | Field | Meaning |
|------|-------|---------|
| `[2:0]` | Clamp X | Wrap mode for the first coordinate axis. |
| `[5:3]` | Clamp Y | Wrap mode for the second axis. |
| `[8:6]` | Clamp Z | Wrap mode for the third axis. |
| `[11:9]` | Maximum anisotropy | Base-2 logarithm of the ratio, 3-bit. |
| `[14:12]` | Depth Compare Function | Compare function, 0 for no comparison. |
| `[15]` | Force Unnormalized | Take texel coordinates instead of normalized ones. |
| `[18:16]` | Anisotropy Threshold | 3-bit. Not written; meaning UNRESOLVED. |
| `[19]` | Coordinate Truncate | Not written; meaning UNRESOLVED. |
| `[20]` | Force Degamma | Not written; meaning UNRESOLVED. |
| `[26:21]` | Anisotropy Bias | 6-bit. Not written; meaning UNRESOLVED. |
| `[27]` | Truncate Coordinates | Not written; meaning UNRESOLVED. |
| `[28]` | Disable Cube Wrap | 1 filters a cube face in isolation, 0 lets a footprint cross the face edge. |
| `[30:29]` | Filter Mode | 2-bit. Not written; meaning UNRESOLVED. |
| `[31]` | Compatibility Mode | Not written; meaning UNRESOLVED. |
| `[43:32]` | Minimum LOD | Unsigned 8.8 fixed point. |
| `[55:44]` | Maximum LOD | Unsigned 8.8 fixed point. |
| `[59:56]` | Performance MIP | 4-bit. Not written; meaning UNRESOLVED. |
| `[63:60]` | Performance Z | 4-bit. Not written; meaning UNRESOLVED. |
| `[77:64]` | LOD Bias | Signed 8.8 fixed point in a 14-bit field. |
| `[83:78]` | Secondary LOD Bias | 6-bit. Not written; meaning UNRESOLVED. |
| `[85:84]` | Magnification Filter | 0 nearest, 1 linear. |
| `[87:86]` | Minification Filter | 0 nearest, 1 linear. |
| `[89:88]` | Z Filter | 2-bit. Not written; meaning UNRESOLVED. |
| `[91:90]` | MIP Filter | 0 none, 1 nearest, 2 linear. |
| `[92]` | MIP Point Preclamp | Not written; meaning UNRESOLVED. |
| `[93]` | Disable LSB Ceil | Not written; meaning UNRESOLVED. |
| `[94]` | Filter Precision Fix | Not written; meaning UNRESOLVED. |
| `[95]` | Anisotropy Override | Enables the anisotropy fields. |
| `[107:96]` | Border colour pointer | 12-bit index into the border table; 0 selects the zeroed entry. |
| `[124:108]` | Reserved | Write zero. |
| `[125]` | Upgraded Depth | Not written; meaning UNRESOLVED. |
| `[127:126]` | Border colour type | 2-bit. Always written as 1, a table entry. |

The two LOD bounds are unsigned 8.8 values in 12 bits, so they express 0 through
15.99609375; an interface that allows a wider range MUST clamp before packing.
The bias is a signed 8.8 value in 14 bits, whose range is about -32 through
31.996; the range a driver packs is clamped to -16 through 15.99609375, which is a
host choice, not a field limit. The minimum must not exceed the maximum: an
inverted window is a descriptor error whose hardware result is UNRESOLVED, so a
driver MUST order or clamp the pair before writing it.

A non-mipmap minification filter MUST be programmed as mip filter 0, not as a
nearest mip filter with a zero-width window. With a nearest mip filter and
maximum LOD 0 every level of detail collapses onto the base level, and the unit
then treats every footprint as magnified and applies the magnification filter
even where the footprint is minified. Anisotropy is encoded but inert: the ratio,
threshold and bias fields are written and sampling stays isotropic, so a driver
SHOULD leave them zero.

## Level of detail

The unit converts a level of detail into a mip index and clamps it into the
sampler's window:

```text
level = clamp(LOD + bias, base_level + min_lod, base_level + max_lod)
```

`base_level` is the image descriptor's raw base-level field, `bias` the
sampler's LOD bias, `min_lod` and `max_lod` the sampler's bounds. The bias
passes through unshifted: it is not rebased onto the view's first level, and the
window is view-relative, so a view whose first level is not zero has the base
level added by the selector.

The source of `LOD` is a property of the instruction. An implicit sample derives
it inside the quad from the coordinate derivatives, as the base-2 logarithm of the
longer screen-space footprint axis. A bias sample adds a per-lane bias from the
tuple. An explicit-level sample takes its level from the tuple instead of deriving
one. A gradient sample takes two supplied derivative pairs, one per axis
direction. A level-zero form uses a constant level, which on an array view is the
view's base level plus the level operand.

The tuple's bias and the descriptor's bias are separate values, and whether the
tuple bias adds to the descriptor bias or replaces it is UNRESOLVED. A driver MUST
NOT assume either; keep the descriptor bias at 0 when a shader can also supply
one.

## Filtering and wrap

Magnification and minification each select 0 nearest or 1 linear. The mip filter
selects 0 none, 1 nearest or 2 linear: none samples one level and the LOD window
then only decides whether a footprint is in the magnification or the minification
region, nearest takes the nearer of the two levels bracketing the level of detail,
and linear blends both.

Wrap modes apply per axis: 0 repeat, 1 mirrored repeat, 2 clamp to edge, 3 mirror
clamp to edge, 6 clamp to border. Values 4, 5 and 7 are not wrap modes. An axis
the image type does not use carries no footprint, and repeat is the filler a
driver writes for it; all three clamp fields MUST hold one of the six values, so
an unused axis is never left at a value the unit might interpret. Clamp to border
is the only mode that reads the border table.

## Border colour

Border colours live in one fixed 0x10000-byte table at GPU address
`0x0000_0008_001a_8000`, indexed by the sampler's pointer field. An entry is 32
bytes: four floats at `+0x00` that the unit does not read, and one border texel
in the sampled image's format at `+0x10`, in the entry's remaining 16 bytes.

The unit decodes the texel area with the image descriptor's data and number
formats and then applies the descriptor's destination selects, exactly as for a
fetched texel. A driver MUST NOT pre-swizzle the colour: the select fields already
carry the channel order. A colour the chosen format cannot express has no defined
texel. The zeroed entry is transparent black, so a sampler that needs transparent
black can leave the pointer at 0 and never write the table; any other colour,
including opaque black, needs an entry written.

**The addressing unit is UNRESOLVED.** The pointer is a 12-bit index and the table
is 0x10000 bytes, which a 16-byte unit covers exactly and an 8-byte unit covers
only halfway. Two readings are in use: an entry base at `pointer * 8` with the
texel 16 bytes further on, and a texel at `pointer * 16`. They agree only for a
single entry at pointer 2, whose texel lands at byte 0x20 either way, so a table
with more than one written entry is on unresolved ground. The safe form is one
written entry per draw plus the zeroed default, with the pointer kept inside the
range both readings place in the table.

The table base must be announced to the command processor before the records that
use it. A six-dword preamble does that: opcode `0xa1`, the 64-bit table address,
the end-command opcode and two zero dwords. It is legal only at the head of a
command buffer, so a change of table base costs a submission boundary, not a
state-record field.

## Coordinate tuples

A sampling instruction encodes only the base of the coordinate tuple, an
8-register-aligned vector register; the tuple's shape fixes how many consecutive
registers it occupies, between three and ten. The widest shape, a 3D gradient
sample with an offset, spans ten registers. Slots are ordered, and each variant
fills only the slots it needs:

1. texel offset, when the variant has one;
2. LOD bias, when the variant has one;
3. depth-compare reference, when the variant compares;
4. first derivative pair, for a gradient form;
5. second derivative pair, for a gradient form;
6. the coordinates: `(u)` for 1D, `(u, v)` for 2D, `(u, v, w)` for a volume,
   `(s, t, face)` for a cube;
7. array layer, when the view is arrayed;
8. explicit level of detail, for a level form.

| Form | Tuple |
|------|-------|
| `samp` | `{u, v}` |
| `samp_b` | `{bias, u, v}` |
| `samp_l` | `{u, v, lod}` |
| `samp_d` | `{ddx.u, ddx.v, ddy.u, ddy.v, u, v}` |
| `samp_o` | `{offset, u, v}` |
| `samp_c` | `{ref, u, v}` |
| `gather_o` | `{offset, u, v}` |

The offset is not an encoded immediate: it is a tuple slot holding a signed
six-bit X/Y/Z displacement packed into one dword as
`x[5:0] | (y[13:8] << 8) | (z[21:16] << 16)`. The field spans -32 through 31; the
range an interface advertises for sampling is -8 through 7, while a gather offset
uses -32 through 31. Because the value travels in the tuple it is an ordinary
register, but the unit consumes **one offset per four-lane quad**: an offset that
varies inside a quad is not honoured, so a shader MUST keep it uniform across the
quad.

The depth-compare reference is likewise a tuple slot, a 32-bit float before the
coordinates. It is not a coordinate and does not shift the coordinate slots, but
every leading slot shifts the ones after it: an offset compare form reads
`{offset, ref, u, v}` and a compare form with a bias reads `{bias, ref, u, v}`.

## Cube sampling

A cube sample is projected in the shader, not by the unit. The unit receives three
values `(s, t, face)` and never the raw direction, so a shader emits:

```text
v_cubema_f32    ma,     x, y, z    ; twice the dominant component
v_cubes_f32     s,      x, y, z
v_cubet_f32     t,      x, y, z
v_cubeface_f32  face,   x, y, z
v_abs_f32       abs_ma, ma
v_rcp_f32       inv_ma, abs_ma
v_fma_f32       s,      s, inv_ma, bias
v_fma_f32       t,      t, inv_ma, bias
samp            dst,    s, t, face, ...
```

`v_cubema_f32` returns **twice** the dominant magnitude, so `s * (1 / |ma|)` is the
half-scale face coordinate in `[-0.5, 0.5]` and the bias term maps it onto the
face. The bias is 0.5 for a face coordinate the descriptor expects in `[0, 1]`; a
convention that adds a further +1.0 normalisation needs 1.5 instead, and the wrong
choice clamps a whole face onto its edge. The constant is therefore tied to the
descriptor convention, not to the instruction. The face order is
`|z| >= |t| >= |s|`, so a tie goes to the earlier axis of that order; `v_rcp_f32`
is not correctly rounded, so a projected coordinate within an ulp of a texel
boundary can select the neighbouring texel.

The sampler's cube-wrap field decides the edge: 1 filters each face in isolation,
0 lets a linear filter's footprint continue across the edge. The field is a
filter-quality choice, not a face-selection one.

## Gather

A gather instruction returns four texels of one 2x2 block instead of a filtered
value. For a sample point `P` in texel units with `(i0, j0) = floor(P)`, the
block is point-read, not filtered, into X `(i0, j0)`, Y `(i0 + 1, j0)`, Z
`(i0 + 1, j0 - 1)` and W `(i0, j0 - 1)`. The block extends toward decreasing *v*,
unlike the GL convention, and an offset shifts the block origin by a constant
integer displacement. A block that leaves the image at the negative edge reads
zero rather than clamping, and a partly out-of-bounds block keeps no in-bounds
texels. Whether the wrap mode is consulted at all, and what a block at a positive
edge reads, are UNRESOLVED.

## Depth comparison

A comparing sampler reads the depth texel, compares it with the reference from the
tuple using the sampler's 3-bit compare function, and writes 1.0 for pass and 0.0
for failure. The eight values are the ones
[Depth and stencil](depth-stencil.md) lists for the depth test: 0 never, 1 less,
2 equal, 3 less or equal, 4 greater, 5 not equal, 6 greater or equal, 7 always. A
0 in the field means no comparison, so a comparing sampler MUST program a function
other than 0.

The result is a scalar written to the components the instruction's write mask
selects. Whether a four-component mask broadcasts it or only writes the first
component is UNRESOLVED, so a shader SHOULD select one component. Whether the
compared value is the raw texel or a filtered one is also UNRESOLVED, so a
comparing sampler SHOULD use a nearest minification filter.

## Compressed formats

Three formats are decoded natively: RGB9_E5 (data format 34, 4 bytes per texel),
RGTC1 (38, 8 bytes per 4x4 block) and RGTC2 (39, 16 bytes per 4x4 block).
Everything else block-compressed — S3TC/BC1-3, BPTC/BC7, BC6H, ETC2 and EAC — has
no data-format value at all: the unit has no decoder for them. The
sampled surface for such an image is a plain RGBA8 image, or RGBA16 float for
BC6H, described by an ordinary image descriptor with data format 10 or 12, while
the format's own 4x4 blocks are kept in a separate shadow area of the same
allocation so that copies, uploads and readbacks still see the block layout.

The cost falls on the driver, not the unit. The allocation carries both surfaces:
the shadow at a row pitch of `ceil(width / 4) * block_bytes` and
`ceil(height / 4)` rows per slice, and the decoded surface at one stored element
per texel. Every write into the image is decoded by the producer and every readback
is re-encoded or served from the shadow, so a block image is not a direct copy
target: a copy into or out of it goes through the decoder or the shadow. The
descriptor addresses the decoded surface, so mip levels, layers and the array
stride all follow the decoded layout, not the block layout. An sRGB block format
decodes onto a surface whose number format is sRGB and a signed format onto an
SNORM surface; the decoder and the descriptor's number format MUST describe the
same encoding, or sampling returns values in the wrong numeric domain.

## Limits

| Limit | Value | Basis |
|-------|-------|-------|
| Image descriptor size | 32 bytes | Encoding |
| Sampler descriptor size | 16 bytes | Encoding |
| Slot stride | 0x40 bytes | Encoding |
| Mip levels per descriptor | 16 | 4-bit level fields |
| Width, height | 16383 texels | 14-bit fields |
| Depth | 8191 | 13-bit field |
| Pitch | 16383 texels | 14-bit field |
| Array layers | 8192, last array at most 0x1fff | 13-bit field |
| Sampler units per stage | 16 plus one slot | Host layout convention |
| Sampling offset | -32 through 31 encoded, -8 through 7 advertised | 6-bit field, host policy |
| Gather offset | -32 through 31 | 6-bit field |
| Border table | 0x10000 bytes at `0x0000_0008_001a_8000` | Fixed address |

## Open questions

- The meaning of the performance-mode field, and of every image field nothing
  writes: minimum LOD, memory type, interlaced, flip Y, the LOD warning, the counter
  bank and the loss bits.
- Whether the multisample last-level value is a log2 sample count or a layout level
  count, and whether a cube descriptor's depth is 1, the face count or the layer
  count.
- The border table's addressing unit, and therefore whether a multi-entry table is
  possible at all.
- Whether an anisotropic ratio has any effect on the footprint.
- Whether a tuple offset and a tuple bias are per-quad or per-lane, and whether the
  tuple bias adds to or replaces the descriptor bias.
- What the undefined destination-select values 2 and 3 select, and whether a
  compare result is broadcast to every masked component.
- Whether the wrap mode is consulted for a gather block that leaves the image.
