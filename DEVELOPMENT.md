# Notes for Developer

- `main.py`: used for mkdocs-macros-plugin, implement functions used in markdown.
- `check.py`: check generated docs against gcc intrinsics document.
- `check_instr.py`: check generated docs against binutils opcodes.
- `code/find_vd_source.py`: locate functions that have vd/xd as a source operand.
- `code/gen_impl.py`: generate C implementations for intrinsics.
- `code/gen_tb.py`: generate testbench for intrinsics.
- `code/untest.py`: list untested intrinsics.
- `code/gen_measure.py`: generate `measure.h` to measure latency and throughput of instructions.
- `code/Makefile`: use `make run` to validate C implementations against hardware, use `make run-measure` to measure latency and throughput.

## Loongson GPU (LG200) section

The LG200 (Loongson GPU gen-2) docs under `docs/gpu/` are driven by committed
data, with no build-time generator:

- `code/lg200-instructions.tsv` — the vendored instruction table (description,
  encoding words, mask/layout columns) that `main.py` parses at build time.
  Every one of its 768 rows carries a manual-style description: one line that
  states what the instruction computes, a **Semantics.** paragraph with the
  behaviour the page does not show elsewhere, and a **Boundaries.** paragraph
  with the limits that matter.  Operand positions are in the encoding table and
  the pseudocode is in the row's file, so neither is restated in prose.
  Behaviour that has not been established is written as `UNRESOLVED` in place
  rather than guessed, and each pseudo-code file opens with the same one-line
  summary above its reference pseudocode.
- `code/lg200/<mnemonic>.h` — executable reference pseudocode (the `Operation`
  block).
- `code/lg200/<mnemonic>.cpp` + `code/lg200/ops/` — the CPU oracle and its case
  vectors, plus the committed registry and softfloat library.
- `code/lg200/set_desc.py` — safe single-row editor for the vendored
  instruction table (`--show`, `--check`, or `<mnemonic> <markdown-file>`).
  A description cell is markdown; newlines are stored as the two-character
  escape `\n` and expanded by `main.py` at render time.  Every write snapshots
  the table to /tmp first.
- `code/lg200/main.cpp`, `code/lg200/run_cases.py`, `code/lg200/Makefile` — the
  KISS offline runner: `make -C code/lg200 -j verify` compiles every oracle
  and runs every case vector.
- `code/lg200/ops/berkeley-softfloat-3/` — vendored **Berkeley SoftFloat
  Release 3e** (BSD-3-Clause, John R. Hauser); see its `COPYING.txt`. The bit-
  exact IEEE f16/f32/f64 arithmetic in `ops/softfloat.cpp` is delegated to it
  (rounding mode + daz/ftz wrapper; GCN-style min/max NaN rules and integer
  clamps stay local). NaN results follow the library's x86 default: derived
  NaNs are `0xFFC00000`/`0xFFF8000000000000`, propagated NaNs keep the source
  sign and payload.
- `code/lg200/gpu/` — the minimal **real-hardware runner** (PS-generic
  carrier): the open codec (`codec/`), the
  generic fragment envelope (`envelope.*`), and the thin libdrm_loonggpu
  submit layer (`drm.*`). It assembles each instruction's case vectors into
  a PS program, submits once on the GFX ring, reads the result BO back and
  compares with the expected dwords. No allowlist, gate, quarantine, or state
  ladder: a case passes, fails, or cannot-run.
- Build: `make -C code/lg200/gpu` (uses `libdrm_loonggpu`; point `LIBDRMDIR`
  at an uninstalled libdrm build directory, or install the package).
  `make -C code/lg200/gpu matrix` dumps the materializable subset per
  carrier; `python3 code/lg200/gpu/run_gpu.py` runs the entire
  768-instruction universe on the GPU (one process per instruction, fence
  timeout) and writes `build/gpu_ps_generic_results.tsv`; single
  `--mnemonic` runs land in `build/gpu_last_run.tsv`. The codec's typed
  assembly forms live in `code/lg200/gpu/codec/generated/lg200_codec.inc`
  (`generated::kCanonicalVariants`).
- Real-hardware results and remaining work are recorded in
  `code/lg200/gpu/README.md`. The authoritative matrix is **768/768 PASS**
  over the 768-instruction universe (20,694 case passes / 20,729 rows, 0
  failures); launch-state-dependent families (flat, formatted buffer,
  matrix, texture) are observed-mode rows. Hardware-truth findings so far:
  LG200 FP default flushes denormals, derives NaNs as `0x7FC00000` (not
  the Berkeley `0xFFC00000`), rounds round-to-nearest regardless of
  `rtz/rtup/rtdn` policy, min/max propagate NaN, several `fcvt` family
  cases were authored in the reverse conversion direction, some `fcvt`
  encodings are LG200 no-op holes (LG210 legacy), and the `v_rcp_f64`
  low-word mantissa model remains an empirical UNRESOLVED hardware artifact
  (`sf_rcp64` lo-map in `ops/oracle.cpp`).
- `docs/gpu/` — the manual itself, in three parts.  Mechanisms that hold for
  the family are described once per chapter (`concepts/`, `host/`, `graphics/`,
  `shader/`, `isa/`, `tools/`); the values and limits of one chip
  generation live in `chips/<generation>.md` and in the generation column of a
  table, so a new generation adds a profile page and a column rather than a
  second copy of the manual.  The instruction pages are generated from the
  table and the pseudo-code files by `main.py` at build time.
- Microbench: `python3 code/lg200/run_bench.py` sweeps the register-pure
  targets on the PS carrier and writes `code/lg200/measure-lg200.csv`,
  which `main.py` renders into the per-instruction Latency sections
  (relative wall-clock slope values; rows below the ~17 ms submission floor
  render as "-").

