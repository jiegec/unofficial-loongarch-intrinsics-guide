# Loongson LG200 Graphics and Compute

This manual describes the Loongson LG200 GPU and the interface through which
software drives it. It reaches from the commands a client issues through the
kernel to the bit fields of a shader instruction. It is written for driver and
compiler engineers who know graphics and instruction-set architecture but have no
prior knowledge of this device.

## Scope

The manual covers the graphics and compute interface: the commands that allocate
memory, map addresses, submit work on a ring or a user queue, and wait for
completion; the command processors that consume the resulting packets; the
graphics pipeline and the state object that configures it; the shader ABI,
register files, descriptors and stage interfaces; and the instruction set, from
its encoding and operands down to the machine rows, one row per encoded
instruction variant.

Two areas are explicitly outside it.

- **The display interface.** The layer commands
  (`DRM_IOCTL_LOONGGPU_LAYER_*`), gamma (`DRM_IOCTL_LOONGGPU_VIDEO_GAMMA`) and
  metadata (`DRM_IOCTL_LOONGGPU_GET_META`, `DRM_IOCTL_LOONGGPU_SET_META`)
  belong to the display controller and are not part of the graphics or compute
  interface this manual describes.
- **The kernel driver.** Its scheduling, memory management, reset and recovery are
  described only where a client sees their result.

## Structure

The parts are ordered by layer, from the client command to the shader
instruction; the appendices hold material that belongs to no single layer.

| Part | Question it answers | Chapters |
|------|---------------------|----------|
| Architecture | What the device is made of, how a wave executes, how an address is translated, and what values the part has | [Overview](architecture/overview.md), [Execution model](architecture/execution-model.md), [Memory system](architecture/memory-system.md), [LG200 profile](architecture/generation-lg200.md) |
| Host interface | How memory is allocated, how a command stream is built, how a pipe is driven, and how work is submitted and completed | [User-space interface](host/uapi.md), [Buffer objects](host/buffer-objects.md), [MMIO and command words](host/mmio.md), [Rings](host/rings.md), [GPIPE](host/gpipe.md), [BPIPE](host/bpipe.md), [CPIPE](host/cpipe.md), [XDMA](host/xdma.md), [Indirect buffers](host/indirect-buffers.md), [Submissions](host/submissions.md), [Compute submission](host/compute-submission.md) |
| Graphics pipeline | How a draw is described, rasterized and written to a surface; how clears, blits and queries run; what a resource format means | [Pipeline](graphics/pipeline.md), [Draw](graphics/draw.md), [Vertex input](graphics/vertex-input.md), [Rasterization](graphics/rasterization.md), [Depth and stencil](graphics/depth-stencil.md), [Blending](graphics/blending.md), [Texture and sampling](graphics/texture.md), [Formats and layout](graphics/formats.md), [Multisampling](graphics/multisampling.md), [Queries](graphics/queries.md), [Streamout](graphics/streamout.md), [Clear and blit](graphics/clear-blit.md), [State records](graphics/state-records.md), [State object](graphics/state-object.md) |
| Shaders | How a stage is launched, what it receives, what it must produce, and what a compiler must respect | [Stages](shaders/stages.md), [Shader ABI](shaders/shader-abi.md), [Registers and allocation](shaders/registers.md), [Descriptors](shaders/descriptors.md), [SIO](shaders/io.md), [Geometry stage](shaders/geometry.md), [Interpolation](shaders/interpolation.md), [Compute](shaders/compute.md), [Compiler contract](shaders/compiler-contract.md) |
| Instruction set | How an instruction is encoded, what its operands select and how it executes | [Encoding](isa/encoding.md), [Operands](isa/operands.md), [Execution](isa/execution.md), [Addressing](isa/addressing.md); families: [vector](isa/families/vector.md), [scalar](isa/families/scalar.md), [memory and image](isa/families/memory.md), [atomic](isa/families/atomic.md), [control and ordering](isa/families/control.md), [SIO and export](isa/families/sio.md), [interpolation](isa/families/interpolation.md) |
| Instruction reference | How to read a row, and the entry that defines each instruction's own fields | [Reference](isa/reference.md), [Vector ALU](instructions/vector-alu.md), [Scalar ALU](instructions/scalar-alu.md), [Memory and image](instructions/memory-image.md), [Atomic](instructions/atomic.md), [Control and ordering](instructions/control-prefix.md), [SIO and export](instructions/sio-export.md), [Interpolation](instructions/interpolation.md) |
| Appendices | Terms, open questions, one worked example, relative cost | [Glossary](appendices/glossary.md), [UNRESOLVED register](appendices/unresolved.md), [Walkthrough](appendices/walkthrough.md), [Performance](appendices/performance.md) |

## Reading paths

| Reader | Order |
|--------|-------|
| Graphics driver | [Overview](architecture/overview.md) → [User-space interface](host/uapi.md) → [Buffer objects](host/buffer-objects.md) → [Indirect buffers](host/indirect-buffers.md) → [Submissions](host/submissions.md) → [State records](graphics/state-records.md) → [State object](graphics/state-object.md) → [Draw](graphics/draw.md) |
| Compute driver | [Rings](host/rings.md) → [CPIPE](host/cpipe.md) → [Compute submission](host/compute-submission.md) → [Compute](shaders/compute.md) |
| Shader compiler | [Execution model](architecture/execution-model.md) → [Shader ABI](shaders/shader-abi.md) → [Registers and allocation](shaders/registers.md) → [Compiler contract](shaders/compiler-contract.md) → [Encoding](isa/encoding.md) → [Operands](isa/operands.md) → [Execution](isa/execution.md) → [Reference](isa/reference.md) |

The [walkthrough](appendices/walkthrough.md) is one complete path: it follows a
draw from context creation to fence wait and links the chapter that defines each
step.

## Values

The manual describes the LG200 generation. The values, limits and feature support
of the part are collected in the [LG200 profile](architecture/generation-lg200.md).

## Conventions

Normative terms carry fixed meanings. The key words are **MUST**, **MUST NOT**,
**SHOULD**, **SHOULD NOT** and **MAY**: **MUST** marks a requirement of the
hardware or of the interface, **SHOULD** a recommended practice that may be
departed from only with a stated reason, and **MAY** an option. **Host policy**
marks a choice of one driver rather than a property of the device. Those terms,
the notation for numbers and bit ranges, the register names and the columns of a
field table are defined in [Conventions](conventions.md).

## Open questions

A property that the manual does not establish is marked `UNRESOLVED` where it
matters, with its consequence for a driver or a compiler. The
[UNRESOLVED register](appendices/unresolved.md) collects those items by area.
