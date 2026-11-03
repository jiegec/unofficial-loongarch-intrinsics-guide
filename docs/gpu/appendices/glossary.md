# Glossary

This glossary defines the terms used throughout the manual. A term is defined
once here and linked from the chapters that use it.

## Hardware

| Term | Meaning |
|------|---------|
| **Wave** | The unit of scheduling: up to 32 lanes executing one instruction stream. |
| **Lane** | One invocation slot in a wave. |
| **Stream processor (SP)** | The block that executes shader waves. |
| **VGPR** | Vector general-purpose register, private to a lane. |
| **SGPR** | Scalar general-purpose register, shared by a wave. |
| **Lane mask** | A 32-bit register with one bit per lane; `p0..p3`. |
| **SCC** | The scalar condition code, one bit per wave. |
| **LDS** | Workgroup shared memory, addressed through the shared-memory instructions. |
| **SIO** | The channel-addressed interface between shader stages and fixed-function blocks. |
| **Pipe** | A command-processing engine with its own ring and completion source. |
| **GPIPE** | The graphics command processor. |
| **BPIPE** | The inner graphics and blit command processor. |
| **CPIPE** | The compute command processor. |
| **XDMA** | The transfer engine for linear copy and fill. |
| **DPIPE / EPIPE** | Display and encode routes; not used on LG200. |
| **Rasterizer** | The fixed-function block that turns primitives into fragments. |
| **Texture unit** | The block that reads an image, selects a mip level and filters samples. |
| **Output merger** | The depth, stencil and blend blocks after the fragment stage. |

## Commands and memory

| Term | Meaning |
|------|---------|
| **BO** | Buffer object: a GPU allocation with a handle, a size and a domain. |
| **VA** | GPU virtual address. |
| **PA** | Physical address. |
| **VMID** | Virtual-machine identifier; selects a page-table root. |
| **PASID** | Process address-space identifier. |
| **Ring** | A circular buffer of command words driven by a write and a read cursor. |
| **IB** | Indirect buffer: a nested command stream in GPU memory. |
| **State record** | The command sequence that writes a state object or a delta of it. |
| **State object** | The 512-dword graphical payload of a draw. |
| **Envelope** | The header, state, terminator and end command around a full-state record. |
| **SCMD32** | A stream command word, consumed by a command processor. |
| **ICMD32** | An immediate command word, used by the kernel to program a pipe. |
| **Fence** | A completion record: a sequence number in memory and a sync object. |
| **Epoch** | The device generation between resets; a completion from an older epoch is stale. |

## Shaders

| Term | Meaning |
|------|---------|
| **Shader-info record** | The 96-byte per-stage record with sizes, entry, roots and arguments. |
| **Root** | A shader-info address field that points at a descriptor table. |
| **Descriptor** | A fixed-size record that describes a resource. |
| **Attribute descriptor** | A buffer descriptor that describes one vertex element. |
| **Push constant** | A 256-byte block in the constant root. |
| **Kernel descriptor** | The 96-byte compute record addressed by the AQL packet. |
| **Kernarg** | The compute argument block, reached through `s[6:7]`. |
| **AQL** | The 64-byte compute dispatch packet written to a user queue. |
| **MQD** | The 14-dword memory queue descriptor. |
| **User queue** | A compute ring owned by a user-mode driver and rung by a doorbell. |

## Instructions

| Term | Meaning |
|------|---------|
| **Machine row** | One instruction variant: mnemonic plus the operand form it encodes. |
| **MCS queue** | The user-owned ring of a CPIPE user queue; the user writes its packets and write pointer. |
| **Format** | A machine encoding layout, such as `vop` or `flat`. |
| **Family** | A documentation grouping of rows, independent of the format. |
| **Selector** | The encoded operand class; 8-bit or 11-bit. |
| **Tuple** | Consecutive registers named by one encoded base. |
| **Modifier** | An encoded control field, `M.*`. |
| **Reserved bit** | A bit whose required value is not established; emit the base value. |
| **Ignored bit** | A bit the hardware does not use for the row. |

## Documentation

| Term | Meaning |
|------|---------|
| **UNRESOLVED** | A property the manual does not establish. |
| **Host policy** | A driver or compiler convention, not a hardware requirement. |
| **MUST / SHOULD / MAY** | Requirement, recommendation and option, as defined in [Conventions](../conventions.md). |
