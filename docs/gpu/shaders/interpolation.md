# Interpolation

The fragment stage never receives a varying value directly. For a smooth or
noperspective input the device delivers a pair of barycentric values in fixed
vector registers, and the shader reads one coefficient word per component from a
coefficient window and combines them with a pair of vector rows. A flat input
skips the pair and moves its coefficient word into the destination. Two state
contracts carry the mapping: the fragment input config names the coefficient
streams the device computes and the window the shader reads, and the attribute
map names the producer output behind each of the 32 input windows. Either one
wrong yields a plausible but wrong value rather than a fault.

## Modes and registers

| Mode | Pair | Enable bit |
|------|------|-----------:|
| Smooth, pixel centre | `v2:v3` | `0x0002` |
| Smooth, centroid | `v4:v5` | `0x0004` |
| Noperspective, pixel centre | `v8:v9` | `0x0020` |
| Noperspective, centroid | `v10:v11` | `0x0040` |
| Any mode, sample window | `v0:v1` | see below |

The bits form the fragment input config's enable word, a request per stream
rather than a per-varying mask: they combine, so one smooth and one noperspective
varying program `0x0022`. A shader with no varying still programs a stream bit,
the linear request when it reads nothing else and `0x0f02` when it reads the
coordinate, which is the coordinate group `[11:8]` plus the perspective request.
Reading the primitive face flag adds `0x1000`.

The config's other half, Input Address, describes the delivered window:
`0xf077` for the ordinary window, `0xff77` when the coordinate is read. Only
whole words are established; the meaning of its individual bits is not.

A pair is two ordinary 32-bit vector registers, so the mode is a compile-time
choice of register number, not a runtime selector. Centroid is a separate stream,
not an offset applied to the centre stream, and a centroid pair is computed only
when its bit is set: a shader that reads a pair whose bit is clear reads whatever
the window holds, so a driver MUST raise the bit for every pair the shader reads.
Only centre and centroid requests exist; per-sample shading is a pipe-level mode
that runs the fragment stage once per sample, and the shader still reads the pair
its qualifiers name. The coordinate sits at `v12..v15` (`v2..v5` in the sample
window), the coverage mask, when read, at `v2` (`v6` when the coordinate is
also read), and the face flag at `v16` when the coordinate is read, else `v12`.

## The sample window

A fragment stage that reads the per-sample coverage mask uses one fixed window,
and both halves of the input config then carry the same word: `0x4f02` when the
coordinate is also read, `0x4002` with a smooth varying, `0x4020` otherwise. Every
mode then reads its pair from `v0:v1`, so a centroid or noperspective request
loses its dedicated stream and is interpolated from the sample pair.

> UNRESOLVED: the point at which the sample window evaluates `v0:v1`, and whether
> the face flag is delivered there at all, since the window word has no face bit.
> The registers are fixed, but whether the pair is taken at the covered sample, at
> the pixel centre or at a fixed corner is not established, so a compiler MUST NOT
> assume an evaluation point for a centroid or noperspective input, and MUST NOT
> read the face flag in this window without another source for it.

## Coefficient window

Every varying owns one dense attribute window of up to four channels, numbered
from zero; the window index is the index of the attribute map entry that
describes it. One coefficient word exists per channel, and a component is
addressed by

```text
slot = 4 * window + channel + component
```

where `channel` is the first component the producer writes for that varying. The
slot is the eight-bit unsigned offset of the shared-memory interpolation load,
whose scalar base register is launch state; with 32 windows the largest slot is
127, which fits the field. Coefficient Count, six bits, holds the number of
four-channel groups in use, the highest window index plus one, derived from the
shader's own layout.

The load is an asynchronous shared-memory read drained by the shared-memory wait
class, so a consumer MUST wait before using the coefficient. The window is launch
state: the shader reads it and never initializes it.

> UNRESOLVED: the unit of the eight-bit offset. The encoder passes the slot number
> unchanged, so the device scales it, but whether the offset is a word index or a
> byte offset is not established. An out-of-range slot has no defined result.

## Flat inputs

A flat input carries no barycentric value. The shader loads the coefficient word
for the varying's slot and passes it through `v_interp_mov_b32`, whose scalar
operand selects the word to keep. Which vertex supplies it is a draw-state choice:
Pipe Enables `[0]` (Flat shade) makes flat-qualified inputs take the provoking
coefficient, and `[1]` (Flat shade first) makes the provoking vertex the
primitive's first vertex, clear selecting the last. A driver MUST set the
flat-shade bit for a shader that reads flat inputs: the pass-through selects a
provoking coefficient that only that state defines. Because the flat value never
mixes a barycentric pair, it does not depend on the enable word's stream bits.

## Two-phase rows

A smooth or noperspective component comes from a fixed three-row sequence, one
coefficient word per component:

```text
c = load_sm_interp base, slot
d = v_interp_1_f32 b0, c
d = v_interp_2_f32 d, b1, c
```

`b0` and `b1` are the two registers of the pair. The first phase combines one
barycentric value with the coefficient; over the row's reference cases the result
is `c * (b0 + 1)`, including the cases where either operand is zero. Whether the
sum and the product each round, or only the product, is not established.

| Field | Bits | Role |
|-------|------|------|
| `VD8` | `[31:24]` of dword 0 | Vector destination, and the in-place source of the second phase |
| `GJ11` | `[7:0]` of dword 0 and `[2:0]` of dword 1 | First operand: the first barycentric value, or the flat move's scalar selector |
| `GK11` | `[15:8]` of dword 0 and `[5:3]` of dword 1 | Second operand: the coefficient word |
| `PJ2` | `[17:16]` of dword 1 | Mask-register selector |

Neither `v_interp_2_f32` nor `v_interp_mov_b32` has a separate destination field:
each names its destination as its first source, so both are read-modify-write
rows and the phases may not be reordered against a write to that register.
`GJ11` and `GK11` are eleven bits: the low eight carry a register number and bit
8 selects the vector file, set for a vector register and clear for a scalar one.
The rows are per-lane operations whose lane mask comes from the mask register
named by `PJ2`, so a row updates only the lanes the mask selects.

> UNRESOLVED: the second phase's second term, its rounding and its denormal
> handling, and whether a gated row clears or preserves the destination lanes
> outside its mask. The phase's only reference case returns the first source
> unchanged and is not compared against an independent model. A compiler MUST treat
> the two rows as one unit, MUST NOT substitute its own interpolation formula, and
> MUST build a masked merge from a per-lane select rather than from several gated
> writes.

## Attribute map

The map has 32 entries of two bytes at byte offset `0x4f0`. Entry *i* describes
fragment input window *i*.

| Bits | Field | Meaning |
|------|-------|---------|
| `[4:0]` | Attribute id | Window index of the producing stage's output for this varying |
| `[5]` | Generated | The input stage supplies the value; the producer does not write this varying |
| `[6]` | Sprite texture | The window holds a generated point coordinate |

The producer numbers its outputs with the same dense window index, so entry *i* of
an ordinary varying names the producer's window for that varying. A geometry
producer numbers by the ordinal of the varying among the outputs it writes, which
differs from the window index when a varying is dropped. A window the producer
does not write is marked generated with id zero; a point-coordinate window holds
two components and is marked sprite.

The state block persists across draws, so a driver MUST write all 32 entries on
every draw. An entry left from an earlier draw maps a window to the wrong producer
output, and the shader reads another varying's coefficients.

> UNRESOLVED: the value a generated window delivers, and whether the sprite flag
> replaces the whole window or only its first two channels. The input stage
> decides both and no source states the delivered value.
