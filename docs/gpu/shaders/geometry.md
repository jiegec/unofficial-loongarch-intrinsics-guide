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
| Compact | Default when the Multi and Wide gates do not apply. | Stride `2 + S + P - 1 + max(N, 1)`; total `1 + 32 * stride`; varying row 4 for `N <= 1`, otherwise `align_up(4 * N, 16)`. |
| Multi | `N` is 2 or 3, `P = 1` and no special chunk. | For `N = 2`: window 96, export 97, stride 3, attributes 3, threads 31. For `N = 3`: window 128, export 125, stride 4, attributes 3, threads 30. |
| Wide | `S = 0` and the compact shape would exceed eight vertices or the encoded attribute and export fields. | `cut = 32 * ceil(M / 32)`; `slots = 8 * ceil(M / 8)`; `window = cut + 4 * (P + N) * slots`; stride `floor(window / 32)`; position size `P * slots / 8`. |

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

A cut is not an emit. It resets the primitive separator, updates the header slot
with the emitted-vertex count, restores the active lane mask and orders before
the next emit. A runtime emitter MUST preserve the separator state between
vertices of the same strip.

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

The current compiler rounds a wide record's window to a multiple of 32 channels,
streams constant entry-block input reads in channel order, and uses an allocated
temporary for the saved mask. These are implementation choices, not hardware
requirements.

## Open questions

- Geometry invocations with different per-invocation emit counts.
- Arbitrary per-layer output counts and the full primitive-cut protocol.
- The complete hardware condition for a partial SIO row.
- Whether the streamout stage can consume a geometry record directly.
