# Geometry Stage

This chapter records the LG200 geometry-stage contract used by the open LoongGPU
compiler and drivers. It distinguishes observed hardware behavior from the
current Mesa layout policy. The latter is a compiler/host agreement, not an
independent hardware guarantee. Instruction semantics and encodings remain in
the instruction reference; the SIO operand roles are summarized in
[SIO](sio.md).

## Scope and status

The supported API stages are vertex, geometry, and fragment. Geometry input is
read from the SIO0 ring, and geometry output is committed through SIO1. The
vertex and geometry roles share the domain-stage launch path; the geometry stage
uses shader type 4 and receives its six packed input-vertex selectors in `v0`.
Selector `i` is the five-bit field at bit offset `5 * i`, for `i` in `0..5`.

The compiler and host share one geometry-record decision. The compiler publishes
the chosen SIO1 window, export count, stride, position size, attribute count,
and thread count in shader info. Gallium and loongvk consume these published
values; their fallback calculation exists only for older binaries without the
record. Shader caches must invalidate entries when this shared record contract
changes. A cached binary cannot be reinterpreted using a different plan.

The current implementation has passing deterministic boundary probes for static
point records without clip distances, runtime line-strip emission, multi-varying
strip channels, and static and uniform-loop triangle-strip clip distances. Four-
and eight-distance point-size combinations also pass static and uniform-loop
probes; the one-distance combination remains failing. It does not establish
arbitrary non-uniform invocation emission, every dynamic distance-output shape,
transform-feedback parity, or complete layered rendering. The Vulkan geometry
CTS and Piglit geometry suites remain the correctness oracles for composed
programs.

## Vertex-to-geometry input

The hardware input selector contains six five-bit vertex selectors. The compiler
accepts constant selectors in `0..5`; dynamic selectors are clamped to that
range before extraction. Input position uses channels `0..3`. Clip and cull data
occupies the position-adjacent payload described below. Generic varying quads
follow the entire position block in the linked layout order; the consumer and
producer use the same linked varying bases, including slots that are present in
the link but not directly loaded by one stage.

Let `D` be the producer's clip/cull payload component count and `E` indicate a
position miscellaneous value such as the edge flag. The current position channel
count is:

```text
clip_base(D, E) = 8, if D > 4 or E; otherwise 4
position_channels(D, E) = 4, if D = 0 and not E
                           clip_base(D, E) + 4 * ceil(D / 4), otherwise
```

The generic varying at linked ordinal `b` starts at `position_channels + 4*b`;
component `c` is at that base plus `c`. Distance payload slots are not counted a
second time as generic varying quads. In particular, a miscellaneous quad or a
second distance group creates a gap between position and the distance payload;
that gap remains part of the position block even though it is not a user
varying.

For `V` linked generic varying quads, the current implementation chooses the
SIO0 window in channels as:

```text
max(position_channels + 4*V, 4*(V + 2), 12)
```

Both stages receive the same producer-derived dimensions. All producer writes
and consumer reads must agree on that window. The vertex stage feeding geometry
omits its final SIO export; the geometry stage consumes the staged SIO0 record
directly. SIO reads are issued in ascending ring-channel order; the compiler
copies each four-channel result out of the hardware's fixed destination sequence
before issuing a later read. The sequence and its wrap are documented in
[SIO](sio.md#reading-channels).

## Geometry output record

The record equations below describe the current implementation, including paths
with unresolved conformance failures. They are not a promise that every
combination renders correctly.

For output planning, `N` is the number of linked output varying planes, `M` is
the API `max_vertices`, `P` is the number of position quads reserved by the
geometry output, and `S` indicates a special layer/viewport record chunk. A quad
is four 32-bit channels. Distance and point-size payloads are included in `P`.
Linked distance varyings are also included in `N` when the interpolation layout
needs their second copy; point size is not a generic varying.

The current setup derives distance groups from the declared output mask and
array sizes:

```text
G = max(number of declared distance output groups,
        ceil((clip_distance_array_size + cull_distance_array_size) / 4))
point_size_group = 1 for a multi-vertex stage that writes PointSize; else 0
distance_offset = 8 channels, when G > 1 or point_size_group = 1
                  4 channels, otherwise
P = next_power_of_two(distance_offset / 4 + G), when G > 0
    1 + point_size_group, otherwise
point_size_offset = 4 channels
```

Point size uses the miscellaneous quad immediately after position. A second
distance group also requires this quad even when point size is absent.
Position-size and varying-row layout then depend on the record class. This is
the implemented decision table, not a claim that every input is admitted by
hardware:

| Record | Selection and layout |
|--------|----------------------|
| Compact | Default class when the Multi/Wide gates below do not apply. `payload = P - 1`; `stride = 2 + S + payload + max(N, 1)`; `total = 1 + 32*stride`; `channum = total - 1`; export count is `total`; position size is `1 + S + payload`; position descriptor base is 32; varying descriptor base is `64 + 32*(S + payload)`; varying row is 4 for `N <= 1`, otherwise `align_up(4*N, 16)`. |
| Multi | Applies to `N=2` or `N=3` when `P=1` and no special chunk is present. For `N=2`, `(channum, export, stride, attributes, threads) = (96, 97, 3, 3, 31)`; for `N=3`, `(128, 125, 4, 3, 30)`. Position size is 1 and varying descriptor base is 64. These are measured LG200 ABI cases, not an extrapolated formula. |
| Wide | Applies when `S=0`, `(M>8 or N>5)`, and `(N>=1 or M>32)`. Also selected for `S=0`, `P>1`, and either `M>8` or `1+P+max(N,1)>=8`. `cut = 32*ceil(M/32)`; `slots = 8*ceil(M/8)`; `window = cut + 4*(P+N)*slots`; `stride = floor(window/32)`; position size is `P*slots/8`; position descriptor base is `cut`; varying descriptor base is `cut + 4*P*slots`; varying row is `4*next_power_of_two(max(N,1))`. Export count is `floor(4096/window)*stride + 1`, the current whole-window commit rule. The thread field is `floor(4096/window)-1`. A window above 4096 channels is rejected; direct encoding of 4096 in a 12-bit field requires separate validation. |

For payload-bearing records, the varying row is rounded to
`4*next_power_of_two(max(N,1))` in both compact and wide paths. The compact
route is promoted when the declaration exceeds eight vertices or its encoded
attribute/export shape would overflow the compact field. The Multi route is
limited to its measured two- and three-varying, payload-free shapes. The current
plan supports at most five geometry output varying planes, including distance
varying planes; this is an implementation limit, not a statement about the
maximum linked varying count of every LG200 producer.

The varying descriptor for attribute `a`, component `c`, and emitted vertex `e`
is `varying_descriptor_base + varying_row*e + 4*a + c`. Position and distance
descriptors are record-relative. A position component uses
`position_descriptor_base + 4*P*emit + component`. The fixed-function distance
payload starts at channel offset 4 for one group without point size, or at
offset 8 when point size or two distance groups are present; the compiler also
writes clip-distance values to the SIO1 varying plane when fragment
interpolation consumes them. The channel and descriptor quantities are separate:
window width is measured in channels, while descriptor values name staged
records.

## Staging registers and static site capacity

The staging channel is a VGPR selector, not the semantic descriptor address. The
current static plan uses these formulas (`e` is the static site, `c` a
component, and `a` a varying ordinal):

| Mode | Position channel | Varying channel |
|------|------------------|-----------------|
| Adjacent, for `N>1` or `N=1` and more than four sites | `2*c` | `2*(4+4*a+c)` |
| Position only, more than eight sites | `9+c` | none |
| Other static records | `4+4*e+c` | `varying_base+4*a+c` |

Here `varying_base = 4 + 4*P*(adjacent ? 5 : site_count)`. Distance descriptors
use even channels `2*(4+distance_component)` so they do not overwrite the odd
registers carrying window state. Reusing a descriptor channel is permitted after
its previous write has been ordered; it does not reuse the record address stored
in that channel.

The point-stream emitter keeps its separator mask in scalar register `s0`
between emit sites. Descriptor addresses outside the inline-immediate range
must therefore use an allocated scalar temporary; borrowing `s0` corrupts the
mask and drops later points or their varying payload. This lifetime rule is
specific to the current emitter schedule and is separate from the record
address formulas above.

The descriptor-plane capacity of the static plan is
`floor((varying_descriptor_base-position_descriptor_base)/(4*P))` sites. This is
a separation bound, not proof that an arbitrary control-flow shape is correct.
The declared maximum, static emit-site count, and number of vertices actually
emitted are distinct quantities.

The current compiler reserves `v33` for the window counter, `v35` for the
separator, `v19` for the final separator copy, and `v45` for the emit index.
These are compiler register choices. The ABI inputs `s4:s5` remain live as a
64-bit constant root: saving an execution mask into `s5` corrupts later constant
or UBO loads. Window maintenance must use a temporary whose lifetime is
represented to the register allocator.

## Emit and export sequence

The compiler selects `v_emit3` for triangle output and `v_emit2` for point and
line output. A runtime line-strip regression checks the first segment as well as
later output. A primitive cut is separate from the emit opcode. Geometry output
is ordered after each staged emit before the next payload overwrites its
registers. Before final export, the stream header is updated with the emitted
vertex count, then the SIO1 export commits the record's published export count.

The selector uses the measured `GK`, `GA`, and `VJ` operand roles described in
[SIO](sio.md#emitting). The instruction pages define the encoded fields; the
exact relationship between those fields and every possible stream record remains
an ISA-level `UNRESOLVED` item where the instruction reference says so.

## Defaults and limits

The defaults are the common plan-based static emitter, ordered SIO0 reads,
compiled record dimensions shared with the host, hardware invocation when the
shader shape is eligible, and `Output Layer` disabled in geometry-stage
configuration. Dynamic layer handling uses driver replay/filter paths where
selected; this does not imply that arbitrary layered GS output is complete.
Point records use `v_emit2`. Geometry input `PointSize` without a producer
carrier reads as the implementation's default 1.0.

The selector rejects more than five output varying planes, unsupported input
vertex selectors above 5, clip/cull payloads above eight components, and SIO1
windows wider than the 4096-channel ring. The register allocation record must
also fit the 256-register VGPR file. A legal API shader shape can therefore
still be refused when its LG200 record cannot be represented.

## Unresolved behavior

* The SIO0 ring window and actual producer/consumer position are confirmed for
  passing shapes, but the clipping CTS case
  `clip_cull_distance_dynamic_index.vert_geom.1_7` still times out after
  multiple different compiler/ABI inputs. It remains quarantined and is not a
  passing case.
* Geometry invocations with different per-invocation dynamic emit counts are
  not proven. The selector's current runtime descriptor path broadcasts an
  ordinal from the first active lane; this does not define a correct shared
  record for divergent invocation counts.
* Layered readback and some `render_different_content` cases remain failing or
  intermittent. Replaying layers is a driver policy; it is not part of the
  hardware geometry ABI established here.
* Point size combined with a single clip distance still fails the static and
  runtime point probes. Four- and eight-distance combinations pass, so those
  results must not be generalized to a smaller distance payload.
* Divergent loop-carried clip/cull outputs, transform feedback, and the complete
  SIO1 record-capacity boundary need
  additional direct probes and full CTS coverage.

See the [SIO unresolved list](sio.md#unresolved) and the stage-level CTS results
recorded by the Mesa worktree. A failure or timeout is evidence about that
tested pipeline only; it does not identify a kernel fault without a matching
kernel event.

## Implementation references and validation scope

The source of the layout equations is the open Mesa LoongGPU backend:
`src/mesa/src/loonggpu/compiler/instruction_selection/lco_gs_plan.c`. Input
selection and output setup are split into
`src/mesa/src/loonggpu/compiler/instruction_selection/lco_select_gs_input.c` and
`src/mesa/src/loonggpu/compiler/instruction_selection/lco_select_gs_setup.c`.
The shared host consumer is `src/mesa/src/loonggpu/common/loong_shader_abi.c`.

### Validation snapshot (2026-11-03)

These results describe one current Mesa worktree and LG200 device. They are
implementation evidence, not a statement of complete Geometry Shader
conformance.

| Suite | Scope | Result |
|-------|-------|--------|
| Point-record pixel probes | 110 cases: 3-32 emits, 0/1/2/3/5 varying planes, point size off/on | 110 pass |
| Multi-site and width probes | 30 deterministic site/attribute cases | 30 pass |
| Distance output probes | 12 cases: 1/4/8 distances, static/runtime point and triangle output | 10 pass, 2 fail; both failures are single-distance point output with point size enabled, using static/runtime emission |
| Piglit geometry corpus | 37 cases | 24 pass, 1 fail (`max-input-components`), 12 skip |
| Vulkan CTS geometry matrix | 199 cases | 177 pass, 12 fail, 10 NotSupported, no GPU fault in this matrix |

The Vulkan failures include per-invocation dynamic emission, some basic varying
output cases, and layered readback/render-to-all cases. The separate
`dEQP-VK.clipping.user_defined.clip_cull_distance_dynamic_index.vert_geom.1_7`
case timed out in an isolated run and triggered a graphics-ring reset; the
health control passed after recovery. It remains an unresolved failure, not a
NotSupported case. Exact case logs and binary hashes are kept by the Mesa
worktree in its GS review artifacts and quarantine ledger.

The deterministic regression sources are under
`src/mesa/src/loonggpu/compiler/tests/geometry/`. The point matrix checks all
four components of every varying, individual point locations, and point-size
bounds for 3, 4, 5, 8, 9, 12, 13, 16, 17, 24, and 32 emitted vertices with 0, 1,
2, 3, or 5 varying planes. The distance tests exercise positive and negative
clipping and fragment distance reads. An arithmetic/layout unit test proves
bounds for its input matrix; it does not replace these rendered-pixel oracles or
the API conformance suites.

Instruction-vector observations made by the fragment-stage ISA runner do not
establish geometry launch, record, or invocation behavior. They remain subject
to the [verification boundaries](../appendix/verification.md).
