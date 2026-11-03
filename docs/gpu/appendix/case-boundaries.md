# Case boundary coverage

Every row's case vectors must exercise the boundary values its operand types
admit.  This page records what that means, how it is measured, and what the
measurement found.

**A note on the `@@...@@` markers.**  They are a build-time substitution
convention: the name or value inside comes from the case tools, and in this source
it stays literal because the site build does not run in this checkout (`mkdocs`
fails on a missing dependency).  Read what is inside as a code identifier or a
value from `gen_cases.py`; the figures that matter are also written in prose beside
them, and the ones that are current are bold, so nothing here depends on the
substitution having run.

## 1. The required classes

Derived from the mnemonic's own type suffix, so a mixed row (`v_cvt_f32_i32`)
is asked for both sides.  A floating-point operand must see the whole signed
space: both zeros, both infinities, quiet **and** signalling NaN in both signs,
the finite limits, and the denormal range.  An integer operand must see 0, ±1,
and the width extremes.

| format | classes |
|---|---|
| f32 | `+0` `-0` `+1` `-1` `+0.5` `max_norm` `-max_norm` `min_norm` `-min_norm` `max_denorm` `min_denorm` `+inf` `-inf` `qnan` `-qnan` `snan` `-snan` (17) |
| f16 | the same 17 against the f16 encoding, in the low half of the word |
| f64 | the same 17 as word pairs |
| i8/i16/i32/i64 | `0` `1` `max` `min` `-2` `-1` |

The count read (19) until 2026-10-13 while the list beside it named 17 - the row was
counted from memory rather than from its own contents, which is the defect this page
records elsewhere.  And `case_coverage.py` required only 12 of the 17 for f64 rows, so those
rows were judged against a weaker requirement than f32 and f16: no negative max/min, no
`max_denorm`, no negative NaN.  Both are fixed, and the full set still passes 738 of 738 -
which means the generated vectors had carried the five all along.

## 2. Measurement

`code/lg200/case_coverage.py` joins the table's type suffixes with the parsed
case set and reports, per instruction, which required classes its inputs never
take.

**The parsed case set has two sources, and this matters to anyone checking by hand.**
The parser reads the `LG200_CASE_BEGIN` block in each `<mnemonic>.cpp` and then merges
`generated_cases.tsv`, which is much the larger of the two.  `atomic_sm_cmpswap_gt_f32`
shows the ratio: four hand-written cases in its `.cpp`, 365 input rows in total.  So a
reader who opens the `.cpp`, counts its cases and concludes a class is missing will be
wrong - and that has now happened twice in this campaign, once in each direction: an
audit that read only `CASE_FP` lines reported classes missing that the generated vectors
carried, and a hand check of a `.cpp` reached the same conclusion from the other side.
Parse through `cases.py` instead of counting a file.

**The split, measured independently (2026-10-22).**  A count that does not use the parser
agrees with it exactly: 4,625 hand-written `CASE`/`CASE_FP` lines across the 768 `.cpp`
files plus 81,472 rows in `generated_cases.tsv` is 86,097, which is what `parse_case_files`
returns.  So the merge is complete, and that is worth stating because the sum is the only
thing here that does not depend on the parser being right.

**What the parse does not carry is the source.**  A parsed `Case` has `id`, `mnemonic`,
`inputs`, `expected` and `fp_policy` - no origin.  A provenance question, such as which of
the two sources a vector came from, has to be re-derived by matching keys against the two
files; it cannot be looked up in the parse.  The first attempt at that comparison here got
it wrong in the usual way - it compared 4,288 declared case LINES against 85,645 expanded
VECTORS and reported 617 mismatches in a parser that is correct.

    python3 code/lg200/case_coverage.py --tsv build/case_coverage.tsv

## 3. What it found (2026-10-12)

The generated boundary grid previously carried a ten-value f32-only list, so
before phase 4: **398 of 768** instruction rows exercised every class their
operands admit.  After the generator gained per-format tables (f16/f64 included),
the full signed value space (signalling NaN and negated NaN/limits/denormals
included), fp64 pair operands and the four-dword binary-f64 shape: **499 of 768**.

A further step reaches the atomic families, which the generator had excluded
along with the other memory rows.  Their carrier stages the cell and the operand
straight from the case inputs - the address comes from the case index, not from a
case value - so a boundary grid is stageable even though the bench cannot time
them; @@case_targets()@@ adds those fifty rows without touching the bench's own
target set.

A second exclusion turned out to be a false positive: the "immediate slot" test
matched @@X0_16@@, the modifier field the atomic layouts carry and no case supplies
a value for.  Distinguishing it from a real immediate admits a further 32 atomic
rows and leaves the bench's target set at 308 unchanged.

The last large exclusion was rows whose last input is an **immediate**.  Their
register columns belong in the value grid and the immediate column belongs in its
own - the immediate is the operand the ISA prints last, and it carries the row's
own type, so it takes that type's boundary list.  @@grid_cols()@@ expresses a grid
over per-column lists (with @@grids()@@ as the single-list case), and
@@case_targets()@@ now admits 156 such rows.

Finally the integer grid became **width-aware**.  A 16-bit row's boundary classes
are the 16-bit extremes (@@0x7fff@@/@@0x8000@@/@@0xfffe@@/@@0xffff@@), not the 32-bit
ones, so a single INT list left every narrow row short of the values its operands
admit.  The narrow sets are UNIONS with the 32-bit list - a narrow row still wants
the wider patterns, since its neighbours and destination may be wider - which is
why the first attempt (replacement rather than union) cost two rows.

Two more corrections came from looking at what the remaining gaps actually were:
the boundary set is chosen by the **name's** type token even when the row's
semantic model is not a value class (an f32 compare-and-swap is an atomic, but its
operands are still f32), and the texture-atomic family belongs in the same
group as the other atomics.

The 64-bit integer compares are two pair operands, exactly like a binary f64, but
the bench's own set is f64-only for the four-dword shape.  @@case_targets()@@
therefore admits them without moving @@targets()@@ (still **308**) - which is the reason
the two functions exist separately.

The last shapes are the six-dword rows - a 64-bit compare-and-swap or a binary
f64 multiply-add, i.e. three pair operands.  The grid now anchors on PAIRS when
its column list is tuple-valued, and a 64-bit operand routes through the pair path
in every family (including the @@b64@@ atomics, whose names carry no type token).

A row may also name **more than one width** (@@v_fcvt_f32_f16@@,
@@v_dp4a_i8_i8_i32@@).  The audit asks for every tag's classes, so the boundary
list is a UNION over the row's type tags rather than the last tag's list alone: a
32-bit class is matched on the whole word and a 16-bit one on either half, so one
column can carry both.

A row whose source is 64-bit can still name 32-bit tags (@@v_fcvt_f32_f64@@), so the
pair list also carries those tags' boundaries as a pair's low word.

The sub-word loads turned out to be the same kind of omission: their carriers
**already** stage the memory content from the case's first input (the LDS path
stores it first, the flat and buffer paths preload it), so a boundary grid
exercises exactly the classes the element width admits.  Their second input is the
address offset, which must stay inside the carrier's window and is therefore held
at zero.  Coverage read **765 of 768** at the time; **as of 2026-10-13 it is 768 of
768 rows accounted for - 738 value-expressible with every required class and 30
launch-shape** - and the earlier figure is kept rather than overwritten.  Both
current measures are anchored in `check_doc_numbers.py`, so this sentence cannot
drift again without the suite saying so.

The plain-width (@@bNN@@) rows were the last omission of the same kind: their
content is staged like the sub-word loads', and once the audit also asks for the
raw bit patterns a @@bNN@@ operand admits, they need the same treatment.  With that
**every row whose boundaries are expressible as an input value is covered**:
@@case_coverage.py@@ now reports **738 of 738 value-expressible rows**, 0 gaps.

The **30** that remain are **launch-shape** rows, and the auditor names them
explicitly rather than counting them as gaps: the execution-environment and SIO
reads, the geometry-stream emits, the interpolators (a launch-owned coefficient
window) and the index read.  Their result is launch state, not an operand, so they
need a launch shape rather than a case.  The vector<->scalar moves are NOT in that
set - their carrier stages the value, so their boundaries are ordinary input
coverage.

## 4. The boundary cases changed the answer the hardware gives

Running the expanded set on the compute pipe surfaced **78 rows where the oracle
and the hardware disagree** (previously 1).  They are not random: the boundary
values are exactly where the oracle's floating-point MODEL is wrong.

| behaviour | what the hardware does | evidence |
|---|---|---|
| denormal **inputs** | read as zero (DAZ) | `v_cmp_eq_f32(0.0, min_denormal)` is **true** on hardware, false in the oracle |
| denormal **outputs** | flushed (FTZ) | `v_mul_f32(min_normal, 0.5)` is **0**, not the denormal `2^-127` (`0x00400000`) |
| NaN sign | the source NaN's sign is propagated | `v_add_f32(+0.0, 0xffc00000)` is `0xffc00000`, the oracle says `0x7fc00000` |

**Caveat (2026-10-13).**  That NaN row is a hardware OBSERVATION from the early run, and the oracle still disagrees with it: the current oracle canonicalises that input to `0x7fc00000`.  The grid was regenerated AFTER the hardware matrix and has never been run, so the top row above is evidence about the DEVICE while the generated expectation is only the oracle.  Re-measurement decides, and the affected clauses are left open (register entry 36).

So the hardware runs **round-to-nearest-even with DAZ and FTZ**, while the oracle's
default policy is plain `rtne`.  The case protocol already carries an FP policy
(`CASE_FP(id, "rtne|daz|ftz", ...)`): the generator now emits it for every
floating-point row, which fixed the arithmetic rows' denormal cases.  The FP
**compare** oracles do not consult the policy yet (`oracle_cmp_*` take only the
two words), so they still disagree on denormal inputs - that is the remaining fix,
together with the NaN-sign rule.

## 5. The emitted policy is now declared everywhere it matters

A case's `CASE_FP` policy is what tells the reference how to model the hardware's
flush, so a floating-point row whose cases omit it is silently evaluated as
"round to nearest, no flush".  The 89 compare rows - every `v_cmp_*` and its
immediate form - were in exactly that state, so they now carry the measured
`rtne|daz|ftz`.  No expectation changed, which is itself the finding: **none of
the hand-written compare vectors had ever used a denormal**, so that cross-check
came only from the generated grid.

case_coverage.py now reports this every run - "FP rows: N declared a policy, M did NOT" - so it cannot drift back: a floating-point case with no policy is silently evaluated as round-to-nearest with no flush, which is not the measured mode and would disagree with the device on any denormal.  The current run is 173 and 0.

The rounding DIRECTION is a different matter, and the reason is now sharper than
"the modifiers are fixed".  The direction is chosen by the **opcode**: the producer
emits `v_ceil.f32`, `v_floor.f32`, `v_trunc.f32` and `v_rndne.f32` as four distinct
instructions, and in two whole linked programs (the runtime libraries included,
~30,000 lines each) the printed modifiers are **`bl1` only** - every occurrence of
`mod2`, `clmp1`, `dmask` and `un` is absent, meaning the producer emits them all at
zero.  So there is no field a case could drive to change the rounding direction: the
instruction IS the choice, and a case picks it by picking the mnemonic.  A row whose
hardware value does not match any instruction's (the v_sqrt approximation) still
works around it by choosing the policy whose reference result equals the observed
value.  What `mod2` means when it is NOT zero remains a hardware item.

The policy TOKENS are a third thing, and they are measured rather than assumed
(`code/lg200/fp_mode_audit.py`, 2026-10-13): of the 173 rows with `CASE_FP` vectors,
`rtne` appears in all 173, `ftz` in 92, `daz` in 91, `rtz` in 28, `rtup` in 28 and
`rtdn` in 28 - the rounding-sensitive arithmetic rows, whose `rtne` cases were mirrored
under the other three modes on 2026-10-13, before which `rtz` and `rtup` appeared on one
row and `rtdn` on none - so 145 rows have no non-default rounding case and 81 no flush case.

Those missing tokens are deliberately **not** counted as gaps, and the reason is a
carrier boundary rather than an oversight: the PS carrier runs a fixed floating-point
default (RN + denormal-flush + positive NaN) and ignores the tokens - stated in its own
rows (`v_add_f32.cpp`, `v_mul_f32.cpp`) - so a case that changed only the token could
not produce a different result there.  Filling them in would add cases that cannot
differ, which is the opposite of coverage.  Where a carrier CAN set the mode - the
compute path - is an open question and needs a device; it is the same question as
`mod2` above, seen from the policy side.

## 6. Status

| measure | value |
|---|---|
| rows exercising every required class | **738 / 738 value-expressible** (2026-10-13) |
| rows still with gaps | **0** |
| launch-shape rows with no case by construction | 30 (they need a launch state; listed by case_coverage.py) |
| hardware rows failing under the expanded set | 78 (measured earlier, before the boundary fill; the device is parked, so this number is stale and is the next hardware task) |

A correction to the record.  The boundary cases added on 2026-10-13 were generated by
an audit that read only `CASE_FP` lines and missed the plain `CASE(...)` vectors, which
are most of a row's inputs.  Those vectors already covered the classes, so the additions
are redundant with existing coverage rather than new: they are kept because they are
correct and explicitly named, but they are not evidence that the classes were missing.
The audit now parses through `cases.py` like every other consumer, and it agrees with
case_coverage.py: 166 of 168 floating-point rows complete, the two exceptions being the
interpolation forms, which are launch-shape rows and take a launch state instead.

The first row read 499/768 when this table was written, and 269 rows had gaps.  The
floating-point work then filled the value classes row by row.  Two corrections during
that work are worth keeping: `case_coverage.py`'s class list was COARSER than this
page's own requirement, so it reported zero gaps while no row had ever seen a signalling
NaN or a finite limit; and the audit classified every case word as 32-bit, so a 16-bit
NaN read as an ordinary number.  Both now use the finer classes - quiet and signalling
NaN, the largest finite value, the smallest normal one - and both report 173 of 173
floating-point rows complete.

The 78 failures are the input to the next fix; until the oracle models DAZ/FTZ in
the compares and the NaN-sign rule, the boundary suite is expected to be red.
