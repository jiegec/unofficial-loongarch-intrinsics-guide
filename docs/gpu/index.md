# Loongson GPU (LG200)

This section documents the Loongson GPU shader instruction set and its
graphical ABI: enough to write a driver, and enough to understand what the
hardware does at the level of a command, a state object, a descriptor and an
instruction.

## How this section is organised

The chapters are grouped by the layer they describe, in the order a command
travels, so the directory tree and the navigation agree:

| Part | What it answers |
|------|-----------------|
| [Generation profile](chips/lg200.md) | the values and limits of one chip generation |
| [Glossary](concepts/glossary.md) | what each term means, once |
| [Execution model](concepts/execution-model.md) | the four layers, the pipes and the stream processor |
| *Host and commands* | layers 0 and 1: who owns what, and the command vocabularies |
| [Host interface](host/interface.md) | the ownership split and the kernel interface |
| [Stream command word](host/stream.md) | `SCMD32` and the payloads the pipes share |
| [Graphics submission](host/graphics-submission.md) | the indirect buffer, batching, packets, fences, memory |
| [GPIPE](host/pipes/gpipe.md), [BPIPE](host/pipes/bpipe.md), [XDMA](host/pipes/xdma.md), [CPIPE](host/pipes/cpipe.md) | one chapter per pipe vocabulary |
| [Compute](host/compute.md) | the compute queue and the dispatch (still thin, see the page) |
| *Graphics state* | layer 2's graphical payload |
| [The state object](graphics/state-object.md) | the 512-dword payload, block by block |
| [Draw and rasterization](graphics/draw-raster.md) | the draw description and the rasterization controls |
| [Depth, stencil and blending](graphics/depth-blend.md) | the output-merger state |
| *Shaders* | layer 3's contract with the driver |
| [Shader ABI](shader/abi.md) | the stage record and its roots |
| [Stage interfaces](shader/stages.md) | how to write each stage, end to end |
| [Registers and allocation](shader/registers.md) | the register files, the reserved windows and the size rules |
| [Descriptors](shader/descriptors.md) | buffer, image, sampler and attribute records, bit by bit |
| [Geometry Stage](shader/geometry.md) | geometry record formulas, staging and validation boundaries |
| [SIO](shader/sio.md) | the channel interface between stages and the pipeline |
| *Instruction set* | layer 3's encoding |
| [Instruction encoding](isa/encoding.md) | the instruction word, the formats and the mask discipline |
| [Operands and registers](isa/operands.md) | selector space, conditional execution, addressing shapes |
| [Instruction families](isa/family-vector.md) | what the rows of each family share: forms, operands and boundaries |
| *Appendices* | reference material, consulted rather than read in sequence |
| [Verification tooling](appendix/verification.md) | the oracle, the hardware runner and the benchmark |
| [Measurements](appendix/measurements.md) | the microbenchmark tables |
| [UNRESOLVED register](appendix/unresolved.md) | where every open item is, and which ones block work |

## Reading order

A reader with no prior knowledge should start with the
[execution model](concepts/execution-model.md), then read
[graphics submission](host/graphics-submission.md) and the
[shader ABI](shader/abi.md) before the instruction pages, which are the
reference for individual machine rows.

## Instruction reference

The instruction set is grouped into seven families.  A family is a grouping of
the reference chapters, not an encoding property, and the two are orthogonal: a
family may draw on several machine formats and a format may serve several
families.  Each family has a chapter for what its rows share and an index of its
machine rows; each row then has its own page with a synopsis, a description of
that exact variant, the encoded operands, reference pseudocode, and measured
cost where a measurement exists.

| Family | Rows | Chapter | Index | Machine formats |
|--------|-----:|---------|-------|-----------------|
| Vector ALU | 374 | [family](isa/family-vector.md) | [index](instructions/vector-alu.md) | `vop`, `vop-imm`, `shared-memory` |
| Scalar ALU | 119 | [family](isa/family-scalar.md) | [index](instructions/scalar-alu.md) | `sop`, `sop-imm-control` |
| Memory and image | 138 | [family](isa/family-memory.md) | [index](instructions/memory-image.md) | `smem`, `tex-buffer`, `flat`, `shared-memory` |
| Atomic | 97 | [family](isa/family-atomic.md) | [index](instructions/atomic.md) | `tex-buffer`, `flat`, `shared-memory` |
| Control prefix | 20 | [family](isa/family-control.md) | [index](instructions/control-prefix.md) | `sop-imm-control`, `jump-stack`, `shared-memory`, `prefix` |
| SIO and export | 17 | [family](isa/family-sio.md) | [index](instructions/sio-export.md) | `sio-id`, `sio-imm` |
| Interpolation | 3 | [family](isa/family-interpolation.md) | [index](instructions/interpolation.md) | `vop` |

Four rows sit across the family boundary, and each is documented in the chapter
that describes its mechanism rather than the one its name suggests:
`v_bperm_sm_b32` and `v_perm_sm_b32` are permutes over the workgroup allocation,
and `copy_sm_from_global` and `copy_sm_to_global` move blocks between the
workgroup allocation and global memory.

## How the descriptions are written
**A current figure is bold; a historical one is not.**  Where a page states a
number that a tool computes - coverage, the bench counts, the UNRESOLVED tallies -
the figure that is true now is written in bold, and a superseded figure is kept
plain, usually with the date it was true.  `check_doc_numbers.py` enforces the
bold side: it scans every page for bold figures near those measures and fails if
one is not the live value.  So a stale number stays readable as history, and a new
copy of a live number cannot appear without the suite noticing - which is the only
reason it is safe to repeat a computed figure in prose at all.


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
