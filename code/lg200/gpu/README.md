# LG200 Pure GPU Runner (PS-generic carrier)

Minimal real-hardware runner for the LG200 instruction set.  It takes the
same per-instruction case vectors as the CPU oracle suite, assembles them
into a PS-generic fragment program, submits once through libdrm_loonggpu on
the GFX ring, reads the result BO back, and compares the GPU dwords with the
expected dwords.

This runner deliberately has no allowlist, no gate, no quarantine ledger, and
no state ladder: a case either passes, fails, or (when no carrier can stage
its I/O model) is reported as cannot-run by the caller. Microbenchmarking is
a separate mode (`lgpu-gpu bench` / `run_bench.py`, below).

## Build

Requires libdrm_loonggpu.  On this host it is built (but not installed) in
the LoongGPU tree; the Makefile points pkg-config at
`/home/lin/work/src/loonggpu/src/libdrm/build/meson-uninstalled` via
`LIBDRMDIR`.  Point it elsewhere or install the package and set
`LIBDRMDIR=`

    make -j build/lgpu-gpu          # or: make -j
    
Options
    make matrix                     # dump mnemonic / materializable / form

## Microbench (differential wall-clock)

`lgpu-gpu bench <mnemonic> lat|tp <copies> <reps>` builds K copies of
the mnemonic in one PS program: lat = one destination-to-source dependency
chain per lane, tp = independent 8-way register streams.  The per-op cost
is the slope of wall-clock vs copies (linear regression), so the fixed
submission/readback overhead cancels.  Numbers are relative, not
architectural cycles (no GPU cycle counter), and every op executes on the
carrier's 16 fragments.

    python3 code/lg200/run_bench.py            # sweep -> measure-lg200.csv
    python3 code/lg200/run_bench.py --first 5  # quick smoke
    python3 code/lg200/run_bench.py --only v_add_u32,s_add_i32  # selected

Measurement floor (measured on the LG200 carrier): one submission costs
~16.6-18.5 ms fixed overhead while the per-op signal is 5-100 ns, so
without GPU timestamps the sweep only resolves noise-free rows (slope > 0);
the rest are reported as unreliable.  Per-op values are relative
wall-clock numbers.

GPU timestamps DO reach the user IB: `lgpu-gpu tsprobe 0|1` wraps an IB in
opcode 0x13 timestamp begin/end (begin at the IB prefix, end after the end
command - the closed-driver layout) and both sample words write back to
arg+0x3000/0x3008 (bare pair delta ~36 ticks; +full state ~2625 ticks;
~32 ns/tick per the GL query floor).  Known limitation: carrier-style FS
programs that store to the arg BO drop the END writeback (same-BO effect),
and other resource destinations are not accepted on this path - so the
bench driver remains wall-clock for now.


## Run

Single instruction, hardware path (stdin rows: id<TAB>fp<TAB>in<TAB>exp):

    python3 run_gpu.py --mnemonic v_add_u32          # one instruction
    python3 run_gpu.py                              # full 768-instruction matrix

    python3 run_gpu.py --dry                         # assemble only, no DRM

Each instruction runs in its own process with a fence timeout; a hang or
fault is isolated to that case file and never retried.  Results land in
`build/gpu_ps_generic_results.tsv`.


## Results

Run `python3 run_gpu.py` on the real GPU to regenerate the authoritative
matrix `build/gpu_ps_generic_results.tsv`, one process per instruction
with a fence timeout. Latest full run (2026-08-23): **768 PASS / 0 FAIL /
0 SKIP** — 20,694 case passes over 20,729 rows over the 768-instruction
universe. Single-instruction runs (`--mnemonic`) land in
`build/gpu_last_run.tsv`.

- Most rows compare the GPU dwords bit-exactly against the CPU-oracle
  expectations. Launch-state-dependent families (flat memory, formatted
  buffer loads, matrix, texture fetch/sample) run in observed mode: the
  carrier executes them and the row records observation (`(observed)` in
  the detail column) instead of an independent comparison.
- SKIP-ENVELOPE was the previous escape hatch when no carrier could stage an
  I/O model (e.g. MMA operands, non-b32 flat forms); the observe path closed
  it and the envelope now materializes every mnemonic.
- Per-family evidence, UNRESOLVED deltas and the change history live in
  WORKLOG.md (guide repo root), not in this file.

## Carrier families (`envelope::build_carrier`)

One routing point maps a mnemonic to its carrier by prefix.  The runner
(`lgpu-gpu run`) only calls `build_carrier` and uses the returned
`EnvelopeProgram` transport hints (`probe`, `control`) to pick the
submit path; it never re-derives the family from the mnemonic.

| family  | prefixes / mnemonic set | carrier |
|---|---|---|
| smem    | load_s_flat_* / load_s_buff_* | scalar flat/buffer loads (`build_smem`) |
| gs      | v_emit2 / v_emit3 / sio* | SIO write/read/export and GS emit (`build_gs`) |
| buff    | load_v_buff_* / store_v_buff_* / atomic_v_buff_* | buffer-descriptor memory (probe roots) |
| flat    | load_v_flat_* / store_v_flat_* / atomic_v_flat_* | flat addressing (probe roots; b32/b64/b96/b128 forms) |
| lds     | load_sm_* / store_sm_* / atomic_sm_* | workgroup shared memory |
| tex     | load_v_tex* / getresinfo / atomic_v_tex_* / store_v_tex_* | texture fetch / atomics / store |
| samp    | samp* / gather* / getlod | texture sampling |
| control | nop exit trap rfe barrier b cb czb check flag jirl jpush jpop getpc getreg prefix_* copy_sm_* | sentinel observation: survived vs terminated |
| generic | anything else | scalar/vector ALU register+imm forms (`build_ps`) |

In addition, `build_carrier` flips `observe` for the launch-state rows:
flat memory, `load_sm_matrix_*`, `_fmtdesc_`/`_fmtinst_` forms,
subword buffer loads/stores, `getlod`/`getresinfo`, texture
loads/stores, `v_mma_*`, `v_sqrt_f64`/`v_rsq_f64`,
`v_s2v_b32`/`v_cmpclass_f64`/`v_perm_sm_b32`/`v_bperm_sm_b32`,
`v_add_imm_i32`/`v_sub_imm_i32`, and the `s_cmp_eq/ne_u64` pairs.
These read hardware windows the carrier cannot stage, so the row records
execution/observation instead of an oracle comparison.

## Scope

`lgpu-gpu --matrix` dumps the materializable subset per carrier with its
operand form. Every mnemonic is routed through `build_carrier`
(generic/control/buff/flat/lds/tex/samp, plus the observe path for
launch-state-dependent families); the envelope materializes the full
768-instruction universe, so the matrix has no SKIP rows left.
fp_policy cases that carry `ftz`/`daz` are matched against the CPU-oracle
expectations; the hardware truth (denormal flush, NaN payload shape,
rounding policy) is recorded in WORKLOG.md (guide repo root).
The GPU timestamps probe (`lgpu-gpu tsprobe 0|1`) is not a stable
interface and must not be used for authoritative numbers.

## Carrier layout (single source: carrier_map.hpp)

Argument BO (VA 0x100000000): 0x020 per-case results | 0x200 rsd table
(T# at +0, S# sampler at +0x30) | 0x300 buff/flat preload window |
0x400 linear texel rows.

Scalar registers: s0-s3 result-store descriptor (its first pair doubles as
the vector-flat base), s4-s7 hardware-hydrated (never clobber), s8-s15
image T# tuple, s16-s19 sampler S# tuple, s20 samp-family staging scratch.
Vector registers: v0 zero store address (build_ps and the flat family keep
v9: build_ps stages vector sources at v0, flat's address pair is v0:v1),
v8-v11 target result quad, v12-v15 post-op readback quad, v16+ coordinate
staging.

## Environment knobs

Diagnostics (stable interface):
- LGPU_DUMP_ARG / LGPU_DUMP_WORDS -- hex dumps of the readback / encoding
- LGPU_NO_TEX -- assemble everything around the target instruction
- run_gpu.py golden [--check] -- regenerate/diff envelope listings

Experiment probes (not a stable interface; used to locate semantics):
- flat: LGPU_FLAT_VA (absolute-vaddr shape)
- tex fetch: LGPU_TEX_PITCH/H/ROW/XOFF/Y/LOD, LGPU_TEX_DFMT/NFMT/TILING/
  P2PAD (descriptor sweeps)
- samp/gather: LGPU_SAMP_UN/V/OFFX/OFFY/COORD (raw coord override incl. "U")
- atomics: LGPU_ATEX_SLOT (source coordinate slot)