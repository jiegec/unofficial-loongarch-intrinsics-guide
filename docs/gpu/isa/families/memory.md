# Memory and Image Family

The memory and image family moves data between memory and the vector and scalar
register files. This chapter describes the addressing modes, the load and store
forms and the shared-memory instructions. The rows are in the
[memory index](../../instructions/memory-image.md); the address computation is in
[Addressing](../addressing.md).

## Addressing modes

| Mode | Address source | Use |
|------|----------------|-----|
| Vector buffer | A buffer descriptor plus an offset and an optional index. | Uniform and storage buffers, vertex attributes, formatted loads. |
| Flat | An absolute 64-bit address pair. | Generic pointers, images without a descriptor. |
| Image | An image descriptor plus a coordinate window. | Texture fetch and store, image loads and stores. |
| Shared memory | An offset relative to the workgroup allocation. | Workgroup communication, interpolation coefficients. |
| Scalar | An address pair or a descriptor. | Scalar loads. |

There is no scalar image mode.

## Buffer addressing

```text
address = descriptor base + index + constant offset
```

The descriptor is four scalar registers. The index is a vector register holding
a byte offset; the constant offset is an unsigned field. Forms without an index
encode the architectural zero source; forms without a constant offset encode
zero.

The descriptor's byte-size field bounds a load. A store is not range-checked by
that field.

## Flat addressing

The address is an absolute 64-bit value in an aligned vector register pair. The
scalar pair position holds the architectural zero source. A flat access has no
descriptor bound.

## Image addressing

An image access takes an eight-SGPR image descriptor and a coordinate window.
The window is an upper bound; the consumed length depends on the coordinate
tuple. The image descriptor supplies the base, dimensions, format, type, layout
and mip bounds.

## Format conversion

Three kinds of load exist:

- **Typed loads** fetch a fixed element width and sign-extend or zero-extend it.
- **Formatted loads** (`*_fmtdesc_*` and `*_fmtinst_*`) convert through a format
  description attached to the buffer or encoded in the instruction.
- **Packed image loads** (`load_v_tex_pck`, `load_v_tex_mip_pck`) convert the
  resource representation to the destination representation during the fetch.

The format mapping is in [Formats](../../graphics/formats.md).

## Sub-word access

An 8- or 16-bit buffer access reads or writes an element narrower than a
register. The 8-bit loads carry `RK8`, `RJ8x4`, `VD8`, `UO12` and the enable bits
and no index operand; the 16-bit loads and the 8-bit store name an index
operand. An 8-bit load of `0x80` returns `0xffffff80` for the signed form and
`0x00000080` for the unsigned form; a 16-bit load follows the same rule.

## Shared memory

Shared-memory instructions address the workgroup allocation with an offset:

- Loads and stores of 8, 16, 32 and 64 bits with an explicit offset.
- A coefficient load used by fragment interpolation.
- Matrix forms `load_sm_matrix_x1`, `x2` and `x4`, which read one, two or four
  planes into a contiguous register tuple.

For a matrix read, with `s` the per-lane address stride in bytes:

```text
byte = base + 8 * s * w + 4 * (lane % 4) + s * (lane / 4)      w = 0..3
```

Plane 1 is `8 * s` past plane 0. The fill grows with the number of active lanes:

| Form | Registers | Complete fill |
|------|-----------|---------------|
| `x1` | 1 | 4 lanes |
| `x2` | 2 | 16 lanes |
| `x4` | 4 | 32 lanes |

A window that is too narrow for the active lanes fills only some planes. The
rule behind the partial fill is UNRESOLVED.

- Swizzle forms load or store a 64-bit element with a permutation. The offset
  unit, the permutation's action and whether it is an involution are UNRESOLVED.

## Scalar memory

A scalar load reads memory into the scalar register file. The address is a
scalar pair or a descriptor, plus a 20-bit constant offset. A wide load writes a
tuple of scalar registers and is modelled by its leading words.

## Atomics

Atomics perform a read-modify-write on a location in shared memory, a buffer, a
flat address or an image. The operations, the destination contract and the
counter semantics are in [Atomic family](atomic.md).

## Family map

| Form | Addressing |
|------|-----------|
| `load_v_buff_*`, `store_v_buff_*` | Vector buffer. |
| `load_v_flat_*`, `store_v_flat_*` | Flat. |
| `load_s_buff_*`, `load_s_flat_*` | Scalar buffer and flat. |
| `load_v_tex*`, `store_v_tex*` | Image. |
| `load_sm_*`, `store_sm_*` | Shared memory. |
| `atomic_sm_*`, `atomic_v_buff_*`, `atomic_v_flat_*`, `atomic_v_tex_*` | Atomics. |
| `getlod`, `getresinfo` | Image queries. |

## Open questions

- The shared-memory offset unit and lane mapping for the swizzle forms.
- The exact matrix partial-fill rule.
- The address unit and lane mapping of shared-memory addressing.
- The operand form of the 8- and 16-bit buffer accesses beyond the layout.
- Whether the image queries share the texture coordinate path.
