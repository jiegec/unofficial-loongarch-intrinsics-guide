# Loongson LG200 Graphics and Compute

This manual describes the Loongson LG200 GPU: its command interface, its
graphics pipeline, its shader programming model and its instruction set. It is
written for driver and compiler engineers who know graphics and instruction-set
architecture but have no prior knowledge of this device.

## Scope

The manual covers the LG200 graphics and compute engines from the user-space
interface down to the shader instruction. It includes:

- the kernel interface a user-mode driver uses: buffer objects, virtual
  addresses, command submission, fences and user queues;
- the command processors, their rings and their packet formats;
- the fixed-function graphics pipeline, the state object and the resource
  formats;
- the shader ABI, the register model, descriptor tables and the stage
  interfaces;
- the complete instruction encoding, operand model and instruction families.

The display controllers, video decode and encode engines, and the internal
implementation of the kernel driver are outside this manual. The kernel
interface is described only to the extent that a user-mode driver depends on it.

## Structure

The manual is organised by layer, following the path of a command from the
application to the shader:

| Part | Contents | Answers |
|------|----------|---------|
| I. Architecture | [Overview](architecture/overview.md), [execution model](architecture/execution-model.md), [memory system](architecture/memory-system.md), [LG200 profile](architecture/generation-lg200.md) | What the device is made of, how work executes, how memory is addressed |
| II. Host interface | [UAPI](host/uapi.md), [buffer objects](host/buffer-objects.md), [MMIO](host/mmio.md), [rings](host/rings.md), [GPIPE](host/gpipe.md), [BPIPE](host/bpipe.md), [CPIPE](host/cpipe.md), [XDMA](host/xdma.md), [indirect buffers](host/indirect-buffers.md), [submissions](host/submissions.md), [compute submission](host/compute-submission.md) | How a driver allocates memory, builds commands and submits work |
| III. Graphics pipeline | [Pipeline](graphics/pipeline.md), [draw](graphics/draw.md), [vertex input](graphics/vertex-input.md), [rasterization](graphics/rasterization.md), [depth and stencil](graphics/depth-stencil.md), [blending](graphics/blending.md), [texture](graphics/texture.md), [formats](graphics/formats.md), [multisampling](graphics/multisampling.md), [queries](graphics/queries.md), [streamout](graphics/streamout.md), [clear and blit](graphics/clear-blit.md), [state records](graphics/state-records.md), [state object](graphics/state-object.md) | How a draw is described and rasterized |
| IV. Shaders | [Stages](shaders/stages.md), [shader ABI](shaders/shader-abi.md), [registers](shaders/registers.md), [descriptors](shaders/descriptors.md), [I/O](shaders/io.md), [geometry](shaders/geometry.md), [interpolation](shaders/interpolation.md), [compute](shaders/compute.md), [compiler contract](shaders/compiler-contract.md) | How a shader is launched, what it receives and what it must produce |
| V. Instruction set | [Encoding](isa/encoding.md), [operands](isa/operands.md), [execution](isa/execution.md), [addressing](isa/addressing.md), [families](isa/families/vector.md), [reference](isa/reference.md) | How an instruction is encoded and what it does |
| VI. Appendices | [Glossary](appendices/glossary.md), [UNRESOLVED register](appendices/unresolved.md), [walkthrough](appendices/walkthrough.md), [performance](appendices/performance.md) | Terms, open questions, one end-to-end example, relative cost |

## Reading paths

A driver engineer should read Part I, then Part II, then the state and pipeline
chapters of Part III. A compiler engineer should read the execution model, then
Part IV, then Part V. A reader who wants one concrete path through the system
can start with the [walkthrough](appendices/walkthrough.md) and follow its
links.

## Values and generations

Mechanism chapters describe behaviour that is common to the LG200 generation.
Numeric values, limits and feature support are collected in the
[LG200 profile](architecture/generation-lg200.md). A future generation adds a
profile page and a column; it does not duplicate the mechanism chapters.

## Conventions

Bit ranges, numeric notation, normative terms and the marking of unresolved
behaviour are defined in [Conventions](conventions.md). Every chapter uses that
notation without restating it.

## Open questions

Behaviour that the documentation does not establish is marked `UNRESOLVED` at
the point where it matters. The [UNRESOLVED register](appendices/unresolved.md)
lists every open item by area and gives its consequence for a driver or a
compiler.
