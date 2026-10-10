# Loongson GPU (LG200)

This section documents the Loongson GPU shader instruction set and its
graphical ABI: enough to write a driver, and enough to understand what the
hardware does at the level of a command, a state object, a descriptor and an
instruction.

## How this section is organised

| Part | What it answers |
|------|-----------------|
| [Generation profile](chips/lg200.md) | the values and limits of one chip generation |
| [Glossary](concepts/glossary.md) | what each term means, once |
| [Execution model](concepts/execution-model.md) | how work reaches the hardware and how a shader runs |
| [Host interface](host/interface.md) | who owns which layer, and what a user driver submits |
| [Graphics submission](host/submission.md) | the indirect buffer, batching, packets, fences, memory |
| [The state object](graphics/state-object.md) | the 512-dword payload, block by block |
| [Draw and rasterization](graphics/draw-raster.md) | the draw description and the rasterization controls |
| [Depth, stencil and blending](graphics/depth-blend.md) | the output-merger state |
| [Shader ABI](shader/abi.md) | the stage record, its roots and the launch registers |
| [Stage interfaces](shader/stages.md) | how to write each stage, end to end |
| [Descriptors](shader/descriptors.md) | buffer, image, sampler and attribute records, bit by bit |
| [SIO](shader/sio.md) | the channel interface between stages and the pipeline |
| [Instruction encoding](isa/encoding.md) | the instruction word, the formats and the mask discipline |
| [Operands and registers](isa/operands.md) | selector space, register files, conditional execution |
| [Memory and image family](isa/family-memory.md) | addressing modes and the memory instruction families |
| [Verification tooling](tools/verification.md) | the oracle, the hardware runner and the benchmark |
| [Measurements](measurements.md) | the microbenchmark tables |

## Reading order

A reader with no prior knowledge should start with the
[execution model](concepts/execution-model.md), then read
[graphics submission](host/submission.md) and the
[shader ABI](shader/abi.md) before the instruction pages, which are the
reference for individual machine rows.

## Instruction reference

The instruction set is grouped into seven families.  Each family page carries
the family's semantics and an index of its machine rows; each row then has its
own page with a synopsis, a description of that exact variant, the encoded
operands, reference pseudocode, and measured cost where a measurement exists.

| Family | Instructions |
|--------|-------------:|
| [Vector ALU](vector-alu.md) | 374 |
| [Scalar ALU](scalar-alu.md) | 119 |
| [Memory and image](memory-image.md) | 138 |
| [Atomic](atomic.md) | 97 |
| [Control prefix](control-prefix.md) | 20 |
| [SIO and export](sio-export.md) | 17 |
| [Interpolation](interpolation.md) | 3 |

## How the descriptions are written

Each instruction page states what the row computes, the operands with their bit
positions, the reference pseudocode and the boundary behaviour that matters.
Behaviour that has not been established is written as `UNRESOLVED` in place
rather than guessed, and behaviour that diverges from a comparable architecture
is marked as a difference.

## Generations

Values in this documentation belong to the generation under "Generation
profile".  Chapters describe mechanisms; a chip-specific value lives in that
profile page or in a table column, so a new generation adds a profile and a
column rather than a second copy of the manual.
