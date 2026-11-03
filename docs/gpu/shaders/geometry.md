# Geometry Stage

The geometry stage runs one invocation per input primitive. It reads the vertex
stage's SIO0 record, emits vertices into an SIO1 record, and cuts that record
into output primitives. This chapter defines the input record, both output
record classes, the emit and cut rules, the limits, and the host state that runs
the stage. Registers are in [Registers](registers.md); SIO transfers are in
[SIO](io.md).

## Where the stage runs

The geometry program is shader slot 3 of the six shader-info slots, numbered VS
0, HS 1, DS 2, GS 3, SS 4, PS 5. Slot records start at state offset `0x540`
and are `0x60` bytes apart, so the geometry record is at `0x660`, with `Shader
Type` (bits 32 to 39) set to 4. The vertex program runs in the domain-stage
slot and its compiled form depends on whether a geometry stage is present; see
[Stages](stages.md).

The stage is optional. A draw without a bound geometry program MUST still write
the geometry slot and the geometry configuration, because the command processor
keeps the last state block it was given and would otherwise run the previous
draw's program. The disabled form is an all-zero shader-info record and an
all-zero `GEOMETRY_STAGE_CONFIG`.

With a geometry program bound, the shader config word at `0x4c0` sets DS Enable
(bit 2), GS Enable (bit 3) and DS As VS (bit 5), and `GS Primitive Type`
(bits 12 to 15) takes 1 for points, 2 for lines and 3 for triangles. That field
is a category, not an assembly mode: a triangle strip and a triangle fan both
program 3.

## Input record

Per input primitive the stage receives one SIO0 record and two vector registers.

| Register | Content |
|----------|---------|
| `v0` | Six 5-bit vertex selectors. Selector `i` occupies bits `5i+4 : 5i` and names the entry-register index, 0 to 5, of input vertex `i`. |
| `v1` | The primitive id when `Input Primitive ID` (bit 12) is set; the invocation id when `Input Invocation ID` (bit 13) is set. |

A constant vertex index above 5 is rejected; a dynamic index is clamped to 5
before the selector is extracted. The two `v1` enable bits are independent, and
both meanings read `v1`, so a stage that reads both `gl_PrimitiveIDIn` and
`gl_InvocationID` has no established lane assignment; see
[Open questions](#open-questions).

Position occupies channels 0 to 3 of the ring. The block after position carries
clip and cull distances, and the generic varyings follow it in linked-layout
order. Let `D` be the producer's distance component count and `E` indicate
that the producer writes an edge flag:

```text
clip_base(D, E)         = 8, if D > 4 or E; otherwise 4
position_channels(D, E) = 4, if D = 0 and not E
                          clip_base(D, E) + 4 * ceil(D / 4), otherwise
```

A generic varying whose ordinal in the linked layout is `b` starts at
`position_channels + 4 * b`. Distance slots are skipped when the ordinals are
counted, so they are not also paid for as generic quads; a cull slot the
producer never wrote answers zero. For `V` linked generic varying quads the
SIO0 window is:

```text
minimum = max(position_channels + 4 * V, 4 * (V + 2), 12)
window  = minimum,                 if minimum <= 32
          32 * ceil(minimum / 32), otherwise
```

Producer and consumer MUST program the same window: the vertex stage's export
width and the geometry stage's read width are one number. Wide records round up
to complete 32-channel rows so that the input quads of adjacent vertices do not
overlap; whether a partial row would overlap is UNRESOLVED, so the rounding is
required.

An input read takes a channel number and returns an aligned quad; a scalar
declaration issues one read per component and keeps one lane. Reads SHOULD be
issued in non-decreasing channel order, because a read that moves backwards in
the ring stalls every later export of the stage and raises no fault. The read's
own register field does not select its destination; the window is fixed by the
read's position in the sequence, so the lanes MUST be copied out before the next
read.

## Output record

Every emitted vertex contributes a position block of `P` quads and a varying
row holding `N` quads padded to `next_pow2(max(N, 1))`. `N` is the number of
live raster output varying planes, `M` the declared output vertex count, `S` a
layer or viewport chunk, and `G` the number of distance groups:

```text
G              = max(declared distance groups, ceil((clip + cull) / 4))
has_point_size = the stage writes PointSize
P              = next_pow2(max(distance_offset_quads, 1 + has_point_size) + G),
                   when G > 0; otherwise 1 + has_point_size
distance_offset_quads = 2, when G > 1; otherwise 1
```

`P` is the position block's quad count, position included. A stage with
distances therefore has `P >= 2`, and point size with one distance group gives
`P = 4`: point size occupies the payload quad at channel offset 4 and the
distances start after it. A second distance group needs that quad even when
point size is absent.

`S` is 1 when the stage writes `gl_Layer` or `gl_ViewportIndex` and the
matching output is enabled. Such a record is always compact, and its declared
vertex count is capped at 32 because a layer that is not a draw constant is
replayed one layer per launch.

### Classes

| Class | Chosen when |
|-------|-------------|
| Wide | `S = 0` and any of: `M > 8` with `(N >= 1 or M > 32)`; `P > 1` with `(M > 8 or 1 + P + max(N, 1) >= 8)`; or `P = 1` with `N = 2` or `N = 3`. |
| Compact | Everything else, including every record with `S = 1`. |

The two- and three-varying cases take the wide layout because a single-varying
compact stride cannot describe two full varying quads.

For a wide record:

```text
cut                      = 32 * ceil(M / 32)
slots                    = 8 * ceil(M / 8)
varying_row              = 4 * next_pow2(max(N, 1))
window                   = cut + (4 * P + (N > 0 ? varying_row : 0)) * slots
stride                   = window / 32
export_count             = floor(4096 / window) * stride + 1
position_size            = P * slots / 8
attribute_count          = max(N, 1)
maximum_threads          = floor(4096 / window) - 1
position_descriptor_base = cut
varying_descriptor_base  = cut + 4 * P * slots
```

The padding in `varying_row` is allocated for every vertex: three varying quads
occupy a four-quad row and five an eight-quad row. Advancing descriptors by the
padded row while sizing the ring from the live count overruns the window at
later vertices.

For `M = 8`, `P = 1` and no special chunk:

| Live varying planes | Row channels | Window | Stride | Export count | Maximum threads |
|---------------------|--------------|--------|--------|--------------|-----------------|
| 2 | 8 | 128 | 4 | 129 | 31 |
| 3 | 16 | 192 | 6 | 127 | 20 |

For a compact record:

```text
stride                   = 2 + S + (P - 1) + max(N, 1)
window                   = 32 * stride
export_count             = window + 1
position_size            = 1 + S + (P - 1)
attribute_count          = max(N, 1)
maximum_threads          = 31
position_descriptor_base = 32
varying_descriptor_base  = 64 + (S + P - 1) * 32
```

The compact varying row is four channels for one plane and
`align_up(4 * N, 16)` above that, except that a position payload replaces it
with `4 * next_pow2(max(N, 1))`. Compact descriptors rotate through the window
instead of staying inside one pass over it, so the wide record's per-vertex
containment rule does not apply to them. `window`, `stride` and
`export_count` are three different units — channels, 32-channel rows and
committed records — so export count is not generally `window + 1`.

### Descriptors

A varying component at attribute `a`, component `c` and emitted vertex `e` is
named by `varying_descriptor_base + varying_row * e + 4 * a + c`; a position
component uses `position_descriptor_base + 4 * P * e + component`. The
fixed-function distance payload starts `distance_offset` channels into the
position block, where `distance_offset` is 8 when `G > 1` or `P >= 4`, and 4
otherwise. Clip distances that fragment interpolation consumes are also written
to an ordinary varying plane, so they appear twice in one record: in the
fixed-function payload and as an interpolated attribute.

## Emit and cut

`v_emit2` and `v_emit3` share one layout: `GK11` in bits 7:0 of the low word
plus bits 2:0 of the high word, `GA11` in bits 15:8 plus bits 5:3, and `VJ8` in
bits 31:24. `GK` selects a vector register, `GA` names a scalar register, and
`VJ` is an eight-bit field holding either a physical vector register number or a
raw chunk constant. The `M.mod2` and `M.clmp1` modifier bits MUST be zero.

The emit recipe uses `v_emit3` for a triangle-strip output and `v_emit2` for
every other topology, including points, line strips and triangle fans. The pair
does not split into a triangle opcode and a line opcode.

`GK` carries the emitted-vertex ordinal within the invocation. `GA` carries the
descriptor of that vertex's final live varying component:

```text
wide:    varying_descriptor_base + varying_row * e + 4 * max(N, 1) - 1
compact: varying_descriptor_base + 4 * e + 3
```

`VJ` names the separator register; the plan keeps the separator state in vector
register 35. The first emit of a run seeds that register to zero and later emits
of the same run MUST NOT reseed it. A run left open when the invocation exits is
closed implicitly, so a stage need not emit a final cut after its last vertex.
A cut resets separator state but does not restart the vertex ordinal: two
three-vertex strips in one invocation use ordinals 0 through 5. Position and
varying descriptors therefore continue advancing across cuts. The ordinal
starts at zero for each new invocation.

The strip and point recipes take the record from the same decision, so two
stages with equal `N`, `M`, `P` and `S` write the same record. The runtime
emitter scales the position block and the varying row by the live ordinal
separately, because their strides differ.

## Transform feedback and raster outputs

Transform feedback captures shader outputs independently of fragment input
consumption. An output used only for capture does not need a generic varying
plane in the raster record. Consequently, the record formulas use the number
of live raster outputs for `N`, not the number of captured attributes or the
transform-feedback buffer stride. Position, point size, and clip/cull payloads
retain their fixed-function roles even when the fragment shader reads none of
them.

The shared LCO frontend accepts a consumer slot mask and a separate validity
flag. Gallium and Vulkan supply fragment input slots through this same
interface. A valid zero mask denotes a linked fragment program with no varying
inputs; the default, invalid mask denotes an unknown consumer and does not
discard outputs on that basis. NIR's `is_xfb_only` variable flag and
`no_varying` output-store flag also exclude capture-only generic outputs from
raster layout discovery. Clip and cull distance slots are retained because
they also describe clipping payloads.

Filtering the raster layout preserves the linked bases and interpolation
modes of surviving slots. Geometry raster attributes use dense ordinals in
the surviving output set; fragment attribute descriptors map their linked
input bases to those ordinals. Transform-feedback stores use the capture
declarations and emitted values independently of this filtering. In
particular, capturing fifteen vec4 outputs with no fragment varying inputs
must not request fifteen raster varying planes or lose captured components.
This separation is a compiler/driver ABI contract, not an increase in the
hardware raster record limits below.

Shader selection must track whether capture targets are active, including
temporary changes made by the driver. In Gallium, an internal query drain draw
can hide the targets without changing the bound application state. A cache
indexed only by that state's generation can then retain the shader variant
with capture stores disabled when the application draw resumes. Feedback
activity must therefore participate in shader selection independently of the
bound-state generation, for both vertex-only and geometry pipelines. This is
a driver cache contract; it does not change the SIO record layout or provide
ordering between buffer stores and rasterization.

### Captured point size

`PointSize` keeps its fixed-function payload quad even when transform feedback
captures it. It does not consume a generic varying plane. A capture declaration
must not disable this carrier: rasterization still needs the value while capture
is active, paused, or inactive. The vertex stage writes position channel 4;
the geometry stage uses the point-size slot in its position record, so `P` must
include that quad in both cases.

Capture observes the shader's original value. Rasterization may require a
separate clamped copy because LG200 does not enforce the API's point-size range
on the streamed value. Gallium currently uses `[0.125, 2048]`: for example, a
shader output of `2064` must be captured as `2064` while the raster point spans
at most `2048` pixels. This is a frontend policy, not an ISA constant. LCO's
shared target options default to a zero maximum, which disables this extra
clamp; a frontend that already lowered the size, or uses another API policy,
can retain that default.

For captured sizes, the compiler preserves the original value for buffer
stores and applies `v_max_f32` followed by `v_min_f32` only to the raster copy.
Bounds that are not inline VOP constants must first be materialized in an
encodable source. The point-size carrier and its position-record width remain
present when capture stores are disabled. VS and GS share this rule; it does
not add Vulkan transform-feedback support or change Vulkan's advertised point
size features.

## Limits

| Limit | Value |
|-------|-------|
| Input vertex selectors | Six, 5 bits each, values 0 to 5 |
| SIO0 window | 12 to 4096 channels, rounded up to 32 above 32 |
| Output varying planes | 5 |
| Declared output vertices | 256 |
| Static emit sites | 16 |
| Record window | at most 4096 channels, refused above |
| Compact stride | at most 7 rows |

The compact stride is seven rows at most because `Total Size` is an 8-bit field,
which also confines five varying planes to records without a special chunk and
without a position payload. A stage that needs more than 16 emit sites, or more
than 16 declared vertices, takes the runtime route instead. The 8-bit
output-vertex field cannot encode 256, so a host MUST truncate it to 0 and MUST
NOT reject the record on that basis. A record window above 4096 channels is
refused at pipeline creation: `maximum_threads` is `floor(4096 / window) - 1`,
which underflows and wedges the rasterizer.

## Host configuration

`GEOMETRY_STAGE_CONFIG` is eight bytes at state offset `0x4e0`.

| Bits | Field | Meaning |
|------|-------|---------|
| 4:0 | Invocations | Hardware invocations per input primitive, encoded as `n - 1`. |
| 6:5 | Stream Count | Number of output streams. |
| 11:7 | Maximum Threads | Geometry invocations per wave. |
| 12 | Input Primitive ID | Fills the primitive-id lane. |
| 13 | Input Invocation ID | Fills the invocation-id lane. |
| 19:16 | Output Clip Distance Count | Distance components the consumer reads. |
| 20 | Output Point Size | The per-vertex record's payload quad carries a point size. |
| 21 | Output Layer | The stream carries a layer record. |
| 22 | Output Viewport Index | The stream carries a viewport-index record. |
| 23 | Output Primitive ID | The stream carries a primitive-id record. |
| 31:24 | Output Vertices | Declared vertices per invocation. |
| 39:32 | Position Size | 32-channel rows per emitted vertex's position block. |
| 47:40 | Output Stride | 32-channel rows per emitted vertex. |
| 55:48 | Total Size | Committed records in the SIO1 window. |
| 61:56 | Attribute Count | Varying planes the consumer reads, minimum one. |

`Position Size`, `Output Stride`, `Total Size` and `Attribute Count` are one
record's four numbers and MUST come from the same computation; deriving any of
them a second time lets the two halves disagree.

`Maximum Threads` takes the record's computed value except in two cases that
MUST be pinned to 1: a stage whose emit ordinal is a runtime counter, and a
layered stage with more than one hardware invocation. Both derive ring addresses
from a value that is uniform across the wave, so two invocations sharing a wave
write the same SIO1 slots and corrupt each other's stream. The field is a launch
parameter, not an architectural maximum.

`Output Layer` is a stream-shape gate, not "the shader wrote `gl_Layer`". With
the bit set and no matching layer record in the stream, the rasterizer drops the
trailing vertices of the strip. No geometry layer record shape is established,
so a host SHOULD leave the bit clear, which routes layer writes into layer 0.

`Output Clip Distance Count` is zero when the stage writes no distance slot. A
record that carries a point size declares the count rounded up to four
components, because the emitter zeroes the unused components and the consumer
must use the same vertex addressing as the emit plan.

A fragment stage behind a geometry stage initialises at least five coefficient
groups, even when fewer varyings are live; programming only the active prefix
leaves stale coefficient state across draws and contexts.

## Descriptor roots

The geometry Shader Info record carries five 64-bit roots. Offsets are bytes
from the start of the record; the kernel entry is at `0x10`.

| Offset | Root | Value |
|--------|------|-------|
| `0x20` | Read/write buffers | Zero. |
| `0x28` | Constant buffers | Zero. No value in this field reaches a launch register; the constant descriptor is taken from `Constant`. |
| `0x30` | Constants | Argument base plus `0x1ce4`. |
| `0x38` | Resources and samplers | Argument base plus `0x3d20`, with bit 62 set. |
| `0x40` | Attributes | Zero; vertex output is read through SIO0, not through an attribute root. |

The resource root's flag is bit 62 of the field, bit 30 of its upper half. It is
not part of the address: a range check MUST clear it before comparing, and a
rebase MUST preserve it, because the flag alone puts the raw value above every
valid range and hides a sub-base address. Only the constant and resource roots
are live, so a host that rebases records into a new argument block MUST move
both and MAY leave the other three alone.

The geometry resource table is its own: 17 slots of `0x40` bytes at argument
offset `0x3d20`, so a geometry stage pointed at the fragment table reads that
stage's binding for the same descriptor index. The geometry argument layout is
also longer than the vertex and fragment layouts alone: a vertex-stage constant
window sits at `0x1d00`, while the geometry constant descriptor at `0x1ce4`
plus the compiler's offsets reaches `0x247c` for the uniform table and `0x3c20`
for the storage table. A host that sizes the argument block from the vertex and
fragment layouts truncates the geometry tables, so the range MUST cover the
geometry layout whenever a geometry stage can be bound.

## Validating a compiled pipeline

Transform feedback and raster output observe different consumers of the emitted
vertices. Correct captured positions and attributes do not establish that the
SIO record reached the rasterizer or that the fragment stage wrote an
attachment. Validate both against independent references: emitted order,
assembled primitive count and buffer guards for capture; coverage and
interpolated values across the attachment for raster output. Comparing two
images alone can miss a draw that leaves both images at the clear value.

When diagnosing missing output, retain the clear and draw contents of the
attachment allocation as well as API readbacks. CPU access to an exported
DMA-BUF requires completed rendering and the DMA-BUF CPU-access synchronization
protocol. An unchanged allocation narrows that observation to a missing write
or an earlier pipeline failure; it does not identify an SIO instruction or a
particular stage. Likewise, an occlusion-query result is not a fragment-shader
execution count. Shader storage side effects can provide a separate progress
observation, but their exact-lane mask and memory visibility remain part of
that observation's contract.

To isolate a consumer, verify that the producer's encoded binary and launch
state stay identical when changing the fragment program. A linker can move
arithmetic between stages or remove outputs, so source similarity alone does
not establish that control. Separate shader programs can help retain the
producer, but the resulting binaries and state still need comparison. A
constant fragment program also does not bypass coefficient generation when
the host requests coefficient groups. Record that request independently of
the shader's coefficient loads.

Inspect wait domains in the encoded instructions. An unassigned compiler IR
domain can select a default during packing; it is not necessarily an encoded
zero. A full-domain wait covers the documented ordering domains, but does not
by itself prove the lifetime of a fixed-function record or completion of an
attachment write.

These checks validate the compiled program and its launch state. They do not
extend an instruction's documented semantics to other operand forms or stages,
and a finite successful run does not resolve an intermittent failure.

## Open questions

- **Primitive id and invocation id share `v1`.** Both lanes read vector
  register 1 and the two enable bits are independent, so a stage that uses both
  has no established lane assignment.
- **Partial SIO rows.** Whether a window below 32 channels, or a tail that is a
  partial 32-channel row, aliases a neighbouring vertex is not established.
- **Layer records.** The shape of the layer record that `Output Layer` expects
  is not established, which is why the bit is left clear and layer writes land
  in layer 0. Arbitrary per-layer output counts and the complete cut protocol
  are open with it.
- **Invocations with different emit counts.** Invocations that emit different
  vertex counts work only through the runtime route with one thread per wave.
- **Streamout from a geometry record.** A geometry stage that captures
  transform feedback stores its emitted records itself. Whether a streamout
  stage can instead walk a geometry record directly is not established.
