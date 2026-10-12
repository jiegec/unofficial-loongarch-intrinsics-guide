# Case boundary coverage

Every row's case vectors must exercise the boundary values its operand types
admit.  This page records what that means, how it is measured, and what the
measurement found.

## 1. The required classes

Derived from the mnemonic's own type suffix, so a mixed row (`v_cvt_f32_i32`)
is asked for both sides.  A floating-point operand must see the whole signed
space: both zeros, both infinities, quiet **and** signalling NaN in both signs,
the finite limits, and the denormal range.  An integer operand must see 0, ±1,
and the width extremes.

| format | classes |
|---|---|
| f32 | `+0` `-0` `+1` `-1` `+0.5` `max_norm` `-max_norm` `min_norm` `-min_norm` `max_denorm` `min_denorm` `+inf` `-inf` `qnan` `-qnan` `snan` `-snan` (19) |
| f16 | the same 19 against the f16 encoding, in the low half of the word |
| f64 | the same set as word pairs |
| i8/i16/i32/i64 | `0` `1` `max` `min` `-2` `-1` |

## 2. Measurement

`code/lg200/case_coverage.py` joins the table's type suffixes with the parsed
case set and reports, per instruction, which required classes its inputs never
take.

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
therefore admits them without moving @@targets()@@ (still 308) - which is the reason
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
at zero.  Coverage is **765 of 768**.

The plain-width (@@bNN@@) rows were the last omission of the same kind: their
content is staged like the sub-word loads', and once the audit also asks for the
raw bit patterns a @@bNN@@ operand admits, they need the same treatment.  With that
**every row whose boundaries are expressible as an input value is covered**:
@@case_coverage.py@@ now reports **738 of 738 value-expressible rows**, 0 gaps.

The 30 that remain are **launch-shape** rows, and the auditor names them
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

The rounding DIRECTION is a different matter.  The 2-bit `M.mod2` and 1-bit
`M.clmp1` fields are the operation modifiers, and the register records that the
carrier emits them at fixed values - so a case cannot yet drive the hardware's
rounding direction.  A row that needs it (the v_sqrt approximation) works around
it by choosing the policy whose reference result equals the observed hardware
value.  Establishing `mod2` is a hardware item, not a case-authoring one.

## 6. Status

| measure | value |
|---|---|
| rows exercising every required class | **499 / 768** |
| rows still with gaps | 269 (dominated by the hand-written families: atomic 206, plus rows with immediate slots) |
| hardware rows failing under the expanded set | 78 (a work list, not a regression: the oracle model is incomplete) |

The 78 failures are the input to the next fix; until the oracle models DAZ/FTZ in
the compares and the NaN-sign rule, the boundary suite is expected to be red.
