# Glossary

This glossary gives the sense in which the manual uses each term and points at
the chapter that establishes it. An entry is a summary: where an entry and the
chapter it links disagree, the chapter governs. A property the sources do not
establish is not given a meaning here; it is marked `UNRESOLVED` in the entry
where it matters.

## Hardware

The blocks are named in [Architecture overview](../architecture/overview.md);
the register files, masks and quads are in
[Execution model](../architecture/execution-model.md).

| Term | Meaning |
|------|---------|
| **Invocation** | One execution of a shader stage. A wave runs one invocation per lane; a compute invocation is identified by its workgroup id and its local invocation id. |
| **Lane** | The slot in a wave that runs one invocation and holds that invocation's copy of the vector registers. |
| **Wave** | The 32 lanes that execute one instruction stream together and share one scalar file and one set of lane masks. 32 is the ABI width; whether the machine can run a wider wave is UNRESOLVED. |
| **VGPR** | Vector general-purpose register: `v0..v255`, 256 per lane, 32 bits each, one copy per lane. |
| **SGPR** | Scalar general-purpose register: `s0..s101`, 102 per wave, 32 bits each. Every lane of the wave reads the same value. |
| **Lane mask** | One of `p0..p3`: four 32-bit registers per wave at scalar selectors `0x7c..0x7f`. A register holds one bit per lane, so one register covers exactly one wave; the four are scalar addresses, not a fifth register file. |
| **Mask selector** | The 2-bit `PJ2` field that names the lane mask an instruction runs under. It is not an operand; eight of the twelve formats carry it, and a row without it cannot be gated. |
| **SCC** | The wave's condition register: one bit at scalar selector `0x80`, written by a scalar compare and read as a carry or a borrow. |
| **Quad** | Four adjacent lanes. A fragment stage widens its mask to whole quads when it runs a texture instruction, a derivative, a demote or a helper query. |
| **Helper lane** | A lane that executes under a widened mask but lies outside the coverage mask, so it may supply operands and may not write. |
| **LDS** | Workgroup shared memory. A shared address carries the aperture tag `0x0001` in bits 63:48; LDS is not part of the client virtual range and never appears in a page table. The per-workgroup capacity is UNRESOLVED. |
| **Scratch** | Per-invocation private memory, reached through aperture tag `0x0002` and expanded by work-item id. A graphics stage's spills use an ordinary buffer object instead. |
| **SIO** | The channel interface between a shader and the fixed-function blocks around it. A transfer names a stream, a channel and a window, and has no address. |
| **Pipe** | A command processor with its own ring and completion source. LG200 has four: GPIPE, BPIPE, CPIPE and XDMA. |
| **GPIPE** | The graphics pipe. It executes the state records an indirect buffer carries and drives a draw's stages and fixed-function state. |
| **BPIPE** | The blit pipe: 2D copy, fill and blit, with no shader. |
| **CPIPE** | The compute pipe. It consumes an MCS command stream from a user queue; the ring the kernel manages for it installs and removes queues and carries no dispatch. |
| **XDMA** | The transfer pipe: copy and fill over linear memory. |
| **DPIPE / EPIPE** | The display and encode pipes. The ring numbering and the command blocks define both, but LG200 has neither block: a submission that names one is refused, and their cursor registers are unused. LG210 has both. |
| **HIQ** | The hardware interrupt queue: the firmware packet ring the kernel writes for queue installation and removal, doorbells and status. |
| **Rasterizer** | The fixed-function block that turns primitives into fragments and decides coverage. |
| **Texture unit** | The block that reads an image, selects a mip level and filters samples. |
| **Output merger** | The depth, stencil and blend blocks that follow the fragment stage. |

## Commands and memory

Packet and queue mechanics are in [Rings](../host/rings.md) and
[Submissions](../host/submissions.md); addresses, VMIDs and the segment
apertures are in [Memory system](../architecture/memory-system.md).

| Term | Meaning |
|------|---------|
| **BO** | Buffer object: an allocation with a size, a placement and a handle. The placement is system memory, GTT or VRAM; the domain flags also name non-client blocks (GDS, GWS, ordered append and a doorbell page). |
| **VA** | GPU virtual address. A client address is 40 bits in one 1 TiB range, with the low 1 MiB a floor that is never mapped and addresses at or above 2^47 a hole. |
| **PA** | Physical address. A page-table entry holds a 4 KiB-aligned page frame in bits 39:12. |
| **VMID** | Virtual-machine identifier: one of eight contexts. A VMID names a page-directory root and the PASID last bound to it; 0 is the kernel's, 1–3 are the client pool and 4–7 the compute pool. |
| **PASID** | Process address-space identifier. The firmware identifies a queue by `(PASID, queue_id)`, and a PASID may have one queue. |
| **Ring** | A circular buffer of 32-bit command words with a write cursor and a read cursor. A kernel ring keeps its cursors in registers or writeback words; a user queue keeps byte positions in client memory. |
| **User queue** | A CPIPE work queue that lives in client memory. The client writes the commands, publishes the write position and rings a doorbell. |
| **Index block** | The 64 bytes immediately before a user queue's ring: a 64-bit write position at `ring - 64` and a 64-bit read position at `ring - 56`, both byte positions. |
| **Doorbell** | The publish step for a user queue. It names `(PASID, queue_id)` and carries no position; the position is the write index in the index block. |
| **IB** | Indirect buffer: a command stream in GPU memory that a ring packet names by address and length. Admission guarantees 4-byte alignment of address and length and nothing stronger. |
| **Submission** | One `CS` ioctl: a context, an optional residency set and an array of chunks. It names one ring. |
| **Chunk** | One 16-byte record in a submission's chunk array: an id, a length in dwords and a pointer to the payload. |
| **State object** | The 512-dword, `0x800`-byte graphics state block a draw runs from. It persists across records, so a record carries only the dwords that changed. |
| **State record** | The command sequence that establishes or changes the state object. A full record is 516 dwords; a delta record is 4 to 516. |
| **Window descriptor** | One dword that names a run of state dwords: opcode 1 in bits 7:0, the first state dword in bits 19:8, and the run length in bits 31:20. |
| **Draw Info** | The nine-dword, 36-byte structure at state offset `0x000`: topology, element source, counts and index address. There is no separate draw packet. |
| **Envelope** | A buffer that holds one full record and nothing else: 528 dwords, being the 516-dword record, the end command `0xf05` and eleven zero dwords. |
| **Stream packet** | A 32-bit word of a ring, an indirect buffer or a compute queue: opcode in bits 7:0, configuration above it, and a length implied by the opcode. |
| **Immediate command** | A command issued through the command port instead of a ring: block in bits 7:0, operation in bits 15:8, configuration in bits 31:16, started by a write to the trigger register. |
| **Fence** | The completion object of one submission, named by a 64-bit sequence handle. Retirement is established by the kernel and queried by that handle; the sequence word the engine writes into the fence buffer is a progress marker that can arrive before the fence. |
| **Epoch** | A reset epoch: the identity of the interval since the device last reset. A submission is admitted in the current one; a submission admitted in an older one belongs to a device state that no longer exists. |
| **MCS** | Micro-code stream: the command stream a CPIPE user queue carries. One command is a dword with the opcode in bits 7:0 and an opcode-specific payload, and the stream is self-delimiting. |

## Shaders

The record and its slots are in [Shader ABI](../shaders/shader-abi.md); the
compute launch is in [Compute submission](../host/compute-submission.md).

| Term | Meaning |
|------|---------|
| **Shader-info record** | The 96-byte, 24-dword record that describes one graphics stage: code size and entry, register counts, and the descriptor roots. Six slots at `0x540` in the state object hold one record per stage slot. |
| **Root** | An address field of a shader-info record that points at a descriptor table. The constant root is loaded into `s4:s5`, the resource root into `s6:s7`, and the read-write root into `s0:s1` for the streamout stage only. |
| **Descriptor** | A fixed-size record that describes a resource: four dwords for a buffer or a sampler, eight for an image. A row names the base register, and the record is the consecutive run from it. |
| **Attribute descriptor** | The 16-byte buffer descriptor for one vertex input, at `s8:s9 + 16 × n` for `n` in 0 to 15. |
| **Push constant** | The 256-byte window the application writes through the API: at `+0x4000` in the graphics constant root, and at `+0x90` from the compute kernarg base. |
| **Kernel descriptor** | The 96-byte record a compute dispatch names: flags, workgroup and grid sizes, private and shared sizes, entry address, kernarg base and two resource words. The entry is a field inside it, not the object. |
| **Kernarg** | The compute argument block, reached through `s[6:7]`. The same pair is the root of the compute descriptor tables; the implicit-argument block and the push constants follow the set pointers. |
| **AQL** | The 64-byte kernel-dispatch record. It is the vocabulary of a launch, not a packet on the queue: the CPIPE consumes MCS commands, and an AQL record placed at the write pointer is ignored. |
| **MQD** | Memory Queue Descriptor: the 14-dword record that describes a queue, holding the ring base, the read and write pointers, the length, the PASID, the queue id and type, the doorbell offset and the context-save fields. |
| **Workgroup** | The invocations one dispatch launches together. It is not a wave: a workgroup of more than 32 invocations spans several waves, and only `barrier` synchronises them. |

## Instructions

Encodings, operands and the row pages are in [Encoding](../isa/encoding.md),
[Operands](../isa/operands.md) and the
[instruction reference](../isa/reference.md).

| Term | Meaning |
|------|---------|
| **Machine row** | One instruction variant: a mnemonic with one format and one operand form. Another width, modifier or tuple is another row, and a row's documented behaviour binds only the variant it encodes. |
| **Format** | One of the twelve machine formats: `sop`, `sop-imm-control`, `smem`, `jump-stack`, `vop`, `vop-imm`, `tex-buffer`, `flat`, `shared-memory`, `sio-id`, `sio-imm` and `prefix`. A format fixes the opcode field's position and width and the slots its rows use; the word itself carries no format tag. |
| **Family** | One of seven documentation groupings of rows: vector, scalar, memory and image, atomic, control and ordering, SIO and export, interpolation. A family is not a format; every family but interpolation spans more than one. |
| **Selector** | The value of a general-source field: one byte naming a scalar register, the constant zero, an inline constant, a lane mask or `SCC`. An 11-bit field adds a vector-register namespace and the absolute and negate modifiers above that byte. |
| **Tuple** | A run of consecutive registers named by one encoded base. The width comes from the field's `xN` suffix and the alignment from the row; an unaligned run is rejected rather than split. |
| **Modifier** | An encoded bit whose effect is on another operand or on the row, written `M.name`: the clamp, the two buffer-address enables, the memory form bits, and one bit of the ordering domain. |
| **Base word** | A row's encoding with every operand zero. An encoder starts from it, inserts each operand in its parameter bits, and never changes a fixed bit. |
| **Reserved bit** | A bit the row does not define. It takes no part in the row match; the base holds zero there, but whether the machine requires zero is UNRESOLVED, so an encoder SHOULD write the base value. |
| **Ignored bit** | A bit inside a field slot that the row does not encode as an operand. The base holds zero there; what the machine reads from it is UNRESOLVED. |

## Documentation

| Term | Meaning |
|------|---------|
| **UNRESOLVED** | Marks a property the manual does not establish. It states the question where the property matters and never supplies a guess; a host that must act chooses a behaviour and records it as host policy. |
| **Host policy** | A driver or compiler choice that the device does not require, so another host may choose differently. It is not part of the device contract. |
| **MUST / SHOULD / MAY** | Requirement, recommendation and option, applied to what a host produces rather than to the device. Defined in [Conventions](../conventions.md#normative-terms). |
