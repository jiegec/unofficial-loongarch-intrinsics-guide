# LG200 hardware runner: the GFX (PS) and CPIPE (compute) carriers

Real-hardware runner for the LG200 instruction set.  It takes the same
per-instruction case vectors as the CPU oracle suite and runs them through one
of two launch engines:

* **`--pipe ps`** - the PS-generic fragment program on the GFX ring
  (`lgpu-gpu run`), and
* **`--pipe cs`** - the compute carrier on a CPIPE user queue
  (`lgpu-gpu csrun`), dispatched with an MCS stream.

Both compare the same GPU dwords against the same expected dwords, so a row
that passes through one engine and fails through the other is a statement about
the two launch environments rather than about two comparators.

This runner deliberately has no allowlist, no gate, no quarantine ledger, and
no state ladder: a case either passes, fails, or (when no carrier can stage
its I/O model) is reported as cannot-run by the caller. Microbenchmarking is
a separate mode (`lgpu-gpu bench` / `run_bench.py`, below).

**Before resuming after a fault, read [HARDWARE-SESSION-PLAN.md](HARDWARE-SESSION-PLAN.md)**: it records the `trap` wedge, the preconditions, the recovery ladder, and the ordered list of experiments that still need the device.

## Build

Requires libdrm_loonggpu.  Point `LIBDRMDIR` at an uninstalled libdrm build
(meson-uninstalled directory), or install the package and leave `LIBDRMDIR`
empty.

    make -j build/lgpu-gpu          # or: make -j

`LIBDRMDIR` is only honoured when that directory contains
`libdrm_loonggpu-uninstalled.pc`, which is the meson layout.  An *installed*
prefix has its `.pc` under `lib/pkgconfig` instead, and then neither
`LIBDRMDIR` nor the system `pkg-config` finds it unless the prefix is on
`PKG_CONFIG_PATH` - the link then fails with `undefined reference to
loonggpu_bo_va_op` and leaves no binary behind.  Pass the two variables
explicitly in that case:

    P=/path/to/prefix
    make LGPU_CFLAGS="-I$P/include" \
         LGPU_LIBS="-L$P/lib -ldrm_loonggpu" \
         LGPU_RPATH="-Wl,-rpath,$P/lib" \
         LGPU_DEF="-DLGPU_HAVE_LIBDRM -DLGPU_HAVE_DRM" build/lgpu-gpu

Pass all four.  `LGPU_LIBS`/`LGPU_RPATH` alone only fix the LINK: any source that
has to be recompiled still needs `LGPU_CFLAGS` and `LGPU_DEF`, and without the
defines a DRM source compiles against the no-DRM stubs and fails with errors like
`variable or field 'cs_fill_request' declared void`.
    
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
command) and both sample words write back to arg+0x3000/0x3008 as full 64-bit
values (bare pair delta 36 ticks = 48 ns; +full state ~2700 ticks = ~3.6 us).
Known limitation: carrier-style FS
programs that store to the arg BO drop the END writeback (same-BO effect),
and other resource destinations are not accepted on this path - so the
bench driver remains wall-clock for now.

The counter advances at ~749 MHz (1.334 ns/tick), measured by reading the
absolute stamp twice around a known wall-clock interval, not inferred: five
10 s runs give 748.5-749.8 MHz.  An earlier revision of this file said
"~32 ns/tick per the GL query floor" and `tsprobe` multiplied by that, which
made every nanosecond figure it printed about 24x too large.  The rate is a
clock and this part has GPU DPM disabled, so it is stable here; measure it
again on another part rather than reusing the constant.  Because the absolute
stamps are printed, the rate is re-derivable from any two runs.


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
- Per-family UNRESOLVED deltas are recorded with the family that owns them,
  not in this file.
- **Not re-run on hardware since the SIO operand fix (2026-10-12).**  The four
  identifier forms `siordrot_id`, `siowrrot_id`, `siordrot_id_c4` and
  `siowrrot_id_c4` now emit their third vector source (`VK8`); the last full
  hardware run encoded that field as the zero selector.  These rows are
  observed-mode (their oracle is a data passthrough), so their earlier PASS said
  nothing about the field.  The change is host-verified (codec encode/decode
  round trip, audit rule E, `--dry` for both pipes) but has not been submitted
  again.

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
rounding policy) is recorded with the affected instruction families.
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

## The compute (CPIPE) carrier

`lgpu-gpu csrun <mnemonic>` runs the same case rows through the compute engine.
The transport is the vendor/`loongvk` one, in full:

1. a ring BO holding a 64-byte user-queue index block followed by the ring
   (`wr_ptr` = ring_va - 64, `rd_ptr` = ring_va - 56, as the kernel's MQD
   expects), zeroed at allocation;
2. a 96-byte kernel descriptor (`.kd`) at a fixed VA, built with
   `loongvk_mcs_patch_kd` from the vendored LoongVK builder plus the `+0x00`
   flag word;
3. a kernarg block whose first eight bytes are the output BO's VA - the layout
   the vendor producer's own kernels read through `s[6:7]` - followed by the
   implicit-argument block (block_count / group_size / grid_dims /
   global_offset);
4. an MCS stream built by the vendored builder and written at ring byte 0, with
   its end published as the byte write pointer immediately before the doorbell;
5. a completion signal armed with one packet, waited on until it moves
   (the device decrements it, so 1 -> 0; the value is reported either way).

### What makes the compute program a compute program

The instruction stream, the source staging and the family quirks are shared
with the PS carrier; only the two things a launch actually provides differ, and
they are confined to `CarrierEnv` in `envelope.cpp`:

* **The result path.**  Graphics assembles a buffer descriptor in `s0-s3` and
  stores with `store_v_buff_b32`; compute assembles the output BO's VA as a
  literal into a VGPR pair and stores with the **flat** form, which is the shape
  the vendor's own compiled kernels emit (`store.v.flat.b32 p0, v0, v[4:5]` with
  the full address in the pair and the architectural zero base in `s20:s21`).
  Keeping the result path independent of launch registers is deliberate: a wrong
  launch register must not be able to masquerade as a wrong instruction.
* **The register windows.**  `LOONGVK_KD_RSRC1` pins `kernel_sgpr_cnt` to 11, so
  the hardware writes group id x/y/z into `s11/s12/s13`; the compute scalar
  source window therefore starts at `s8` and the carrier never writes
  `s11-s13`.  The program also reads the kernarg pointer through `s[6:7]` first,
  before the emitter's VCC operand (the same register pair) can clobber it - and
  a wrong kernarg pointer faults there rather than surfacing later as a wrong
  value.
* **The declared register counts.**  The kernel descriptor declares the wave's
  SGPR/VGPR allocation, and a program that uses a register outside its own
  declaration is undefined.  The emitter therefore reports its own high-water
  mark (`sgpr_high_water` / `vgpr_high_water`) and the submit path declares that
  (raised to 14 SGPRs so the allocation always covers `s11-s13`).

### Safety: the preflight is not optional

The CPIPE user queue has **no BO validation list**, so a stream the firmware
rejects does not return an error - it wedges the pipe and costs a platform power
cycle (measured twice: "the CP latch needs platform power (S3/reboot)").  Every
compute submission therefore runs `drm::compute_selfcheck` first, which rebuilds
the MCS stream and the kernel descriptor for two byte-exact vendor references
(the live 1x1x1 `add.cl` dispatch and the CTS 2x2x4-groups/1x4x2-local geometry)
and refuses to submit on any difference:

    lgpu-gpu csverify        # host-only: no device is opened

`LGPU_CS_NO_PREFLIGHT=1` bypasses it, and exists only to demonstrate that the
mistake is not caught later.

A submission that does not come back clean prints everything needed to reason
about it without another power cycle: the signal value, every VA and geometry
field, and (with `LGPU_DUMP_WEDGE=1`) the MCS stream, the descriptor and the
kernarg block.  A queue whose teardown fails is deliberately leaked: releasing a
ring the firmware may still own would let CPIPE dereference a recycled address.

### Bring-up order and coverage

**Run `lgpu-gpu csprobe` first.**  It dispatches the one program whose job is the
launch rather than an instruction, and whose semantics are eight register reads:
the kernarg block's first user argument and the group ids are checked
deterministically, the local ids are reported as an observation (every lane
stores to the same offset, so which lane's value comes back is not established).

    lgpu-gpu csverify                 # host preflight
    lgpu-gpu csprobe                  # one dispatch: launch state + signal
    python3 run_gpu.py --pipe cs --mnemonic v_add_u32
    python3 run_gpu.py --pipe cs      # the whole universe

`--pipe cs` executes **685 of the 768 mnemonics**; the other 83 are reported as
`SKIP-ENVELOPE` with the reason rather than assembled into a stream, in two
groups:

* **81 T#/S# rows** - `load_v_tex*`, `store_v_tex*`, `atomic_v_tex*`, `samp*`,
  `gather*`, `getlod`, `getresinfo`.  The shapes are right and the carrier's
  tuple assignment satisfies every alignment the driver's rows demand (T#
  `RK9x8`, 8-aligned; S# `RA8x4`, 4-aligned); what a compute dispatch cannot
  supply is the *image* the T# describes.  Graphics hydrates it from the
  packet's resource root, and the atomic row declares an R32_UINT image with a
  2D address while `dmask:0xf` spans four texels - so the addressed texel needs
  four texels of declared extent behind it, which a fetch clamps and an atomic
  does not.  Rather than guess that geometry, the family runs on the graphics
  side only.
* **2 `copy_sm_*` rows** - LDS-to-global and global-to-LDS transfers.  The
  closed driver's own `TODO.md` (row C15) keeps these fail-closed: *"no
  source-level Vulkan operation or launch contract for them ... do not add an
  unconnected raw opcode selector."*  A real carrier needs two bindings this one
  does not make: the workgroup shared size (the kernel descriptor's
  `shared_segment_size` plus the resource word's granulated-shared field) and the
  launch-bound global region the copy names.

The graphics transport executes all 768.

### Launch and memory path vs loongvk (Axis 1)

The compute carrier is checked against the open driver's own compiled-kernel
path (`src/mesa/src/loonggpu/vulkan/loongvk_kd.h`, `loongvk_mcs.c`, and
`src/mesa/src/loonggpu/compiler/`), not only against itself.

| fact | loongvk | this carrier | verdict |
|---|---|---|---|
| kernarg pointer | `.kd` flag `en_sgpr_kernarg_ptr` (bit 5) makes the hardware load the kernarg VA into `s[6:7]`; the compiler addresses every user argument from it (`lco_select_nir_compute.c`: "addressed from the KERNARG POINTER in s[6:7], NOT from a constant root") | `make_cs_dispatch` sets `kd_flags = LOONGVK_KD_FLAGS` (`0x0fac`, bit 5 set) and `kernarg_address = kCsKernargVa`; the MCS stream is built by the vendored `loongvk_mcs_build_dispatch` | **aligned** |
| kernel/group SGPRs | `kernel_sgpr_cnt=11` and group ids at `s11:s13` (`LOONGVK_KD_KERNEL_SGPR_CNT`, `LCO_ABI_CS_GROUP_ID_SGPR`) | the same fields in `rsrc1` (now `0x08002076`; the denorm field differs on purpose, next row) | **aligned** |
| FP denorm mode | graphics flushes denormals natively and has no field for it; compute selects the mode through `rsrc1` bits 14:21, and the vendor capture `0x083C2076` is `FLUSH_NONE` - `loongvk_mcs.c` records that as "why every fp32/fp64 denorm case failed" | `make_cs_dispatch` inherited the capture (`FLOAT_MODE_DEFAULT`); it now sets `FLUSH_SOURCE_OUTPUT`, so `rsrc1 = 0x08002076` and compute flushes like graphics and the u-l-i-g oracle | **deviation fixed 2026-10-12** |
| image/sampler tuple | loaded from the kernarg block (`s[6:7] + field offset`) | the same: `emit_tex_prelude` scalar-flat loads through `s[6:7]` with `env.rsd_load_off` (`kRsdTable` on compute, 0 on graphics) | **aligned** |
| result/staging buffer addresses | derived at runtime from `s[6:7]`; the BO VAs are the driver's choice | assembled as literal VAs (`carrier::kCsArgVa` in `emit_store_desc` / `emit_load_desc` / `emit_result_store`) | **deviation** |
| flat base | the flat access names a base pair the descriptor supplies | names `s[20:21] = 0` with an absolute vaddr pair | **deviation** |
| output pointer | the host writes the output BO VA as the first user argument at `kernarg+0`; the kernel loads it from `s[6:7]` | the host writes the same word (`drm.cpp`), and `build_generic` reads it once as launch-state evidence, then ignores it | **half-aligned**: produced, not consumed |

The two deviations are safe **only because** the carrier and the host share
`carrier_map.hpp`: `drm.cpp` defines its BO VAs as `carrier::kCs*`, so an
assembled program cannot store to an address the host mapped elsewhere.  They
are not free.  The assembled program is not what the closed compiler emits, so
it cannot exercise a kernarg-relative addressing bug, and any future change that
gives the driver a free choice of BO VA would turn every literal into a fault.
Aligning them means deriving the output VA from `s[6:7]` and re-mapping the BO
at a driver-chosen address - a change to the launch contract that has to be
verified on the device, so it is recorded as a deviation rather than changed
blind.

### Per-lane instructions and the lane sweep

The atomics apply the operation **once per executing lane**, so the addressed
cell's final content is a function of the launch's lane count: with a uniform
operand it is `initial <op> operand` applied N times.  The u-l-i-g vectors state
*one* lane's view (`dst[0] = old`, `dst[1] = old <op> src`), which is the
`N == 1` case, so no expected value can be computed without knowing how many
lanes the dispatch runs.

That number is a property of the launch, so it lives in `CarrierEnv::lanes`: the
graphics fragment launch executes 16 (`kGraphicsLanes`), and a compute dispatch
executes the workgroup it was handed, i.e. the product of `cs_group()` - the same
value the launcher hands the stream, so the expectation and the launch cannot
drift apart.

A single lane count is a single point on that curve, and what it hides is real.
`--lane-sweep` re-runs the per-lane families at several counts and requires every
point to hold:

    python3 run_gpu.py --pipe cs --lane-sweep        # 1..8, 16, 32
    python3 run_gpu.py --pipe cs --lane-sweep 32     # every count 1..32

The sweep is also what verifies the *launch*: under the documented per-lane
semantics the cell value measures the executed lane count.  Measured for
`atomic_sm_add_u32` (initial `0x5`, operand `0x3`) over every count 1..32:
`5 + 3N`, zero residual - and identical across repeats at N=32, so the 32-way
read-modify-write on one cell is serialized rather than racing.  `--lane-sweep`
is compute-only, because the graphics launch's lane count is not host settable.

### Accumulator destinations and `v_dp4a_*`'s hidden source

`v_mac*` reads its destination as an addend, so the generic emitter stages it
from the case's third input; `v_mma_*` (119/119) and `v_mac_imm_f32` (1/1)
carry their addend their own way.

`v_dp4a_*` does **not**.  Its page says "the destination's previous value is
not an accumulate input ... it produces the product sum alone", and zeroing the
destination changed no row on hardware.  What it *does* read is a third source
the driver row never names.  The row is `GJ11 GK11 VD8`, but the encoding also
carries `LO[23:16]`, which the driver's own mask marks *print-ignored*, and the
hardware adds the scalar register that field selects; template 0 selects `s0`.
The compute prelude fills `s0` with the result-store descriptor base
(`0x000b0000`), so every case came back as `expected + 0x000b0000`, while the
same program on graphics - where the packet hydrates `s0` to `0x00000000` -
passed.  The flat result store does not read `s0`, so the carrier zeroes it
before the instruction on compute.  Measured after the fix: all four forms
39/39 on the compute pipe.  The durable fix is to model `LO[23:16]` in the
catalog as the third source it is; until then this is a carrier-side correction
of a launch dependency the row does not state.

### `v_cmp*`: the destination is a lane mask, not a boolean word

The compare family's oracle value is a **lane mask**: one bit per lane, set for
the lanes whose comparison held.  The oracle spells it `0x0000ffff` because
u-l-i-g's runner launches 16 lanes, so "every lane true" is 16 bits; a compute
dispatch launches the workgroup it was handed, so the same "every active lane
true" is `(1 << lanes) - 1` - `0xf` at the default 4-lane group.  Modelling
that is `is_lane_mask_family` in `envelope.cpp`: it maps the one all-ones
oracle value to the N-lane mask and leaves every other expected value alone.

Evidence, all host-side:

* The closed driver states the model directly.  `lco_register_allocation.c`
  keeps compare results in the ordinary SGPR window - *not* the `0x7c-0x7f`
  predicate space - and records that "the S5 probes read them back from s8";
  reading s8 is what this carrier already does.  `lco_spill.c` records that
  "the hardware evaluates a compare under the current exec mask".
* The saved CS matrix shows the pattern exactly: every `v_cmp*` row whose
  oracle mask is non-zero FAILs and every zero-mask row PASSes.  `v_cmp_f_f32`
  (all-zero) passes 102/102 while `v_cmp_tru_f32` (all-ones) fails 0/102.
  Across the family the pass count equals the number of zero expectations: for
  `v_cmpclass_f32` the 8 `.cpp` cases add 4 zeros to the registry's 90, and
  the measured pass is 94.
* The SCC-compare family does *not* show it: `s_cmp*` all PASS on compute,
  because it materialises `0xffff` with an explicit `s_select` instead of
  reading the mask register.  So the comparison logic is right and the mask
  width is what differs.

`LGPU_DUMP_EXPECT=1` prints the computed expectation, so the change is
checkable without a device:

    LGPU_DUMP_EXPECT=1 LGPU_CS_GROUP=4 ./build/lgpu-gpu csrun v_cmp_tru_f32 --dry-run
    # case <id> 0x0000000f

The change is expectation-only: the emitted stream is byte-identical before and
after (a `--dry` diff is empty), the graphics 16-lane path is unchanged, and a
wrong width leaves a row FAILing rather than manufacturing a PASS.

**HW-confirmed 2026-10-12.**  `v_cmp_tru_f32` passes 102/102 at lane counts
1, 2, 4, 8 and 16 on the compute pipe, i.e. every lane read back as all-true is
exactly `(1 << N) - 1`.  The other variants read the same mask register
through the same emitter, and the sweep probes remain in `--lane-sweep` so a
later encoding change that moves the mask is caught rather than assumed.

### The compute dispatch had the wrong FP denorm mode (2026-10-12)

Denormal behaviour is **per-dispatch on compute and fixed on graphics**.
`loongvk_kd.h` documents four 2-bit fields in `rsrc1` bits 14:21
(round32, round16_64, denorm32, denorm16_64) and states that the compute pipe
"cannot get DenormFlushToZero any other way - the mode is per-dispatch, and the
graphics pipe, which has no such field, flushes natively".  The vendor's own
`loongvk_mcs.c` adds that the captured kernel word `0x083C2076` carries
`FLUSH_NONE` for both widths - "that is why every fp32/fp64 denorm case
failed".

This carrier had inherited the captured word: `make_cs_dispatch` set
`float_mode = LOONGVK_KD_FLOAT_MODE_DEFAULT` (NO_FLUSH), so the compute pipe
kept denormals while the graphics pipe - and therefore the u-l-i-g oracle,
whose expectations are graphics-observed - flushed them.  The same instruction
disagreed with itself between the two pipes:

| probe on the compute pipe | before | after |
|---|---|---|
| `v_med3_f32` gen13458 (expected 0x00000001) | `0x00000000` | `0x00000001` |
| `v_add_f32` add(0, 0x00000001) | `0x00000000` | `0x00000001` |
| `v_fma_f32` fma(0.5, 0x00800000, 0) | `0x00000000` | `0x00400000` |
| `v_frexp_mant_f64` of a denormal input | `0x00000000` | the significand |

The fix is one mode word, not one oracle: `float_mode` is now
`FLUSH_SOURCE_OUTPUT` (both denorm fields 0), which makes `rsrc1`
`0x08002076`.  `csverify` still requires the stream to be byte-identical to
the capture except those four fields - `with_carrier_float_mode` rewrites them
in a copy of the reference, so any other drift is still caught.  The u-l-i-g
oracle, its hand-written vectors and `generated_cases.tsv` are **unchanged**.

Measured after the fix, compute pipe, against that unchanged oracle:
`v_med3_f32` / `v_min3_f32` / `v_max3_f32` 312/312, `v_add_f32` 111/111,
`v_mul_f32` 107/107, `v_sub_f32` 105/105, `v_fma_f32` / `v_mac_f32`
312/312, `v_fma_legacy_f32` 311/311, `v_cubet_f32` 334/334,
`v_frexp_mant_f64` 101/101.  One mode bit explains the whole float cluster
that the previous section had misclassified as an oracle-policy gap.

### Compute timestamps (phase-2 enabler, 2026-10-12)

The vendor's OpenCL profiling gets START/END for compute dispatches from the
firmware queue, and the signal allocation is already sized for a firmware-
written block (`kCsSignalBytes`), so the offsets only had to be located rather
than guessed.  `LGPU_CS_DUMP_SIGNAL=1 lgpu-gpu csprobe` dumps the non-zero
rows of the 4 KiB signal BO; two runs show **four 64-bit counters at signal
+0x20, +0x28, +0x30 and +0x38**.  All four advance with wall-clock between runs
and share a high dword, so they are the dispatch's timestamp block, not data;
the completion counter at +0x00 reads 0 because the host waits on the signal
word.

Which pair is the wave START/END is deliberately **not** pinned yet: all four
deltas are non-zero even for the ~100 us launch probe, and only a program whose
execution time can be varied (the K-copy bench template) separates "enqueue /
wave start / wave end / retire" from "one dispatch".  That is the first step of
the compute microbench.

### CS microbench (2026-10-12)

**Deliverable record: [`MICROBENCH.md`](MICROBENCH.md)** - the phase-2 document
(generation/layer/source, method, calibration, artifacts, coverage, scope,
unknowns, UNRESOLVED) in one place.

`lgpu-gpu csbench <m> lat|tp <copies> <reps>` and `csbench-batch` (stdin, one
`mnemonic kind copies reps` per line) measure a mnemonic on the compute pipe
through the *same* generic emitter the validation matrix uses -
`envelope::build_cs_bench` calls `CarrierEnv::compute` and `materializable`,
so a benchmark cannot exercise a code path the CS sweep did not validate.
`lat` binds every source to the previous destination (one dependency chain, so
the time is latency x copies); `tp` rotates eight independent source registers
so consecutive copies do not depend on each other (throughput).

**The timestamp window.** The K sweep on `v_add_f32` fixed it.  With a chain,
`+0x28` and `+0x38` both grow with copies while `+0x30` stays at a fixed
2081 after `+0x20`, and the ordering is always `+0x20` (enqueue) <=
`+0x30` (wave start) <= `+0x38` (wave end) <= `+0x28` (retire).  The
window is therefore `+0x38 - +0x30`; the fixed `+0x28 - +0x38` retire cost is
not part of it:

| copies | lat win (ticks) | tp win (ticks) |
|---|---|---|
| 1 | 1095 | 1095 |
| 16 | 1517 | 1517 |
| 256 | 3634 | 2132 |
| 1024 | 10838 | 4581 |

**One fixed overhead, removed.** Even copies=1 spends ~1095 ticks on the launch
and the store, so `win/copies` is not the instruction cost at small K.
`run_bench.py` therefore runs two sizes and reports the **marginal** cost
`(win(K=1024) - win(K=256)) / 768`, which for `v_add_f32` is about 9.4
ticks/op chained and about 3.0 ticks/op independent - both the latency and the
throughput number.

**Safety envelope.** `materializable` rejects `ScalarQuad8` /
`OffsetSource8` / `Immediate12`, so every descriptor- and offset-taking
(memory) family is refused before a program is built, and the only memory
operation any bench stream contains is the final flat store to the output BO at
`kCsArgVa` (mapped, in bounds).  `v_movindex_b32` is refused explicitly by
name because the CS carrier itself refuses it (the register-window shape wedged
the pipe once).  `run_bench.py` runs 64 mnemonics per process and every
submission in that process shares one session, so the sweep does not churn
create/destroy cycles.

**The CPIPE wedge: root cause and fix (2026-10-12).**  Two full sweeps faulted
at `0x100020000` - the compute **user-queue ring / index block** - and the
second left the gfx / xdma0 / bpipe rings parked, needing a cold boot.  The
three layers, cross-compared against the reference driver:

* **libdrm** (`loonggpu_userq.c`) is a correct pass-through: CREATE takes
  `(ip_type, queue_va, queue_size)`, FREE takes `(queue_id, ip_type)`, and
  DOORBELL rejects a byte write pointer that is not 64-byte aligned.
* **loonggpu-dkms** is built around **one long-lived queue per client**.
  `loonggpu_drm_userq.c` CREATE rejects a second concurrent queue on the same
  PASID ("the LG200 userq teardown packet is filtered by PASID, not by queue
  id"); FREE runs the fence-backed async `UNMAP_QUEUE` and only then unpins
  the ring, quarantining on failure; the doorbell flushes **all** VMIDs when the
  page tables changed (K39).  Its own comments name the failure shape: "a DRM
  queue executed in vmid 4 with nothing mapped at its ring address" (the K16
  wedge).
* **mesa (loongvk)** follows that contract: `struct loongvk_cs_queue` is created
  once per logical device and destroyed once (`cs_queue_create` /
  `cs_queue_destroy`), with a persistent ring and a `slot` advanced by
  `loongvk_mcs_next_offset` and wrapped by `loongvk_mcs_ring_offset`; the
  published write pointer is `slot + mcs_len`.
* **this carrier did not**: it called `loonggpu_userq_create` and
  `loonggpu_userq_free` **once per submission**, each with a fresh ring BO at
  the same VA.  A sweep therefore churned the ring's VA map/unmap, the
  PASID-filtered UNMAP_QUEUE and the all-VMID TLB flush thousands of times, and
  opened a window in which the firmware reads the index block after the
  frontend dropped the mapping.  That is the fault at `0x100020000`.

The fix is that a persistent session owns **the whole dispatch buffer set**, not
just the ring.  `CachedSession` holds the ring, text, kernel descriptor,
kernarg, output, signal and trampoline BOs plus the queue id and the ring slot;
each is allocated once at its fixed VA and rewritten in place per submission, so
CPIPE never sees a fixed VA whose mapping was dropped and re-created underneath
it - the class behind both measured faults (`0x100020000`, then the text-region
`0x100084000`).  The cached output BO is cleared every submission so a program
that does not write every offset cannot be judged against stale bytes.  The
queue is created on the first persistent submission and freed **once** by
`close_cached_session()` (drain, free, close); a failed submission retires the
session's queue rather than reusing it, and a failed teardown leaks deliberately.

**The write pointer is never wrapped.**  A first attempt wrapped `slot` back to
0 when the ring was full; measured, that is refused (`wptr 384` with
`rptr 65472` -> "userq_doorbell failed (refused, not dispatched)") and the
wrong-offset parse then faulted instruction fetch at `0x100084000`.  A full ring
now **recycles the queue** instead: drain, fence-backed free, zero the index
block, create a fresh queue (its MQD starts at rptr 0), begin at slot 0.  That is
one create/destroy per ~341 streams rather than one per submission, and every
published pointer stays forward.  A host-side simulation of the recycled
arithmetic (padded 192/256/1024/4096, 5,000 streams each) shows every pointer is
64-aligned and strictly inside the ring, with recycles at the expected
boundaries.

**Verification plan after the reboot.**  Ascending steps, each with `dmesg`
checked before the next; stop at the first `GPU fault not resolved` /
`quarantined` / `parked` line and report:

1. host `lgpu-gpu csverify`, then `lgpu-gpu cspersist 1` - one submission, one
   queue create, one free;
2. `cspersist 10`, then `cspersist 100` - the same queue and ring, printing
   the signal and timestamp window each time; `dmesg` must show ONE
   `userq: created` line for the whole run, not one per submission;
3. **`cspersist 350`** - crosses the ring-full boundary at stream 341, so it is
   the only step that exercises the queue **recycle**;
4. `cspersist 1000` - several recycles; then `run_gpu.py --pipe cs --mnemonic
   v_add_f32`, then the full `run_bench.py`.

`cspersist N` reuses one persistent session, one queue and one ring for all N
submissions and calls `close_cached_session()` at the end, so a single command
exercises exactly the lifetime the fix changed.  Expect
`userq: created compute queue` in `dmesg` once per ~341 submissions (one per
recycle), not once per submission.

The file was re-measured after the fix (2026-10-12, fresh boot): **497 OK /
271 SKIP / 0 NO-DATA / 0 FAULT of 768**, with **0** GPU faults in `dmesg` and the
device still usable afterwards (`cspersist 1` passes, and a full
`run_gpu.py --pipe cs` matrix gave 683 PASS / 1 FAIL / 84 SKIP, identical to the
pre-fix matrix).  The two void runs it replaces produced 233 OK / 250 NO-DATA /
17 FAULT and wedged CPIPE; the old file is kept at `/tmp/cs_bench_OLD_void.tsv`
for comparison only.  The UNRESOLVED tick unit below still applies:
`lat_ticks_per_op`/`tp_ticks_per_op` are raw ticks.

**Tick unit: calibrated, no longer UNRESOLVED (2026-10-12).**  The window is
counted in ticks of the firmware's free-running timestamp counter, and two
single submissions separated by a known host sleep give that counter's rate
directly - `python3 calibrate_ticks.py [seconds]` does exactly that:

| sample | host seconds | tick delta | ns/tick |
|---|---|---|---|
| 30 s | 30.00 | 22,545,662,594 | 1.3306 |
| 10 s | 10.045 | 7,531,374,567 | 1.3337 |

So **1 tick = 1.333 ns (~750 MHz)**, reproducible to 0.3 %.  This also resolves
the earlier contradiction with the PS layer's "~32 ns/tick": at 1.333 ns/tick the
dependent `v_add_f32` above (9.4 ticks) is **12.5 ns**, an ordinary FP32 add
latency, so the 32 ns/tick figure does not describe this counter.  The CSV keeps
raw ticks - multiply by 1.333e-9 for seconds.

**Scope and unknowns of this CSV.**  Scope: all 768 mnemonics through the CS
generic envelope, `K_SMALL=256`/`K_LARGE=2048`, `REPS=1`, marginal
`(win(K_LARGE)-win(K_SMALL))/(K_LARGE-K_SMALL)`; latency = one dependency
chain, throughput = eight rotating sources.

**The 271 SKIP rows, by family, with the exact reason** (`build_cs_bench` needs
a class assignment from the codec's decodable variants; these families have none,
so it stops at "no canonical variant" before `materializable` is reached):

| family | rows | why it is not benchable here |
|---|---|---|
| buffer memory (descriptor) | 61 | needs a hydrated buffer descriptor |
| sampler / gather | 56 | needs a texture + sampler descriptor |
| shared memory (LDS) | 46 | `load_sm`/`store_sm`/`atomic_sm`/`copy_sm` need an LDS window |
| flat memory | 45 | needs a flat address operand; the correctness path builds it from the hardcoded `build_flat` template, not the codec |
| texture memory | 23 | needs a texture descriptor |
| control / flow / system | 22 | `nop`/`exit`/`check`/`barrier`/`trap`/`rfe`/`b`/`jirl`/\`jpop`/\`jpush`/\`flag`/\`get*`/\`prefix_*`/\`v_emit*` - no cost to chain |
| SIO | 17 | `sioread`/`siowrite`/`siordrot`/`siowrrot` are outside the codec's variants |
| `v_movindex_b32` | 1 | refused by name (its register window wedged CPIPE) |

Closing these needs a **per-family bench template** (the correctness carrier
already has one per family: `build_buff`, `build_lds`, `build_flat`,
`build_tex`, `build_samp`, `build_control`) extended to K copies, not a
change to the generic path.

**The gap is machine-readable: `build/cs_bench_coverage.tsv`** (regenerate with
`python3 bench_coverage.py`), which joins this CSV with
`build/gpu_cs_generic_results.tsv` so the 271 SKIPs split into two very different
kinds:

| split | rows | meaning |
|---|---|---|
| SKIP **but correctness PASS** | **187** | the CS carrier builds and passes them; only the bench has no template |
| SKIP **and correctness SKIP** | **84** | the carrier itself refuses them (descriptor/sampler/texture families) |

Of the 187, twenty are control/flow ("no cost to chain"), so the real remaining
bench work is **~167 rows**: buffer memory 61, flat memory 45, shared memory 44,
SIO 17.  Those 84 carrier-limited rows are not a bench gap at all - they are the
same carrier limitation the correctness matrix already records.

**`v_mbcnt_u32` on CS: root cause localised.**  It fails the CS correctness
matrix (30 PASS / 11 FAIL of 41; PS is 41/41), so its row is not a valid timing.
u-l-i-g's documented oracle (`oracle_mbcnt_u32`: build a mask from
`popcount(a[14:0]) + popcount(b[14:0])`, extend it by one bit when `b` has the
mask's MSB set, then OR into `b`) reproduces **every expected value**.  The CS
**observed** values are exactly what that oracle returns when `a` is a *small*
value (`0x1`...) rather than the case's `src0` - e.g. `pc8-b8`
(`a=0xff, b=0x8`, expect `0x1f`) was observed as `0x0b`, which is the
oracle's answer for `a=0x1`.  So the CS stream executes this instruction with
the wrong **popcount source**, not with wrong semantics; cases whose answer is
insensitive to `a`'s exact popcount (`masked`) pass by coincidence.

**It is exec-mask dependent, and the sweep proves it.**  Running the same case
set at more lane counts shows the contributed mask tracking the *executed lane
count*, not `a`:

| lanes | PASS/FAIL | `pc8-b8` (a=0xff, b=0x8) observed | mask contributed |
|---|---|---|---|
| 1 | 17/24 | 0x08 | 0 |
| 2 | 28/13 | 0x09 | 1 |
| 3 | 30/11 | 0x0b | 3 |
| 4 | 30/11 | 0x0b | 3 |
| 8 | 30/11 | 0x0f | 7 |
| 16 | 25/16 | 0x10 | 0x10 |

`no-cover` (a=0xff, b=0x100) shows the **identical** 0/1/3/3/7/8 series, and
PS (16 lanes) matches the oracle 41/41.  So `v_mbcnt_u32` reads the executing
lane mask as part of its input - the same family behaviour U5 documents for
`v_cmp` (the compare/lane-mask pool) - and the oracle models only the 16-lane
reference launch.  **The CS 4-lane carrier therefore differs legitimately; this is
a launch-shape dependence, not a miscompile.**  It cannot be fixed by the
`v_cmp` value remap (the dependence is a whole mask ladder, not a bit width),
so the row stays a known CS-matrix exception rather than being tuned away.

**Other unknowns.**  The number is the marginal cost of the *generic emitter's*
program, not of a hand-optimised issue pattern.  `atomic_*` and the
memory families use the observe-mode correctness carrier, so their CS semantics
are "ran and was observed", not bit-compared.

### PS hardware result (2026-10-12)

The full graphics sweep runs **768 / 768 mnemonics PASS**, 19,066 compared case
passes over 20,729 rows, with **zero** failures and zero fault lines in dmesg.
The 1,663 rows that are neither pass nor fail belong to the 137 observe-mode
mnemonics (`atomic_v_flat_*`, the texture loads/samples and friends), where the
carrier records what it read instead of comparing - the u-l-i-g pages mark those
as observations, so a PASS there means "the program completed", not "the value
matched".  The matrix this replaces was all `loonggpu_bo_va_op MAP failed` - a
harness failure, not instruction results - so this is the first clean PS
baseline.

### CS hardware result (2026-10-12)

The full compute sweep after the structural fixes (the `v_cmp*` lane-mask model,
`v_dp4a_*`'s hidden source, the SIO identifier operand) and the **FP
denorm-mode fix** runs **683 PASS / 1 FAIL / 84 SKIP**, 20,470 case passes over
20,729 rows, with zero fault lines in dmesg.  From the pre-fix matrix of
431 / 171 / 84, **252 mnemonics now pass** and the entire 171-row float cluster
is gone.

The single remaining FAIL is `v_mbcnt_u32` (11 rows).  It is **not** an
oracle-policy gap, and not the `v_dp4a_*` hidden source either: the same 41
vectors pass 41/41 on the graphics pipe, the emitted instruction word and the
staged registers are **byte-identical** on the two pipes (`v_mbcnt_u32 p0, v10,
v0, v1` with v0=0xf, v1=0xff; `lo=0x0a000100 hi=0x1019c009` on both), and
zeroing the s0 that fixed `v_dp4a_*` changes no row.  Compute reads `0x103`
where graphics reads `0x1ff` for that case, at every lane count (1: 17/41,
4: 30/41, 16: 25/41, 32: 16/41), so the implicit input is something else - most
plausibly exec/lane state - and it is kept open rather than guessed at.

Every structural family passes: all 105 `v_cmp*` rows, all four `v_dp4a_*`
forms (39/39 each), every SIO identifier row, the whole `vop3_vec3` three-source
family, and every floating-point family the previous round had misattributed to
the oracle - all against the **unchanged** u-l-i-g oracle.  A FAIL count is not a
defect count, and one launch-state word explains the whole float cluster.

### `atomic_sm_rsub_u32/u64`: a measured no-write (`LOONGGPU_DELTA`)

u-l-i-g documents these two forms as `src - old`.  The hardware does not write
the cell at all.  The differential is exact: the carrier emits a word
byte-identical to `atomic_sm_sub_u32`'s except for the opcode field
(`lo=0x0d08000b`, the same operands; `hi=0x20340000` for rsub against
`0x200c0000` for sub), and `sub` writes while `hi[26:18] = 0x00d` does not.
Measured on the compute pipe at lane counts 1,2,3,4,5 for the u32 rows and 1,2
for the u64 rows, the cell holds its initial value at every count - including
count 1, where one `src - old` application would already differ.  The closed
driver's own derivation string for these rows is
`...ps-lds-atomic-no-write-both-hi30-values...`, so its encoding was derived
from the same no-write observation.

Modelling the documented `a - old` is worse than merely wrong: it oscillates
with period two (`v1 = a - old`, `v2 = a - v1 = old`), so it *agrees* with
"unchanged" at every even lane count - a false PASS at N=2,4,8,... and a failure
at odd counts.  The model compares against the measured no-write invariant
instead, which is falsifiable: it fails loudly if a later encoding or part does
write.

### One queue per submission: the churn hazard

`submit_compute` opens a DRM file and creates a user queue **per call**, and
`run_gpu.py` uses one process per (mnemonic, chunk), so a full sweep is several
hundred create/teardown cycles at the same queue VA and VMID:

    [drm] vmid: reserved DRM vmid 1 for pasid 0x800d
    [drm] userq: created compute queue id=0 pasid=0x800d va=0x100020040 size=0x10000

After ~330 of them a teardown failed to unmap the queue's VM.  The kernel then
reported `compute queue unmap failure` / `retaining DRM VM`, the command
processor faulted at the *ring* address (`addr 0x100020000`, the queue's index
block - no carrier addresses that), the CPIPE was quarantined, and the recovery
path could not restore the GFX ring, so the device needed a power cycle.  This
is a driver lifecycle defect rather than a carrier one, but it is what makes
long sweeps expensive; cutting the number of sessions per run is the mitigation
that does not wait on a driver fix.

## Static safety audit

Four defect classes have each wedged this GPU (or corrupted a result) and cost a
platform power cycle, and all four are visible in the emitted program - so they
are checked before a submission, not after:

    python3 run_gpu.py --dry && python3 audit.py ps         # PS listings
    python3 run_gpu.py --dry --pipe cs && python3 audit.py cs

Both the per-mnemonic and the `--batch` paths write
`build/gpu-run-logs/<mnemonic>.log` for every mnemonic they run.
`csrun-batch --dry-run` emits each block's listing wrapped in `BATCH-BLOCK`
markers that `run_gpu.py` splits back out, so a batch dry run regenerates
exactly the logs the audit reads instead of leaving the previous run's programs
in place.  That was the stale-log trap: a batch run wrote no per-mnemonic log at
all, and `audit.py` then audited the last non-batch listings and reported their
refusal counts.  `.retry.log` files are excluded - the mnemonic's own log
already carries the retried listing.

* **control flow** - a linear carrier may contain only transfers whose target it
  controls (`b` with zero displacement, which the ISA reference documents as
  continuing at the next instruction).  A `jirl` whose source pair the carrier
  never initialised fetched at address `0x8` and quarantined the CPIPE; the
  control instructions whose behaviour the ISA reference leaves `UNRESOLVED`
  (`jirl`, `jpush`, `jpop`, `trap`, `rfe`) are now refused rather than run.
* **address registers** - a memory operand is added to a descriptor base, so a
  register holding case data puts the access at `base + data + imm`.  Both
  address faults were exactly that (`kArgVa + 0x12345678 + imm`), and both came
  from a register that a *later* instruction in the same program overwrote.
  Multi-register destinations such as `v[14:15]` count.
* **window** - an immediate naming a VA must name the carrier's own argument BO.
* **operand roles** - an emitted instruction must cover the roles its driver row
  prints.  The check reads the row's canonical seed assembly from
  `code/lg200-instructions.tsv` and requires the emitted operand list to
  supply at least the predicate/register/immediate roles the seed prints
  (modifiers and the `(scc)` write marker excluded; a register field may print
  `zero` or a vector, so both collapse to one role).  Measured:
  `store_v_buff_b16` was emitted with the 8-bit store's five-operand shape
  although its row declares the address VGPR (`RA8 RK8x4 VQ8 VJ8 UO12`, six
  printed fields), so the descriptor landed in the VQ8 slot, the hardware
  addressed through it and faulted at `0x100160300`; it passed rules A-D
  because none of them looked at the operands.  The same check found the SIO
  identifier rows (`siordrot_id*`/`siowrrot_id*`): their class carries three
  vector sources (`VJ8` `VA8` `VK8`) and the emitter supplied two, so the
  encoder's vector-field omission rule zeroed `VK8` silently.  Both are fixed.

The check is a lower bound, and says so: a field the seed itself omits (buffer
atomics print no `VQ8` while the codec encodes it as the zero selector) is
outside it, and a swap between two same-class registers is invisible.  Rule F
also compares the tuple width the driver spells (`VJ8x4`) with the codec's
effective tuple and records a note where they differ; those notes are the
codec's width gaps (the flat scalar loads address a 2-register pair while the
shared `sbuff-load` class says 4) plus the deliberate overrides (gather reads 3
of its 8 coordinate slots).  Rows whose
seed disagrees with a role the catalog writes `UNRESOLVED` are reported as
notes, not failures - `jpush`/`jpop` print a third register where the codec
class models the 23-bit `X2_23` immediate, and neither reading may be guessed.

These checks are validated against injected defects, so a clean report means the
checks ran rather than that they cannot fail.

## Encoding cross-check against the vendor's own assembler

`code/lg200/gpu/enc_probe.py` encodes a probe set through this tree's codec and
through the vendor's shipped `llvm-mc -arch=lggpu -mcpu=lg200`, and diffs the
two.  This is the only available authority that is independent of both this
table and Mesa's: the two were synchronized from the same artifact, so
comparing them proves transcription and nothing else.  It runs CPU-side, so it
works when the GPU is wedged.

    make -C code/lg200/gpu enc-probe

A disagreement means one of the two encoders is wrong, and it is worth knowing
which before an instruction reaches hardware - the carriers emit scaffolding
around every case, so a wrong scaffolding encoding corrupts results silently.
