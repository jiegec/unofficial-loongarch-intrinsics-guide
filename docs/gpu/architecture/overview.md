# Architecture Overview

The LG200 is a GPU with four command-processing pipes, a stream processor that
executes shader waves, a set of fixed-function graphics blocks, and a memory
system shared with the CPU. This chapter describes how those parts fit
together. The values of one generation are in the [LG200 profile](generation-lg200.md).

![Ownership and command path](../assets/fig-system.svg)

The figure shows which layers own which objects and where the rings sit between the
kernel and the command processors.

## Blocks

| Block | Role |
|-------|------|
| Host interface | Kernel ioctls, buffer objects, virtual addresses, submissions and fences. |
| GPIPE | Graphics command processor. Consumes the graphics ring and executes indirect buffers. |
| BPIPE | Inner graphics and blit command processor. Consumes the commands inside a graphics indirect buffer and drives blits. |
| CPIPE | Compute command processor. Drives compute queues and user queues. |
| XDMA | Transfer engine for linear copy and fill. |
| Stream processor (SP) | Executes shader waves: vector ALUs, scalar ALU, lane masks, texture and memory instructions. |
| Fixed-function graphics | Primitive assembly, rasterizer, interpolator, depth and stencil test, blending, texture filtering, streamout and queries. |
| Memory system | GPU virtual memory, page tables, VMIDs, caches and the memory interface. |

The display controllers, video decode and encode engines are separate blocks and
are not described here.

## Two command paths

A graphics command and a compute command take different paths to the SP.

A **graphics** submission is an indirect buffer (IB) submitted on the graphics
ring. GPIPE consumes the ring, executes the IB, and applies the state record it
contains. The state record selects the shader stages and the fixed-function
state; BPIPE commands inside the IB carry draw and blit payloads. The shader
stages then run over the primitives produced by the draw.

A **compute** submission uses a compute queue. The kernel creates the queue, the
user-mode driver writes the queue's user ring, and the kernel signals the queue
through a doorbell packet on the management ring. CPIPE consumes the queue and
launches the compute kernel described by the kernel descriptor.

An **XDMA** submission is a linear copy or fill on the transfer ring. It does
not use the graphics IB format or the compute queue record.

## Ownership

| Layer | Owner | What the layer controls |
|-------|-------|-------------------------|
| Application | Application | API objects and commands. |
| User-mode driver | Application process | Buffer objects, virtual addresses, IBs, queue records, shader binaries and descriptor tables. |
| Kernel driver | Kernel | Rings, VMIDs, page tables, queue provisioning, fences, reset and recovery. |
| Hardware | Device | Command processors, SP, fixed-function blocks, caches and memory. |

A user-mode driver never programs a ring cursor or a page table directly. It
builds memory objects and command buffers, submits them through the kernel, and
waits on a fence.

## A draw in one paragraph

A draw is a state record applied by GPIPE, followed by the draw's payload. The
record carries the state object: draw information, rasterization state, depth,
stencil and blend state, surface records, stage configuration and the shader-info
records. The vertex stage reads attributes through descriptor tables and writes
its output to stream SIO0. The rasterizer assembles primitives, clips and
rasterizes them, and the fragment stage reads interpolated inputs and writes
colour to stream SIO1. The output-merger blocks apply depth, stencil and blend
state and write the surface. See [Graphics pipeline](../graphics/pipeline.md) for
the detailed path.

## Execution in one paragraph

A shader runs as a wave of up to 32 lanes. Each lane has its own vector
registers; the scalar registers are shared by the wave. A lane-mask register
holds one bit per lane, and an instruction can be gated by any of the four mask
registers. Branches are wave-wide; per-lane control flow is expressed by
narrowing and restoring masks. See
[Execution model](execution-model.md) and [Execution](../isa/execution.md).

## Memory in one paragraph

Every address in a command, a descriptor or a shader is a GPU virtual address in
the VMID selected for the submission. The MMU walks a page table; buffers are
mapped explicitly and must remain resident until the submission's fence signals.
There is no cache-management packet: ordering is established by the submission
fence and by the shader's ordering instructions. See
[Memory system](memory-system.md).

## What this manual does not cover

- Display, video decode, video encode and the display controller registers.
- The kernel's internal scheduler, recovery state machine and debug interfaces,
  except where a user-mode driver observes their result.
- Performance in architectural cycles. The performance appendix gives relative
  wall-clock measurements and states their precision.
