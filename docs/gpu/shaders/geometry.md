# Geometry Stage

The geometry stage reads the vertex stage's SIO0 record, emits primitives and
writes an SIO1 record. This chapter defines the input layout, the output record
classes and the emit/cut rules. The stage's registers are in
[Registers](registers.md); the SIO instructions are in [SIO](io.md).

## Input

The geometry stage runs in slot 3 with shader type 4. It receives six 5-bit
input-vertex selectors in `v0`: selector `i` is at bit offset `5 * i`. It also
receives the primitive identifier in `v1`.

Input positions use channels 0 through 3. Clip and cull data occupies the
position-adjacent payload. Generic varying quads follow the position block in
the linked layout order.

Let `D` be the producer's clip and cull payload component count and `E` indicate
a miscellaneous position value such as an edge flag:

```text
clip_base(D, E) = 8, if D > 4 or E; otherwise 4
position_channels(D, E) = 4, if D = 0 and not E
                          clip_base(D, E) + 4 * ceil(D / 4), otherwise
```

The generic varying at linked ordinal `b` starts at
`position_channels + 4 * b`. Distance payload slots are not counted a second
time as generic varying quads.

For `V` linked generic varying quads, the current implementation chooses the SIO0
window as:

```text
minimum_window = max(position_channels + 4 * V, 4 * (V + 2), 12)
window = minimum_window, if minimum_window <= 32
         32 * ceil(minimum_window / 32), otherwise
```

Wide records reserve complete 32-channel rows. The complete hardware condition
for a partial row is UNRESOLVED.

## Descriptor roots

In the current LG200 graphics ABI, the geometry Shader Info record uses the
following 64-bit roots. Offsets are bytes from the beginning of that record.

| Offset | Root | Geometry-stage value |
|--------|------|----------------------|
| `0x20` | Read/write buffers | Zero in the current GS ABI. |
| `0x28` | Constant buffers | Zero; buffer descriptors are reached through the constant root. |
| `0x30` | Constants | Address of the GS constant descriptor and its relative descriptor tables. |
| `0x38` | Resources and samplers | GS descriptor-table address with bit 62 set. |
| `0x40` | Attributes | Zero; vertex-stage output is read through SIO0. |

The resource-root flag is separate from the GPU virtual address. An address
range check MUST exclude the flag; relocating the root MUST preserve it.
Geometry resources have their own descriptor table and cannot alias the
fragment-stage table merely because the sampler indices match.

A host that caches Shader Info records must rebase both live GS roots when
moving to a new argument block. Its range must cover the complete GS argument
layout, including tables beyond the shorter VS/FS-only layout. Comparing the
flagged resource root directly with an unflagged address range, or bounding it
by the VS/FS reservation, leaves the old table address in an otherwise updated
record. These are requirements of the current descriptor ABI, independent of
the SIO output-record formulas below.

## Output record

The output record is an SIO1 window. The record is described by three classes.
The formulas below are the current host and compiler agreement; they are not a
claim that every combination is accepted by the hardware.

Let `N` be the number of linked output varying planes, `M` the API maximum
vertex count, `P` the number of position quads reserved by the output, `S` a
special layer or viewport chunk, and `G` the number of distance groups.

```text
G = max(declared distance groups,
        ceil((clip_distance_array_size + cull_distance_array_size) / 4))
point_size_group = 1 if the stage writes PointSize; otherwise 0
distance_offset = 8 channels, when G > 1 or point_size_group = 1
                  4 channels, otherwise
P = next_power_of_two(distance_offset / 4 + G), when G > 0
    1 + point_size_group, otherwise
```

Point size uses the miscellaneous quad immediately after position. A second
distance group also requires that quad even when point size is absent.

| Class | Condition | Layout |
|-------|-----------|--------|
| Compact | Default when the dense gates below do not apply. | Stride `2 + S + P - 1 + max(N, 1)`; window `32 * stride`; export count `window + 1`; position size `1 + S + P - 1`. |
| Dense | No special chunk, and either `P = 1` with `N = 2` or `N = 3`, more than eight declared vertices with a varying, more than 32 position-only vertices, or a position payload that would overflow compact storage. | Complete position and varying planes, with the formulas below. |

The small two- and three-varying cases use the dense layout too. A
single-varying stride cannot describe two full varying quads. Attribute count
counts the live varying quads, with a minimum of one for a position-only record;
it does not add a position/control quad to a two-varying output.

For a dense record:

```text
cut = 32 * ceil(M / 32)
slots = 8 * ceil(M / 8)
varying_row = 4 * next_power_of_two(max(N, 1))
varying_channels = varying_row * slots, if N > 0; otherwise 0
window = cut + 4 * P * slots + varying_channels
stride = window / 32
position_size = P * slots / 8
position_descriptor_base = cut
varying_descriptor_base = cut + 4 * P * slots
attribute_count = max(N, 1)
export_count = floor(4096 / window) * stride + 1
maximum_threads = floor(4096 / window) - 1
```

Allocate the padding in `varying_row` for every vertex. Three varying quads
have a four-quad row, and five have an eight-quad row. Using the live quad count
in the allocation while advancing descriptors by the padded row overruns the
window at later vertices. The window and export count have different units;
export count is not generally `window + 1`.

For `M <= 8`, `P = 1`, and no special chunk:

| Live varying quads | Row channels | Window | Stride | Export count | Maximum threads |
|--------------------|--------------|--------|--------|--------------|-----------------|
| 2 | 8 | 128 | 4 | 129 | 31 |
| 3 | 16 | 192 | 6 | 127 | 20 |

Compact records retain a row of four channels for `N <= 1` and
`align_up(4 * N, 16)` otherwise. A compact position payload uses
`4 * next_power_of_two(max(N, 1))` instead. Compact records rotate through their
window; the dense record's per-vertex containment rule must not be applied to
them without accounting for that rotation.

A window above 4096 channels is rejected. The current plan supports at most five
geometry output varying planes, including distance planes.

The varying descriptor for attribute `a`, component `c` and emitted vertex `e`
is:

```text
varying_descriptor_base + varying_row * e + 4 * a + c
```

A position component uses:

```text
position_descriptor_base + 4 * P * emit + component
```

The fixed-function distance payload starts at channel offset 4 for one group
without point size, or at offset 8 when point size or two distance groups are
present. The compiler also writes clip-distance values to the SIO1 varying plane
when fragment interpolation consumes them.

## Emit and cut

`v_emit2` is used for points and lines, `v_emit3` for triangles. The emit index
is in `GK11`, the record argument in `GA11` and the separator register in `VJ8`.
For a dense record, the argument names the final live varying component:

```text
GA = varying_descriptor_base + varying_row * emit + 4 * max(N, 1) - 1
```

The static and runtime emitters use the same record decision. The runtime
emitter scales the emitted-vertex ordinal separately for the position block
and the varying row; these strides need not be equal.

A cut is not an emit. It resets the primitive separator, updates the header slot
with the emitted-vertex count, restores the active lane mask and orders before
the next emit. A runtime emitter MUST preserve the separator state between
vertices of the same strip.

## Interpolation preservation

Uniformity across geometry invocations does not imply equality between emitted
vertices. A uniform loop counter changes on each iteration of one invocation;
a varying derived from it can require smooth interpolation. A compiler MUST
preserve the interpolation qualifier unless it proves that all vertices of a
primitive receive the same value. Reusing the same SSA definition at one store
inside an emit loop is insufficient evidence: the definition can have a new
value on each execution of the store. Values computed before the loop can
still be propagated or treated as constant across its emitted vertices.

## Limits

| Limit | Value |
|-------|-------|
| Input vertex selectors | Six 5-bit fields, values 0–5. |
| Output varying planes | Five, including distance planes. |
| Clip and cull payload | Eight components. |
| SIO ring | 4096 channels. |
| Output vertices | 8-bit field. |
| Static emit sites | 16 in the emitter's bookkeeping. |

## Host policy

The current compiler reserves dense records in complete 32-channel rows,
streams constant entry-block input reads in channel order, and uses an allocated
temporary for the saved mask. Runtime emitters use one active geometry thread;
layered hardware invocations also use that limit. This overrides the record's
computed maximum-thread default. For a GS-to-fragment handoff, the current
host programs at least five fragment coefficient groups, even when fewer are
live; reducing that initialized window can expose stale endpoint terms across
draws or contexts. These are implementation choices, not a claim that larger
thread counts are architecturally forbidden or that every coefficient-count
setting is interchangeable.

## Open questions

- Geometry invocations with different per-invocation emit counts.
- Arbitrary per-layer output counts and the full primitive-cut protocol.
- The complete hardware condition for a partial SIO row.
- Whether the streamout stage can consume a geometry record directly.
