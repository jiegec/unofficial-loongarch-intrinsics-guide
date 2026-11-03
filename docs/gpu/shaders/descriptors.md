# Descriptors

A descriptor is a record in GPU memory that tells the hardware where a resource
lives and how to read it. A shader never receives a resource pointer in a
register: each stage starts from 64-bit root addresses, and a descriptor is
reached by adding a byte offset to a root, or to a pointer stored at a root. This
chapter defines the record layouts, the root chain, the fixed windows, the
packing of bindings into slots, and the encoding of an absent resource.

![Descriptor indirection](../assets/fig-descriptors.svg)

The figure shows the resource chain (root, 16-byte set pointer, 0x40-byte slot,
record base) and the two fixed pages: the graphics constant root and the compute
kernarg page, which is also the compute resource root. The figure's footnote
leaves the attribute-table register pair open; that pair is s8:s9.

## Binding slots

A resource slot is 0x40 bytes. Images and samplers share one slot:

| Offset | Size | Content |
|-------:|-----:|---------|
| `+0x00` | 32 | Image record, eight dwords. |
| `+0x20` | 16 | Zero-filled gap. |
| `+0x30` | 16 | Sampler record, four dwords. |

A buffer record is 16 bytes at `+0x00`. A combined image-sampler binding fills
both halves; a sampled or input-attachment image binding leaves `+0x30` zero,
and a sampler-only binding leaves `+0x00..+0x2f` zero. A storage image fills the
whole slot with a four-descriptor tuple: backing store at `+0x00`, width at
`+0x10`, height at `+0x20`, row pitch at `+0x30`. A storage texel buffer
reserves the same span, with the buffer record first and the block size and texel
count in the next two records. Both reserve four physical slots so that the next
binding cannot alias the metadata, but the tuple itself is one slot. A
multi-plane sampler reserves one slot per plane; every other binding reserves one
slot per array element.

The slot size is a host layout. The device sees a root value and the scalar loads
the shader issues; it does not know that the image and sampler records were
placed 0x30 bytes apart.

## Handles and the root chain

A descriptor handle is a 32-bit value. Bits [26:25] select the set and bits
[24:0] are the byte offset of the record inside that set's window:

1. Load 16 bytes at `resource_root + set * 0x10`. The low 64 bits are the base
   address of the set window; the upper 64 bits are unused.
2. Add the handle's [24:0] byte offset to that base.
3. Load the record: four dwords for a buffer or a sampler, eight for an image.
4. The record's base-address field names the payload. A buffer access adds its
   own element offset to that base; a sampler load uses `offset + 0x30`.

The pointer table has five 16-byte records. The first four name set windows. The
fifth, at `+0x40`, names a driver-private descriptor slot at `+0x80` when one
is present; it is not itself a descriptor. The graphics root places the set
windows from that slot's offset: `+0x80` when no private descriptor is present
and `+0xc0` when one is, each 64-byte aligned and padded. The compute root places
them from `+0x1c0`, each 16-byte aligned. A set window holds that set's physical
slots, each 0x40 bytes. The byte-offset field is 25 bits, so a window is at most
32 MiB and holds at most 524288 slots.

## Buffer descriptor

The buffer record is 16 bytes, four dwords. It is the record format for buffers,
texel buffers, vertex attributes, streamout targets and private driver
descriptors.

| Bits | Field | Use in this ABI |
|------|-------|-----------------|
| `[47:0]` | Base address | 48-bit GPU address of the buffer. |
| `[61:48]` | Stride | Distance between elements; zero unless the access steps. |
| `[62]` | Cache swizzle | 0 |
| `[63]` | Swizzle enable | 0 |
| `[95:64]` | Byte size | Accessible range from the base. |
| `[98:96]` | Destination select X | Channel source. |
| `[101:99]` | Destination select Y | Channel source. |
| `[104:102]` | Destination select Z | Channel source. |
| `[107:105]` | Destination select W | Channel source. |
| `[110:108]` | Number format | Numeric interpretation. |
| `[114:111]` | Data format | Component width and count. |
| `[116:115]` | Element size | 0 |
| `[118:117]` | Index stride | 0 |
| `[119]` | Add thread ID | 0 |
| `[120]` | ATC | 0 |
| `[121]` | Hash enable | 0 |
| `[122]` | Heap | 0 |
| `[125:123]` | Memory type | 0 |
| `[127:126]` | Type | 0 |

A field shown as 0 is written zero by every record this ABI builds; the behaviour
of a nonzero value is UNRESOLVED. The channel select is 0 zero, 1 one, 4 X, 5 Y,
6 Z, 7 W; 2 and 3 are undefined. The number format is 0 UNORM, 1 SNORM, 2
USCALED, 3 SSCALED, 4 UINT, 5 SINT, 7 float; 6 is undefined. The data format is 1
R8, 2 R16, 3 RG8, 4 R32, 5 RG16, 10 RGBA8, 11 RG32, 12 RGBA16, 13 RGB32, 14
RGBA32; 0, 6 through 9 and 15 are undefined.

The base field is 48 bits, the byte size 32 bits and the stride 14 bits; a value
that does not fit its field cannot be encoded. A zero stride means the access does
not step through the buffer: uniform, storage, streamout and private descriptors
write zero, while a vertex attribute carries the vertex stride and a texel buffer
the block size. The buffer packer refuses a byte size of zero, so an all-zero
buffer record exists only as the encoding of an absent binding.

## Image descriptor

The image record is 32 bytes and is loaded as eight dwords in one operation. The
field layout is in [Texture and sampling](../graphics/texture.md#image-descriptor).
Its base-address field is 40 bits rather than 48, matching the 40-bit client
address range; width and height are 14 bits, depth and the layer indices 13, pitch
14, the LOD levels 4 and the tiling index 5. The definition names no field above
bit 223, so bits [255:224] are zero.

A record can name a real image only with a nonzero pitch, a base level not above
the last level, a base array layer not above the last layer, and a last layer at
most 0x1fff.

## Sampler descriptor

The sampler record is 16 bytes at `+0x30` of an image-sampler slot; its field
layout is in
[Texture and sampling](../graphics/texture.md#sampler-descriptor). The clamps X,
Y and Z are three bits each: 0 repeat, 1 mirror repeat, 2 clamp to edge, 3 mirror
clamp to edge, 6 clamp to border. Minimum and maximum LOD are 12-bit unsigned
fixed point with 8 fraction bits, range 0..15.99609375. LOD bias is 14-bit signed
fixed point with 8 fraction bits, clamped by this ABI to -16..15.99609375. The
border colour pointer is 12 bits and the border colour type is 1 for a table
entry. A border clamp needs a resident border table and a pointer no larger than
0xfff, or the record is refused. An absent sampler is left all zero.

## Attribute descriptor

The vertex attribute table is 16 buffer descriptors of 16 bytes, 256 bytes in
total, at `+0x4130` in the constant root. The shader-info Attributes field at
record `+0x40` names the table; a vertex input fetch reads entry *n* with a
16-byte scalar load at `s8:s9 + n * 16`.

| Buffer field | Attribute meaning |
|--------------|-------------------|
| Base address | Binding base plus binding offset plus attribute offset. |
| Stride | The binding's vertex stride, or the format block size when it has no static stride. |
| Byte size | Remaining bytes of the binding from the base. |
| Data and number format | The element format. |
| Destination selects | The element's channel swizzle. |

A slot above 15 is not fetched: the shader reads the default tuple (0, 0, 0, 1).
An element that spans two 16-byte slots is not supported.

## Roots and windows

| Root | Register | Content |
|------|----------|---------|
| Constant | `s4:s5` | Page base: push constants at `+0x4000`, draw parameters at `+0x4100`, attribute table at `+0x4130`. |
| Resource and sampler | `s6:s7` | Descriptor page: four set pointers and one private pointer at `+0x00..+0x4f`, then the set windows. |
| Attribute | `s8:s9` | 16 attribute records of 16 bytes, read at `s8:s9 + slot * 16`. |
| Read-write | `s0:s1` | Read by the streamout stage; its transform-feedback descriptor is at `+0x20`. |

The constant pair hydrates from the shader-info Constant field at `+0x30`, the
resource pair from Resource Sampler Descriptors at `+0x38`; the Const Buffer
field at `+0x28` reaches no launch register. The Attributes field at `+0x40` is
the constant-root base plus the attribute-table offset, so the attribute root
aliases the constant page. The read-write field at `+0x20` is read only by the
streamout program, through `s0:s1`; no vertex, fragment or geometry shader reads
a read-write root.

These offsets are the layout the compiler and driver share, not fields the
hardware decodes; a root is a plain 64-bit address. In this indirect layout the
graphics resource root is only a pointer table and set windows, while a direct
layout may place the image and sampler records at fixed offsets in that page. On
compute the resource root is the kernarg page.

## Push constants

The push-constant window is 256 bytes on both paths, an ABI choice rather than a
hardware field. Each access is one 16-byte scalar load off the root pair:

- Graphics: `s4:s5 + 0x4000`, before the draw-parameter block at `+0x4100`;
  the attribute table follows at `+0x4130`.
- Compute: `s6:s7 + 0x90`, in the kernarg page; compute has no constant-root
  register. The geometry block is at `+0x50`, a 16-byte driver window at
  `+0x190`, an 8-byte completion signal at `+0x1a0`, and slots at `+0x1c0`.

A load must lie inside one 16-byte vector: a 64-bit load may start at offset 0,
4 or 8, and a four-component 32-bit load only at offset 0.

## Binding table packing

Bindings are packed in ascending binding number, not in the order the application
created or bound them. Each binding reserves `array_size * stride` slots, where
`stride` is one except for a storage image or storage texel buffer (four) and a
multi-plane sampler (one per plane). The first physical index is the running
count of slots reserved by the declared bindings below it; the first element's
handle offset is that index times 0x40, and element *e* adds `e * stride * 0x40`.

The descriptor table and the shader's arithmetic MUST agree on the physical slot
of every binding: a shader compiled against a different packing reads another
binding's record. A binding's index is a property of the layout, not of the draw,
so a set with sparse binding numbers still reserves the slots of every declared
binding below it.

## Absent resources

An absent resource still needs a well-formed record, and the record MUST NOT name
an unmapped address. Address 0 is never mapped, so a record whose base is zero
faults on the first unsuppressed access. The encoding therefore has to match the
access:

- If the shader bounds-tests the access, an all-zero buffer record is safe: a
  zero byte size puts every element outside the range before the base is
  dereferenced. An image record cannot be all zero because its pitch must be
  nonzero; a null image keeps a mapped base and is marked by a zero width and
  height.
- If the access can execute regardless, the record MUST name mapped memory. The
  usual filler is a real record over a shared zeroed scratch range, carrying a
  format the access can read; reads return zero and discarded writes land in the
  scratch.

| Resource | Absent record |
|----------|---------------|
| Buffer | A 16-byte buffer record: all zero only when the access is bounds-tested; otherwise a mapped base and a nonzero byte size. |
| Texel buffer | A buffer record with the block stride, a mapped base and a nonzero byte size; the fetch executes. |
| Image | A 32-byte image record with a nonzero pitch and a mapped base. A zero width and height mark a null image when the fetch mask honours it. |
| Storage image | The four-descriptor tuple; all zero only when the buffer access is bounds-tested. |
| Sampler | All zero: the record is inert. |
| Attribute | A buffer record over a zeroed range, either a 16-byte window in the constant root or the shared zero buffer. |

A filler's format is host policy, not a hardware field. R32F over the scratch
range is the common choice, and a sampled image may instead use a 1x1 RGBA8
descriptor over a mapped page. The invariant is that no reachable record names
unmapped memory.

## Alignment

| Object | Requirement |
|--------|-------------|
| Image descriptor tuple | Eight consecutive SGPRs, base a multiple of 8, at most 248. |
| Sampler descriptor tuple | Four consecutive SGPRs, base a multiple of 4, at most 252. |
| Texture destination | Four-VGPR aligned. |
| Channel mask | Nonzero. |
| Descriptor record | 16-byte aligned; a buffer or sampler record is read with a 16-byte load and an image record with a 32-byte load at a 0x40 slot base. |
| Set window | 64-byte aligned and padded on the graphics root; 16-byte aligned on the compute root. |
| Push constant | Loads are 16-byte aligned and must not cross a 16-byte boundary. |
| Attribute record | 16 bytes at `constant_root + 0x4130 + n * 16`. |

The image and sampler tuples are register windows, not memory records: the
alignment is on the SGPR number that holds the first dword. Each base is an 8-bit
register number and the tuple must end at s255, which gives the maxima above.

## Open questions

- UNRESOLVED: the behaviour of the buffer-descriptor bits that this ABI writes
  zero (cache swizzle, swizzle enable, element size, index stride, add thread ID,
  ATC, hash enable, heap, memory type and type).
- UNRESOLVED: whether the read-write field at record `+0x20` is hydrated into
  `s0:s1` for a vertex or fragment launch. Only the streamout program is
  established to read it.
