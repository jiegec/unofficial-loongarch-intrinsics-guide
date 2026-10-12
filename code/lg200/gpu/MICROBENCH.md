# LG200 CS microbench (compute path) - deliverable record

The phase-2 deliverable: per-instruction GPU timing for the LG200 ISA through the
**compute (CPIPE) pipe**, with the generation/layer/source/scope/unknowns recorded
and every number reproducible.

## 1. Generation, layer, source

| axis | value |
|---|---|
| generation | LG200 (LoongGPU(TM), DRM 1.0.0) |
| pipe / layer | CPIPE compute, MCS user queue over the KCD/DRM COMPUTE path |
| ISA semantics | u-l-i-g `code/lg200/<mnemonic>.{h,cpp}` (documented / HW-observed / UNRESOLVED marks) |
| program source | this repo's generic emitter (`envelope::build_cs_bench` -> `CarrierEnv::compute`), i.e. the **same** emitter the CS correctness matrix validates |
| measurement device | the firmware timestamp block in the completion-signal BO |
| reference for comparison | the closed producer's own `code/lg200/measure-lg200.csv` (see §6) |
| sample | 768 mnemonics (the full universe) |

## 2. Method

- **Window**: four 64-bit firmware counters at signal `+0x20/+0x28/+0x30/+0x38`
  (enqueue, retire, wave start, wave end).  The measured window is
  `+0x38 - +0x30`; ordering is always `+0x20 <= +0x30 <= +0x38 <= +0x28`.
- **Marginal, not absolute**: each mnemonic runs at `K_SMALL = 256` and
  `K_LARGE = 2048` copies; the reported cost is
  `(win(K_LARGE) - win(K_SMALL)) / (K_LARGE - K_SMALL)`.  That removes the fixed
  launch + result-store overhead (about 1095 ticks at copies=1).
- **latency** = one dependency chain (each copy's sources bound to the previous
  copy's destination).  **throughput** = eight rotating independent sources.
- `REPS = 1`; 64 mnemonics per process; one process stops on the first
  non-clean submission.

## 3. Tick unit (calibrated)

`python3 calibrate_ticks.py [seconds]` - two single submissions separated by a
known host sleep give the free-running firmware counter's rate:

| sample | host seconds | tick delta | ns/tick |
|---|---|---|---|
| 30 s | 30.00 | 22,545,662,594 | 1.3306 |
| 10 s | 10.045 | 7,531,374,567 | 1.3337 |

**1 tick = 1.333 ns (~750 MHz)**, reproducible to 0.3 %.  The CSV keeps raw
ticks; `bench_ns.py` derives `lat_ns`/`tp_ns`.

## 4. Artifacts and reproduction

| file | content |
|---|---|
| `build/cs_bench_results.tsv` | 768 rows: mnemonic, status, lat_ticks_per_op, tp_ticks_per_op, note |
| `build/cs_bench_results_ns.tsv` | the same + `lat_ns` / `tp_ns` (`bench_ns.py`) |
| `build/cs_bench_coverage.tsv` | per-mnemonic bench vs correctness status + family/reason (`bench_coverage.py`) |

    cd code/lg200/gpu
    ./build/lgpu-gpu csverify          # host preflight, no submission
    python3 calibrate_ticks.py 30      # 1.333 ns/tick
    python3 run_bench.py               # 768 rows -> build/cs_bench_results.tsv
    python3 bench_ns.py                # -> build/cs_bench_results_ns.tsv
    python3 run_gpu.py --pipe cs       # correctness matrix (for the coverage join)
    python3 bench_coverage.py          # -> build/cs_bench_coverage.tsv

## 5. Coverage

**497 OK / 271 SKIP of 768.**  The SKIPs are not one thing:

| split | rows | meaning |
|---|---|---|
| SKIP and correctness **SKIP** | 84 | the carrier itself refuses them (descriptor / sampler / texture) |
| SKIP for any other reason | 187 | the bench lacks a template |

The 84 are the **stable** side of that split: they are the carrier own
refusal (SKIP-ENVELOPE), which holds whatever else the matrix says.  The 187 are
everything else the bench skips.  Classifying them by correctness **PASS** instead
- which is what this report did - makes the split depend on the matrix being a
clean run, and the matrix on disk is **the run that wedged** (307 PASS / 84
SKIP-ENVELOPE / 377 HANG-FAULT).  Rows that run never reached were neither PASS
nor SKIP, so they silently fell out of both buckets and the gap was undercounted
as 184.  The report now classifies by the carrier refusal and **warns** when the
matrix is not clean; the numbers above are from that run.

Of those, twenty are control/flow ("no cost to chain"), so the actionable
remaining bench work is the memory and SIO families: buffer memory 61, flat memory
45, shared memory (LDS) 44, SIO 17.  Each needs a per-family bench template built
on the setup the correctness carrier already has (`build_buff`, `build_lds`,
`build_flat`).  The 84 carrier-limited rows are the same limitation the
correctness matrix records, not a bench gap.

### 5.1 Why the memory templates are a design problem, not just plumbing

The generic template measures latency by **register chaining**: copy *i*'s
destination is copy *i+1*'s source, so the K copies are one dependency chain and
the window is latency x copies.  A memory access has no such source: a load's only
inputs are an address and a descriptor, and neither depends on the previous load's
*result* unless the template makes it so.  Three constructions are possible, and
they do not measure the same thing:

| construction | measures | caveat |
|---|---|---|
| K independent accesses from one staged address | throughput | not a latency number; report it as `tp` only |
| address chaining: the loaded value becomes the next address | latency | the addresses must stay inside the carrier's window, so the loaded value needs masking; the mask is part of the measurement |
| store/load ping-pong | a round trip, not the load | includes the store's cost - only honest if both are reported |

The correctness carrier already stages everything the first construction needs:
`build_flat` preloads the window and holds the address pair, `build_buff`
hydrates the descriptor, and `build_lds` stores the content before the load.  So
**throughput** for the three memory families is reachable by reusing that setup and
repeating the access; **latency** needs the address-chaining construction and its
mask, which is a new program shape rather than a template.
**Implemented so far: the flat LOAD forms** (`load_v_flat_b32/b64/b96/b128` and
the `i8/u8/i16/u16` sub-words - 8 rows).  `build_cs_bench_flat` stages the LCO
address shape (the full absolute VA in the vaddr pair, a literal-zero scalar base
pair) and preloads a small value, then:

* `tp` issues K independent accesses from that one address;
* `lat` feeds each access value into the next access address through one
  `v_add_imm_u32`, so the copies form a dependency chain.  **The measured window
  therefore includes that add**, and the staged value is small so the chained
  address stays inside the carrier window.

The **shared-memory cmpswap** rows are in too (all six).  They have not one
shape but three, each with its own old-value destination - the eq form takes a
compare in v13 and returns in v17, the f64 gt/lt form returns in the v9 pair, and
the f32 gt/lt form returns in v13 - and the template mirrors the correctness
carrier for each rather than re-deriving any of them.  The chain moves the old
value back into the data register; `tp` re-stages it instead.

The 32-bit **cmpswap** forms of both the flat and buffer atomics are in as well
(`atomic_v_flat_cmpswap_b32`, `atomic_v_buff_cmpswap_b32`).  A cmpswap stages the
compare in the dword above the value, so value and compare are one register pair
that the instruction both reads and writes - the same natural chain as the other
atomics, and independent copies re-stage both halves.  The 64-bit forms need a
compare PAIR as well as a value pair and are refused for now.

The subword/format buffer stores are refused for a sharper reason than shape: the
correctness carrier records that emitting the b8 shape for b16 put the descriptor
in the VQ8 slot and **faulted the pipe at 0x1000b0000 + 0x000b0300 (measured)**.
A bench template has no business re-deriving a shape with that history.

**The plain BUFFER stores are in too** (4 rows: b32/b64/b96/b128).  Their
latency is the same round trip, and the readback is the ordinary buffer load the
correctness carrier already uses - it writes straight into the register the next
store stores, so the trip needs no extra instruction.  The b8/b16 and
fmtdesc/fmtinst forms are refused explicitly: they take the subword/format branch
of the correctness carrier and have a different operand shape.

**The STORES are in too, with a caveat the CSV now carries** (6 `store_v_flat_*`
and 5 `store_sm_*`).  A store has no result, so there is no register to chain
through; the only way to make one copy depend on the previous one is to READ BACK
what was just written and store that value next.  Its `lat` is therefore a
**store+load round trip**, not a store latency.  Because a number that cannot be
read as what its column name implies is a defect in the record, the runner now
emits a per-row note and `run_bench.py` carries it into the CSV: for these rows the
note reads "lat is a store+load round trip, not a store latency".  The `tp`
column is unaffected - independent stores of one staged datum.

**The shared-memory LOADS are in too** (7 rows - the matrix and interpolator forms
are refused, because their addressing is exactly what the chapter leaves
UNRESOLVED and the correctness carrier runs them observed for the same reason).  A
load result is the loaded value and its only operand is the address, so `lat` can
make the DESTINATION the address register: the value just loaded becomes the next
access address, chaining the copies with no extra instruction.  The staged content
is 0x20 - the cell's own base - so the chained address stays on the same
in-window cell.  As with the shared-memory atomics, the cell store needs one
ordering fence and only one.

**The buffer ATOMICS are in too** (24 of the 26 `atomic_v_buff_*` rows).  Their
operand IS their destination - the buffer form stages one register that is both
read and written - which decides the two kinds: `lat` stages the operand once and
lets the returned old value be the next operand, while `tp` RE-stages the constant
operand before every copy so no copy depends on the previous one.  The move
pipelines, so it does not carry the atomic cost.  Cell preload and descriptor
assembly are both shapes the correctness carrier already exercises.

**The shared-memory ATOMICS are in too** (26 of the 32 `atomic_sm_*` rows).
Same two kinds - the destination is the operand, so the returned old value feeds
the next copy - with one extra piece of setup: shared memory starts undefined, so
the cell has to be STORED, and that in-program store needs one ordering fence
(`check k vm vs sm`) before the atomics.  The copies themselves are ordered by the
register dependency, so no per-copy fence is emitted: one fence per copy would put
the ordering cost inside the latency window.

**The flat ATOMICS are in too** (24 rows, `atomic_v_flat_*` minus the two cmpswap
forms).  An atomic takes an operand AND returns the old value, so the two kinds
fall out naturally: `tp` keeps the result in a separate register, and `lat` makes
the destination the operand, so copy i's returned old value is copy i+1's
operand and the copies form one chain.  Both use only positions the correctness
carrier already exercises, and neither needs an untested operand form.

The flat STORES are still skipped, and deliberately: a store has no value to chain
through, so what its latency would measure is the address computation rather than
the store.  Buffer and LDS still need the same treatment on top of their descriptor
and window setup.

**Buffer and LDS: the chain needs an operand form nothing has exercised.**  Their
latency link has to go through an operand that can depend on the previous access,
and the two candidates are the buffer form's index register (with `idxen`) and its
offset register.  The correctness carrier does not exercise either: its buffer load
is `buff_mem(..., address = v0, offset = zero_reg, immediate = off, offen = true)`,
i.e. the offset travels in the *immediate*, and the index and offset registers are
left at zero.  Building a bench chain on an operand form the validation path never
touched would produce a number whose construction is unverified - and a wedge risk
on top, since a wrong index register is an out-of-window access.

The first hardware session therefore has to answer this before those rows can be
benchmarked at all: does `idxen`/`offen` with a live index or offset register
address the way the descriptor's stride implies?  That is now an item in
HARDWARE-SESSION-PLAN.md, not an assumption in a template.

Until a construction is chosen and measured, the honest state for these 150 rows
is `tp`-only or benchmarked, and neither should be reported as a latency.

SIO is a different case: the rows have no canonical codec variant, so the bench
template would have to emit them per mnemonic the way the correctness carrier's
SIO path does.  Their "cost to chain" is also questionable - the SIO rows read and
write lane-crossing state, so a chain of them is not a chain of independent work.

One OK row is **not** a valid timing: `v_mbcnt_u32` fails the CS correctness
matrix (see §7).

### 5.2 Buildable now (host-only, no device)

The coverage above joins a hardware CSV, so a template that was just written
cannot show up until the device returns.  bench_buildable.py asks the runner to
build every row for both kinds in **dry-run** mode - no device, no submission, a
few seconds - and reports the outcome and the reason:

| | rows |
|---|---|
| buildable for both kinds | **605** |
| texture bench: the correctness carrier refuses the family (SKIP-ENVELOPE) | 79 |
| buffer load bench: the chain needs a live index or offset register (item 10) | 21 |
| sio bench: run OBSERVED, and their chain is not a chain of independent work | 20 |
| control bench: control and geometry-stream rows, nothing to chain | 14 |
| buff store bench: only the plain b32/b64/b96/b128 forms share this shape | 10 |
| no canonical variant | 5 |
| environment bench: the row returns launch state, not a computed value | 4 |
| sm load bench: the matrix and interpolator forms address UNRESOLVED state | 4 |
| copy bench: the global<->LDS copy is a control-path op (predicate-only) | 2 |
| 64-bit cmpswap: stages a compare pair as well as a value pair (flat, buff) | 2 |
| not materializable on the generic carrier | 1 |
| refused on the compute pipe (known to wedge) | 1 |



Only five rows are still on a GENERIC reason - the scalar flat loads - and
every other refusal now names the evidence behind it: the texture family is the
one the correctness carrier refuses (SKIP-ENVELOPE), the buffer loads are the ones
whose chain needs a register nothing exercises yet, the control and
geometry-stream rows have nothing to chain, the environment reads return launch
state, and the SIO rows run OBSERVED.  Two of them are worth reading on their own:
the **trap** row says it wedged the pipe on 2026-10-12 and is excluded from every
batch, and the subword/format buffer stores say a measured fault is on record for
the b16 shape.

The "no canonical variant" reason is worth spelling out: it is a gap in the
codecs *variant* table, not in the encode table or in a template.
kCanonicalVariants carries **581 entries over 530 of the 768 mnemonics**; the
other **238 have no variant at all** - atomic 88, load 41, samp 32, gather 24,
store 24, siordrot 6, siowrrot 4, prefix 3, and a few singletons.  They are the
same memory, texture and SIO families the sections above describe, and they have
no variant because the correctness carrier emits them through *named* builder
calls (load_v_flat, buff_mem, sm_op, image_op, ...) rather than through the
generic operand walk - so nothing ever needed one.  The bench does, because its
template assigns registers from the decoded operand classes.

A family template therefore has to do one of two things: derive the classes from
the rows layout instead of from a decoded variant, or give the family variant
entries.  Either way the template is what is missing; the reason string names the
first symptom.
605 is 497 (the last hardware run's OK) plus the eight flat load rows, the 24 flat atomic rows and the 26 shared-memory
atomic rows, the 24 buffer atomic rows the 7 shared-memory load rows the 11 store rows the 4 plain buffer store rows the two 32-bit cmpswap rows and the six shared-memory cmpswap rows this work added - i.e. the offline number and the device number agree, which is
what makes it usable as a target before a session.  The reasons are the same
per-family gaps the sections above describe.

## 6. Reference values and cross-check

| mnemonic | lat (ns) | tp (ns) |
|---|---|---|
| `s_add_i32` | 2.69 | 2.86 |
| `v_add_f32` | 12.31 | 4.18 |
| `v_mul_f32` | 12.25 | 4.17 |
| `v_rcp_f32` | 22.65 | 7.50 |
| `v_mma_323216_i8_u8_i32` | 54.96 | 54.65 |

The closed producer's `measure-lg200.csv` shares 122 rows with this table.  Its
values are **not** usable: latency Pearson r = 0.166 against this measurement,
ratio median 2.15 / max 58.7, and its `reliable` flag does not prevent simple
scalar ops from appearing at 181-361 ns (`s_cmp` 181.17 vs 6.66 measured).  No
constant rescaling fixes it (r is scale-invariant).  **This CSV is the
authoritative table.**

## 7. UNRESOLVED / unknowns

- **Tick unit**: resolved (§3); the PS layer's "~32 ns/tick" does not describe
  this counter (at 1.333 ns/tick the dependent `v_add_f32` is a plausible
  12.3 ns).
- `v_mbcnt_u32`: **exec-mask dependent** (lane sweep 1/2/3/4/8/16 lanes gives
  mask contributions 0/1/3/3/7/8 for the same case; PS matches the 16-lane
  oracle 41/41).  Its CS row is a launch-shape difference, not a miscompile, and
  cannot be fixed by the `v_cmp` value remap.  Not a valid timing.
- **Memory/sampler/SIO families**: not measured here (§5); their cost is unknown.
- The number is the marginal cost of the **generic emitter's** program, not of a
  hand-optimised issue pattern.

## 8. Why these numbers are trustworthy

The table was produced only after the CPIPE wedge was fixed; the run that
generated it finished with **zero** GPU faults (`dmesg` clean) and the device
remained usable.  Both earlier attempts wedged CPIPE at the ring VA
(`0x100020000`) and produced the file now kept as
`/tmp/cs_bench_OLD_void.tsv` (233 OK / 250 NO-DATA / 17 FAULT).  Root cause and
fix are recorded in `README.md` ("The CPIPE wedge: root cause and fix"): a
persistent session now owns the queue, the ring and every dispatch buffer, and a
full ring **recycles the queue** instead of wrapping the write pointer.
