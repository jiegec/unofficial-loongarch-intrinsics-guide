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
probes; subsequent boundaries cover one through eight distances, including
single-vertex point-size records. This does not establish
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
minimum_window = max(position_channels + 4*V, 4*(V + 2), 12)
window = minimum_window, if minimum_window <= 32
         32 * ceil(minimum_window / 32), otherwise
```

Wide records reserve complete 32-channel rows. With the previous unrounded
policy, matching producer/consumer widths and ascending reads still produced
input quads from adjacent vertices in focused six-vertex probes. For example,
128 generic components used a 136-channel window and failed; a 160-channel
window passes. This rounding is the current conservative layout policy, not a
claim that every partial-row window fails. A 156-channel window also passes
that particular varying-only probe. The complete hardware condition for partial
rows remains `UNRESOLVED`.

Both stages receive the same producer-derived dimensions. All producer writes
and consumer reads must agree on that window. The vertex stage feeding geometry
omits its final SIO export; the geometry stage consumes the staged SIO0 record
directly. SIO reads are issued in ascending ring-channel order; the compiler
copies each four-channel result out of the hardware's fixed destination sequence
before issuing a later read. The sequence and its wrap are documented in
[SIO](sio.md#reading-channels).

### Input scheduling and register pressure

A geometry stage can consume six vertices with 128 components each. Copying
all 192 four-channel input reads into persistent temporaries exceeds the
256-VGPR file even though the shader can reduce the values incrementally.
The shared LCO selector therefore streams eligible constant entry-block inputs:

* sort reads stably by their physical SIO0 channel;
* copy each c4 result immediately out of its fixed destination window;
* schedule pure SSA computations after their last required input;
* reassociate single-use scalar boolean `iand`, `ior`, and `ixor` reductions by
  input availability, so intermediate comparisons can die early;
* preserve the relative order of output stores, emits, and primitive cuts.

This route applies to entry blocks containing constant per-vertex input loads,
ALU instructions, constants, and output/emission intrinsics. Other shapes retain
the prologue cache or per-site path; this optimization does not establish a GS
scratch-memory ABI. Both channel-selection sites materialize ring-channel
indices with `v_mov_imm_b32`: an index above 64 does not fit the integer inline
constant range of `v_mov_b32`.

Gallium and loongvk request the same stage-specific NIR compiler options. GS
permits 128 unroll iterations and forces indirect shader-input unrolling, so
constant input loops can resolve before indirect IO becomes a large selection
tree. Other stages retain the default limit of 32. Shared early IO preparation
only runs its extra unrolling when indirect input dereferences exist; it must
not rewrite unrelated emit loops merely to prepare input loads.

## Compiler control-flow and distance IO contracts

These are compiler requirements for composing correct GS programs; they do not
add instruction semantics to the hardware ABI.

Clip and cull distances are compact scalar arrays. Logical element `i` occupies
component `i mod 4` of slot `base + floor(i / 4)`. Before lowering dereferences
to IO instructions, the compiler must lower indirect compact-array accesses to
constant component accesses. Treating a scalar index as a vec4 slot offset reads
the wrong distance. Gallium and Vulkan use the same LCO IO preparation rule,
including the standalone compiler entry point. The resulting constant loads can
be selected by a balanced conditional tree; dynamic vertex selection remains a
separate operation on the packed input selectors.

An emit recipe can split a selected block and append its continuation after the
original NIR blocks. Phi sources must therefore follow **selected CFG predecessor
order**, with each value attached to the actual predecessor that supplies it.
NIR block indices alone cannot order those operands. Swapping the preheader and
latch operands can reset a loop induction variable on every iteration and make
a finite shader run indefinitely.

Register allocation must also use the selected CFG. A value live on entry must
have a register before local scratch definitions execute, even if its first
textual definition appears in a later block. Its lifetime includes blocks that
carry it to a successor without reading it. In particular, a block placed after
the last textual use can execute before that use; freeing the register at the
last textual read lets the incoming edge destroy the value. These rules apply
to both the interval allocator and the fallback instruction walk. LCO regression
tests cover appended emit preheaders, loop-carried values, and backward layout
edges in acyclic control flow.

A destructive two-address instruction, such as `v_mac_f32`, requires its
addend and destination to occupy the same register at execution. This does not
permit the allocator to discard the original addend if it remains live. The
source and destination may share storage only when the source's final read is
that instruction; otherwise the compiler copies the addend into the destination
before the instruction. A single textual read inside a loop can execute many
times, so a loop-invariant source must survive the back edge. Violating this
contract displaced and stretched the geometry in three instancing CTS cases;
it was independent of the SIO strip-cut protocol.

## Ordering and execution masks

`check` waits for work already issued in the domains selected by
`INST_LO[3:0]`. The domain bits are scalar/kernel memory `1`, vector memory `2`,
vector stores `4`, and shared memory `8`; `15` selects all four. The encoding
and divergent-region experiments are described in
[Control and flow: Ordering](../isa/family-control.md#ordering).
It is not a host completion fence or a lane rendezvous. A check before entering
a narrowed region cannot order work subsequently issued inside that region.

For geometry programs, constant-root and layer-filter scalar loads use domain
`1`. The individual domain needed by SIO writes and emits remains
**UNRESOLVED**; LCO uses the full mask for waits covering these producers.
SIO staging must be ordered before the interface consumes it, and storage must
remain valid until the operation that reads it completes. An explicit operand
list alone is insufficient to describe that lifetime: channel-slot selectors
also name descriptor registers, and `PJ2` names the execution-mask register.

The compiler may narrow a wait only if it accounts for every producer that can
reach it, including predecessor blocks. It must recompute the domain after
instruction scheduling or other producer changes. Keeping an earlier scalar-only
mask after an SIO producer has been introduced can leave interface work pending.
In LCO IR, `check_domain = 0` means **unassigned**, with a default encoded mask of
`15`; it does not mean the emitted instruction waits for no domains. The diagnostic
`LOONG_CHECK_DOMAIN` setting changes this fallback only. It does not override
an explicit narrowed domain, and an encoded zero has no established producer
contract.

Post-allocation scheduling must retain the implicit `PJ2` dependency. A load
cannot move above the instruction that installs its selected predicate register,
even when its address and destination registers are independent of that write.
The same distinction between explicit values and interface state applies when
reviewing wait placement and descriptor reuse.

Full-domain checks do not by themselves establish layered-rendering correctness.
The composed program also depends on record layout, primitive cuts, layer replay,
attachment visibility, and host resource lifetime. Additional waits immediately
following runtime emits and submissions split between replay layers have not
eliminated the observed failures.

### Positions retained during layer replay

The implementation replays a dynamic-layer draw against individual attachments.
It skips some nonmatching emits but retains records needed to prime or advance
the stream. Such a record must not introduce visible geometry. The runtime
recipe uses the null homogeneous position `(0, 0, 0, 0)` for all four position
components when the layer does not match. `(0, 0, 0, 1)` is an ordinary point at
the Cartesian origin: together with two live strip vertices it can form a
nonzero-area triangle. A triangle containing a null homogeneous vertex has zero
projected area. This distinction matters even when all the placeholder vertices,
considered as a separate primitive, would coincide.

For statically enumerated layer-writing streams, the current compiler also
prepends eight emits in two four-vertex primitives. Each prefix emit explicitly
stores the complete null homogeneous position using lowered `store_output` IO.
An unwritten application output is undefined; a compiler-generated prefix must
initialize its own position rather than depend on a selector fallback. The
prefix raises the declared output count by eight, is excluded from transform
feedback, and is not inserted while emit sites remain inside loops. Removing
it has regressed other rotating-window shapes. Its presence is an implementation
workaround, not a requirement imposed on application shaders or a complete
explanation of the hardware's stream-window protocol.

Targeted LG200 runs with null positions pass the previously failing layered
readback and differing-content cases, including sequences of separate CTS
processes. The primitive-cut behavior and arbitrary record-capacity boundaries
still require independent validation; those results do not establish general GS
conformance.

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
point_size_group = 1 if the stage writes PointSize; else 0
distance_offset = 8 channels, when G > 1 or point_size_group = 1
                  4 channels, otherwise
P = next_power_of_two(distance_offset / 4 + G), when G > 0
    1 + point_size_group, otherwise
point_size_offset = 4 channels
```

Point size uses the miscellaneous quad immediately after position. A second
distance group also requires this quad even when point size is absent.
The reservation also applies to `max_vertices = 1`: omitting it lets the
point-size write at offset 4 overwrite the first distance group. A negative
distance can then become a positive point size and incorrectly survive clipping.
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

When point size and distances are both present, the shared host ABI declares
`Output Clip Distance Count = 4*ceil(C/4)`, where `C` is the logical merged
clip/cull payload count. Otherwise it declares `C`. This is a physical record
count: the emitter writes all four components of each distance group and uses
zero for components without a shader value. Clip-plane enables, cull counts,
and fragment input array sizes retain their logical values. Padding therefore
adds neither an enabled plane nor a fragment input. With the current point-size
record plan, advertising 1, 2, or 3 instead of the initialized quad loses point
vertices even when clipping is disabled; advertising 4 repairs the same binary
and record dimensions. This observation establishes the required producer/
consumer contract for this plan; it does not establish a different, more densely
packed record format.

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
Static point and strip emitters materialize that index with `v_mov_imm_b32`.
Ordinals 65 through 127 exceed the integer inline-constant range of
`v_mov_b32`; unrolling a 128-vertex output loop must still produce encodable
instructions at every emit site.
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

Static point streams initialize their separator payload VGPR, `v35`, to zero in
the prologue before an emit can read it. Runtime recipes also initialize it once
and preserve it across iterations. Runtime point separators are written for
every active invocation, without the line/triangle recipe's GA-based lane gate.
Applying that gate to points dropped a single runtime output; emitting additional
points could conceal the loss. Leaving the static value uninitialized caused
intermittent missing points when several invocations shared a launch; reducing
the thread count concealed the failure. Initialization repairs the focused
point-depth controls without reducing their thread count.

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

For Vulkan point draws routed through the driver's passthrough GS, depth clipping
uses the homogeneous predicate `w > 0 && z >= 0 && z <= w`. Rejected points receive
position `(2, 2, 0, 1)`, outside the x/y clip volume, and the meta shader emits one
record per input. This avoids depending on conditional-emission behavior in this
internal shader. Depth-clamp and explicit depth-clip-disable select the variant
without that depth predicate. This is a driver emulation policy, separate from
the shared GS record ABI and from the API's user geometry shader.

The selector rejects more than five output varying planes, unsupported input
vertex selectors above 5, clip/cull payloads above eight components, and SIO1
windows wider than the 4096-channel ring. The register allocation record must
also fit the 256-register VGPR file. A legal API shader shape can therefore
still be refused when its LG200 record cannot be represented.

## Unresolved behavior

* The selected 64-case GS clip/cull CTS matrix passes after correcting phi
  edge ordering, compact indirect IO lowering, and register lifetimes. This
  establishes those composed pipelines, not every possible distance-output
  shape or ring-capacity boundary.
* Geometry invocations with different per-invocation dynamic emit counts are
  not proven. The selector's current runtime descriptor path broadcasts an
  ordinal from the first active lane; this does not define a correct shared
  record for divergent invocation counts.
* Layer replay now initializes retained and synthetic positions explicitly.
  Targeted readback and `render_different_content` regressions pass; arbitrary
  per-layer output counts and the full primitive-cut protocol remain unproven.
  Replaying layers is a driver policy, rather than a native layer ABI.
  A 1D layered depth/stencil attachment is a height-one 2D surface. Its array
  layout uses the depth surface's tile mode and per-layer offsets; treating it
  as a linear 1D colour array permutes depth/stencil pixels. Format queries,
  allocation, attachment validation and readback must agree on that layout.
* Point-size distance records now declare initialized distance quads, and a
  single emitted vertex reserves the same miscellaneous quad as multiple
  vertices. Focused probes cover 1–8 distances and static/runtime point output;
  arbitrary record capacities and divergent emission remain unproven.
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
scheduling is in
`src/mesa/src/loonggpu/compiler/instruction_selection/lco_gs_input_schedule.c`;
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
| Distance output probes | 12 cases: 1/4/8 distances, static/runtime point and triangle output | 12 pass after matching the physical distance count to the point-size record |
| Distance record boundary probes | 16 cases: 1–4 distances, clip enable off/on, fragment distance read off/on; 24 cases: 1–8 distances, point size off/on and static/runtime point output | 40 pass; the first matrix previously had 12 failures |
| Single-vertex point records | 7 cases: no distances or 1/4/8 distances, positive/negative distance, with point-size footprint checks | 7 pass; reserving the miscellaneous quad repairs the negative 1/4-distance cases |
| Runtime point emission boundaries | 0–3 loop iterations, all/mixed/no active emit conditions, two independent processes per shape; varying values and distinct output rows | 24 pass after removing the strip separator gate from point output |
| Vulkan point clip-volume controls | `inside`, `outside`, `depth_clamp`, `depth_clip`, and `clipped.large_points`, three separate processes each | 15 pass after initializing the static separator and completing the meta depth predicate |
| Piglit geometry corpus (historical aggregate) | 37 cases | 24 pass, 1 fail (`max-input-components`), 12 skip |
| Vulkan CTS geometry matrix (historical aggregate) | 199 cases | 179 pass, 10 fail, 10 NotSupported, no GPU fault in this matrix |
| Vulkan CTS clip/cull matrix | 64 `vert_geom` cases, static/dynamic indexing, with/without fragment distance reads | 64 pass, no GPU fault |

A later focused `max-input-components` check passes after input scheduling,
full-width channel materialization, and wide SIO0 row rounding. Its negative
control changes every producer value by one and correctly renders the failure
colour. Ten selected input-width cases, including 20, 24, 28, 32, 56, 64, 96,
120, 124, and 128 components, pass with the corrected layout. These targeted
results do not replace the historical aggregate with an inferred suite total.

The two previously unsupported 1D-array readback cases, at 12×1 with six layers
and 64×1 with four layers, pass after enabling height-one depth/stencil
attachments with a matching tiled layout. Focused 1D mip rendering controls for
D16, D24S8 and S8, a 1D-array D24S8 mip control, and a D16 1D-array sampling
feedback-loop control also pass. The other eight historical geometry skips
request 64 or 127 invocations, exceeding the implementation's advertised limit
of 32; these results do not raise that limit.

The aggregate geometry result above predates the null-position correction.
Subsequent targeted checks pass nine previously failing layered cases in three
separate processes each, and twelve prefix-sensitive secondary-command-buffer
cases in three processes each. A subsequent two-address lifetime correction
passes `output_vary_by_attribute_instancing`, `output_vary_by_uniform_instancing`,
and `output_vary_by_texture_instancing` five times each in separate processes.
These focused results do not replace the aggregate matrix with an inferred
total, or establish arbitrary divergent invocation emission.

Layered rendering still has an intermittent VM fault. A later focused sequence
recorded 29 passes before
`layered.cube_array.64_64_12.secondary_cmd_buffer_inherit_framebuffer` faulted
at unmapped address `0x200004100`. Device recovery and its health control failed,
so remaining cases were not executed. Producer and driver causality remain
`UNRESOLVED`. Previously isolated secondary-command-buffer cases remain in the
validation backlog; successful pixel comparisons do not establish fault freedom.

Complete submission captures are available through `LOONGVK_DUMP_SUBMIT`, an
absolute output prefix. Each submission produces `.meta` and `.ib` files, plus
the used `.root` and `.resource` prefixes when present. Unlike
`LOONGVK_DUMP_IB`, which captures draw construction, this records the final IB
length and submission residency. `LOONG_DUMP_VAMAP_FILE` adds the exact pinned
BO mappings, submission token and returned kernel fence sequence. Match the
captures by process, IB address/length and timestamp/token, rather than assuming
file sequence numbers identify the same submission across processes.

With this instrumentation, one planned hardware replay of the isolated
inherited-framebuffer case passes without a new fault or reset. Its three
submissions contain 12 initialization records and 24 GS records. Direct shader,
root/resource and render-target addresses match resident VA mappings; this
check does not validate shader-computed addresses, nested descriptors or lifetime
races. The original fault therefore remains unresolved. Ten historical geometry
failures also pass in two separate processes each with the default configuration
and diagnostics disabled; these focused checks do not replace the earlier
aggregate results.

`LOONG_NO_SUBMIT=1` supports offline construction and capture by completing
the caller's binary and timeline syncobjs on the CPU. It executes no GPU
instructions. Any CTS result reported in this mode is host-flow evidence only
and must never be counted as hardware validation.

The earlier timeout in
`dEQP-VK.clipping.user_defined.clip_cull_distance_dynamic_index.vert_geom.1_7`
was caused by reversed phi-edge values after emit selection. It now passes;
its historical fault and recovery records remain in the quarantine ledger.
The subsequent distance failures exposed compact-array addressing and register
lifetime errors, corrected independently of the SIO record formulas.

Eight geometry NotSupported cases request 64 or 127 invocations, beyond the
current 32-invocation limit encoded by the five-bit `N - 1` field. Two 1D-array
readback cases are rejected by the depth/stencil image-dimensionality query.
Supporting either group requires implementation and validation; changing the
advertised limits alone would not provide that support. Exact case logs and
binary hashes are kept in the Mesa worktree's GS review artifacts.

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
