# Carrier and coverage status

A single view of what the LG200 chapter's four work phases have established, what
the evidence for each is, and what is blocked and on what.  The detail lives on
the pages linked from each row; this page is the index.

Verified together on 2026-10-12: chapter invariants hold (`check_gpu.py`: 768
rows, 12 formats, 7 families, 33 state blocks), the table is intact (768 rows, 23
columns, unique), the offline oracle passes (**768 instructions, 83,253
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
| rows stating an `UNRESOLVED` | 737 | **598** |
| distinct UNRESOLVED sentences | 872 | **800** (1,046 occurrences) |
| explicit placeholder bodies in `code/lg200` | 11 | **0** |

Twenty-six classes are closed, each with its evidence named on
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
| the atomic families, the `X0_16` false positive | 550 |
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
