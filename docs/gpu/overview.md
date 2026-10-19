# Architecture and Graphical ABI

This is the home of the **Loongson GPU (LG200)** documentation section. It
covers the LG200 hardware architecture and the *graphical ABI* — the host,
graphics-state and shader contracts that a compiler or driver must implement
— and links to the per-instruction reference for the shader units.

The architecture, host, and graphics material is an independent clean-room
reference. The documents below cover the architecture and the graphical ABI
end to end, and the instruction pages are generated from the same
authoritative instruction table and reference-pseudocode suite; the
instruction rows are additionally pinned by real-hardware verification (see
[Verification status](#verification-status) below).

## Architecture

- [Architecture](architecture.md) — command and execution layers, wavefront and
  shader-unit model, memory hierarchy.
- [Instruction Encoding](encoding.md) — the 64-bit two-word instruction
  format, operand classes (general source, vector, scalar/descriptor,
  immediate, predicate, modifier, row-defined) and the encoding conventions.

## Host Interface

- [Host Interface](host-interface.md) — rings, launch packets and the GFX
  submission path.
- [Command Words](host/scmd32.md), [Rings and Indirect Buffers](host/rings-and-ib.md),
  [GPIPE](host/gpipe.md), [BPIPE](host/bpipe.md), [XDMA](host/xdma.md),
  [CPIPE](host/cpipe.md) and [Synchronization and Traps](host/sync-fence.md).

## Graphics State

- [Graphics State](graphics-state.md) — the full-state envelope and the state
  map with hyperlinks into the per-table reference.
- [Full-State Envelope](graphics/full-state.md), [Draw and Raster]
  (graphics/draw-raster.md), [Depth, Stencil and Blend]
  (graphics/depth-blend.md), [Stage Configuration](graphics/stage-config.md).

## Shader ABI

- [Shader ABI](shader-abi.md) — register files, address chain, argument
  passing and return conventions.
- [Shader-Info Records](shader/shader-info.md), [Descriptors](shader/descriptors.md),
  [Stage Conventions](shader/stages.md) and [SIO](shader/sio.md).

## Instruction Reference

The shader instruction set is grouped into seven families:

| Family | Instructions |
|--------|-------------:|
| [Vector ALU](vector-alu.md) | 374 |
| [Scalar ALU](scalar-alu.md) | 119 |
| [Memory and Image](memory-image.md) | 138 |
| [Atomic](atomic.md) | 97 |
| [Control Prefix](control-prefix.md) | 20 |
| [SIO and Export](sio-export.md) | 17 |
| [Interpolation](interpolation.md) | 3 |

Each instruction page keeps the guide's anatomy — Synopsis, Description,
**Encoding and operands** (the bit-field visual) and Operation — with the
encoding breakdown and operand chips following the encoding conventions in
[Instruction Encoding](encoding.md).

## Verification status

The architecture, host, and graphics prose in this section is a
**clean-room reference**; the instruction rows below are **hardware-verified
machine rows**. In particular:

- Every one of the 768 LG200 machine rows in `code/lg200-instructions.tsv`
  carries deterministic case vectors. The CPU oracle suite
  (`make -C code/lg200 -j verify`) and the real-hardware PS-generic carrier
  (`python3 code/lg200/gpu/run_gpu.py`, one process per instruction with a
  fence timeout) run the **same vectors**. The authoritative hardware matrix
  is **768/768 PASS** — 20,694 case passes over 20,729 rows, 0 failures —
  and is produced as `code/lg200/gpu/build/gpu_ps_generic_results.tsv`.
- The instruction `Operation` blocks are **reference pseudocode**; the
  deterministic case vectors pin down the intended semantics and are compared
  bit-exactly against the real chip through the carrier.
- Launch-state-dependent families (flat memory, formatted buffer loads,
  matrix multiply, texture fetch/sample) execute on the carrier in
  observed mode: their rows record execution and observation
  (`(observed)` in the matrix) rather than an independent comparison oracle.
- Items whose semantics remain open are marked `UNRESOLVED`; this guide does
  not invent semantics for them.

## Regenerating this section

The instruction data is committed in `code/lg200-instructions.tsv` (the
vendored instruction table) and `code/lg200/` (reference pseudocode `.h`,
oracle + case `.cpp`, and the `ops/` oracle library); `main.py` renders the
pages from them at build time, so the site is fully self-contained. CPU
oracle: `make -C code/lg200 -j verify` (768 instructions, 20,729 vectors).
Real-hardware matrix: `python3 code/lg200/gpu/run_gpu.py`, which writes
`code/lg200/gpu/build/gpu_ps_generic_results.tsv`. Updates are made by
refreshing the committed files from the authoritative table and re-running
both verifiers.
