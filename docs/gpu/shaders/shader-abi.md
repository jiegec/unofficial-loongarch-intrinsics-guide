# Shader ABI

A graphics launch reads a 96-byte Shader Info record for each stage it runs. The
record names the code image, the register footprint and the descriptor roots the
launch loads. This chapter defines the record and its slots, the fields that
become launch registers, the size limits a compiler must respect, the upload and
mapping of the code image, and the only patching a record's addresses receive.
Compute has no such record; see [Compute](compute.md).

## Shader-info record

The record is 24 dwords, packed little-endian from a zeroed buffer, so a bit no
field names reads as zero rather than as the previous draw's value.

| Bits | Field | Byte | Meaning |
|------|-------|-----:|---------|
| `[7:0]` | SGPR size | `+0x00` | Scalar registers the stage needs: floor 16, ceiling 102. |
| `[15:8]` | VGPR size | `+0x01` | Vector registers per lane, floored per stage, rounded up to a multiple of four; 256 is encoded as 255. |
| `[31:16]` | LDS size | `+0x02` | Zero in every record this ABI builds. |
| `[39:32]` | Shader type | `+0x04` | 3 vertex, 4 geometry, 5 fragment, 6 streamout. |
| `[47:40]` | VGPR initial count | `+0x05` | Per-stage constant: 4 vertex, 2 geometry, 15 fragment, 2 streamout. The meaning is UNRESOLVED. |
| `[63:48]` | Code size | `+0x06` | Image length in bytes: a multiple of eight, at most 65528. |
| `[191:128]` | Kernel entry | `+0x10` | Address of the first instruction. There is no shader header. |
| `[255:192]` | Kernel config | `+0x18` | Zero in every record. |
| `[319:256]` | Root 1 (RW Buffer) | `+0x20` | The argument block. Only the streamout record hydrates it, into `s0:s1`. |
| `[383:320]` | Root 2 (Const Buffer) | `+0x28` | Root of the fragment uniform-block records; it fills no launch register. |
| `[447:384]` | Root 3 (Constant) | `+0x30` | Hydrated verbatim into `s4:s5` at vertex and fragment launch. |
| `[511:448]` | Root 4 (Resource Sampler Descriptors) | `+0x38` | Hydrated verbatim into `s6:s7`; base of the image and sampler table. |
| `[575:512]` | Root 5 (Attributes) | `+0x40` | Base of the table the vertex stage reads its attribute records through. |
| `[607:576]` | Argument 0 | `+0x48` | Zero in every record. |
| `[639:608]` | Argument 1 | `+0x4c` | Zero in every record. |
| `[671:640]` | Argument 2 | `+0x50` | Streamout: the record capacity handed to the launch. Zero otherwise. |
| `[703:672]` | Argument 3 | `+0x54` | Zero in every record. |
| `[767:736]` | Tail | `+0x5c` | Streamout: `0x20`, the slot's tail dword. Zero otherwise. |

Two ranges carry no field: bits 64–127, alignment padding that puts the first
address on a 16-byte boundary, and bits 704–735 between the last argument and the
tail.

The kernel config, arguments 0, 1 and 3 and the LDS size are zero in every
graphics record this ABI builds; the tail is zero except on the streamout record.
A record whose kernel entry, constant root, constant-buffer root or resource root
lies below the client allocator's base MUST be refused: the allocator starts at
`0x1_0000_0000`, nothing is mapped below it, and the hardware would fetch or load
from an address no mapping covers.

## Stage slots

Six records sit in the 512-dword state block at a byte stride of `0x60`:

| Slot | Byte offset | Record |
|-----:|------------:|--------|
| 0 | `0x540` | Copy of the vertex record. |
| 1 | `0x5a0` | Hull. No stage builds one, so no live record sits here. |
| 2 | `0x600` | Vertex. |
| 3 | `0x660` | Geometry. |
| 4 | `0x6c0` | Streamout. |
| 5 | `0x720` | Fragment. |

Every graphics draw sets the shader config's DS-as-VS bit, so the vertex stage runs
in slot 2 and its record carries shader type 3 there. The shader type names the
stage, not the slot. Slot 0 is not a second launch: with the vertex stage in slot
2, the slot is read by the instruction prefetcher, and a launch from it is possible
but not relied on. An all-zero record there names address 0, which no client
mapping covers, so slot 0 MUST NOT be left zero; it holds a copy of the vertex
record whenever the block is established.

The block persists across draws, so every draw rewrites the slots it can launch
from: the stage it runs gets its own record and a stage it does not run gets an
all-zero disabled record. A slot whose code size is zero is not part of the draw
and the address audit skips it. Slot 0 is the exception: later draws name only the
dwords they change, so the copy is written once per block and its roots may keep
addresses from an older draw. That is harmless: only a launch reads a root, and the
slot is a prefetch target whose code address, not whose roots, is what has to stay
live.

The block and the record kinds that carry it are defined in [State
object](../graphics/state-object.md).

## Roots

The constant and resource roots are copied verbatim into `s4:s5` and `s6:s7` at
vertex and fragment launch. The launch does not dereference the constant root, so
the record only has to name a region the shader's own loads can use. A compiler
that addresses its descriptor tables from `s4:s5` chooses that convention; the
hardware does not require it. On the geometry record, bit 62 of the resource field
is a stage flag rather than address, and the address is the low 62 bits.

The constant-buffer root fills no launch register, so it cannot affect a draw. How
bindings order inside the table it names is UNRESOLVED.

The read-write root is hydrated into `s0:s1` by the streamout record, and only
there. That program reads the transform-feedback descriptor at `root + 0x20` and
copies the vertex stage's payload records through it.

The attributes root names the table the vertex stage reads its attribute records
through. The compiler keeps `s8`–`s11` clear as its window whenever a vertex stage
reads a vertex input or a descriptor. The field is an address surface, not a
spare: a fetch through a zero value faults, so a record MUST name a mapped table
even for a draw that reads no attribute.

## Size rules

Both register counts are the allocation a stage asks for, not the width of the
file and not a proven hardware limit.

| Field | Rule |
|-------|------|
| SGPR size | At least 16, at most 102. The twelve low registers the ABI reserves are not part of the compiler's count. |
| VGPR size | Floored per stage: 8 vertex, 64 geometry, 16 fragment, 2 streamout. Rounded up to a multiple of four, then 256 encodes as 255; more than 256 is refused. |
| Code size | The image length in bytes: a multiple of eight, at most 65528. |
| Kernel entry | The first instruction's address; it MUST be a mapped client address. |
| LDS size | Zero on every graphics record. |
| VGPR initial count | 4 vertex, 2 geometry plus one when the geometry stage reads the primitive id or captures streamout, 15 fragment, 2 streamout. |

A fragment stage that reads both the fragment coordinate and the facing flag
raises its vector floor to 20: the facing stream lands at `v16`, one register past
the sixteen-register input window, and the floor rounds to the same four-register
granule as the rest of the record. A geometry stage that writes two varying
outputs floors its vector size at 120, and three at 168; both floors apply after
the rounding, so they bound the value that is written, not the compiler's count.

An instruction is eight bytes, two dwords, so the 16-bit code-size field caps a
binary at 8191 instructions, 65528 bytes. The compile step refuses a larger binary
and the ABI stage check refuses a code size above 65535, so a record never needs
the field's upper range.

## Entry and exit

The kernel entry names the first instruction and the code size counts the
instructions, so the image ends exactly at the last one: no header, and no trailer
beyond the zero tail page. Every block ends in a terminator and the last block
must, so an image ends in a terminator.

At entry, `p0` holds the launch's lane mask, one bit per active lane, and the first
instruction copies it into a scalar register. `p2` and `p3` are undefined at
entry, so a stage MUST NOT read them before writing them. A stage that installs a
whole-quad mask first saves `p0` and derives the wider mask from the saved value.

## Upload

The image is copied into a buffer and mapped once:

1. The buffer is the image rounded up to the device's allocation alignment, plus
   one whole alignment page.
2. Every byte after the image is zero, including the tail page.
3. The CPU mapping is removed before the buffer receives a GPU address.
4. The buffer is mapped readable and not writable; that mapping's address is the
   record's kernel entry.
5. The code size equals the image length. An upload handed a binary whose declared
   size differs from its image length is refused.

The tail page exists because the front-end streams instructions ahead of the
program counter and does not stop at the last code byte. The overshoot is finite
and shorter than one allocation page, and the tail is never executed because every
stage ends in its own terminator. A mapping that ended at the image would make the
fetch read the first page past it and raise a protection fault.

### No execute permission

The page table has no execute bit. An entry has a present bit, a huge-page bit, an
exception bit, a writeable bit and a four-bit field at `[7:4]`. Only two of those
come from the mapping's access flags: readable becomes present, writable becomes
writeable. The mapping operation accepts an executable flag and stores nothing for
it, so a code image and any other readable buffer are indistinguishable in the
tables.

A driver MUST map a code image readable. It MAY pass the executable flag, for an
interface that expects it, but MUST NOT rely on it: the flag neither grants nor
restricts a fetch. Because the mapping is read-only, the image cannot be modified
through the address the record names.

## Patching

No code byte is patched after upload. The only code address the record carries is
the kernel entry, the mapping's base address, fixed for the life of the upload. A
re-upload produces a new address, and that address is part of the key under which
a block is cached, so a cached block is never reused across a re-upload.

The roots are data inside the block and are relocated when a cached block is
reused for another draw: each root that points into the argument region moves by
the difference between the two region bases. Only the low dword is written; a
reuse whose high dword would change is abandoned and the block rebuilt. The copy
in slot 0 is outside that relocation, for the reason given under
[Stage slots](#stage-slots).

## Compute

A compute shader has no Shader Info record, no constant root and no slot. Its
launch state is a 96-byte kernel descriptor the dispatch packet points at: the
entry is at `+0x20`, the group (LDS) size at `+0x1c` and the register counts at
`+0x30`. The descriptor is data, not code, but it takes the same allocation path
as a code image, including the zero tail. The kernarg pointer arrives in `s6:s7`
and the group ids in `s11`–`s13`; see [Registers](registers.md).

## Open questions

- The VGPR initial count. The streamout stage receives `v0`/`v1` from the launch
  and the record says 2; the fragment input window is sixteen registers and the
  record says 15; the vertex stage reads `v0` and `v1` and the record says 4. Three
  questions stay UNRESOLVED: whether the field counts input registers, whether the
  count includes the last one, and what a read above it returns.
- The attributes root. The compiler reserves four scalar registers as its window,
  but which pair receives the address, and whether both pairs do, is UNRESOLVED. A
  compiler SHOULD treat `s8`–`s11` as a window it may not allocate rather than
  infer a layout.
- Whether any graphics path honours the LDS-size field. Every graphics record
  writes zero there, so a graphics stage cannot ask for shared memory through the
  record.
- The order of bindings inside the fragment uniform-block table the constant-buffer
  root names.
