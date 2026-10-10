# Verification Tooling

This section ships with the machinery used to establish the behaviour documented
in these pages: a CPU oracle with deterministic case vectors, a hardware runner
that executes the same vectors on a chip, and a differential microbenchmark.

## Case vectors

Every machine row in the instruction table carries deterministic case vectors.
A vector names its inputs and its expected result, so the same vector can be run
twice — once against a CPU model of the semantics and once on hardware — and the
two results compared bit for bit.

The vectors are the reason the instruction pages can state boundary behaviour:
a row's description says what has been established for that row's exact variant,
and anything not covered by a vector is marked `UNRESOLVED` rather than
described.

## CPU oracle

```sh
make -C code/lg200 -j verify
```

This compiles one oracle per mnemonic together with the shared oracle library
and runs every vector, reporting a count of instructions and vectors.  It needs
no device and no driver.

Two conventions matter when reading an oracle:

* `<mnemonic>.h` is the executable reference pseudocode for the row.  It is
  included by the oracle and is written in the vocabulary of the oracle library
  (bit widths, floating-point policy, saturation helpers).
* `<mnemonic>.cpp` holds the oracle body and the case vectors.  Floating-point
  vectors carry an explicit policy (rounding and denormal handling) so that a
  vector states which mode it exercises.

Additional boundary vectors can be generated:

```sh
make -C code/lg200 gen-cases      # regenerate the boundary rows
```

Editing a single description cell is done through the table editor, which reads
and writes the whole table and leaves a snapshot:

```sh
python3 code/lg200/set_desc.py <mnemonic> <markdown-file>
python3 code/lg200/set_desc.py --show <mnemonic>
python3 code/lg200/set_desc.py --check
```

## Hardware runner

```sh
make -C code/lg200/gpu                  # build the runner
python3 code/lg200/gpu/run_gpu.py --mnemonic v_add_u32
python3 code/lg200/gpu/run_gpu.py       # the whole universe
python3 code/lg200/gpu/run_gpu.py --dry # assemble only, no submission
```

The runner assembles a row's vectors into a fragment program, submits once on
the graphics ring, reads the result buffer back and compares the words with the
expected values.  Each instruction runs in its own process with a fence timeout,
so a hang is contained to that case.

Rows whose behaviour depends on launch state the runner cannot stage — flat
memory, formatted buffer loads, matrix reads, texture fetch and sampling, and a
handful of others — execute in **observed** mode: the row records that it ran and
what was observed, instead of comparing against an independent oracle.  A page
that documents such a row states the observation and marks anything beyond it
`UNRESOLVED`.

Results land in the runner's build directory, one file for a full matrix run and
one for a single-instruction run, with a per-instruction log.

`run_gpu.py golden [--check]` regenerates or diffs the assembled listings of one
representative program per carrier shape; a change in the listing is then a
conscious update rather than an accident.

## Microbenchmark

```sh
python3 code/lg200/run_bench.py
python3 code/lg200/run_bench.py --only v_add_u32,s_add_i32
```

The benchmark builds one program containing K copies of a target instruction and
measures the wall-clock cost of one submission per K.  Two patterns are
measured: a dependency chain (latency) and independent register streams
(throughput).  The per-operation cost is the slope across copy counts, so the
fixed submission and readback overhead cancels.

Numbers are relative wall-clock nanoseconds, not architectural cycles: the
shader ISA exposes no shader-readable clock.  A slope that is not positive or
whose fit is poor is reported as unavailable rather than as a value, and the
rendered tables show such rows as "-".

## What the tooling does not cover

* Launch-state behaviour that the runner cannot stage; those rows are observed,
  not compared.
* Behaviour outside a row's exact variant: a vector pins one operand form, and a
  different form of the same mnemonic is a different question.
* Performance in architectural units; see the caveat above.
* Anything that requires a second command stream, a different pipe, or state
  the fragment-stage runner does not build.

## Compute (KCD) carrier - an open gap, and vendor-LLVM evidence (2026-09-29)

The fragment-stage PS carrier cannot prove the shared-memory matrix forms.  The
runner already classifies them as observation-only for exactly this reason
(`envelope.cpp`: `load_sm_matrix_*` and every `v_mma_*` are `launch_state`,
because "load_sm_matrix reads a persistent matrix file (cross-run state)" whose
content the PS carrier cannot stage).  Since the matrix forms read **shared
memory** - the `sm` prefix is the same one as `load_sm_b32` / `atomic_sm_add_u32`,
and `docs/gpu/isa/family-memory.md` lists them under "Shared memory" as "Matrix
forms that read one, two or four planes, whose plane and lane layout is
UNRESOLVED" - the launch state they need is a **workgroup LDS allocation**, which
only a compute (KCD) submission can build.  `drm.hpp` currently exposes
`submit_ps`, `submit_gs`, `submit_probe` and `submit_timestamp_probe`; there is
no compute path, so these rows stay UNRESOLVED by construction rather than by
measurement.

Vendor offline LLVM (LLVM 18.1.0rc, target `lggpu - Loongson GPUs`,
`analysis/lacm/examples/lacm-runtime/opt/lacm-0.1.0`) - two reproducible results:

* it **knows all eight MMA operations**, as builtins with these exact names:

      __builtin_lggpu_v_mma_323216_{i8,u8}_{i8,u8}_i32
      __builtin_lggpu_v_mma_8816_{i8,u8}_{i8,u8}_i32

  (evidence: `strings libLLVM-18rc.so | grep _mma_`), which independently
  confirms the eight `v_mma_*` mnemonics as real operations;
* its **assembler rejects the matrix mnemonics** as written - `llvm-mc -arch=lggpu
  -mcpu=lg200 -show-encoding` reports "invalid instruction" for
  `load_sm_matrix_x1 p0, v0, v0`, `x2`, `x4` and for
  `v_mma_8816_u8_u8_i32 v0, v1, v2, v3`.  So the vendor assembler is **not** an
  encoding source for these rows; the encodings in this repository's own ISA view
  (seed `inst-hi` 0x20c40000 / 0x20c80000 / 0x20cc0000 for x1/x2/x4) remain the
  only recorded ones, and they still need a hardware cross-check.


### Vendor intrinsic definitions (authoritative operand forms)

The same vendor tree ships the LGGPU target's **intrinsic definitions**, which pin
the operand forms the repository's pages leave open
(`lib/llvm/include/llvm/IR/IntrinsicsLGGPU.td`):

    class LGGPUMmaIntrinsic<LLVMType AB, LLVMType CD, string name>
        : Intrinsic<[CD], [AB, AB, CD],
                    [IntrNoMem, IntrConvergent, IntrWillReturn, ...]>;
    defm int_lggpu_v_mma_8816   : LGGPUMmaIntrinsic_i8i8i32<llvm_v4i8_ty,  llvm_v2i32_ty,  ...>;
    defm int_lggpu_v_mma_323216 : LGGPUMmaIntrinsic_i8i8i32<llvm_v16i8_ty, llvm_v32i32_ty, ...>;

That fixes **argument order (A, B, C) -> D** and each operation's **per-lane
fragment**: for `v_mma_8816_*` A and B are `v4i8` and the accumulator `v2i32`;
for `v_mma_323216_*` they are `v16i8` and `v32i32`.  Split over the 32-lane wave
this is exactly an 8x8x16 and a 32x32x16 integer matrix multiply (A 8x16 / 32x16,
B 16x8 / 16x32, C 8x8 / 32x32), which is the shape evidence the `v_mma_*` pages
could not state.  All eight forms are `IntrConvergent`, i.e. wave-wide.

The matrix loads are declared with an explicit addressing pair:

    class LGGPU_SM_LOAD_MATRIX : Intrinsic <[llvm_any_ty],
      [llvm_i32_ty, // addr
      llvm_i32_ty], // off0
      [IntrReadMem], "", [SDNPMemOperand]>;
    def int_lggpu_load_sm_matrix_x1 : LGGPU_SM_LOAD_MATRIX;   // ... x2, x4

So `load_sm_matrix_x*` reads **shared memory at (addr, off0)** and returns a
caller-typed vector tuple - it is an LDS read with a matrix layout, which is
consistent with the `sm` prefix and with `family-memory.md` listing it under
"Shared memory".  What remains genuinely open is only the **lane -> element
mapping** of that layout.

Related, and also newly visible: the shared<->global move is
`__builtin_lggpu_copy_sm_from_global` / `__builtin_lggpu_copy_sm_to_global`
(plus `_async`, all `IntrConvergent`), and `load_sm_swizzle_b64` /
`store_sm_swizzle_b64` are the permuting 64-bit pair.  There is still no matrix
store mnemonic, so a probe must populate the matrix through the ordinary LDS
stores.


### Vendor assembler encodings for the matrix rows (probe result, 2026-09-29)

The vendor assembler **does** know these rows - the earlier "invalid instruction"
was a spelling error: it uses dots, `v_mma.8816.u8.u8.i32` and
`load.sm.matrix.x1`, not the underscore forms.  It even volunteers the
correction (`load_sm.matrix.x1` -> "did you mean: load.sm.matrix.x1?").

    load.sm.matrix.x1 p0, v0, v0, 0x0    ; [00,00,00,00,00,00,c4,20]
    load.sm.matrix.x2 p0, v[0:1], v2, 0x0; [00,00,02,00,00,00,c8,20]
    load.sm.matrix.x4 p0, v[0:3], v4, 0x0; [00,00,04,00,00,00,cc,20]

    v_mma.8816.u8.u8.i32     pdst, <pair>, A, B, <pair>  ; [00,01,02,00,49,00,30,10]
    v_mma.8816.u8.i8.i32                                 ; [00,01,02,00,49,40,30,10]
    v_mma.8816.i8.u8.i32                                 ; [00,01,02,00,49,80,30,10]
    v_mma.8816.i8.i8.i32                                 ; [00,01,02,00,49,c0,30,10]
    v_mma.323216.u8.u8.i32                               ; [00,04,08,00,49,00,31,10]
    v_mma.323216.u8.i8.i32                               ; [00,04,08,00,49,40,31,10]
    v_mma.323216.i8.u8.i32                               ; [00,04,08,00,49,80,31,10]
    v_mma.323216.i8.i8.i32                               ; [00,04,08,00,49,c0,31,10]

Field placement read off the assembler: for the matrix loads, **byte 2** is the
address register and **byte 4** is `off0`; for the MMAs, **byte 5** is the
signedness pair (00 u8/u8, 40 u8/i8, 80 i8/u8, c0 i8/i8) and the shape is the
opcode (0x30 for 8816, 0x31 for 323216).  The per-lane fragments are 1/2/4 words
for x1/x2/x4 (`BuiltinsLGGPU.def`: `..._x1_f32 "fUiUi"`, `..._x2_v2f32 "V2fUiUi"`,
`..._x4_v4f32 "V4fUiUi"`) and A/B/C as recorded above.

### load.sm.matrix.x1 - MEASURED (2026-09-29, hardware)

Everything previously written about the matrix forms in this repository was a
guess.  This is a measurement, taken through the LoongVK Vulkan compute path (not
through the GFX-packet compute recipe, which was designed and never dispatched).  Method: a 32-lane compute shader writes
`sm[i] = 0x1000 + i`, barriers, reads one word back through the ordinary shared
load, and the compiled binary's single load instruction (`load_sm_b32`) is
replaced in place by `load.sm.matrix.x1` with the same address and destination
registers and `off0 = 0` (the operand fields coincide: byte 2 = address register,
byte 3 = destination).

    per-lane address word = L   -> returned word = 0 1 2 3 1 2 3 4 2 3 4 5 3 4 5 6 ...
    per-lane address word = L+8 -> returned word = 8 9 10 11 9 10 11 12 10 11 12 13 ...

**Correction (round 21):** the pattern also depends on the per-lane address
*stride*.  A third probe, `sm2[lid]` with `sm2` a `uvec2[]` (address word = 2L),
returns `0 1 2 3 2 3 4 5 ...`, which the stride-1 form does not predict.  The model
over all three probes (96 points) is

    word = base + (lane & 3) + s * (lane >> 2)

where `s` is the per-lane address stride in words (1, 1 and 2 in the three probes
above) and `base` the constant part of the address - i.e. element `lane` of a
4-column matrix whose row pitch is the caller's stride.  It is *not* a function of
each lane's own address (that model is refuted by the base-8 probe).  Sweeping
`off0` over {0,1,2,3,4,8,16} changes nothing: it is bit-identical every time, and
it is written to the low byte of the high dword as the encoding says.

**x2/x4 are not yet measured**, and the reason is now precise: the tuple harness works
mechanically (replace one of the four single-word shared loads, neutralise the other
three with the benign `check` instruction the binary already carries,
`0000000f 40320000`, and read the four consuming stores), but its readout is
**confounded** - a neutralised load leaves its destination holding its previous value,
so it cannot be told which registers the matrix instruction writes.  The scan does
establish, negatively, that the destination field is **not** a plain register start for
the values tested (only `dst=3`, and only for x1) and that the tuple forms do not
write the plain contiguous registers the operand syntax suggests.  A harness that
initialises the tracked registers to distinct sentinels after the loads is required
before any statement about the tuple encoding or the x2/x4 planes.

**Update (round 23) - the confound is cancelled and the written sets are measured.**
Run a *leftover baseline* with **every** shared load neutralised, then diff each form
against it: any register whose value differs was written by the matrix instruction, so
no sentinels are needed.  With eight tracked registers per lane (v3, v5, v4, v6, v8,
v7, v9, v10) and the destination field set to 3:

| form | registers written | lane 0 | lane 1 |
|---|---|---|---|
| `x1` | **v3 only** | `1000` | `2000` |
| `x2` | **v3 only** | `1000` | `2000` |
| `x4` | **v3 and v5** | `1000 1010` | `2000 2010` |

So the destination is **not contiguous** (x4 wrote v3 and v5 - stride 2) and x4 wrote
**two** registers rather than four, while x2 wrote one whose value equals x1's.  The
mnemonic's 1/2/4 therefore does not mean 1/2/4 plain contiguous destination registers,
and the tuple encoding is still not modelled.  What is solid and usable is `x1`: one
register, word `base + (lane & 3) + s*(lane >> 2)` - confirmed a fourth time here at
stride 4.

**Update (round 23, cont.) - the tuple encoding is resolved, and it RETRACTS the
x2/x4 readings above.**  The vendor assembler composes tuples, so ask it rather than
the hardware (`llvm-mc -arch=lggpu -mcpu=lg200 -show-encoding`, address v4, off0 0):
`x1 v0 -> byte 3 = 00`, `x1 v3 -> 03`, `x1 v4 -> 04`; `x2 v[0:1] -> 00`,
`x2 v[2:3] -> 02`, `x2 v[4:5] -> 04`; `x4 v[0:3] -> 00`, `x4 v[4:7] -> 04`,
`x4 v[8:11] -> 08`.  **Byte 3 is the tuple's start register**, contiguous - and the
tuple must be **aligned**: `x2 p0, v[3:4]`, `x2 p0, v[1:2]`, `x4 p0, v[3:6]` and
`x4 p0, v[2:5]` are all rejected with `error: unaligned sgprs`.

The probes that produced the `x2 wrote v3 / x4 wrote v3 and v5` readings used a
destination field of **3**, which is unaligned for both forms, so those readings are
undefined behaviour of an unaligned tuple and **must not be used**.  x1 is unaffected
(a single register needs no alignment) and its model stands.

**Measured with an aligned destination (round 24).**  Patching the load at destination
**v4** (aligned for x2 and x4) and diffing against the all-neutralised leftover baseline
gives, identically for **all 32 lanes** (so these are per-lane, non-divergent):

| form | registers written |
|---|---|
| `x1` | **1** - v4 |
| `x2` | **2** - v4, v5 (contiguous from the destination) |
| `x4` | **7 tracked** - v4, v5, v6, v7, v8, v9, v10 (v11+ untracked) |

The tuple starts at the destination field and is contiguous, and `x4` writes **more
than four** registers - not what the mnemonic suggests.  Lane-0 values, where the
patched instruction is the `a.z` component so every word is from that address's plane
(pattern decodes as `value = 0x1000*(plane+1) + elem`):

    x1: v4 = 3000               (plane 2, elem 0)
    x2: v4 = 3000 (elem 0)      v5 = 3008 (elem 8)
    x4: v4 = 3000 (elem 0)   v5 = 3008 (elem 8)   v6 = 3010 (elem 16)
        v7 = 3018 (elem 24)  v8 = 3014 (elem 20)  v9 = 301c (elem 28)
        v10 = 3020 (elem 32)

In register order the `x4` element sequence is 0, 8, 16, **24, 20**, 28, 32 - stride 8
for v4..v7, then v8 breaks it, so the sequence is **not modelled yet**, and whether x4
writes 8 registers is untested.  The sizes and shapes above are measured.

Earlier text (superseded): patching one ordinary load to `x2` returns exactly
x1's words in the first destination register, and `x4` returned zero there.  A
proper harness is needed because a 4-word `uvec4` shared read compiles to four
separate `load_sm_b32` (there is no 128-bit LDS load), so a tuple patch must also
neutralise the neighbouring loads.  Nothing about x2/x4's planes should be stated
until then.

Still unmeasured: x2/x4, non-linear per-lane addresses, and larger bases.  The
known-safe control passed before and after every run and no faults appeared.

**What the vendor backend cannot give**: `llc` on the declared load intrinsic
aborts with `Cannot select: intrinsic %llvm.lggpu.load.sm.matrix.x4` - it is
declared but has no instruction selection - so the assembler yields the encoding
but not the **lane -> element mapping**.  That mapping is still the open item, and
it is what the compute (KCD) carrier above must measure.

Conclusion: resolving the matrix plane/lane layout needs a **compute (KCD)
carrier** added to this runner - one that stages an LDS pattern, issues
`load_sm_matrix_x*` per lane and stores the result back.  The submission ABI is
established and available to port (see `test_probe/src/drm_compute_probe.c` and
the LoongVK driver's MCS packet construction), so this is fitting work, not
reverse engineering.

## v_mma_* operand encoding - verified CPU-side (round 25)

This repository's ISA view already carries the field layout
(`GJ11@INST_LO[7:0]+INST_HI[2:0]`, `GK11@INST_LO[15:8]+INST_HI[5:3]`,
`GA11@INST_LO[23:16]+INST_HI[8:6]`, `VD8@INST_LO[31:24]`, `M.mod2@INST_HI[10:9]`,
`M.clmp1@INST_HI[11]`, `PJ2@INST_HI[13:12]`), and the vendor assembler confirms which
operand is which:

    v_mma.8816.u8.u8.i32 p0, v[0:1],   v0,    v1,    v[2:3]     -> 00 01 02 00 49 00 30 10
    v_mma.8816.u8.u8.i32 p0, v[4:5],   v8,    v9,    v[10:11]   -> 08 09 0a 04 49 00 30 10
    v_mma.8816.u8.u8.i32 p1, v[0:1],   v0,    v1,    v[2:3]     -> 00 01 02 00 49 10 30 10
    v_mma.8816.i8.i8.i32 p0, v[0:1],   v0,    v1,    v[2:3]     -> 00 01 02 00 49 c0 30 10
    v_mma.323216.u8.u8.i32 p0, v[0:31], v[0:3], v[4:7], v[8:39]  -> 00 04 08 00 49 00 31 10

With the VGPR code written `0x100 + n` (as the compiler's own IR prints it):

* **GJ11 = A** (single register), **GK11 = B** (single register),
  **GA11 = C** (tuple start), **VD8 = destination** (tuple start);
* `PJ2` is the predicate (`p1` sets `INST_HI[13:12]`), `M.clmp1`/`M.mod2` are the
  modifiers, the shape is in the opcode (`30` = 8816, `31` = 323216) and the
  signedness pair in `INST_HI[15:8]` (`00`/`40`/`80`/`c0`).

An mma can therefore be synthesised for any chosen registers, which is what a
functional probe of the fragment layout needs.  Two harness attempts are recorded in
the probe notes: a placeholder add folds away, and a 64-bit add cannot serve because
LCO lowers `u64` addition to a 32-bit carry sequence, so the remaining step is to
substitute the mma over a no-op slot and land its destination pair where the stores
read (the compiler currently gives the two result words v4 and v6).


## v_mma_8816 fragment layout - first MEASURED result (round 26)

The mma is now synthesised from the field encoding above and substituted over one
result instruction of a compute shader, so the instruction executes through the
working compute path.  With A made one-hot - one lane (chosen by WorkGroupID.x)
carries one set byte (chosen by WorkGroupID.y), all other lanes zero - B four
1-bytes per lane, and C zero, `D = A*B` is nonzero only in the output row holding
that single 1.  Measured:

* A one-hot at lane 1, byte 0 -> **D is nonzero in exactly lanes 0, 1, 2, 3**, both
  words equal to 1, and zero in lanes 4..31.

So for the 8x8 i32 accumulator of `v_mma_8816`: **one output row is held by four
consecutive lanes, two words (two columns) each** - row 0 in lanes 0-3.  Consistent
with this, the clean blocks for lanes 4-6 put their nonzero output in lanes 4-7.

**Caveat, recorded so the provenance is not lost**: the first version of the probe
took its C operand from the output buffer's own first two words, which the probe
writes, so groups running after group (0,0) accumulated a nonzero value and their
whole row lit up ("all 32 lanes nonzero").  Those blocks are **not** measurements.
Group (0,0) is clean by construction, and the rebuilt shader now takes its zero from
`gl_NumWorkGroups` (always zero but opaque to the optimiser) so no group reads
another's output.  The row-0 result above is from the clean case; the lanes 4-7
observation is consistent but came from the version with the flaw, and the sweep is
being re-run on the rebuilt binary.

### Clean sweep (round 27): A's lane -> row, and the accumulator fragment layout

The rebuilt probe (zero accumulator taken from `gl_NumWorkGroups`, so no group reads
another's output) was run over all 32 combinations of (hot lane 0..7, hot byte 0..3),
each workgroup owning its own 64-word output block:

    A one-hot at lane 0,1,2,3 (any byte) -> D nonzero in lanes 0,1,2,3
    A one-hot at lane 4,5,6,7 (any byte) -> D nonzero in lanes 4,5,6,7

All 32 blocks agree, with no stray nonzero lanes.  Measured:

* **the accumulator fragment is four lanes per row, two words (two columns) each** -
  row 0 in lanes 0-3, row 1 in lanes 4-7;
* **A's fragment is four lanes per row** - lanes 0-3 are row 0, lanes 4-7 are row 1 -
  and the byte within a lane does not change the row, so the byte selects the column.

Harness detail that made it clean: the mma was given free registers (destination v6/v7,
B in v8, C in v9/v10) by raising `num_vgprs` in the replayed shader-info from 6 to 16,
and only the two stores' data fields were retargeted (v4 -> v6, v3 -> v7).

### Full fragment layout - MEASURED (round 28)

Both operands were made one-hot and all four parameters (A hot lane/byte, B hot lane/byte)
encoded into three dispatch dimensions, giving the whole coupling in one dispatch.  Every
one of the 32 A positions has **exactly two** B hits, and the set is perfectly regular:

    A(lane 0..3, byte b) -> B(lane same, byte b) -> D.lo of lane 0
                            B(lane + 4, byte b)  -> D.hi of lane 0
    A(lane 4..7, byte b) -> B(lane - 4, byte b)  -> D.lo of lane 4
                            B(lane same, byte b) -> D.hi of lane 4

With both operands one-hot, a hit means the two k indices agree and the lit word is the
column, so:

* **A (8x16 i8): `row = lane/4`, `k = 4*(lane%4) + byte`**;
* **B (16x8 i8): `k = 4*(lane%4) + byte`, `j = lane/4`**;
* **D (8x8 i32): `row = lane/4`, `j = 2*(lane%4) + word`** (word 0 = low).

Cross-check: A(0,b) and A(4,b) share k so they hit the same two B positions - they do,
landing in row 0 (lane 0) and row 1 (lane 4); B(0,b) has j = 0 -> D.lo and B(4,b) j = 1
-> D.hi, exactly the observed pair.

**Scope**: A lanes 0..7 (rows 0-1) and B lanes 0..7 were swept; B lanes 8-31 were not, so
`j >= 2` and rows 2-7 are the model's extrapolation, not separately observed points.

**Now verified over the full range (round 29).**  A second probe encodes the whole range
into three dispatch dimensions - x = A lane (0..31), y = A byte*4 + B byte, z = B lane
(0..31) - so one dispatch of 16384 workgroups covers every (A lane, A byte, B lane, B byte)
combination, each with its own 64-word block (1,048,576 words).  The prediction checked
against every combination was

    hit  <=>  (B lane % 4 == A lane % 4) and (B byte == A byte)
    lit lane = 4*(A lane/4) + (B lane/4)/2,  lit word = (B lane/4) % 2

and the result was **16384 of 16384 combinations matching exactly - 0
nonempty-but-wrong, 0 expected-hit-but-empty**.  The layout above is therefore a measured
law over the whole operand range, not an extrapolation.

**Consequence**: a cooperative-matrix fragment can be built with ordinary per-lane loads,
because it is now known exactly which `(row, col)` element each lane and byte receives, and
the accumulator can be written back the same way; the `load_sm_matrix_x*` forms are an
alternative route (stage in LDS, one instruction per fragment) rather than a requirement.

**Harness traps fixed while getting this**: with `num_vgprs = 7` the shader occupies v6,
so an mma destination pair there loses its high word to the shader's own code - which
produced a plausible-looking but wrong "all lanes nonzero" dataset before it was caught;
and one sweep set the mma's C to v9 while zeroing only v8/v9, leaving v10 live.  All mma
registers are now >= v8 with `num_vgprs` raised to 16.




