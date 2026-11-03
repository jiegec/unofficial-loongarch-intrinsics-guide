# Addressing

An LG200 memory instruction forms its address in one of five spaces. Each space
takes its base from a different place, adds a different offset, and is bounded
differently. This chapter gives the arithmetic, the base registers, the alignment
the encoding imposes and the behaviour outside a bound. The rows are in
[Memory and image family](families/memory.md), the operand classes in
[Operands](operands.md), and the per-format slot positions in
[Encoding](encoding.md).

| Space | Base | Offset | Bound |
|-------|------|--------|-------|
| Buffer | Four-SGPR descriptor | Per-lane index and/or 12-bit constant | Descriptor byte size |
| Flat | 64-bit address in a VGPR pair | None | Page mapping |
| Image | Eight-SGPR descriptor | Coordinate tuple in a VGPR window | Descriptor dimensions |
| Shared | None in the instruction | Per-lane byte offset in a VGPR | Workgroup allocation |
| Scalar | SGPR pair (flat) or four-SGPR descriptor (buffer) | 20-bit constant | Descriptor byte size |

A field name does not fix a bit position. The four-register buffer descriptor is
`RJ8x4` at `INST_LO[15:8]` in a load row and `RK8x4` at the same bits in a store
row, while `RJ8` sits at `INST_LO[7:0]` in the scalar format. Take the range from
the row you encode and do not infer a register class from a name.

A slot for which a space has no operand is still encoded, and carries the
architectural zero selector `0x66`, printed as `zero`. The slot is not spare:
zero would name the first scalar register.

## Buffer

```text
address = descriptor base
        + per-lane index          when M.idxen1 is set
        + 12-bit constant offset  when M.offen1 is set
```

| Field | Bits | Role |
|-------|------|------|
| Descriptor | `INST_LO[15:8]` | First of four consecutive SGPRs. |
| Index | `INST_LO[23:16]` | VGPR holding a per-lane byte offset: `VQ8`, or `RA8` in the 8-bit store. |
| Constant | `INST_HI[11:0]` | Unsigned offset, `UO12`. |
| Payload | `INST_LO[31:24]` | Load result `VD8`, stored data `VJ8`, atomic operand `VA8`. |
| `M.idxen1`, `M.offen1` | `INST_HI[13:12]` | Add the index, apply the constant. |
| `M.bl11` | `INST_HI[14]` | Buffer behaviour bit, not an address bit. |

The index is a byte offset and the base is a byte address, so no scale applies to
either. The constant is an immediate; the format has no register form for it. The
descriptor's base address is bits 47:0, its stride bits 61:48, and its byte size
bits 95:64, which is the backing range in bytes and is independent of the stride.
The base field is wider than the 40-bit client address space, so its top eight
bits cannot be matched by any mapping.

Loads and stores put the zero selector in `INST_LO[7:0]` (`RK8` or `RA8`). A
buffer atomic swaps the slots: its dynamic offset is a **scalar** register there,
while `VQ8` carries the zero source. A scalar register is wave-uniform by
construction, so a buffer atomic adds a uniform offset and cannot add a per-lane
index.

The 8-bit load is the only buffer form with no index register. The 8-bit store
names its byte offset in `RA8` instead of `VQ8`, and the meaning of that field
together with `UO12` is UNRESOLVED.

## Flat

| Field | Bits | Role |
|-------|------|------|
| Address | `INST_HI[7:0]` | 64-bit value in a VGPR pair: `VJ8x2` in a load or atomic, `VK8x2` in a store. |
| Zero | `INST_LO[15:8]` | Companion slot, which must select the zero source (`RK8x2`, `RA8x2`). |
| Result | `INST_HI[15:8]` | Load or atomic result, `VD8`. |
| Data in | `INST_LO[7:0]` | Store data `VJ8`, atomic operand `VA8`. |

The address is absolute and no descriptor accompanies it, so nothing in the
instruction bounds the access; only the page tables do. The pair index must be
even, and its companion slot must hold the zero selector rather than a register
pair that happens to contain zero.

## Image

| Field | Bits | Role |
|-------|------|------|
| Image descriptor | `INST_LO[15:8]` + `INST_HI[11]` | Eight consecutive SGPRs, `RK9`. |
| Sampler | `INST_LO[7:0]` | Four consecutive SGPRs, `RA8`. |
| Coordinates | `INST_LO[23:16]` | Consecutive VGPR window, `VJ8`, width fixed by the row. |

The image descriptor's base address is bits 39:0: 40 bits, the full width of the
client address space, and it has no size field. The coordinate window is the only
bound the instruction carries, and it is an upper bound: the encoding stores the
window's base register and the instruction reads a prefix of it in a fixed order,
so a tuple that begins with a compare value and one that begins with a packed
offset share the encoded field at different widths. A texture gather carries its
offsets as signed six-bit X and Y fields with the inclusive range -32 to 31.

## Shared memory

| Field | Bits | Role |
|-------|------|------|
| Address | `INST_LO[23:16]` | Per-lane byte offset: `VJ8` in a load or atomic, `VK8` in a store. |
| Immediate | `INST_HI[15:0]` | Alternate offset `X0_16`. |

A shared-memory instruction carries the whole address. It has no base register,
and the launch supplies the segment. The value is an ordinary byte offset in a
32-bit VGPR, and a dword index is scaled by the producer, not by the instruction.

UNRESOLVED: the aperture's base, how the instruction selects it, and the unit of
the `X0_16` immediate. A producer must use the register form until the immediate
form is established.

## Scalar memory

| Field | Bits | Role |
|-------|------|------|
| Address | `INST_LO[7:0]` | Even SGPR pair `RJ8x2` (flat rows) or four-SGPR descriptor `RJ8x4` (buffer rows). |
| Offset | `INST_LO[27:8]` | Unsigned 20-bit constant, `UO20`. |
| Zero | `INST_HI[7:0]` | The zero source `0x66`, `GK8`. |
| Result | `INST_HI[15:8]` | Destination group `RD8`, one to sixteen SGPRs. |

A scalar load addresses with a wave-uniform base: both the address and the offset
are scalar, so one address serves the whole wave. The format has no index
register, so the offset must be a constant known at encode time. Only loads
exist; the instruction set has no scalar store.

## Alignment

| Group | Rule |
|-------|------|
| Buffer descriptor | Index a multiple of four, at most 252. |
| Flat address pair | Even index. |
| Image descriptor | Index a multiple of eight, at most 248. |
| Sampler descriptor | Index a multiple of four, at most 252. |
| Vector result | Width 2: even index. Width 4: multiple of four. Others: consecutive only. |
| Scalar result | Index a multiple of the group width. |
| Constant offset | Unsigned: 12 bits in the buffer space, 20 bits in the scalar space. |

Every rule here constrains the encoded index and is checked when the instruction
is built. Whether the memory unit ignores the low bits of a misaligned group, or
faults on it, is UNRESOLVED; satisfy the table and rely on neither behaviour.

## Out of bounds and faults

A buffer descriptor's byte size names the range the producer promises. It is the
only bound the buffer space expresses, and the sources do not establish that the
memory unit compares an access against it: treat the field as a promise that
every address the shader can form lies inside the range, not as a check the unit
performs. A flat address that no mapping covers faults, and so does the first
access through a descriptor whose base lies outside the client address space.

The client address space is 1 TiB, and the bottom 1 MiB is not available to a
client. That floor is host policy rather than a hardware rule: it is
configurable, and it exists so that a fault report naming a small address can be
told apart from one that carries no address at all.

## Open questions

- Whether an out-of-range buffer or image access returns zero, clamps or faults.
- Whether a store is compared against the descriptor's byte size.
- Whether a misaligned multi-register group faults or has its low index bits
  ignored.
- The shared-memory aperture base, and the unit of its immediate field.
- Whether a formatted buffer load applies its constant offset: that form carries
  `UO12` without setting `M.offen1`.
- The address semantics of the 8-bit store's `RA8` and `UO12` pair.
