# Walkthrough: One Draw

This chapter follows one indexed triangle from the client interface to the
fragment stage and back to a fence. It fixes the byte counts and offsets a
driver has to produce: a 36-byte Draw Info at offset 0 of a 512-dword state
block, a 516-dword record, a 528-dword indirect buffer, and the completion rule
that says when the buffers may be touched again.

## Create the context

A client opens the render node and issues `DRM_IOCTL_LOONGGPU_CTX`. The ioctl
returns a 32-bit context id. Every later submission and every wait names that id
together with the engine tuple `(ip_type, ip_instance, ring)`; for a draw the
tuple is `(0, 0, 0)`, because `LOONGGPU_HW_IP_GFX` is 0 and this generation
has one instance per engine type. The context also owns the reset state and the
priority the submission runs at.

See [User-space interface](../host/uapi.md).

## Allocate and map the buffers

| Buffer | Content | Requirement |
|--------|---------|-------------|
| Command buffer | The state record and its tail | Mapped and resident; the start needs 4-byte alignment |
| Index buffer | Three 32-bit indices | Mapped; its address goes into Draw Info |
| Vertex buffer | Attribute data | Mapped; fetched through the attribute table |
| Colour buffer | The render target | Mapped; described by a 32-byte colour buffer record |
| Fence object | One sequence dword | Exactly one host page; the offset leaves room for 8 bytes |

Each object is created with `GEM_CREATE` and receives an address from
`GEM_VA`. The interface defines readable, writeable and executable mapping
flags, but the page table carries only a present bit and a writeable bit, so
`LOONGGPU_VM_PAGE_EXECUTABLE` selects nothing. Instruction fetch does not
consult it, and neither does submission admission (see below).

See [Buffer objects](../host/buffer-objects.md).

## Fill the 512-dword state block

The state block is a flat array of 512 dwords (2048 bytes). Offsets are byte
offsets. A field is live only where the draw uses it; the rest of the block
still has to be present, because a full record writes all 512 dwords.

| Offset | Bytes | Dwords | Content |
|--------|-------|--------|---------|
| `0x000` | 36 | 9 | Draw Info |
| `0x030` | 12 | 3 | Pipe enables |
| `0x048` | 40 per slot, 44-byte stride | 10 | Viewports, 16 slots |
| `0x308` | 16 | 4 | Guardband |
| `0x318` | 12 | 3 | Line raster |
| `0x324` | 16 | 4 | Depth control |
| `0x334` | 4 | 1 | Stencil, front faces |
| `0x338` | 4 | 1 | Stencil, back faces |
| `0x33c` | 4 | 1 | Blend control |
| `0x340` | 4 per target | 1 | Per-target blend words, 8 targets |
| `0x360` | 16 | 4 | Blend constants |
| `0x370` | 32 per target | 8 | Colour buffer records, 8 targets |
| `0x470` | 32 | 8 | Depth buffer record |
| `0x4c0` | 8 | 2 | Shader config |
| `0x4d8` | 8 | 2 | Domain stage config |
| `0x4e0` | 8 | 2 | Geometry stage config |
| `0x4f0` | 2 per entry | 16 | Fragment attribute map, 32 entries |
| `0x530` | 8 | 2 | Fragment input config |
| `0x538` | 4 | 1 | Fragment output config |
| `0x540` | 96 per slot | 24 | Shader Info slots, 6 slots |

The last slot ends at `0x780`; the remaining 128 bytes of the block hold no
named field and a full record writes them as zero. The same is true of the gaps
between named regions, for example `0x490` to `0x4bf`, and of the last four
bytes of every 44-byte viewport slot.

Draw Info occupies nine dwords at offset 0:

| Bits | Field | Use in the walkthrough |
|------|-------|------------------------|
| `[2:0]` | Index size | 0 none, 2 = 16-bit, 4 = 32-bit |
| `[11:8]` | Primitive type | 4 = triangle list |
| `[16]` | Primitive restart | Set only with a strip or fan cut index |
| `[17]` | Reset primitive id | 0 |
| `[18]` | Primitive start offset | 0 |
| `[63:32]` | Count | 3 |
| `[95:64]` | Instance count | 1 |
| `[127:96]` | Vertices per patch | 0 outside tessellation |
| `[159:128]` | Vertex offset | 0 |
| `[191:160]` | Restart index | Ignored without primitive restart |
| `[223:192]` | Index buffer, low | Low dword of the index address |
| `[255:224]` | Index buffer, high | High dword of the index address |
| `[287:256]` | Primitive id input | First primitive id |

A colour buffer record needs three fields for a minimal draw: the base address
at `[63:0]`, the hardware format at `[71:64]` (0 = RGBA8, 3 = RGBA32F), and
the surface type at `[75:72]` (0 = 2D). Width, height, pitch and tiling follow
the format chapter.

The vertex program does not run from its own slot. Shader config sets
DS enable and DS as VS, clears VS enable, and leaves HS and GS clear; the
fragment stage has no enable bit at all. Slot 2 at `0x600` therefore holds the
vertex record and slot 5 at `0x720` holds the fragment record. Slot 0 at
`0x540` receives a copy of the vertex record, because an instruction prefetch
through the VS slot reads the kernel entry from there and a zero entry names
address 0. Unused slots (HS, GS, SS) are written as zero records.

Each Shader Info slot is 96 bytes: SGPR size at `[7:0]`, VGPR size at
`[15:8]`, LDS size at `[31:16]`, shader type at `[39:32]` (3 vertex,
4 geometry, 5 fragment, 6 streamout), VGPR initial count at `[47:40]`, code
size in dwords at `[63:48]`, kernel entry at `[191:128]`, kernel config at
`[255:192]`, and five 64-bit descriptor roots at bytes 32, 40, 48, 56 and 64.

See [State object](../graphics/state-object.md) and
[Draw](../graphics/draw.md).

## Wrap the block in a record

A record is the command sequence `0xf04` + write windows + `0x03`. The first
record of an indirect buffer writes the whole block as one window:

```text
0x000  0x00000f04    record opcode
0x004  10            mode: one full-block window
0x008  0x20000001    window: opcode 1 in bits 7:0, start 0, length 512
0x00c  ...           512 state dwords (2048 bytes)
0x80c  0x00000003    terminator
0x810  0x00000f05    end command
0x814  0x00000000    eleven pad dwords
```

The record is 516 dwords (2064 bytes): 3 header dwords, 512 state dwords and
one terminator. The 528-dword envelope adds the end command and eleven zero
dwords, 2112 bytes. The submitted length must cover both, because the command
processor stops at the end command but the ring needs the dwords behind it; a
length that stops at the terminator wedges the pipe.

Every later record in the same buffer is a delta: `0xf04`, one window per run
of changed dwords, `0x03`. A delta is 4 to 516 dwords. The command processor
keeps the state the previous record left and a record never unsets a field.

Batching is a host choice, not a device limit: up to 80 records, up to 64
buffers of 4096 dwords each, and 160 resident buffers per submission.

See [State records](../graphics/state-records.md) and
[Indirect buffers](../host/indirect-buffers.md).

## Submit

The submission is one `CS` ioctl: a context id, a residency-list handle, a
chunk count, and an array of 16-byte chunk headers.

| Chunk | Payload | Bytes |
|-------|---------|-------|
| `LOONGGPU_CHUNK_ID_IB` | VA start, byte length, engine, instance, ring, flags | 32 |
| `LOONGGPU_CHUNK_ID_FENCE` | Fence object handle and dword offset | 8 |
| `LOONGGPU_CHUNK_ID_BO_HANDLES` | Residency list | Header plus one record per buffer |

The indirect buffer is named by a 64-bit VA and a byte length. The length must
be a nonzero multiple of four and at most `0xFFFFFF` dwords, because the ring
packet packs the dword count and the VMID into one control word:
`length_dw | vmid << 24`. The VA must start on a 4-byte boundary.

Admission then checks, in order: one mapping covers the whole range, that
mapping is present, the backing buffer belongs to this submission's residency
list, and the range fits inside that buffer. Nothing checks an execute
permission, because the page table has none. A refused chunk fails the ioctl
before any packet reaches a ring.

The kernel turns the chunk into one 5-dword ring packet: a VMID word, an
indirect-buffer header, the address in two dwords, and the control word. The
frame that carries it ends with the user-fence write, a barrier, and the kernel
fence.

See [Submissions](../host/submissions.md) and [GPIPE](../host/gpipe.md).

## The record is the draw

No draw command exists. The record's terminator is what makes the accumulated
writes the draw's state, and Draw Info at offset 0 is what the state draws with.
This is why a draw costs one record and no packet of its own: change the count,
the index address or a viewport, and the next record is a different draw.

When the command processor reaches the packet it fetches the buffer, applies the
record, and launches the vertex stage from the DS slot in DS-as-VS mode. The
rasterizer assembles triangles and generates fragments; the fragment stage runs
from the PS slot, and the output merger applies depth, stencil and blend and
writes the colour buffer record.

A blit is a different path: BPIPE has its own ring and context, and a blit is
not a packet appended to a draw.

See [Graphics pipeline](../graphics/pipeline.md),
[Vertex input](../graphics/vertex-input.md) and [SIO](../shaders/io.md).

## Wait, and what the fence covers

`CS` returns a handle: the sequence of this submission on this context and
ring. `DRM_IOCTL_LOONGGPU_WAIT_CS` waits for one such sequence;
`DRM_IOCTL_LOONGGPU_WAIT_FENCES` waits for an array of them, all or any. A
fence query reports `RETIRED` when the kernel has established that the
submission can no longer access its buffers; `ERROR` reports an execution
failure and does not prove retirement.

The same fence is attached to every buffer in the residency list. That is the
fence's coverage, and it is exactly the list: a buffer that is mapped and
reachable through a descriptor, but absent from the list, gets no fence and no
retirement evidence. The command buffer must be listed, because admission
requires it; the index, vertex and colour buffers are listed by the client so
that the fence covers them.

Retirement is not a drain. The fence is written by the command processor, which
runs ahead of the graphics pipe, so a signalled fence can precede the pipe's
last read of the buffers the submission named. Retirement also does not
establish CPU visibility, and it describes this submission only: an imported,
exported or scanout buffer may still be in use elsewhere. A completion seen
through an interrupt or a writeback poll has the same meaning and the same
limits.

`DRM_IOCTL_LOONGGPU_GEM_WAIT_IDLE` is not submission completion. It takes a
buffer handle, not a sequence, waits on that one buffer's reservation, and
returns a status word: 0 idle, 1 busy. A buffer that no submission ever named is
idle immediately, an execution error cannot be reported through it, and it says
nothing about any other buffer. It answers "the fences recorded on this object
have signalled", which by the paragraph above still does not mean the pipe has
finished with it. Use it to reclaim one buffer, never to conclude that a
submission completed.

To read the colour buffer through a CPU mapping, the client waits for the
submission and takes the mapping path that serializes GPU access; the fence
alone does not make the bytes visible.

See [Submissions](../host/submissions.md) and
[User-space interface](../host/uapi.md).

## What can go wrong

| Symptom | Cause |
|---------|-------|
| Ring stops consuming | The submitted length stops at the terminator instead of covering the end command and eleven pad dwords |
| Submission refused as too large | The length exceeds `0xFFFFFF` dwords |
| Submission refused as invalid | Length is zero or not a multiple of four, or the VA start is not 4-byte aligned |
| Submission refused as not present | The buffer's mapping is missing, or its backing object is not in the residency list |
| No fragments | Degenerate scissor, culling, or a viewport outside the guardband |
| Wrong colours | Colour hardware format, channel order, or blend state |
| Fault at address 0 | A zero descriptor root, or a shader slot holding a zero kernel entry |
| Fence never signals | The frame never reached its end: a record without a terminator or a truncated buffer |
| Buffer read too early | Waiting on the wrong object, or on a buffer the submission did not list |

## Open questions

- Whether the command processor requires the indirect buffer to start on a
  32-byte or 64-byte boundary: admission guarantees 4-byte alignment only, and
  the alignment the batching path uses is a host convention. UNRESOLVED.
- Whether the eleven pad dwords behind the end command are a processor
  requirement or a ring requirement. The failure of a shorter length is known;
  why is not. UNRESOLVED.
- Whether any client-visible operation establishes that the graphics pipe has
  drained. The fence does not, and no interface in the set provides a pipe-idle
  query. UNRESOLVED.

## Where to go next

- [Instruction reference](../isa/reference.md) for the rows in the shaders.
- [Descriptors](../shaders/descriptors.md) for the resource records.
- [Formats and layout](../graphics/formats.md) for the colour buffer.
