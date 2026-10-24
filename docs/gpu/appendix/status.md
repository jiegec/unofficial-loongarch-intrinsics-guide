# Carrier and coverage status

A single view of what the LG200 chapter's four work phases have established, what
the evidence for each is, and what is blocked and on what.  The detail lives on
the pages linked from each row; this page is the index.

Verified together on 2026-10-12: chapter invariants hold (`check_gpu.py`: 768
rows, 12 formats, 7 families, 33 state blocks), the table is intact (768 rows, 23
columns, unique), the offline oracle passes (**768 instructions, 86,013
vectors**), and the hardware runner builds.

## Phase 1 - the PS/CS carrier

| item | state | evidence |
|---|---|---|
| operand ROLES per family, not position | done | driver ISA rows + seed assembly; the classified defects (sio VK8, the v_cmp lane mask, the v_dp4a accumulator) are fixed |
| the flat-store encoding | done | 57 rows corrected; the store now executes |
| `audit.py` field coverage (incl. tuple widths) | done | Rule F; refuses to pass on a stale log |
| memory/launch path vs loongvk, compute absolute-VA vs kernarg | done | Axis-1: the flat address is the absolute 64-bit VA with a literal-zero scalar base pair; in compute `s[6:7]` is the kernarg pointer |
| one queue per submission, per-case isolation, stale-log trap | done | the CPIPE wedge fix; `cspersist 1000` completes with 0 faults |

The one hardware hazard found here is recorded in
[HARDWARE-SESSION-PLAN.md](../../../code/lg200/gpu/HARDWARE-SESSION-PLAN.md): the
`trap` control row wedged the pipe and the device is parked pending a platform
power cycle.

## Phase 2 - the compute microbench

| item | state |
|---|---|
| CS microbench over the universe | 497 OK / 271 SKIP of 768, 0 faults |
| method | marginal cost from the firmware timestamp block; K-copy chain (latency) vs eight independent streams (throughput) |
| tick calibration | 1.333 ns (two independent samples, 0.3 % apart) |
| deliverable record | [MICROBENCH.md](../../../code/lg200/gpu/MICROBENCH.md) |

The closed producer's `measure-lg200.csv` shares 122 rows and does not agree
(Pearson r = 0.166); the local table is the authoritative one.  The remaining
coverage split - 187 template gaps vs 84 carrier limits - is in the microbench
record.

## Phase 3 - UNRESOLVED writeback

| measure | start | now |
|---|---|---|
| rows stating an `UNRESOLVED` | 676 | **512** |
| distinct UNRESOLVED sentences | 872 | **676** (899 occurrences) |
| rows whose every open clause states a reason | — | **383** |
| explicit placeholder bodies in `code/lg200` | 11 | **0** |

### The rules for citing evidence (2026-10-13)

Before a clause is settled by "hardware", answer three questions, all of them
mechanically checkable with case_provenance.py:

1. **Was it executed?**  As of the 2026-10-22 full PS run, yes for the whole grid: one
   process, one session, all 86,013 vectors in 79.5 s.  Before that run the answer
   depended on the vector - the 10-13 regeneration added up to 62,957 vectors that were
   oracle expectation only - so a clause settled in that window had to name the grid it
   rested on.
2. **How strong is the record?**  Of the 768 PS rows, 629 were compared against the
   oracle, 137 were merely OBSERVED (the hardware value recorded without an independent
   comparison), one failed (`v_mbcnt_u32`, whose two pipes read different widths) and one
   could not be enveloped.  Observed is a weaker class and a claim resting on it must say
   so - entry 16 was rewritten for exactly that.
3. **Is the artifact clean?**  Everything written after the 2026-10-13 01:42 trap
   carries "cannot open /dev/dri/renderD128".  The CS matrix has 377 HANG/FAULT
   rows, and that band IS the wedge, not 377 failing rows; read as evidence it says
   the opposite of the truth.

A claim that cannot answer all three does not settle a clause: it stays
UNRESOLVED, and says which of the three it is missing.

### Which evidence was actually executed

`case_provenance.py` answers this mechanically.  The hardware matrix records a
per-row row count, and comparing it with the grid shows how much has been added since.
The measured baseline is pinned by the matrix itself rather than by a date:
`v_add_f32` row says row=111 with detail `PASS gen2510`, and the 10-10 grid
(`4f26f81`, 18,542 vectors) carries 11 hand cases plus 100 generated vectors for that
row - 111.  The matrix measured THAT grid.

Everything the 10-13 regeneration added was oracle expectation rather than reading, and
for a while that was a real gap: at its worst the check reported **457 rows grew,
+62,957 vectors, about three quarters of the current grid**.  The **2026-10-22 full PS
run** closed it - one process, one session, 768 rows in 79.5 s, 80,887 passes and the 11
documented `v_mbcnt_u32` divergences - so the same check now reports
**0 rows grew, +0 vectors** and the matrix covers all 86,013.  The two consequences worth
remembering from when it did not:

| vector | in the 10-10 grid? | therefore |
|---|---|---|
| `v_add_f32 0x80000000 0x80000000 -> 0x80000000` | yes | measured, so the zero-sign clauses stand |
| `v_add_f32 0x0 0xffc00000 -> 0x7fc00000` | no, until the 10-22 run | the NaN clause it left open was settled by the probe that fixed `norm_nan32`, which took the row from 357/37 to 394/0 |

### What the remaining sentences are about

The remaining sentences are no longer a documentation problem: settled from
recorded facts, they would be.  Grouped by the kind of question they are, and by
the hardware-plan item that would settle each:

Measured 2026-10-22: **676 distinct sentences, 899 occurrences** across the 512
rows that still carry the word.  An earlier version of this line said 786 and 1,048,
from a keyword census that split sentences on the escaped newlines as if they were
breaks; the tracked count above, which the drift check reads, is the one used here.

This paragraph used to repeat half a sentence and carried a row count of 580 that no
check read, both left behind by an earlier edit - the kind of damage a monitored
figure prevents and an unmonitored one hides.

The table below stays indicative because its kinds are assigned by first matching
keyword: a sentence is attributed to the first kind it mentions, and the last row is
what no keyword claimed - 227 occurrences of one-off behavioural questions (a single
encoding bit, whether a wrap carries, what a flag holds), which is why they are
grouped rather than listed.  They are the same kind of question as the rows above,
answered one at a time.

One question is shared rather than per-row and is deliberately not counted among
these sentences: the **machine lane count**.  Sixteen `v_cmp` rows record that the
destination holds one bit per lane, measured at 4, 8 and 16 lanes and narrowing to
`(1<<lanes)-1`, and that the wave's own lane count is still not established.  No row
can settle it - the lane-id read is observation-only - so it is tracked here instead
of being repeated in sixteen descriptions as sixteen UNRESOLVEDs.

| kind | occurrences | settled by |
|---|---|---|
| atomic bound / order among lanes | 245 | plan 9, plus same-location ordering (a new shape) |
| alignment / misaligned access | 193 | plan 6 |
| out-of-window / out-of-allocation | 132 | plans 6 and 8 |
| texture sampling / lod / wrap | 75 | plan 8 |
| flush / rounding / nan | 73 | plans 1 and 2 |
| selector / reserved range | 35 | which encodings a decoder should reject |
| visibility / memory ordering | 29 | plan 7 |
| sub-word / narrowing / read-modify-write | 11 | hardware |
| modifier semantics (mod2, clmp1) | 4 | plan 3 |
| smaller, ungrouped questions | 227 | mostly hardware, one row at a time |

Thirty-four classes are closed, each with its evidence named on
[UNRESOLVED Status](unresolved-status.md) - hardware measurements for the
behavioural ones, closed-producer censuses and encoding-uniqueness proofs for the
static ones.  What remains is dominated by **memory-access semantics** (address
formation, alignment, page or cache-line straddling, unmapped behaviour, sub-word
narrowing and read-modify-write), the **atomic counter bound**, the **SIO and wide
selector reserved ranges**, and the **matrix plane/lane layout** (three code-layer
markers).  All of them need hardware or a carrier shape that does not exist yet.

## Phase 4 - case boundary coverage

Boundary coverage of the required classes, as measured by `case_coverage.py`:

| stage | rows covered |
|---|---|
| before phase 4 | 398 |
| per-format tables, f64 pairs, the full signed value space | 499 |
| the atomic families, the `X0_16` false positive | 512 |
| the immediate rows, the width-aware integer tables | 680 |
| type-token selection, texture atomics, 64-bit integer compares | 710 |
| the six-dword three-pair shape, the type union, pair low words | 753 |
| the sub-word loads | 765 |
| the plain-width (`bNN`) rows, and separating launch-shape rows | **738 of 738 value-expressible** |

The vector count went 20,732 -> 83,547.  The auditor now separates the rows whose
result is **launch state rather than an operand** - the environment and SIO reads,
the stream emits, the interpolators and the index read, 30 in all - and reports
them as launch-shape: they need a launch state, not a case.  Every row whose
boundaries *are* expressible as an input value is covered, with 0 gaps.  The full
record is in [Case Boundary Coverage](case-boundaries.md).

## What the boundaries changed

Running the expanded set on hardware surfaced 78 rows where the oracle and the
hardware disagree, and they identified three measured behaviours the model lacked:
denormal inputs read as zero (DAZ), denormal outputs are flushed (FTZ), and a
source NaN propagates its sign.  The arithmetic rows are fixed by the emitted
policy; the FP compare oracles' flush is landed but **not yet measured**, because
the device parked during the run.  That measurement is the first item in the
hardware plan.

### The safety and field audit, and how it fails safe  (2026-10-13)

`code/lg200/gpu/audit.py` reads the runner's own listings, not the source, so it audits
the programs that were actually assembled.  Over a freshly regenerated corpus
(`run_gpu.py --dry`, host only, 768 PASS / **86,013** rows in **54 s**; two runs, 53.7
and 56.6, on 2026-10-13, replacing an earlier 23.4 s measurement taken before the 252
rounding-mode cases were added) it reports **768
programs audited, 0 refused, 0 problems**.

Two properties are worth recording, because they are why the number means something.
First, the audit **refuses a stale corpus** rather than passing it: before regeneration
it said `STALE LOGS: no auditable programs ... (768 unusable) - re-run run_gpu.py --dry
first`.  A log from an older carrier would otherwise be audited as if it were today's
program.  Second, its three hard classes are the ones that have wedged this GPU and cost
a power cycle - control flow, address registers, and the VA window - so a clean run is a
safety statement, not a style check.

Its **F class is the field-coverage check** the carrier work asked for: every field a
driver row declares must be modelled by the codec's semantic rule class, no field may be
invented, tuple widths are compared, and the rows' optional `X` parameter bits are
allowed to stay at the template.  The run records **16 note classes**: one E (2 rows -
`X2_23` prints as a register in the driver seed and the catalog marks its role
UNRESOLVED) and fifteen F - `X0_16` unmodelled on 42 rows, and tuple-width differences
such as `VJ8x8` against a codec tuple of 3 on 64 rows.

The tuple differences are recorded rather than failed, and the reason is structural: the
per-mnemonic tuple override lives outside this repository, so the audit can name the
disagreement but the fix belongs with the catalog.  Naming them is the deliverable;
silently passing them would not be.

One tool defect was fixed while doing this: the note detail was truncated to 66
characters, which cut off exactly the part that says WHY.  The E note now reads in full,
and that reason is what points back at the catalog's UNRESOLVED.

## How these claims are kept true

The figures above are recomputed by `code/lg200/check_all.sh`, and every gate it runs is
required to be able to fail: `gate_check.py` reads the scripts the suite actually invokes and
refuses any without a failure path, and each of those has a case in `mutation_check.py` that
breaks it and requires the break to be noticed.  Four categories are deliberately not gates,
and each is named with its reason rather than left silent - an unnamed exception is how the
next sweep ends up rediscovering it.

| category | tools | why it is not a gate |
|---|---|---|
| reports by design | `prose_audit`, `doc_register_audit` | their findings include expected ones, so there is no pass or fail |
| session-time | `audit.py` | reads a device-produced runner listing, which is not tracked; it runs when one is present and says so when it is not |
| cross-repo | `jumpstack_check.py`, `enc_probe.py` | compare against the vendor's ISA row and assembler, so they need the LCO checkout and say so without it |
| diagnostic | `fp_boundary_audit.py` | checks a coarser subset of boundary classes than `case_coverage.py`, which is the gate for the full required set |
| library, runner or generator | `cases.py`, `gen_cases.py`, `bench_buildable.py`, `bench_coverage.py`, `calibrate_ticks.py`, `run_gpu.py`, `sweep2d.py`, `run_bench.py`, `bench_ns.py`, `fp_bounds_gen.py` | not checks at all: some are imported rather than run, some produce an artefact, and the runners need a device |
