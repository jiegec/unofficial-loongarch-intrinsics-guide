# State Object

A graphics draw hands the command processor one 512-dword state object, 0x800
bytes. It holds the draw description, the rasterization and output-merger
state, the surface records and the shader records. The object is persistent:
a record names only the dwords it changes, and the processor keeps everything
else from the previous record. This chapter maps the object and tabulates the
blocks that no other chapter defines.

Offsets below are byte offsets from the first dword of the object. In a full
record the object starts at packet byte 12: a three-dword header
(`0x00000f04`, mode 10, range `0x20000001`) precedes it and one terminator
dword (`0x03`) follows it, so such a record is 3 + 512 + 1 = 516 dwords.

## Map

| Offset | Size | Block | Written |
|-------:|-----:|-------|---------|
| `0x000` | `0x24` | [Draw Info](draw.md#draw-info) | Every draw |
| `0x024` | `0x0c` | Unassigned | Never |
| `0x030` | `0x0c` | [Pipe Enables](#pipe-enables) | Every draw |
| `0x03c` | `0x0c` | Unassigned | Never |
| `0x048` | `0x2c0` | [Viewport](rasterization.md#viewport), 16 slots | Slot 0 |
| `0x308` | `0x10` | [Guardband](rasterization.md#guardband) | Every draw |
| `0x318` | `0x0c` | [Line Raster](rasterization.md#point-and-line) | Every draw |
| `0x324` | `0x10` | [Depth Control](depth-stencil.md#depth-control) | Every draw |
| `0x334` | `0x04` | [Stencil, front](depth-stencil.md#stencil-face-record) | Every draw |
| `0x338` | `0x04` | [Stencil, back](depth-stencil.md#stencil-face-record) | Every draw |
| `0x33c` | `0x04` | [Blend Control](blending.md#blend-control) | Every draw |
| `0x340` | `0x20` | [Blend per target](blending.md#per-target-blend-word), 8 words | All eight |
| `0x360` | `0x10` | [Blend constants](blending.md#blend-constants) | Every draw |
| `0x370` | `0x100` | [Colour buffer](#colour-buffer), 8 slots | All eight |
| `0x470` | `0x20` | [Depth buffer](#depth-buffer) | Every draw |
| `0x490` | `0x30` | Pipeline configuration, no field defined | Never |
| `0x4c0` | `0x08` | [Shader Config](#shader-config) | Every draw |
| `0x4c8` | `0x08` | [Vertex stage config](#vertex-stage-config) | Never |
| `0x4d0` | `0x08` | [Hull stage config](#hull-stage-config) | Never |
| `0x4d8` | `0x08` | [Domain stage config](#domain-stage-config) | Every draw |
| `0x4e0` | `0x08` | [Geometry stage config](#geometry-stage-config) | Every draw |
| `0x4e8` | `0x08` | Unassigned | Never |
| `0x4f0` | `0x40` | [Fragment attribute map](#fragment-state), 32 entries | All 32 |
| `0x530` | `0x08` | [Fragment input config](#fragment-state) | Every draw |
| `0x538` | `0x04` | [Fragment output config](#fragment-state) | Every draw |
| `0x53c` | `0x04` | Unassigned | Never |
| `0x540` | `0x240` | [Shader Info](#shader-info-slots), 6 slots | Slots 0, 2–5 |
| `0x780` | `0x80` | Unassigned | Never |

The **Written** column describes the OpenGL interface, whose object persists
across draws. The Vulkan interface starts each draw from a zeroed object and
writes only the records that draw uses: one colour record and one blend word
per attachment, one viewport slot per declared viewport. Every slot it does not
write holds zero, so the device reads a defined value in every slot.

The strides inside the repeated blocks are not the record sizes: a viewport
slot is `0x2c` apart although its record defines `0x28` bytes, so the last
dword of every slot carries no field; the blend word is `0x04`, the colour
record `0x20`, the fragment attribute entry `0x02` and the shader-info
record `0x60`.

The object starts zeroed, and the five regions that no record defines never
receive a field: `0x024`, `0x03c`, `0x490`, `0x53c` and `0x780`. They travel as
zero inside a full record, and a driver MUST NOT put anything there: nothing in
the sources says what the device reads from them.

A depth-only draw still installs one null colour record at slot 0: that record
is what carries a fragment-written depth export to the depth surface while the
MRT count is zero. The record's base has to name mapped memory even though its
blend mask is zero, because a store the encoder does not mask would otherwise
land at virtual address 0.

## Pipe Enables

Twelve bytes at `0x030`, three dwords. Bit 0 is the low bit of the first
dword and bit 95 the high bit of the third. Both host interfaces write the
whole record on every draw.

| Bits | Field | Meaning |
|------|-------|---------|
| `[0]` | Flat shade | Flat interpolation enable. Raise it when the fragment stage reads a flat-qualified input, not only when the API's shading model is flat; the provoking-vertex selection below is inert without it. |
| `[1]` | Flat shade first | Provoking vertex for flat interpolation: set selects the first vertex of the primitive. |
| `[2]` | Pipe count | Set on a draw that exists to drain the pipe rather than to produce fragments: the draw that closes an occlusion bracket, and a draw whose scissor covers nothing. What it changes in the pipeline is UNRESOLVED. |
| `[3]` | Line stipple | Apply the Line Raster stipple pattern to line primitives. |
| `[4]` | Front face cull | Discard front-facing primitives. |
| `[5]` | Back face cull | Discard back-facing primitives. |
| `[6]` | Early Z | Run the depth test before the fragment stage (early fragment tests). |
| `[7]` | Hi Z | Hierarchical depth test. Not programmed by either interface. |
| `[8]` | Front CCW | Front-winding select: set makes clockwise the front face. The value is the inverse of an API counter-clockwise flag, independent of the viewport's Y sign. See below. |
| `[9]` | Bottom edge rule | Use the bottom-left fill rule for coverage tests. |
| `[11:10]` | Raster order | Rasterization-order selector. Not programmed. |
| `[13:12]` | Fill front | Front-facing fill mode: 0 fill, 1 line, 2 point. |
| `[15:14]` | Fill back | Back-facing fill mode, same encoding. |
| `[16]` | Flip Y | Mirror window-space Y. Not programmed. |
| `[17]` | Flip coordinate Y | Emit a top-down fragment coordinate and interpolant Y. Not programmed; the interfaces keep the lower-left convention. |
| `[18]` | Invert vertex position Y | Negate the vertex position's Y. Not programmed. |
| `[19]` | Sprite origin | Point-sprite coordinate origin: set selects upper left. |
| `[20]` | Offset point | Apply the polygon offset to points. |
| `[21]` | Offset line | Apply the polygon offset to lines. |
| `[22]` | Offset fill | Apply the polygon offset to filled primitives. |
| `[23]` | Window rectangle mode | Window-rectangle rasterization mode. Not programmed. |
| `[24]` | Edge flag | Edge-flag value passed to the rasterizer. Not programmed. |
| `[25]` | Edge flag enable | Enable the edge flag. Not programmed. |
| `[26]` | Fixed point size | Take the point size from Line Raster instead of the shader's point-size export. |
| `[27]` | Stencil write | Stencil write participation. Never set; the stencil face record carries its own enables. |
| `[30:28]` | MRT count | Number of colour targets minus one: 0 for one target, 3 for four. |
| `[39:32]` | Clip plane enable | One bit per clip distance the rasterizer tests. |
| `[47:40]` | Cull plane enable | One bit per cull distance. |
| `[48]` | MSAA enable | Multisample rasterization. Set for a multisampled framebuffer; the bit does not carry the sample count. |
| `[49]` | Line smooth | Line antialiasing. |
| `[50]` | Point smooth | Point antialiasing. Not programmed. |
| `[51]` | Polygon smooth | Polygon antialiasing. Not programmed. |
| `[55:52]` | Raster scan | Scan-pattern selector. The two interfaces program different values here, 1 and 0. |
| `[57:56]` | Sample rate | Per-pixel sample stamps minus one. It only acts while MSAA enable is set. |
| `[60]` | Integer coordinates | Pixel-centre integer coordinate mode. Not programmed. |
| `[75:64]` | Layer | Render layer this draw targets, when Layer enable is set and the geometry stage does not supply the layer. |
| `[79:76]` | Viewport index | Viewport slot used by this draw. Not programmed in the default configuration. |
| `[80]` | Layer enable | Select the layer from the Layer field, or from the geometry stage's layer export, instead of layer 0. |
| `[81]` | Viewport index enable | Select the viewport from the Viewport index field. Not programmed in the default configuration. |

Front CCW is the bit a driver is most likely to get backwards. Set means
clockwise is the front face, so the stored value is the inverse of the API's
front-face-counter-clockwise flag and nothing else: it does not depend on the
sign of the viewport's Y scale, and a driver that copies its flag straight
through culls the opposite faces from the ones the application asked for.

UNRESOLVED: the frame in which the rasterizer measures winding. The encoding
does not say whether the comparison happens in normalized device coordinates
or after the viewport transform. [Rasterization](rasterization.md#culling-and-winding)
states the same open question from the rasterizer's side.

The MRT count is a count minus one, so a zeroed object describes exactly one
target. A driver that programs a literal attachment count writes one target too
many.

## Colour buffer

A colour buffer record is 32 bytes at `0x370 + 0x20 * n` for `n < 8`.

| Bits | Field | Meaning |
|------|-------|---------|
| `[63:0]` | Base | GPU virtual address of the attachment, already rebased onto the attached layer. |
| `[71:64]` | Hardware format | Colour hardware format code. |
| `[75:72]` | Type | Surface type; the values are listed below. |
| `[79:76]` | Tiling mode | 0 linear, 2 tiled; those are the only two values either interface programs. |
| `[91:80]` | Swizzle | Twelve-bit channel swizzle; see below. |
| `[92]` | Swizzle reverse | Reverse the swizzle. Not programmed. |
| `[93]` | Power-of-two pad | The resource stores its mip chain padded to a power of two, so the record's pitch describes the base level. |
| `[111:96]` | Width | Extent in elements. |
| `[127:112]` | Height | Extent in elements. |
| `[143:128]` | Depth | Extent in elements; 1 for a plain 2D attachment. |
| `[159:144]` | Pitch | Row span in elements, not bytes, and taken from the attached level rather than from the resource's base level. |
| `[171:160]` | Base layer | First layer of a layered attachment. Zero for a single layer, whose base is rebased instead. |
| `[183:172]` | Last layer | Last layer of a layered attachment. |
| `[187:184]` | Base level | First mip level. |
| `[191:188]` | Last level | Last mip level present in the resource. |
| `[195:192]` | Level | The mip level this attachment binds. |
| `[199:196]` | Alignment | Alignment code; see below. |

Surface type values: 0 plain 2D, 2 2D with a nonzero mip level, 4 layered, 6
layered with a nonzero mip level, 8 multisampled 2D, 12 multisampled layered.
The sample count is not in this word: a 2x and a 4x attachment serialize the
same surface type, so the count can only live in the surface allocation. A
cube attachment that is not layered selects one face through the base layer.

Alignment carries a host convention rather than a device constant. A padded
cube chain stores log2 of the base pitch in elements there, and a multisampled
surface stores log2(256 / texel bytes), which is 6 for a 4-byte texel; what the
device does with either encoding is UNRESOLVED.

## Depth buffer

One 32-byte record at `0x470`. A draw with no depth attachment writes the
all-zero record. The depth test itself is switched by Depth Control, not by
this record.

| Bits | Field | Meaning |
|------|-------|---------|
| `[63:0]` | Base | GPU virtual address of the depth surface, rebased onto the attached layer. |
| `[71:64]` | Hardware format | Depth hardware format code: 37 Z24, 38 Z32 float, 39 Z24 with stencil, 40 Z32 float with stencil. |
| `[75:72]` | Type | Surface type, same encoding as the colour record. |
| `[79:76]` | Tiling mode | 0 linear, 2 tiled. |
| `[93]` | Power-of-two pad | Padded mip chain. |
| `[111:96]` | Width | Extent in elements. |
| `[127:112]` | Height | Extent in elements. |
| `[143:128]` | Depth | Extent in elements; 1 for a plain 2D attachment. |
| `[159:144]` | Pitch | Row span in elements of the attached level. |
| `[171:160]` | Base layer | First layer of a layered attachment. |
| `[183:172]` | Last layer | Last layer of a layered attachment. |
| `[187:184]` | Base level | First mip level. |
| `[191:188]` | Last level | Last mip level present in the resource. |
| `[195:192]` | Level | The mip level this attachment binds. |
| `[199:196]` | Alignment | Alignment code. Both interfaces program 6, whatever the format or sample count. |

There is no swizzle and no swizzle-reverse field here, unlike the colour
record: a depth surface has one channel order.

## Shader config

A Shader Output Sizes record describes a stage's output window in one dword.
Neither interface emits one, and only the field encoding is established.

| Bits | Field | Meaning |
|------|-------|---------|
| `[7:0]` | Position size | Size of the position record. |
| `[15:8]` | Total size | Total output size. |
| `[23:16]` | Attribute count | Number of attributes in the record. |

Shader Config is eight bytes at `0x4c0`.

| Bits | Field | Meaning |
|------|-------|---------|
| `[0]` | VS enable (vertex) | Launch the vertex slot. Never set: the vertex program runs in the domain slot. |
| `[1]` | HS enable (hull) | Launch the hull slot. Never set. |
| `[2]` | DS enable (domain) | Launch the domain slot. Set on every draw. |
| `[3]` | GS enable (geometry) | Launch the geometry slot. Set when a geometry stage is bound. |
| `[4]` | SS enable (streamout) | Launch the streamout slot. Never set: no streamout stage is installed. |
| `[5]` | DS as VS (domain as vertex) | The domain slot carries the vertex role. Set on every draw. |
| `[6]` | DS includes GS | Fuse the geometry stage into the domain slot. Never set. |
| `[7]` | VS Includes HS | Fuse the hull stage into the vertex slot. Never set. |
| `[15:12]` | GS primitive type | Geometry output primitive as a category, not an assembly mode: 1 points, 2 lines, 3 triangles. |
| `[17:16]` | DS primitive type | Tessellation output primitive. Never set. |
| `[18]` | DS point mode | Tessellation point mode. Not set in the default configuration. |
| `[19]` | DS clockwise | Tessellation winding. Never set. |
| `[21:20]` | DS spacing | Tessellation spacing. Not set in the default configuration. |
| `[36:32]` | DT vertex max | Domain-stage vertex maximum. Never set. |
| `[41:37]` | DT primitive max | Domain-stage primitive maximum. Never set. |
| `[42]` | DT reuse disable | Disable domain-stage reuse. Never set. |
| `[52:48]` | TT vertex max | Tessellation vertex maximum. Never set. |
| `[57:53]` | TT primitive max | Tessellation primitive maximum. Never set. |
| `[58]` | TT reuse disable | Disable tessellation reuse. Never set. |

Two configurations exist. A vertex-and-fragment draw sets DS enable and DS as
VS and nothing else: the vertex program is the domain stage's program. A draw
with a geometry stage adds GS enable and the primitive category, and still
leaves VS enable clear.

The tessellation half of the record is unused: the domain primitive type, the
winding and spacing fields, both vertex and primitive maxima and both reuse bits
are zero in every state the device receives. A driver MUST NOT read meaning into
them on this generation.

## Vertex stage config

Nothing writes `0x4c8`. The vertex program runs in the domain slot, so this
record only describes a configuration that launches the VS slot, and no
interface selects one.

| Bits | Field | Meaning |
|------|-------|---------|
| `[3:0]` | Output clip distance count | Number of clip distances the stage exports. |
| `[4]` | Output edge flag | The stage exports an edge flag. |
| `[5]` | Output point size | The stage exports a point size. |
| `[6]` | Output layer | The stage exports a render layer. |
| `[7]` | Output viewport index | The stage exports a viewport index. |
| `[8]` | Input instance id | Supply the instance id to the stage. |
| `[12]` | Input wave index | Supply the wave index. |
| `[13]` | Input no VPP | Disable vertex post-processing for this stage. |
| `[39:32]` | Position size | Size of the position record. |
| `[47:40]` | Total size | Total output size. |
| `[53:48]` | Attribute count | Number of attributes. |

## Hull stage config

Nothing writes `0x4d0`, and the hull shader-info slot is left zero. Whether
the device runs a hull stage at all is UNRESOLVED: the encoding exists, but no
state it receives ever selects the stage.

| Bits | Field | Meaning |
|------|-------|---------|
| `[0]` | Input primitive id | Supply the primitive id. |
| `[1]` | Input invocation id | Supply the invocation id. |
| `[4]` | Input wave index | Supply the wave index. |
| `[39:32]` | Level size | Size of the tessellation-level payload. |
| `[47:40]` | Total size | Total output size. |
| `[53:48]` | Attribute count | Number of attributes. |

## Domain stage config

Eight bytes at `0x4d8`, written on every draw. This is the vertex program's
record. In the two-stage configuration it describes that program's output
window; in the geometry-fusion configuration it describes the input ring the
geometry stage consumes.

| Bits | Field | Meaning |
|------|-------|---------|
| `[0]` | Input primitive id | Supply the primitive id. |
| `[1]` | Input tessellation level | Supply a tessellation level. Never set. |
| `[2]` | Input instance id | Supply the instance id. Must be set or instancing does not reach the shader. |
| `[7:4]` | Output clip distance count | Number of clip distances the stage exports. |
| `[8]` | Output edge flag | The stage exports an edge flag. |
| `[9]` | Output point size | The stage exports a point size through the miscellaneous vector rather than through Line Raster. |
| `[10]` | Output layer | The stage exports a render layer. |
| `[11]` | Output viewport index | The stage exports a viewport index. |
| `[16]` | Input wave index | Supply the wave index. |
| `[17]` | Input no VPP | Disable vertex post-processing. |
| `[39:32]` | Position size | Size of the position record. Zero in the geometry-fusion configuration, where the vertex stage's output is an input ring rather than a position. |
| `[47:40]` | Total size | Total output size in channels. |
| `[53:48]` | Attribute count | Number of attributes, which is the count of varying slots the stage wrote, or the ring width divided by four in the fusion configuration. |

## Geometry stage config

Eight bytes at `0x4e0`, written on every draw: a draw without a geometry stage
writes the all-zero record so that the previous draw's record cannot survive.

| Bits | Field | Meaning |
|------|-------|---------|
| `[4:0]` | Invocations | Hardware invocations per input primitive, minus one. |
| `[6:5]` | Stream count | Number of output streams. Not programmed. |
| `[11:7]` | Maximum threads | Threads the stage may run per wave. |
| `[12]` | Input primitive id | Supply the primitive id. |
| `[13]` | Input invocation id | Supply the invocation id, raised when more than one invocation is programmed. |
| `[19:16]` | Output clip distance count | Number of clip distances the stage exports. |
| `[20]` | Output point size | The per-vertex record carries a point size. |
| `[21]` | Output layer | The stage's output record carries a layer. It is a stream-shape gate, not a "the shader wrote a layer" flag: setting it without the matching record makes the rasterizer drop trailing vertices. |
| `[22]` | Output viewport index | The stage exports a viewport index. |
| `[23]` | Output primitive id | The stage exports a primitive id. |
| `[31:24]` | Output vertices | Maximum vertices the stage may emit. The field is eight bits, so a request for 256 truncates to 0. |
| `[39:32]` | Position size | Size of the position record. |
| `[47:40]` | Output stride | Stride between emitted vertices in the output record, counted in 32-channel rows. |
| `[55:48]` | Total size | Total output size, 1 + stride * 32. |
| `[61:56]` | Attribute count | Number of attributes in the output record. |

## Unassigned 0x4e8

Eight bytes at `0x4e8` have no field definition and no writer. The streamout
shader-info slot is left zero: transform feedback runs through the vertex or
geometry stage, which writes its records directly. A driver MUST NOT program
this range from an API stream-out description; whether the device reads it at
all is UNRESOLVED.

## Fragment state

The fragment attribute map is 32 Fragment Attribute Map Entry records of two
bytes at `0x4f0`, all written on every draw. Entry *i* is the fragment stage's
input *i*.

| Bits | Field | Meaning |
|------|-------|---------|
| `[4:0]` | Attribute id | Shader-output ordinal that supplies this input. |
| `[5]` | Generated | The rasterizer generates the value instead of reading it from the stage output. |
| `[6]` | Sprite texture | The input is a point-sprite coordinate. |

The fragment input config is eight bytes at `0x530`.

| Bits | Field | Meaning |
|------|-------|---------|
| `[15:0]` | Input address | Base of the input window inside the interpolator payload. |
| `[31:16]` | Input enable | One bit per input the fragment stage consumes. |
| `[32]` | Primitive id input | Supply the primitive id. |
| `[33]` | Stencil output | The stage exports a stencil value. |
| `[34]` | Depth output | The stage exports a depth value. |
| `[35]` | VMask output | The stage exports a validity mask, which a discard produces. |
| `[41:36]` | Coefficient count | Number of interpolation coefficient planes the stage uses. |
| `[44]` | Sample mask output | The stage exports a coverage mask. |
| `[48]` | Scalar init align | Scalar-initialization alignment. Not programmed. |

The fragment output config is four bytes at `0x538`.

| Bits | Field | Meaning |
|------|-------|---------|
| `[31:0]` | MRT format | One four-bit format code per colour target, target *n* in bits `4n+3:4n`; with a single target the low eight bits carry that target's format code alone. |

## Shader info slots

Six 96-byte records start at `0x540`, one per stage slot. The record layout is
in [Shader ABI](../shaders/shader-abi.md#shader-info-record).

| Offset | Slot | Stage | Written |
|-------:|-----:|-------|---------|
| `0x540` | 0 | vertex | Only in a full record, and then as a copy of slot 2 |
| `0x5a0` | 1 | hull | Never; left zero |
| `0x600` | 2 | domain, carrying the vertex program | Every draw |
| `0x660` | 3 | geometry | Every draw, zero when no geometry stage is bound |
| `0x6c0` | 4 | streamout | Every draw, as a zero record |
| `0x720` | 5 | fragment | Every draw |

A slot's entry field is an instruction-fetch address, not dead state. Slot 0
is the slot whose entry is fetched even though its stage does not launch, so it
is filled with a copy of the vertex program's record and a fetch through it
lands on real code. The copy is rewritten only when the record is full, not on
every delta, because the roots inside it are only read by a launch.

A driver MUST NOT leave slot 0 zero, and MUST NOT point an unused slot at code
it may free. Whether any other disabled slot is ever fetched is UNRESOLVED;
zero is the safe value and the one every unused slot carries.

## Open questions

- The five regions with no field. Nothing in the sources says what the device
  reads from `0x024`, `0x03c`, `0x490`, `0x53c` or `0x780`; the state the
  device receives has zeros there.
- Viewport slots 1 to 15. The slots exist and are `0x2c` apart, but only slot
  0 is selected: the OpenGL interface writes slot 0 alone and the Vulkan
  interface writes every declared slot without ever raising the viewport-index
  enable bit. How the device picks a slot, and whether the record's unused
  fourth dword is read, is UNRESOLVED.
- The 12-bit colour swizzle. Values such as `0x688` show that the field packs
  per-channel selects, but nothing establishes their width or their order.
- Whether Base layer is a bound the record promises to respect or an offset the
  hardware adds. A single-layer attachment rebases the base and leaves the
  field zero, which is consistent with either reading.
- The streamout stage config fields.
- Whether the device runs a hull stage at all, given that neither its config
  record nor its shader-info slot is written.
