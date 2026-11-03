# Memory and Image Family

The memory and image family moves data between memory and the register files, and
between the lanes of one workgroup. It has five address shapes: a buffer
descriptor, a flat address, an image descriptor, a shared-memory offset and a
scalar record. This chapter gives the fields that carry them, the extension rules
of the narrow forms, and what the encoding leaves open. The rows are in the
[memory index](../../instructions/memory-image.md); atomics are in the
[atomic family](atomic.md).

A per-lane row carries its execution mask in `PJ2` at `INST_HI[17:16]`, which
selects one of the four per-wave mask registers `p0`..`p3`; it is not an SGPR
number. A lane whose bit is clear performs no access, and whether its destination
keeps its previous value or is cleared is UNRESOLVED. The scalar loads carry no
mask operand: they are uniform across the wave.

## Address shapes

| Shape | Address source | Rows |
|-------|----------------|------|
| Vector buffer | Four-SGPR buffer descriptor, per-lane byte index, 12-bit constant offset | `load_v_buff_*`, `store_v_buff_*` |
| Flat | Absolute 64-bit address in an even-aligned VGPR pair | `load_v_flat_*`, `store_v_flat_*` |
| Image | Eight-SGPR image descriptor, four-SGPR sampler, coordinate window | `load_v_tex*`, `store_v_tex*`, `samp*`, `gather*`, `getlod`, `getresinfo` |
| Shared memory | 32-bit byte offset in one VGPR, inside the window the launch declares | `load_sm_*`, `store_sm_*` |
| Scalar | Two- or four-SGPR record plus a 20-bit constant offset | `load_s_*` |

Only the vector file is indexed per lane, and no row here writes an image into the
scalar file. A group is named by its base register, so the base MUST keep the
tuple inside its file: 102 scalar registers `s0`..`s101`, 256 vector registers per
lane.

## Buffer addressing

```text
address = descriptor base
        + (constant offset, when M.offen1 is set)
        + (per-lane byte index, when M.idxen1 is set)
```

| Field | Bits | Role |
|-------|------|------|
| `RK8` / `RA8` | `INST_LO[7:0]` | Zero source, or the byte offset on the 8-bit store |
| `RJ8x4` / `RK8x4` | `INST_LO[15:8]` | Buffer descriptor; a load names it in `RJ8`, a store in `RK8` |
| `VQ8` | `INST_LO[23:16]` | Per-lane byte index |
| `VD8` / `VJ8` | `INST_LO[31:24]` | Destination on a load, data on a store |
| `UO12` | `INST_HI[11:0]` | Constant byte offset, 0..4095 |
| `M.offen1`, `M.idxen1` | `INST_HI[13:12]` | Enable the constant offset and the per-lane index |
| `M.bl11`, `M.bl21` | `INST_HI[15:14]` | Behaviour controls; `M.bl11` MUST be set, `M.bl21` is set on the stream-out store and its effect is UNRESOLVED |
| `PJ2` | `INST_HI[17:16]` | Execution-mask register |

The descriptor supplies the base at `[47:0]` and the byte size at `[95:64]`; the
byte size bounds the accessible range of a load and of a store, and an access that
leaves the window is UNRESOLVED, as is the destination value after such a read.
[Buffer descriptor](../../shaders/descriptors.md#buffer-descriptor) lists the rest.

The zero source is encoded as the selector `0x66` in a scalar or a vector slot. A
load names its descriptor in `RJ8x4` and the zero source in `RK8`; a store swaps
the two slots. With `M.bl11` clear the word is encodable but does not perform the
access.

The enables do not separate the two contributions in the forms that work. The plain
load and store carry a per-lane index in `VQ8` and a constant in `UO12`, and set
`M.offen1` with `M.bl11` while leaving `M.idxen1` clear; the formatted-descriptor
load carries the same operands and sets `M.idxen1` alone. Which contribution each
enable gates is UNRESOLVED, so the pair is a property of the form, not a choice the
producer makes.

## Sub-word access

| Row | Index field | Result |
|-----|-------------|--------|
| `load_v_buff_u8`, `_i8` | None named | `0x80` becomes `0x00000080`, or `0xffffff80` when signed |
| `load_v_buff_u16`, `_i16` | `VQ8` | `0xbeef` becomes `0x0000beef`, or `0xffffbeef` when signed |
| `store_v_buff_b8` | `RA8` | The low eight bits of the source |
| `store_v_buff_b16` | `VQ8` | The low sixteen bits of the source |

The 8-bit load names no index register at all, so how a lane's byte offset reaches
it is UNRESOLVED; the 8-bit store puts its byte offset in `RA8`, the slot the wider
stores use for the zero source, and that pair's meaning is UNRESOLVED. Buffer
halfwords are little-endian with respect to the two register bytes: the byte at the
effective address is the low byte. A byte access has no alignment requirement; an
odd halfword address is UNRESOLVED. The extension is exact for every value of the
source width, so a producer that needs another numeric domain scales after the
load.

## Flat addressing

The address is an absolute 64-bit value in an even-aligned VGPR pair. The scalar
slot beside it holds the architectural zero source, which contributes no segment
base, so the access has no bound. A flat store names its data in `VJ8`, its address
pair in `RA8x2` and the zero scalar pair in `VK8x2`. On the flat rows the two
modifiers sit at `INST_LO[31:30]`, not at `INST_HI[15:14]` as elsewhere.

UNRESOLVED: the word order of the pair and its lane mapping, alignment and page or
cache-line splitting, whether an unmapped address returns zeros, drops the access
or faults, and the order of two lanes naming one location. The narrow rows
(`load_v_flat_u8`, `_i8`, `_u16`, `_i16`) name the same 32-bit destination as the
32-bit row and move whole words, so their extension is UNRESOLVED; a producer that
needs a widened byte or halfword MUST form it from a 32-bit flat load.

## Image addressing

The image descriptor is eight SGPRs in `RK8x8` at `INST_LO[15:8]`, the sampler four
SGPRs in `RA8x4` at `INST_LO[7:0]`, the coordinate window `VJ8x8` at
`INST_LO[23:16]`, and the result the four-register tuple `VD8x4` at
`INST_LO[31:24]`, whose base MUST be four-register aligned.

The window is an upper bound, not a count. Each variant consumes a prefix fixed by
the image type: `u` for 1D, `u` and `v` for 2D, three components for 3D and cube,
and an appended integer layer for an array. A sampler form adds the four-register
sampler and widens the window to hold bias, offset, compare and gradients, from
three slots up to ten.

`M.dmask4` at `INST_HI[3:0]` selects which result components are written; a zero
mask writes nothing, and the gather forms require a non-zero mask. `M.un1` at
`INST_HI[10]` reads coordinates in texels instead of `[0,1)`. The effects of
`M.na1` at `INST_HI[9]` and `M.rsz1` at `INST_HI[11]` are UNRESOLVED. The level
form borrows `INST_HI[11]` for the ninth bit of its descriptor selector (`RK9x8`),
so it has no `M.rsz1` and no result-size control.

An image store names its descriptor in `RA8x8`, its coordinates in `VK8x8` and its
data in `VJ8x4`, and reads no sampler. `getresinfo` reads the descriptor alone;
which component reports the width, the height, the depth or the level count is
UNRESOLVED. A gather reads the 2x2 block around the coordinate, growing toward
decreasing `v`, and writes four 32-bit components at an implicit level; a lane
outside the image reads zero rather than the edge texel. Its offset forms take one
packed value whose `x` and `y` are signed six-bit fields, so a constant offset lies
in [-32, 31].

The fields these rows consume are the base at `[39:0]`, the width and height at
`[77:64]` and `[91:78]`, the base and last level at `[111:108]` and `[115:112]`, and
the base and last array at `[172:160]` and `[185:173]`
([Image descriptor](../../graphics/texture.md#image-descriptor)).

## Format conversion

A width-only load extends and does not interpret: the buffer `_u8`, `_i8`, `_u16`
and `_i16` rows are the only narrow loads whose extension is established, and a
byte is never read as a floating-point element. A formatted load (`*_fmtdesc_*`)
takes its format from the buffer descriptor — `Data Format` at `[114:111]`,
`Number Format` at `[110:108]`, destination selects at `[107:96]` — and its
`_c1`..`_c4` suffix is the number of components. An instruction-formatted load
(`*_fmtinst_*`) carries no format in the instruction word: the format is the
variant's own opcode. A packed image load (`load_v_tex_pck`, `_mip_pck` and the
`_sgn` forms) converts the stored representation while fetching, with `M.dmask4`
selecting the destination components.

The image descriptor's format fields are wider than the buffer descriptor's: `Data
Format` is six bits at `[57:52]`, `Number Format` four at `[61:58]`. Which
conversion a format pair selects, whether a destination word is a float or an
integer, the mapping of a normalized component, and whether an out-of-range
conversion saturates or wraps are UNRESOLVED. A `_sgn` form keeps the sign of an
integer or signed-normalized component.

## Shared memory

The workgroup's shared memory is a segment the launch declares, not a descriptor. A
row carries a 32-bit byte offset in one VGPR and no base register; because the
offset is in bytes, a dword index is multiplied by four before use. The window is a
launch field
([Memory system](../../architecture/memory-system.md#segment-apertures)).

| Row group | Opcode `INST_HI[26:18]` | Address | Data |
|-----------|-------------------------|---------|------|
| `load_sm_u8`, `_i8`, `_u16`, `_i16`, `_b32`, `_b64` | `0x020`..`0x025` | `VJ8` | `VD8`, one or two registers |
| `store_sm_b8`, `_b16`, `_b32`, `_b64` | `0x028`..`0x02b` | `VK8` | `VJ8`, one or two registers |
| `load_sm_interp` | `0x030` | `RJ8` scalar base, plus `UO8` | `VD8` |
| `load_sm_matrix_x1`, `_x2`, `_x4` | `0x031`..`0x033` | `VJ8` | `VD8`, one to four registers |
| `load_sm_swizzle_b64` | `0x037` | `VJ8` | `VD8` pair |
| `store_sm_swizzle_b64` | `0x03a` | `VK8` | `VJ8` pair |

The load and store rows swap their two register fields: a load takes the address in
`VJ8` and writes `VD8`, a store takes the data in `VJ8` and the address in `VK8`.
The address stays a single register in every form, including the 64-bit ones, so
only the data operand grows.

A 16-bit field `X0_16` sits at `INST_HI[15:0]` on the load and store rows. No
operand names it, no established behaviour depends on it, and its effect is
UNRESOLVED; a producer SHOULD encode zero. The narrow rows extend as the buffer
rows do, so `load_sm_u8` returns `0x00000080` for a stored `0x80` and `load_sm_i8`
returns `0xffffff80`. The byte order inside a halfword is UNRESOLVED, as are an odd
halfword address, an address outside the window, and whether a read of the last
byte differs from one past it. Two lanes that store to one element have no defined
order, so a reader that must see another lane's store needs a workgroup barrier
([Barriers](control.md#barriers)).

`load_sm_matrix_x1` reads one word per lane from the launch-persistent matrix file,
at

```text
word = base + (lane & 3) + s * (lane >> 2)
```

where `s` is the row pitch in words: lane `n` receives element `n` of a four-column
matrix, and the mapping follows the pitch rather than each lane's own address, so a
constant added to every lane shifts the whole result. The destination is the tuple
start and MUST be aligned to the tuple width. `_x2` writes the two registers it
names; how the second plane follows the first is UNRESOLVED. `_x4` writes more
registers than the four it names, and how many, and how the four planes map onto
them, are UNRESOLVED. A plane the launch has not written is UNRESOLVED.

`load_sm_swizzle_b64` and `store_sm_swizzle_b64` move eight bytes per lane through a
permutation, with no offset field. Which side, the address or the data, is
permuted, and the mapping itself, are UNRESOLVED, and the mapping is not established
to be its own inverse; both sides of a hand-off MUST use the same form, so a plain
`load_sm_b64` of a location written through the swizzle store cannot be relied on
to return the stored value.

`load_sm_interp` reads one 32-bit coefficient. Its base is a scalar register, uniform
across the wave, and `UO8` at `INST_HI[7:0]` selects a slot whose unit is
UNRESOLVED; an offset past the workspace is also UNRESOLVED
([interpolation](interpolation.md)).

## Scalar memory

The scalar forms read memory into the scalar file; the family has no scalar store.
Two record shapes exist: a four-SGPR buffer descriptor, or a two-SGPR absolute
address pair. A 20-bit unsigned offset is added to the record.

| Field | Bits | Role |
|-------|------|------|
| `RJ8x4` / `RJ8x2` | `INST_LO[7:0]` | Buffer descriptor, four SGPRs, or the absolute address pair, two |
| `UO20` | `INST_LO[27:8]` | Constant offset, 0..0xfffff |
| `X0_1` | `INST_LO[30]` | Single parameter bit; effect UNRESOLVED |
| `GK8` | `INST_HI[7:0]` | Architectural zero source |
| `RD8` | `INST_HI[15:8]` | Destination tuple, one to sixteen scalar registers |

The result lands in consecutive scalar registers, lowest first, and the `_w1` to
`_w16` suffix is the number of 32-bit words. Whether `UO20` counts bytes or elements
is UNRESOLVED, and so is whether the buffer form consults the record's format
fields. A destination that overlaps the address record makes the result
order-dependent: whether the read completes before the write-back is UNRESOLVED. An
access outside the record's byte size is UNRESOLVED, and so is the base a
sixteen-register tuple needs, because alignment is stated for four- and
eight-register tuples only.

## Atomics

An atomic reads a location, computes and writes back, using the address shapes of
this chapter: a shared-memory atomic takes the same 32-bit byte offset in the same
window, a buffer atomic the four-SGPR descriptor with its per-lane offset source and
constant, a flat atomic the absolute pair with the zero scalar pair, and an image
atomic the image descriptor with a coordinate window. The operation list, the
two-destination contract, the counter forms and the ordering rules are in the
[atomic family](atomic.md).

Two facts cross the boundary. A shared-memory atomic addresses the same segment as
`load_sm_*`, so a workgroup that mixes them MUST order them with a barrier. And the
shared-memory atomic rows name `VK8` at `INST_LO[7:0]` and `VJ8` at
`INST_LO[23:16]`, where a load names `VJ8` alone in byte 2; which of the two carries
the byte offset there is UNRESOLVED, so a producer cannot transpose the load's
operand order into them.

## Open questions

- Masking: whether a gated-off lane's destination keeps its previous value or is
  cleared.
- Buffer: which contribution the two enables gate; the effect of `M.bl21`; the index
  of the 8-bit load; `RA8` on the 8-bit store; an odd halfword address; an access
  outside the byte size.
- Flat: the word order and lane mapping of the address pair; alignment, splitting and
  unmapped behaviour; the extension of the narrow rows.
- Image: the effects of `M.na1` and `M.rsz1`; which component `getresinfo` reports
  each field in; the level a sampler-less fetch reads.
- Shared memory: `X0_16`; the byte order inside a halfword; an address outside the
  window; how `_x2` and `_x4` place their extra planes; how many registers `_x4`
  writes; the swizzle mapping and whether it is its own inverse; the unit of the
  interpolation slot.
- Scalar: whether `UO20` counts bytes or elements; `X0_1`; whether the buffer form
  reads the record's format; the base a sixteen-register tuple needs.
- Atomics: which field carries the byte offset on the shared-memory rows.
