# LG200 Generation Profile

This chapter is the value table for the LG200 generation: the constants a
driver, a compiler or a command-stream builder must agree with. Mechanism
chapters describe behaviour; a number lives here, once.

Every row carries a kind. **Hardware** is a field width, encoding or
architectural constant a conforming driver cannot exceed. **ABI** is a register
or window count the compiler and the driver agree on; changing it needs both
sides, and it is not a hardware capability. **Policy** is a value one layer
publishes; another layer may publish a different value for the same silicon,
which is not a hardware difference. **UNRESOLVED** is a value the sources do not
establish. The page shows two different 2D image maxima for that reason: both
are policy.

## Identity

The instruction table lists 768 machine rows across 12 encoding formats in 7
documentation families.

| Property | Value | Kind |
|----------|-------|------|
| Generation | LG200 | — |
| Instruction word, alignment | 64 bits, two 32-bit dwords; 8-byte aligned | Hardware |
| Shader types the record can name | vertex, geometry, fragment, streamout; compute has no type here | Hardware |
| Stage enables in the shader-config word | vertex, hull, domain, geometry, `SS` | Hardware |
| Domain-stage roles | domain, and domain-as-vertex | Hardware |
| Tessellation stages exposed | none | Policy |
| API level published by each layer | OpenGL 3.3, Vulkan 1.3 | Policy |

The stage-enable word holds one enable bit per stage plus three fusion bits:
domain as vertex, domain includes geometry, vertex includes hull. A geometry
pipeline serialises vertex-enable clear, domain-enable set, domain-as-vertex
set and geometry-enable set, so the vertex shader runs in the domain slot. Hull
and domain bits and a tessellation-level input exist, so the encoding has room
for tessellation, while no layer exposes such a pipeline: tessellation is a
policy that is off, not an absent encoding. The `SS` bit has no stage-config
record of its own, so the stage it enables is UNRESOLVED; the record's type
enum names a streamout type, which is the other trace of that stage.

## Pipes

| Pipe | Command opcode (MOP) | Write cursor | Read cursor | Ring on LG200 | Completion source |
|------|---------------:|-------------:|------------:|---------------|-------------------|
| GPIPE | `0x01` | `0x18` | `0x1c` | Yes | `0x01` |
| BPIPE | `0x02` | `0x30` | `0x34` | Yes | `0x02` |
| CPIPE | `0x03` | `0x38` | `0x3c` | Yes, client queues | `0x22` |
| XDMA | `0x04` | `0x20` | `0x24` | Yes | `0x23` |
| DPIPE | `0x0c` | `0x48` | `0x4c` | No | — |
| EPIPE | `0x0d` | `0x50` | `0x54` | No | — |

Cursor offsets are byte offsets in the command-interface register window, whose
layout is in [MMIO](../host/mmio.md); the last column is the interrupt source id
raised when the ring drains. Retirement is tracked by a writeback fence compared
against the submitted sequence, so the read cursor is diagnostic.

DPIPE and EPIPE are a generation boundary: their opcodes, cursors and block
types exist, but this generation's block list registers neither, so no ring is
created, an inventory query reports none, and their interrupt source ids are
named for the next generation.

A pipe does not own a VMID, and a packet names its own address space. Eight
contexts exist: 0 kernel, 1–3 the client pool for graphics, blit and transfer,
4–7 the compute pool, claimed only while a compute queue is live.

## Execution resources

| Resource | Value | Kind |
|----------|-------|------|
| Wave width | 32 lanes | ABI |
| Scalar registers | 102 ordinary, `s0..s101` | ABI |
| Vector registers | 256 per lane, `v0..v255` | ABI |
| Lane-mask registers | 4, scalar selectors `0x7c`–`0x7f` | Hardware |
| Condition register | 1, scalar selector `0x80` | Hardware |
| Shader-record fields | SGPR `[7:0]`, VGPR `[15:8]`, LDS `[31:16]`, code size `[63:48]` | Hardware |
| Program size field | 16-bit byte count, so 8191 instructions at most | Hardware |

The wave width is what the register allocation, the 32-bit lane-mask registers
and the launch wave count agree on, not a proven ceiling (see
[Execution model](execution-model.md)). The VGPR field is 8 bits and encodes 255
as the "v255 in use" high-water case, so 256 is never written into it. The
program ceiling is arithmetic: a 16-bit byte count cannot exceed 65535 bytes and
an instruction is 8 bytes, so an overrun is unencodable, never truncatable.

## Compute limits

A launch is bounded twice: by what the launch word can encode and by what the
layer publishes. The two differ by a factor of four in the invocation limit and
of sixteen in the LDS window.

| Limit | Value | Kind |
|-------|-------|------|
| Local size per dimension | 10-bit field holding size − 1, so 1–1024 | Hardware |
| Published invocations per workgroup | 256 | Policy |
| Published workgroup size, x/y/z | 256, 256, 64 | Policy |
| Published workgroup count, x/y/z | 65535 each | Policy |
| Published subgroups per workgroup | 8 | Policy |
| LDS window an encoding can name | 11-bit granule field, 128-byte granule, `0x3f8` bias: 262016 bytes | Hardware |
| Published LDS per workgroup | 16384 bytes | Policy |
| Compute queues | 8 | Hardware |

The workgroup-size word holds one 10-bit field per dimension and is the only
place the local size reaches the hardware; a launch that omits it runs one wave
of one lane whatever the shader declared. The product is what a launch costs and
it is bounded separately from the per-dimension fields, so compare it in 64 bits
and reject an overflow.

The LDS granule with its `0x3f8` bias encodes 0, 128, 256, 512, 4096 and 16384
bytes exactly; a window above 262016 bytes would encode as a smaller one and the
shader would address past the region the firmware reserved, so reject it at
launch.

## Memory

| Property | Value | Kind |
|----------|-------|------|
| Client address width | 40 bits, one 1 TiB window | Hardware |
| Address form the hardware sees | 48-bit linear, upper 16 bits dropped | Hardware |
| Excluded head | first 1 MiB | Policy |
| VA hole | `[0x0000_8000_0000_0000, 0xffff_8000_0000_0000)` | Hardware |
| Page-table entry | 8 bytes | Hardware |
| Page size | 4 KiB or 16 KiB, fixed at kernel build | Policy |
| Walk levels for a client VM | 3: dir0, dir1, dir2 | Hardware |
| VMIDs | 8 | Hardware |
| LDS aperture word | `0x0001_0000`, naming a 4 GiB span | Hardware |
| Scratch aperture word | `0x0002_0000`, naming a 4 GiB span | Hardware |

The head is an exclusion, not a mapped reservation, and mapping below the 1 MiB
floor is a separate decision. The hole is half-open and rejects a client address
outright; an address above the hole is masked to its 48-bit linear form first,
which can alias back into the excluded head, so the check runs again after
masking.

The two aperture words place the group (LDS) and private (scratch) segments. Each
names a 4 GiB span, which is also the range of a 32-bit LDS byte offset, and the
kernel fixes both values while userspace reads them back when it creates a
queue. A zero group aperture is not "no LDS this dispatch": it is a queue that
cannot run LDS at all, and queue creation is refused.

## Graphics capabilities

| Capability | Value | Kind |
|------------|-------|------|
| Colour attachment records | 8 | Hardware |
| MRT count field | 3 bits, count − 1 | Hardware |
| Fragment colour exports | 8 | Hardware |
| Published fragment outputs | 4 | Policy |
| Depth attachment records | 1 | Hardware |
| Viewport indices | 4-bit field, 16 | Hardware |
| Vertex attributes and buffers | 16 each | Policy |
| Uniform-buffer slots per stage | 12 | Policy |
| Storage-buffer slots per stage | 16 | Policy |
| Push constants | 256 bytes | Policy |
| Clip distances | 8 | Hardware |
| Fragment attribute map | 32 entries of 2 bytes | Hardware |
| Interpolation coefficients | 6-bit count, 0–63 | Hardware |
| Sampler and image slots per stage | 16 | Policy |
| Mip levels | 16, from 4-bit level fields | Hardware |
| Array layers | 12-bit descriptor fields; 256 published | Hardware / Policy |
| Multisampling | one enable bit; the count is not encoded | UNRESOLVED |
| Anisotropic filtering | ratio, threshold, bias, override fields; effect not established | UNRESOLVED |
| Compressed textures | no decoder in the texture unit | Hardware |
| 2D image maximum | 4096 (Vulkan), 8192 (OpenGL) | Policy |

Four values stand behind the attachment row: eight colour-buffer records and a
3-bit MRT count, eight exportable targets in the compiler, four published
fragment outputs in the Vulkan layer and eight render targets in the OpenGL
layer. A pipeline writing more than four targets rests on a contract no layer
claims.

Compressed images keep their blocks in a transfer-visible shadow and the sampled
surface is an ordinary RGBA8 image (RGBA16 for the HDR blocks) the driver
decodes and refreshes, so sampling costs no format support. Multisampling is the
opposite: one enable bit and a 2-bit sample-rate field, neither carrying the
count, so two- and four-sample surfaces serialise identically.

## Instruction set

| Family | Rows | Machine formats |
|--------|-----:|-----------------|
| Vector ALU | 374 | `vop`, `vop-imm`, `shared-memory` |
| Scalar ALU | 119 | `sop`, `sop-imm-control` |
| Memory and image | 138 | `smem`, `tex-buffer`, `flat`, `shared-memory` |
| Atomic | 97 | `tex-buffer`, `flat`, `shared-memory` |
| Control and ordering | 20 | `sop-imm-control`, `jump-stack`, `shared-memory`, `prefix` |
| SIO and export | 17 | `sio-id`, `sio-imm` |
| Interpolation | 3 | `vop` |

A family is a documentation grouping and a machine format an encoding class.
Every family but interpolation spans more than one format, and several formats
serve exactly one family, so a row's format must be read from the row.

## Host limits

These belong to the submission interface, not the engine, and each is enforced
before anything reaches a ring (see [Submissions](../host/submissions.md)).

| Property | Value |
|----------|-------|
| Full-state record | 3 header dwords + 512 state dwords + 1 terminator |
| Full-state envelope | 528 dwords, of which 517 are used |
| Delta record | 4–516 dwords |
| Client indirect buffer | at most `0x00ff_ffff` dwords |
| Published start / size alignment | 32/8 graphics and blit, 64/64 compute, 256/8 transfer |
| Guaranteed alignment | 4 bytes |
| Submission chunks | 1–4096 |
| Chunk payload | at most 4 MiB |
| Fence buffer | exactly one page, eight bytes left below the fence offset |

The `0x00ff_ffff` bound is the width of the count field a ring emitter packs
beside the VMID in one control word; a larger count would rewrite the VMID field
and switch address space mid-buffer, so the bound is the encoder's, not the
hardware's, and it must still be enforced because the failure is silent. The
published per-engine alignment is stricter than the 4 bytes admission checks, and
whether the command processor faults on a start honouring only 4 is UNRESOLVED.

## Open questions

| Item | Consequence |
|------|-------------|
| Vector units, lane capacity, resident waves | Throughput and occupancy cannot be derived from the ISA. |
| LDS capacity per compute unit | A window above the published 16 KiB cannot be sized against a documented limit. |
| Issue width and latency | Instruction selection cannot be tuned to a documented model. |
| The `SS` stage enable | The bit exists but no stage-config record describes the stage, so what it starts is not established. |
| Sample-count encoding | Two- and four-sample surfaces are indistinguishable in the state. |
| Anisotropic filter effect | No layer can advertise sampling it cannot guarantee. |
| Largest image dimension | Two layers publish different maxima for the same target. |
| Cull-plane behaviour | Eight planes are encodable, but per-plane versus per-primitive discard is not established, so a shader writing cull distance can over-cull. |
| DPIPE and EPIPE contracts | These engines have no ring on this generation and no payload contract. |
| Buffer start alignment enforcement | A driver cannot tell a hardware requirement from a convention. |
| Workgroup products above 256 invocations | The field encodes 1024 per dimension; nothing above 256 is established end to end. |
