# Draw

A draw is described by Draw Info and the Pipe Enables word. Draw Info selects the
primitive topology, the index buffer and the vertex range; Pipe Enables selects
the rasterization behaviour. This chapter defines Draw Info and the primitive
model. Rasterization is in [Rasterization](rasterization.md).

## Draw Info

Draw Info is 36 bytes at state offset `0x000`.

| Bits | Field | Meaning |
|------|-------|---------|
| `[2:0]` | Index size | 0 no index buffer, 2 16-bit indices, 4 32-bit indices. |
| `[11:8]` | Primitive type | See the primitive table. |
| `[16]` | Primitive restart | Enable primitive restart. |
| `[17]` | Reset primitive id | Reset the primitive id at the start of the draw. |
| `[18]` | Primitive start offset | Start mode. |
| `[63:32]` | Count | Number of elements or vertices. |
| `[95:64]` | Instance count | Number of instances. |
| `[127:96]` | Vertices per patch | Written as 3 for the ordinary vertex role. |
| `[159:128]` | Vertex offset | First vertex or index bias. |
| `[191:160]` | Restart index | Index value that restarts the primitive. |
| `[223:192]` | Index buffer low | GPU virtual address, low 32 bits. |
| `[255:224]` | Index buffer high | GPU virtual address, high 32 bits. |
| `[287:256]` | Primitive id input | Primitive id for the first primitive, or zero. |

The block is 36 bytes; the last field ends at bit 287.

## Primitive types

| Value | Primitive | Value | Primitive |
|------:|-----------|------:|-----------|
| 0 | Point list | 1 | Line list |
| 2 | Line loop | 3 | Line strip |
| 4 | Triangle list | 5 | Triangle strip |
| 6 | Triangle fan | 7 | Quad list |
| 8 | Quad strip | 9 | Polygon |
| 10 | Line list with adjacency | 11 | Line strip with adjacency |
| 12 | Triangle list with adjacency | 13 | Triangle strip with adjacency |

A primitive outside this table is rejected before state is built.

## Indexing

With index size 0, vertices are fetched sequentially from the vertex offset.
With index size 2 or 4, the index buffer supplies one index per element; the
index is added to the vertex offset, and the resulting value selects the vertex.

Primitive restart is enabled by bit 16. When the fetched index equals the
restart index, the current primitive is ended and the next index starts a new
one. A host MAY instead split the draw on the CPU; the result is equivalent for
the topologies that the hardware accepts.

## Instancing

The instance count is the number of times the vertex stream is executed. The
instance identifier is supplied to the vertex stage in `v1`. The vertex offset is
applied before the vertex stage reads `v0`.

> UNRESOLVED: whether the hardware supports a per-attribute instance divisor,
> and how a divisor interacts with the vertex offset.

## Primitive id

The primitive id is supplied to the geometry and fragment stages. Bit 17 resets
the counter at the start of the draw; the input field seeds the first primitive.
A draw that does not need a primitive id writes zero.

## Vertices per patch

The field belongs to the tessellation path, which LG200 does not implement. The
ordinary vertex role writes 3. A host MUST NOT use the field to describe a
patch.

## Open questions

- The exact interaction between primitive restart and each strip topology.
- The behaviour of adjacency primitives in the rasterizer.
- Whether the vertex offset is applied before or after index fetch for every
  index size.
