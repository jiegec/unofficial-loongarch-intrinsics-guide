# Vertex Input

The vertex stage reads its inputs through attribute descriptors. An attribute
descriptor is a buffer descriptor that describes one vertex element; the
attribute table is an array of those descriptors. This chapter defines the table,
the fetch and the format rules.

## Attribute table

The vertex attribute table is rooted at `s8:s9`. It holds 16 entries of 16 bytes.
The entry for attribute ordinal `i` starts at byte `i * 16`.

An attribute descriptor has the buffer-descriptor layout:

| Bits | Field | Meaning |
|------|-------|---------|
| `[47:0]` | Base address | GPU virtual address of the element in the vertex buffer. |
| `[61:48]` | Stride | Distance between consecutive vertices, in bytes. |
| `[63:62]` | Cache swizzle, swizzle enable | Not used for vertex fetch. |
| `[95:64]` | Byte size | Readable extent from the base. |
| `[98:96]` | Destination select X | Channel selector. |
| `[101:99]` | Destination select Y | Channel selector. |
| `[104:102]` | Destination select Z | Channel selector. |
| `[107:105]` | Destination select W | Channel selector. |
| `[110:108]` | Number format | UNORM, SNORM, UINT, SINT, float. |
| `[114:111]` | Data format | Element width and component count. |
| `[116:115]` | Element size | Not used for vertex fetch. |
| `[118:117]` | Index stride | Not used for vertex fetch. |
| `[119]` | Add thread id | Not used for vertex fetch. |
| `[120]` | ATC | Not used for vertex fetch. |
| `[121]` | Hash enable | Not used for vertex fetch. |
| `[122]` | Heap | Not used for vertex fetch. |
| `[125:123]` | Memory type | Not used for vertex fetch. |
| `[127:126]` | Type | Not used for vertex fetch. |

The base address already includes the element offset within the vertex buffer.
The stride is the distance between the same element of consecutive vertices.

## Fetch

For vertex index `n` and attribute `i`, the fetch address is:

```text
address = base[i] + n * stride[i]
```

The element is decoded according to the data format and the number format. The
destination selects map the decoded components to the shader's input channels.
The default select is `(X, 0, 0, 1)` when the descriptor does not override it.

The byte-size field bounds the readable range. A fetch beyond it is outside the
descriptor's contract:

> UNRESOLVED: whether an out-of-range vertex fetch returns zero, clamps or
> faults.

## Formats

A vertex element is one of:

- a scalar 8-, 16- or 32-bit integer;
- a two- or four-component vector of 8-, 16- or 32-bit integers;
- a packed format such as `10_10_10_2` or `11_11_10`;
- a floating-point format.

A packed format with no matching element tuple is fetched as a raw 32-bit
element and unpacked in the shader. There is no dual-slot element: one attribute
is fed by one descriptor.

The data-format encoding is shared with the buffer descriptor; the values are in
[Formats](formats.md).

## Limits

| Limit | Value |
|-------|-------|
| Attributes | 16 |
| Vertex buffers | 16 |
| Attribute record size | 16 bytes |
| Stride field | 14 bits, below `0x4000` |
| Base address field | 48 bits |

## Open questions

- Whether an attribute divisor exists and how it is encoded.
- The exact out-of-range behaviour of a fetch.
- Whether the attribute table can be relocated per draw.
