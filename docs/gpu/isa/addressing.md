# Addressing

The memory and image instructions use five operand shapes. This chapter defines
the address computation and the fields that carry it. The instruction families
are in [Families](families/vector.md); the operand classes are in
[Operands](operands.md).

## Vector buffer

```text
address = descriptor_base
        + (index_register when the index form is enabled)
        + (constant_offset when the offset form is enabled)
```

| Field | Role |
|-------|------|
| `RK8` | Descriptor base, or the architectural zero source. |
| `RJ8x4` | Four-register buffer descriptor. |
| `VQ8` | Vector register holding the byte offset, or the zero source. |
| `VD8` | Destination tuple. |
| `UO12` | Constant offset. |
| `M.offen1`, `M.idxen1` | Enable the offset and index contributions. |
| `M.bl11` | Set for the buffer forms. |

The descriptor is four scalar registers, so its base is aligned to four. The
descriptor's byte-size field bounds a load; a store is not range-checked by that
field. A host MUST keep stores inside the backing allocation.

## Flat

```text
address = 64-bit value in an aligned vector register pair
```

| Field | Role |
|-------|------|
| `VJ8x2`, `VK8x2` | The absolute 64-bit address pair. |
| `RK8x2` | A zero scalar pair. |
| `VA8` | Compare or read-modify-write data. |
| `VD8` | Destination. |
| `M.bl11`, `M.bl21` | Memory behaviour controls. |

Flat addressing is a raw 64-bit address, not a descriptor-relative one. It is
not bounded by a descriptor.

## Image

An image access takes an eight-SGPR image descriptor and a window of vector
registers holding the coordinate tuple:

| Field | Role |
|-------|------|
| `VJ8x8` | Coordinate window. |
| `RK8x8`, `RK9x8` | Image descriptor. |
| `RA8x4` | Sampler descriptor. |
| `VD8x4` | Four-register result. |
| `X0_4` | Channel mask. |
| `M.un1`, `M.na1`, `M.rsz1` | Coordinate and resource modifiers. |

The window is an upper bound. Each variant consumes a prefix, and the consumed
length depends on the coordinate tuple of that variant.

## Shared memory

| Field | Role |
|-------|------|
| `VK8`, `VJ8`, `VA8`, `VD8` | Vector registers. |
| `X0_16` | Byte or element offset. |

A shared-memory address is a byte offset within the LDS aperture. The register
form carries 32 bits; the immediate form carries 16 bits.

## Scalar memory

| Field | Role |
|-------|------|
| `RJ8x2`, `RJ8x4` | Address pair or descriptor. |
| `UO20` | Constant offset. |
| `GK8` | Architectural zero source. |
| `RD8` | Result tuple. |

A scalar load reads memory into the scalar register file. A wide load is
modelled by its leading words.

## Alignment

- A buffer descriptor is four-SGPR aligned.
- A flat address pair is two-VGPR aligned.
- An image descriptor is eight-SGPR aligned; a sampler descriptor is four-SGPR
  aligned.
- A destination tuple is aligned to its width.

## Out-of-bounds behaviour

A load past a descriptor's byte size is outside the descriptor's contract. A
flat access to an unmapped page faults. The exact result of an out-of-range
buffer or image access is UNRESOLVED:

> UNRESOLVED: whether an out-of-range buffer or image access returns zero,
> clamps or faults.
