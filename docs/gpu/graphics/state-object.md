# State Object

A graphics draw carries one 512-dword state object. The object holds the draw
description, the rasterization and output-merger state, the surface records and
the shader records. This chapter is the map of the object and the field tables
for the blocks that are not defined in a dedicated chapter.

State offsets are relative to the first dword of the object. The packet byte
offset is the state offset plus the 12 bytes of the record header.

## Map

| Offset | Size | Block | Written per draw |
|-------:|-----:|-------|------------------|
| `0x000` | 0x24 | [Draw Info](draw.md#draw-info) | Yes |
| `0x024` | 0x0c | Draw tail | Never |
| `0x030` | 0x0c | [Pipe Enables](#pipe-enables) | Yes |
| `0x03c` | 0x0c | Pipe tail | Never |
| `0x048` | 16 × 0x2c | [Viewport](rasterization.md#viewport) | Index 0 only |
| `0x308` | 0x10 | [Guardband](rasterization.md#guardband) | Yes |
| `0x318` | 0x0c | [Line Raster](rasterization.md#point-and-line) | Yes |
| `0x324` | 0x10 | [Depth Control](depth-stencil.md#depth-control) | Yes |
| `0x334` | 0x04 | [Stencil, front](depth-stencil.md#stencil-face-record) | Yes |
| `0x338` | 0x04 | [Stencil, back](depth-stencil.md#stencil-face-record) | Yes |
| `0x33c` | 0x04 | [Blend Control](blending.md#blend-control) | Yes |
| `0x340` | 8 × 0x04 | [Blend per target](blending.md#per-target-blend-word) | Up to the attachment count |
| `0x360` | 0x10 | [Blend constants](blending.md#blend-constants) | Yes |
| `0x370` | 8 × 0x20 | [Colour buffers](#colour-buffer) | Up to the attachment count |
| `0x470` | 0x20 | [Depth buffer](#depth-buffer) | When depth is bound |
| `0x490` | 0x30 | Pipeline configuration | Never |
| `0x4c0` | 0x08 | [Shader Config](#shader-config) | Yes |
| `0x4c8` | 0x08 | [Vertex stage config](#vertex-stage-config) | Never |
| `0x4d0` | 0x08 | [Hull stage config](#hull-stage-config) | Never |
| `0x4d8` | 0x08 | [Domain stage config](#domain-stage-config) | Yes |
| `0x4e0` | 0x08 | [Geometry stage config](#geometry-stage-config) | When geometry runs |
| `0x4e8` | 0x08 | [Streamout stage config](#streamout-stage-config) | When streamout runs |
| `0x4f0` | 32 × 0x02 | [Fragment attribute map](#fragment-state) | All 32 entries |
| `0x530` | 0x08 | [Fragment input config](#fragment-state) | Yes |
| `0x538` | 0x04 | [Fragment output config](#fragment-state) | Yes |
| `0x53c` | 0x04 | Fragment tail | Never |
| `0x540` | 0x60 | [Shader Info, slot 0](#shader-info-slots) | Prefetch copy |
| `0x5a0` | 0x60 | [Shader Info, slot 1](#shader-info-slots) | Never |
| `0x600` | 0x60 | [Shader Info, slot 2](#shader-info-slots) | Yes |
| `0x660` | 0x60 | [Shader Info, slot 3](#shader-info-slots) | When geometry runs |
| `0x6c0` | 0x60 | [Shader Info, slot 4](#shader-info-slots) | When streamout runs |
| `0x720` | 0x60 | [Shader Info, slot 5](#shader-info-slots) | Yes |
| `0x780` | 0x80 | State tail | Never |

The object starts zeroed. A block marked "never" is not written by the current
path; a consumer MUST NOT read meaning into it.

## Pipe Enables

Pipe Enables is 12 bytes at `0x030`.

| Bits | Field | Meaning |
|------|-------|---------|
| `[0]` | Flat shade | Flat interpolation mode. |
| `[1]` | Flat shade first | Provoking-vertex selection. |
| `[2]` | Pipe count | Participates in the query and streamout pipe count. |
| `[3]` | Line stipple | Stipple enable. |
| `[4]` | Front face cull | Cull front faces. |
| `[5]` | Back face cull | Cull back faces. |
| `[6]` | Early Z | Early depth test. |
| `[7]` | Hi Z | Hi-Z test. |
| `[8]` | Front CCW | Winding convention. |
| `[9]` | Bottom edge rule | Raster rule. |
| `[11:10]` | Raster order | Selector. |
| `[13:12]` | Fill front | 0 fill, 1 line, 2 point. |
| `[15:14]` | Fill back | Same. |
| `[16]` | Flip Y | Flip the Y coordinate. |
| `[17]` | Flip Coordinate Y | Emit a top-down fragment coordinate. |
| `[18]` | Invert vertex position Y | Invert the vertex Y. |
| `[19]` | Sprite origin | Sprite coordinate origin. |
| `[20]` | Offset point | Polygon offset for points. |
| `[21]` | Offset line | Polygon offset for lines. |
| `[22]` | Offset fill | Polygon offset for fills. |
| `[23]` | Window rectangle mode | Window rectangle mode. |
| `[24]` | Edge flag | Edge flag value. |
| `[25]` | Edge flag enable | Enable the edge flag. |
| `[26]` | Fixed point size | Take the point size from state. |
| `[27]` | Stencil write | Stencil participation. |
| `[30:28]` | MRT count | Attachment count minus one. |
| `[39:32]` | Clip plane enable | 8-bit mask. |
| `[47:40]` | Cull plane enable | 8-bit mask. |
| `[48]` | MSAA enable | Multisampling enable. |
| `[49]` | Line smooth | Line smoothing. |
| `[50]` | Point smooth | Point smoothing. |
| `[51]` | Polygon smooth | Polygon smoothing. |
| `[55:52]` | Raster scan | Written as 1. |
| `[57:56]` | Sample rate | Selector. |
| `[60]` | Integer coordinates | Pixel-centre integer mode. |
| `[75:64]` | Layer | Render-layer value. |
| `[79:76]` | Viewport index | Viewport selector. |
| `[80]` | Layer enable | Layer system output. |
| `[81]` | Viewport index enable | Viewport system output. |

The effective front-face direction is the `Front CCW` bit combined with the sign
of the viewport's Y scale; the `Flip Coordinate Y` bit is a separate coordinate
control. See [Rasterization](rasterization.md#culling-and-winding).

## Colour buffer

A colour buffer record is 32 bytes at `0x370 + 0x20 * n`.

| Bits | Field | Meaning |
|------|-------|---------|
| `[63:0]` | Base | GPU virtual address of the attachment. |
| `[71:64]` | Hardware format | Colour hardware format. |
| `[75:72]` | Type | Surface type. |
| `[79:76]` | Tiling mode | 0 linear, 2 tile2. |
| `[91:80]` | Swizzle | 12-bit channel swizzle. |
| `[92]` | Swizzle reverse | Reverse the swizzle. |
| `[93]` | Power-of-two pad | Padded layout. |
| `[111:96]` | Width | Extent in elements. |
| `[127:112]` | Height | Extent in elements. |
| `[143:128]` | Depth | Extent in elements. |
| `[159:144]` | Pitch | Row span in elements. |
| `[171:160]` | Base layer | First layer. |
| `[183:172]` | Last layer | Last layer. |
| `[187:184]` | Base level | First mip level. |
| `[191:188]` | Last level | Last mip level. |
| `[195:192]` | Level | Attached level. |
| `[199:196]` | Alignment | Mip-chain alignment code. |

## Depth buffer

A depth buffer record is 32 bytes at `0x470`. It has the same fields as a colour
buffer, except that bits `[91:80]` (swizzle) and `[92]` (swizzle reverse) are not
present.

## Shader config

Shader Config is 8 bytes at `0x4c0`.

| Bits | Field | Meaning |
|------|-------|---------|
| `[0]` | Vertex enable | Run the vertex slot. |
| `[1]` | Hull enable | Run the hull slot. |
| `[2]` | Domain enable | Run the domain slot. |
| `[3]` | Geometry enable | Run the geometry slot. |
| `[4]` | Streamout enable | Run the streamout slot. |
| `[5]` | Domain as vertex | The domain slot carries the vertex role. |
| `[6]` | Domain includes geometry | Fuse geometry into the domain slot. |
| `[7]` | Vertex includes hull | Fuse the hull into the vertex slot. |
| `[15:12]` | Geometry primitive type | Output primitive. |
| `[17:16]` | Domain primitive type | Tessellation mode. |
| `[18]` | Domain point mode | Point mode. |
| `[19]` | Domain clockwise | Winding. |
| `[21:20]` | Domain spacing | Tessellation spacing. |
| `[36:32]` | DT vertex max | Domain vertex maximum. |
| `[41:37]` | DT primitive max | Domain primitive maximum. |
| `[42]` | DT reuse disable | Disable domain reuse. |
| `[52:48]` | TT vertex max | Tessellation vertex maximum. |
| `[57:53]` | TT primitive max | Tessellation primitive maximum. |
| `[58]` | TT reuse disable | Disable tessellation reuse. |

## Vertex stage config

| Bits | Field | Meaning |
|------|-------|---------|
| `[3:0]` | Output clip distance count | Number of clip distances. |
| `[4]` | Output edge flag | Export an edge flag. |
| `[5]` | Output point size | Export a point size. |
| `[6]` | Output layer | Export a layer. |
| `[7]` | Output viewport index | Export a viewport index. |
| `[8]` | Input instance id | Supply the instance id. |
| `[12]` | Input wave index | Supply the wave index. |
| `[13]` | Input no VPP | Disable vertex post-processing. |
| `[39:32]` | Position size | Position record size. |
| `[47:40]` | Total size | Total output size. |
| `[53:48]` | Attribute count | Number of attributes. |

## Hull stage config

| Bits | Field | Meaning |
|------|-------|---------|
| `[0]` | Input primitive id | Supply the primitive id. |
| `[1]` | Input invocation id | Supply the invocation id. |
| `[4]` | Input wave index | Supply the wave index. |
| `[39:32]` | Level size | Tessellation level size. |
| `[47:40]` | Total size | Total output size. |
| `[53:48]` | Attribute count | Number of attributes. |

The hull stage is not programmable on LG200.

## Domain stage config

| Bits | Field | Meaning |
|------|-------|---------|
| `[0]` | Input primitive id | Supply the primitive id. |
| `[1]` | Input tessellation level | Supply a tessellation level. |
| `[2]` | Input instance id | Supply the instance id. |
| `[7:4]` | Output clip distance count | Number of clip distances. |
| `[8]` | Output edge flag | Export an edge flag. |
| `[9]` | Output point size | Export a point size. |
| `[10]` | Output layer | Export a layer. |
| `[11]` | Output viewport index | Export a viewport index. |
| `[16]` | Input wave index | Supply the wave index. |
| `[17]` | Input no VPP | Disable vertex post-processing. |
| `[39:32]` | Position size | Position record size. |
| `[47:40]` | Total size | Total output size. |
| `[53:48]` | Attribute count | Number of attributes. |

## Geometry stage config

| Bits | Field | Meaning |
|------|-------|---------|
| `[4:0]` | Invocations | Invocation count minus one. |
| `[6:5]` | Stream count | Number of output streams. |
| `[11:7]` | Maximum threads | Maximum threads. |
| `[12]` | Input primitive id | Supply the primitive id. |
| `[13]` | Input invocation id | Supply the invocation id. |
| `[19:16]` | Output clip distance count | Number of clip distances. |
| `[20]` | Output point size | Export a point size. |
| `[21]` | Output layer | Export a layer. |
| `[22]` | Output viewport index | Export a viewport index. |
| `[23]` | Output primitive id | Export a primitive id. |
| `[31:24]` | Output vertices | Maximum output vertices. |
| `[39:32]` | Position size | Position record size. |
| `[47:40]` | Output stride | Output record stride. |
| `[55:48]` | Total size | Total output size. |
| `[61:56]` | Attribute count | Number of attributes. |

## Streamout stage config

The streamout stage config is 8 bytes at `0x4e8`. The current path does not
install a streamout stage; the field meanings beyond the record shape are
UNRESOLVED.

## Fragment state

The fragment attribute map is 32 entries of 2 bytes at `0x4f0`.

| Bits | Field | Meaning |
|------|-------|---------|
| `[4:0]` | Attribute id | Shader output ordinal. |
| `[5]` | Generated | Generated attribute. |
| `[6]` | Sprite texture | Point-sprite coordinate. |

The fragment input config is 8 bytes at `0x530`.

| Bits | Field | Meaning |
|------|-------|---------|
| `[15:0]` | Input address | Input window base. |
| `[31:16]` | Input enable | Per-input enable mask. |
| `[32]` | Primitive id input | Supply the primitive id. |
| `[33]` | Stencil output | Export stencil. |
| `[34]` | Depth output | Export depth. |
| `[35]` | VMask output | Export a validity mask. |
| `[41:36]` | Coefficient count | Interpolation coefficient count. |
| `[44]` | Sample mask output | Export a sample mask. |
| `[48]` | Scalar init align | Scalar-initialization alignment. |

The fragment output config is 4 bytes at `0x538`. It holds one MRT format word
at `[31:0]`: one format when a single target is bound, or four bits per target
when several are bound.

## Shader info slots

Six 0x60-byte records start at `0x540`, one per stage slot. The record layout is
in [Shader ABI](../shaders/shader-abi.md#shader-info-record).

Slot 0 is not an execution slot for a vertex-only draw; the current path writes a
copy of the domain record there so an instruction prefetch through the slot's
entry field finds real code. The copy is a prefetch-safety measure, not a second
launch.

## Open questions

- The draw, pipe, pipeline-configuration, fragment and state tails.
- Viewports 1 through 15.
- The streamout stage config fields.
- The 12-bit colour swizzle subfields.
- Whether the base layer is an offset or a bound.
- Tiling values other than 0 and 2.
- Surface types 2 and 6.
