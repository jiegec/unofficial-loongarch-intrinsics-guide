# Descriptors

A descriptor is a fixed-size record in GPU memory that describes a resource. The
shader-info record's roots point at descriptor tables; the tables' entries point
at payload. This chapter defines the record layouts, the binding slots and the
address rules.

![Descriptor indirection](../assets/fig-descriptors.svg)

The figure shows the three levels of indirection: a root, a table entry and the
descriptor that points at the payload.

## Binding slots

Images and samplers share a 0x40-byte slot:

| Offset | Size | Content |
|-------:|-----:|---------|
| `+0x00` | 32 | Image descriptor. |
| `+0x20` | 16 | Not part of a descriptor. |
| `+0x30` | 16 | Sampler descriptor. |

Buffers are bound in 16-byte records. An attribute table is a contiguous array
of the same 16-byte records.

| Binding | Record size | Tuple | Root |
|---------|------------:|-------|------|
| Buffer | 16 bytes | Four SGPRs | Descriptor table |
| Image | 32 bytes | Eight SGPRs | Image field of a slot |
| Sampler | 16 bytes | Four SGPRs | Sampler field of a slot |
| Attribute | 16 bytes per element | Four SGPRs | Attribute table |

## Buffer descriptor

| Bits | Field | Meaning |
|------|-------|---------|
| `[47:0]` | Base address | GPU virtual address of the payload. |
| `[61:48]` | Stride | Element stride; zero for uniform, storage, constant and streamout bindings. |
| `[62]` | Cache swizzle | Not used. |
| `[63]` | Swizzle enable | Not used. |
| `[95:64]` | Byte size | Accessible byte range. |
| `[98:96]` | Destination select X | Channel selector. |
| `[101:99]` | Destination select Y | Channel selector. |
| `[104:102]` | Destination select Z | Channel selector. |
| `[107:105]` | Destination select W | Channel selector. |
| `[110:108]` | Number format | UNORM, SNORM, UINT, SINT, float. |
| `[114:111]` | Data format | Element width and component count. |
| `[116:115]` | Element size | Not used. |
| `[118:117]` | Index stride | Not used. |
| `[119]` | Add thread id | Not used. |
| `[120]` | ATC | Not used. |
| `[121]` | Hash enable | Not used. |
| `[122]` | Heap | Not used. |
| `[125:123]` | Memory type | Not used. |
| `[127:126]` | Type | Not used. |

## Image descriptor

The image descriptor is 32 bytes. Its fields are in
[Texture and sampling](../graphics/texture.md#image-descriptor).

## Sampler descriptor

The sampler descriptor is 16 bytes. Its fields are in
[Texture and sampling](../graphics/texture.md#sampler-descriptor).

## Attribute descriptor

An attribute descriptor is a buffer descriptor whose base, stride and byte size
describe one vertex element:

| Field | Attribute meaning |
|-------|-------------------|
| Base address | Vertex buffer address plus the element offset. |
| Stride | Distance between consecutive vertices. |
| Byte size | Readable extent from the base. |
| Data and number format | Element format. |

There are 16 attribute records. Dual-slot elements are not supported.

## Roots and tables

| Root | Register | Table |
|------|----------|-------|
| Constant | `s4:s5` | Per-stage uniform table: 12 entries of 16 bytes. |
| Image and sampler | `s6:s7` | 0x40-byte slots. |
| Attributes | `s8:s9` | 16 entries of 16 bytes. |
| Read-write buffer | `s0:s1` | 16 entries of 16 bytes; streamout. |

The constant root's entry 0 is the ordinary uniform block; entry `n` is at
offset `(n - 1) * 16`. The image-and-sampler root's slot `n` is at
`n * 0x40`. The attribute root's entry `i` is at `i * 16`.

## Push constants

Push constants occupy 256 bytes at offset `0x4000` in the constant root. The
shader reads them with a scalar flat load.

## Binding table packing

Bound slots may be packed: the logical binding numbers are compacted into
physical slot indices in ascending order of first use. A host MUST use the same
packing for the descriptor table and for the shader's slot arithmetic.

## Absent resources

A descriptor for an absent resource still has to be well formed. The current
path writes the same record shape and points the base at a zeroed range large
enough for any access the shader can make. A zero base is not a valid absent
descriptor; it is an address that faults.

## Alignment

| Object | Requirement |
|--------|-------------|
| Image descriptor tuple | Eight-SGPR aligned, at most 248. |
| Sampler descriptor tuple | Four-SGPR aligned, at most 252. |
| Texture destination | Four-VGPR aligned. |
| Channel mask | Nonzero. |

## Open questions

- Whether the descriptor table may be relocated per draw.
- The exact packing order when bindings are added out of order.
- Whether an absent descriptor may be all zero if the shader never reads it.
