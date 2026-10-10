# Execution Model

This chapter describes how work reaches the hardware and how a shader runs.
Values that differ between generations are in the
[generation profile](../chips/lg200.md); terms are defined in the
[glossary](glossary.md).

## Command and execution layers

Four layers sit between an API call and a shader instruction.  Each has its own
word format and its own cursor domain, and the same numeric opcode can mean
different things in different layers:

| Layer | Format | Producer | Role |
|------:|--------|----------|------|
| 0 | kernel command word | kernel | configure a pipe, queue, MMU, interrupt or VRAM window |
| 1 | stream command word | outer ring | select a virtual memory domain, execute an indirect buffer, poll or write memory |
| 2 | pipe command stream | a pipe front end | graphics, blit, transfer or compute records |
| 3 | two-word shader instruction | shader front end | execute instructions from the kernel entry |

A decoder must carry the layer and the pipe along with a command word; the same
value in two layers is two different commands.

## Pipes

A *pipe* is a command-processing engine with its own ring, vocabulary and
completion source.  The software-visible pipe set is:

| Pipe | Role | Completion |
|------|------|------------|
| Graphics pipe | graphics ring; consumes indirect buffers holding a state object | end-of-pipe event |
| Blit pipe | inner graphics commands and blits | end-of-pipe event |
| Compute pipe | compute queues owned by the kernel compute driver | compute event |
| Transfer pipe | linear copies and fills on its own ring | transfer event |
| Decoder and encoder pipes | owned by a later generation; no launch contract here | not assigned |

Compute work has a different ownership model from graphics: the kernel creates a
queue, the queue's user ring carries the compute command grammar, and submission
happens through a doorbell on the management ring.  There is no user-visible BAR
doorbell.

## Shader execution

A shader is a stream of two-word instructions starting at the stage's kernel
entry.  Invocations are grouped into waves; each lane of a wave runs the same
instruction stream over its own registers.

* **Registers.** Each lane has its own vector registers; the scalar registers
  are shared by the wave.  Lane-mask registers carry the set of active lanes as
  data, and the condition register carries a scalar flag.  See
  [Operands and Execution Registers](../isa/operands.md) for the files, the
  reserved windows and the conditional-execution mechanism.
* **Divergence** is expressed through lane masks and branches, not through
  instruction-level predication.
* **Entry state.** A stage starts with the launch-provided registers described
  in [Stage Interfaces](../shader/stages.md): vertex index, instance identifier,
  interpolation coordinates, descriptor bases.
* **Exit.** A stage finishes by exporting its outputs through the stage
  interface, ordering pending memory work, and terminating the invocation.  The
  ordering step is what makes prior loads and stores visible before the stage
  completes.

## Memory

All addresses in commands and descriptors are GPU virtual addresses of the
selected domain.  The model has three properties a host must respect:

1. **Explicit residency.** Every range a submission touches must be mapped and
   resident until the submission's fence signals.
2. **No cache-management packets.** There is no flush or invalidate command: a
   CPU read of GPU-written memory happens after the fence, and CPU writes become
   visible through the ordering of the submission that consumes them.
   Host-written resources use write-combined mappings.
3. **Fixed map granularity.** Mappings are page-granular and aligned; an
   indirect buffer must additionally be mapped executable.

## Fixed-function blocks

Between and around the shader stages:

| Block | Function |
|-------|----------|
| Primitive assembly | vertex fetch through attribute descriptors, primitive assembly by the primitive type in Draw Info |
| Rasterizer | viewport transform, guardband clipping, scissor, fill mode, sample coverage |
| Interpolator | supplies barycentric coordinates and an interpolation coefficient table to the fragment stage |
| Depth and stencil | compare, write, polygon offset, stencil operations on both faces |
| Blending | per-target blend with optional logic operation, dual-source and independent modes |
| Texture unit | LOD selection, filtering, wrapping, depth comparison, gather, and format conversion on fetch |
| Streamout | writes generated vertices to buffers through descriptors, driven by the streamout stage record |
| Query unit | occlusion, primitives generated and primitives emitted counters with begin/end packets |

Multisampling exists in one configuration only — 4x — and colour resolve runs as
a GPU pass while depth and stencil resolve run through CPU transfers.  See
[Multisampling](../shader/descriptors.md#multisampling).

## Shader input and output

Stages exchange data through SIO, a channel-addressed interface, and through
descriptor-bound memory.  The stage conventions, the streams and the export and
emit rules are in [Stage Interfaces](../shader/stages.md) and
[SIO](../shader/sio.md).

## Synchronisation

Ordering is established at four different scopes, and none of them substitutes
for another:

| Scope | Mechanism |
|-------|-----------|
| Within an invocation | instruction ordering, the wait operation that orders pending memory work |
| Within a workgroup | barrier |
| Between submissions | the submission's fence |
| Host and device | buffer residency and the completion fence |

## UNRESOLVED

* Wave size, and the number of waves a unit can hold.
* Cache hierarchy parameters, sizes and associativity.
* Issue width, dual-issue behaviour and per-instruction latency.
* Whether any launch state beyond the documented entry registers is provided.
* The hardware behind a compute queue beyond its exposed interface.
* The decoder and encoder pipes: no launch contract exists for this generation.
