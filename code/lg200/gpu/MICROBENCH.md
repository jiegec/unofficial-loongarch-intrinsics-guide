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
| SKIP but correctness **PASS** | 187 | the CS carrier builds and passes them; only the bench lacks a template |
| SKIP and correctness **SKIP** | 84 | the carrier itself refuses them (descriptor / sampler / texture) |

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
| buildable for both kinds | **529** |
| no canonical variant | 204 |
| not materializable on the generic carrier | 26 |
| no semantic specification | 6 |
| flat atomic bench: cmpswap stages a compare pair this template does not build | 2 |
| refused on the compute pipe (known to wedge) | 1 |



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
529 is 497 (the last hardware run's OK) plus the eight flat load rows and the 24 flat atomic rows
this work added - i.e. the offline number and the device number agree, which is
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
