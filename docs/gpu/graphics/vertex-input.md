# Vertex Input

The vertex stage fetches its own inputs: each attribute is a formatted buffer
load that the shader emits against a 16-byte attribute record, and the record
plus an element index determines the address.

## Attribute table

The table is 16 records of 16 bytes, 256 bytes in client memory, not in the
state block. The vertex stage reaches it through the fifth root of its
shader-info record, `Attributes`, a 64-bit address at record byte `+0x40` that
the launch copies into `s8:s9`. Slot `n` starts at byte `n * 16` and is read
with a four-dword scalar load from `s8:s9 + n * 16`. Where the table sits is a
client convention: the root MUST name the table the command stream filled.

## Attribute record

A vertex attribute uses the shared buffer-descriptor layout. These are the
fields a fetch reads:

| Bits | Field | Vertex meaning |
|------|-------|----------------|
| `[47:0]` | Base address | GPU virtual address of the first component of element 0 |
| `[61:48]` | Stride | Byte distance between consecutive elements |
| `[95:64]` | Byte size | Bytes readable from the base address |
| `[98:96]` | Destination select X | Channel delivered in lane 0 |
| `[101:99]` | Destination select Y | Channel delivered in lane 1 |
| `[104:102]` | Destination select Z | Channel delivered in lane 2 |
| `[107:105]` | Destination select W | Channel delivered in lane 3 |
| `[110:108]` | Number format | Numeric interpretation of the fetched bits |
| `[114:111]` | Data format | Component count and component width |

Every other bit is zero in an attribute record: cache swizzle `[62]`, swizzle
enable `[63]`, element size `[116:115]`, index stride `[118:117]`, add thread
ID `[119]`, ATC `[120]`, hash enable `[121]`, heap `[122]`, memory type
`[125:123]` and type `[127:126]`.

The record describes a byte window, not a binding: two attributes may read one
buffer at different offsets and strides, and the sixteen slots bound the
attribute count.

The base is the buffer address plus the binding offset plus the attribute's
offset in the element layout. The stride is the binding's vertex stride, or the
element block size when the binding is tightly packed. The byte size runs from
the base to the end of the binding, capped at `0xffffffff`. The selects are the
identity order `(X, Y, Z, W)` up to the tuple's width, then `ZERO` in the
remaining lanes and `ONE` in the fourth; a byte-reversed element such as
`B8G8R8A8` swaps `X` and `Z`.

A base of zero is a fetch at virtual address zero, which no client mapping
covers, and a byte size of zero is not encodable. An attribute with no backing
data still needs a mapped byte and a nonzero size.

## Fetch

For attribute `i` and element index `n`:

```text
address = base[i] + n * stride[i]
```

The fetch is one formatted buffer load of one to four components. The record
travels inline, in four consecutive, four-aligned scalar registers; the index
travels in a vector register, and the 12-bit offset field is zero because the
element offset is already in the base.

The index is `v0`, the vertex number with the draw's first vertex already
applied, for a per-vertex attribute. It is `v1`, the instance number, for a
divisor of 1, a quotient of `v1` for a larger divisor, and the constant zero for
an element baked into the base.

The fetch width is the smaller of the lanes the shader declares and the channels
the tuple delivers. Lanes beyond it become constant zero, and one in the fourth
lane, encoded as integer one for an integer input. The cap matters because the
hardware steps by the stride, not by the tuple width: four components through a
two-channel tuple would read the next vertex.

## Element formats

The data format fixes the component count and width; the number format fixes how
the fetched bits convert.

| Value | Data format | Components |
|-------|-------------|------------|
| 1 | R8 | 1 x 8-bit |
| 2 | R16 | 1 x 16-bit |
| 3 | RG8 | 2 x 8-bit |
| 4 | R32 | 1 x 32-bit |
| 5 | RG16 | 2 x 16-bit |
| 10 | RGBA8 | 4 x 8-bit |
| 11 | RG32 | 2 x 32-bit |
| 12 | RGBA16 | 4 x 16-bit |
| 13 | RGB32 | 3 x 32-bit |
| 14 | RGBA32 | 4 x 32-bit |

Number format: 0 UNORM, 1 SNORM, 2 USCALED, 3 SSCALED, 4 UINT, 5 SINT, 7 float.
Selects use the channel codes `ZERO` 0, `ONE` 1, `X` 4, `Y` 5, `Z` 6 and
`W` 7; 2 and 3 are unassigned. Values absent from either table have no assigned
meaning.

Two shapes have no data format of their own. A three-component 8-bit or 16-bit
element claims the four-component tuple, which reads one or two bytes past the
third component of the last element, so the described window MUST contain them.
A packed `10_10_10_2` element is fetched as `R32` with the `UINT` number format
and the `(X, ZERO, ZERO, ONE)` selects, then unpacked in the shader: channels at
bits 0, 10, 20 and 30 with widths 10, 10, 10 and 2, where UNORM divides by 1023
and the 2-bit alpha by 3, and SNORM divides by 511, leaves alpha undivided and
clamps every channel to at least `-1`. Any other packed layout follows the same
rule: fetch the containing 32-bit word.

## Limits

| Item | Limit |
|------|-------|
| Attribute slots per table | 16 |
| Record size | 16 bytes |
| Stride | 14 bits, 0 to 16383 bytes |
| Components per fetch | 1 to 4 |
| Offset field of the fetch | 12 bits, zero for a vertex fetch |

The base field is 48 bits, the byte size 32 bits, the data format 4 bits, the
number format 3 bits and each select 3 bits. Slots at or above 16 cannot be
fetched, so a shader that declares more inputs MUST receive the constant tuple
`(0, 0, 0, 1)` for them, and a record above slot 15 is never read.

## Instancing and the divisor

No bit of the record sets an update rate, and no other launch state names one. A
client either bakes the element into the base and indexes with zero, which costs
one submission per instance, or computes the index from `v1`, which costs
arithmetic per fetch. For the second form the element is
`first + instance / divisor` when the divisor is nonzero and `first` when it is
zero; the first-instance offset is added outside the division.

> UNRESOLVED: whether the device can step an attribute on its own. No field
> outside the record names a per-slot rate, and a record whose index is always
> zero is consistent with none, without proving it. A driver MUST treat the
> fetch as strictly per-index until it is settled.

## Out-of-range fetch

The byte size records the readable window; behaviour beyond it is not
established.

> UNRESOLVED: the result of a fetch whose address lies outside
> `[base, base + byte size)`.

A driver MUST therefore back every described window with mapped memory and MUST
NOT rely on the byte size to make an out-of-range fetch safe: a fetch through an
unmapped address raises a protection fault on the graphics pipe, which costs the
frame and the pipe, not just the vertex.

The root that names the table is one field of the [shader-info
record](../shaders/shader-abi.md); the record layout is shared with the
[buffer descriptor](../shaders/descriptors.md#buffer-descriptor) used by
storage, streamout and texel access.
