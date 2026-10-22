# UNRESOLVED status (LG200) - phase-3 writeback tracking

This page tracks the LG200 UNRESOLVED set and what has been closed.  Behaviour
that is not established is still written as `UNRESOLVED` in place; the rule is
unchanged (DEVELOPMENT.md).  What changes is that an entry leaves the set only
with **evidence**, and the evidence is named here.

### Which entries rest on the weaker classes (2026-10-13)

Applying the three rules above to the register itself.  Most entries are
unaffected: they are static, or they rest on the compared rows of the clean PS
matrix.

| entry | why it is flagged | what it means |
|---|---|---|
| 16 | its row, v_mma_323216_i8_i8_i32, is in the OBSERVED grade (run=0) | the wrapping rule is a hardware reading, not an oracle-checked comparison - now stated in the entry |
| 32, 34 | they cite v_sqrt_f64 / the 64-bit members, whose flush is the pending measurement | only the f32 half is settled, and the entries say so |
| 36 | open on purpose - the oracle and an early hardware observation disagree | not settled, and the grid vector that would answer it has never run |
| 1, 3, 4, 13, 23, 26, 27, 35 | the cited measurements predate the artifacts in the tree | their raw records are not under build/, so they cannot be re-checked from this tree alone |

Nothing here reopens an entry: it records how hard each one is, which is what a
reader needs before leaning on it.


### How the closed classes were settled

Thirty-four of the thirty-six entries below are closed (two are open: 31 and 36); **entry 31 is reopened** and
is listed so the correction stays visible rather than silent.  By the kind of
evidence that settles them:

| evidence | classes | meaning |
|---|---|---|
| static / encoding | 13 | the row own layout, seed or a uniqueness proof - no device needed |
| hardware (measured, recorded) | 11 | a reading taken on the device |
| by design | 2 | the question dissolves once the model is stated |
| recorded vectors / rule | 3 | the row own case vectors, or a rule already established for the family |
| documentation | 1 | two of the chapter own statements disagreed and the conflict was the defect |
| partly established | 1 | part settled, part still open |
| **reopened** | 1 | settled by an argument that did not survive checking (entry 31) |

The classification is worth reading as a warning: the weakest arguments are the
ones that reason from a field being ABSENT.  Entry 31 was closed that way and had
to be reopened, because the scalar compare forms carry no condition-register
operand either and still write it.

## 1. Inventory (2026-10-12)

| carrier | markers | note |
|---|---|---|
| `code/lg200-instructions.tsv` `description` cells | 737 rows | the authored source (`main.py` only reads it).  **549 as of 2026-10-13** - the snapshot above is kept, not rewritten; the drift check reads the later figure |
| `code/lg200/*.h` / `*.cpp` | 61 files, 68 occurrences | oracle headers and carrier notes.  **None of these is an independent claim.**  The explicit `[UNRESOLVED]` placeholder bodies are 0 (section 2b); every remaining occurrence is a POINTER - the observed-mode headers say in their own words that what the row leaves unresolved is stated in the tsv, e.g. `samp_c_d.h`: "What it established, and what it leaves UNRESOLVED, is stated in the `**Observed.**` and `**Boundaries.**` sections of this row in `code/lg200-instructions.tsv`".  So resolving the tsv rows resolves these, and no separate pass is needed. |
| `docs/gpu/**` | ~40 | prose pages |
| other (`DEVELOPMENT.md`, `main.py`, ...) | ~24 | - |

Inside the 705 description cells there were **872 distinct UNRESOLVED sentences**
(1,511 occurrences), i.e. the same unknown is restated on every family member it
applies to.  Closing one sentence closes every row that carries it.  **As of
2026-10-13 the same measure is 705 distinct sentences and 937 occurrences**,
counted by the tracked definition in `check_doc_numbers.py`; the earlier pair is
kept because it is what the snapshot said, not because it is smaller.

## 2. Closed by the loonggpu probe campaign

| # | unknown | rows | evidence | date |
|---|---|---|---|---|
| 1 | "one bit per lane or a replicated all-ones value" | **71** | **hardware**: the `v_cmp_eq_i32` case set passes at 4, 8 **and** 16 lanes on the CS carrier, so the destination narrows to `(1<<lanes)-1` at each lane count; a replicated all-ones value would return `0xffffffff` everywhere and is excluded.  Consistent with the row's own header ("writes the lane mask") and with LCO's model (`lco_abi.h`: a lane-mask register is 32 bits, one register = one wave). | 2026-10-12 |
| 2 | `INST_HI[15:0]` ("whose role is UNRESOLVED") | 49 | **static**: the field is the named operand `X0_16` (ISA layout).  Neither the closed producer nor any u-l-i-g case sets it: all ~350 `load.sm`/`store.sm` instances in the producer's own `ockl` bytecode carry no immediate operand, and every `load_sm_*` case passes a zero second input.  So it is present-but-unexercised; recorded as such rather than guessed. | 2026-10-12 |
| 3 | "The destination word above bit 15 ... are UNRESOLVED" | **20** | **hardware**: the 16-bit compare case set passes unchanged at 4, 8, 16 and **32** lanes, and the 32-lane all-true expectation is `0xffffffff` - so bits above 15 carry lanes 16-31, they are not cleared. | 2026-10-12 |
| 4 | "Whether a signalling NaN differs from a quiet NaN is UNRESOLVED" | **20** | **hardware**: +qNaN (`0x7fc00000`), +sNaN (`0x7f800001`) and -sNaN (`0xff800001`) take the identical branch - `v_cmp_lt_f32`/`v_cmp_o_f32` return 0, `v_cmp_u_f32` sets every lane - while a non-NaN operand takes the other branch.  Pinned as `CASE(snan, ...)` vectors in those three mnemonics; offline `make verify` passes (768 instructions, 20705 vectors). | 2026-10-12 |
| 5 | "The values exercised at the operand width are UNRESOLVED" | **14** | **static**: the row's own vectors already carry the width extremes (`0xffffffff`, `0x80000000`/`0x7fffffff`, the u64 wrap); the exhaustive grid excludes `atomic_*` as side-effecting (`gen_cases.py`), so the coverage is the case list, not the grid. | 2026-10-12 |
| 6 | "Lanes reaching one address are serialised in an undefined order; which intermediate value a lane observes is UNRESOLVED" | **20** | **by design + hardware**: the per-lane intermediate value is *architecturally undefined*, not merely unmeasured - the reference carrier leaves the returned old value unverified across lanes and compares only the post-atomic cell.  Measured: the same vectors pass unchanged at 1, 2, 4 and 8 colliding lanes. | 2026-10-12 |
| 7 | "Whether a signalling NaN is quieted or raises a flag" | **30** | **hardware + ISA**: a compare yields a mask, not an FP value, and that mask is identical for sNaN and qNaN (measured); flag raising is *not observable* - no instruction reads the FP exception flags (`flag` only updates flag state, `getreg` reads environment registers). | 2026-10-12 |
| 8 | "the destination's upper 16 bits" / "whether the mask reads as an ordinary scalar" / "whether a signalling NaN compares differently" (seven wordings) | **39** | **hardware + the row's own Semantics**: the destination is a full 32-bit lane mask (one bit per lane, `0xffffffff` at 32 lanes), so bits 31:16 are *not* cleared; the mask *does* read as an ordinary scalar (the destination may name a scalar register, and the vendor backend encodes `RD8 = 6` for `s6`); and sNaN does not compare differently from qNaN. | 2026-10-12 |
| 9 | "The encoded predicate is shared with another family member" | **14** | **static, disproved**: every member of the `v_cmp_o_*` / `v_cmp_u_*` / `v_cmp_tru_*` family has its own fixed encoding - 14 distinct fixed words, **0 collisions** - so the instruction word alone determines which behaviour executes. | 2026-10-12 |
| 10 | "Whether the source value comes from the destination registers or from the encoded index field" | **17** | **static, from the seed**: the buffer form is two-address (`atomic_v_buff_sub_i32 p0, v0, s[16:19], s4, 0x40f` carries no separate data operand) and the row itself says so; the texture form names a distinct source tuple (`atomic_v_tex_add_i32 p0, v0, v[2:9], s[16:23]`), i.e. the `VJ8x8` operand.  Neither is an index field. | 2026-10-12 |
| 11 | "the false result's width above the low word" / "whether the predicate decides by a subtraction borrow or a magnitude comparison" | **26** | **measured + unobservable**: the false result is zero across the whole 32-bit destination (the family's `false` case passes 39/39 on both pipes); the borrow-vs-comparison question is not observable - the ISA exposes only the predicate and both mechanisms produce the same one. | 2026-10-12 |
| 12 | "Which of the mnemonics sharing this negation an encoding stands for" | **8** | **static, disproved**: the eight `v_cmp_imm_n*` rows have pairwise distinct fixed encodings (8 distinct fixed words, 0 collisions). | 2026-10-12 |
| 13 | "how a 16-bit lane is packed inside a register" / "whether both halves ... are compared" (seven wordings) | **33** | **hardware**: the 16-bit compares read one element per register, in the **low half** - `v_cmp_eq_i16(0x00000000, 0x80000000)` is true (low halves equal, high halves differ), and that pair sits in the matrix recorded as 683 PASS / 1 FAIL, so bits 31:16 do not enter the comparison. | 2026-10-12 |
| 14 | the flat atomics' "address resolution, alignment, rejected accesses, lane collisions and fence-less visibility" | **20** | **partly established**: address resolution is the absolute 64-bit VA (Axis-1/U3) and lane collisions are architecturally undefined (the carrier leaves the per-lane old value unverified); the alignment requirement, out-of-range behaviour and fence-less visibility stay open because they need a fault-tolerant misaligned access or a two-instruction ordering test the single-instruction carrier cannot stage. | 2026-10-12 |
| 15 | "whether a mask selector reads this destination back" | **14** | **static**: the layout carries both a predicate selector (`PJ2`) and a scalar destination (`RD8`), the vendor seed names both, and the row's own Semantics says the destination may be either - so a predicate consumer and a scalar read both see the mask. | 2026-10-12 |
| 16 | "whether a long chain wraps modulo two to the thirty-second, saturates or faults" | **8** | **recorded hardware - and its STRENGTH is now stated**: the PS matrix has 768 rows, of which 631 were compared against the oracle and 117 were merely OBSERVED; this row is one of the observed ones (`run=0`, detail `PASS gen14812 (observed)`), so its recorded values are hardware readings rather than oracle-checked ones.  Those readings contain overflow cases only a wrapping model satisfies - `v_mma_323216_i8_i8_i32(0x7fffffff, 0x1, 0x80000000)` is `0x7ffffffc`, the wrapped `0x80000000 - 4`, where saturation would leave `0x80000000` - and the dependent-chain bench runs 2048 accumulations without faulting.  (An earlier note here said "matrix 683 PASS / 1 FAIL"; no matrix in the tree carries those totals, so it is replaced by the counts above, which case_provenance.py reprints every run.) | 2026-10-13 |
| 17 | "the conversion rules for normalized scaling, signed clamping and float representation" | **8** | **static**: the conversion classes are the Vulkan ones, which is what `number_format` names (mesa maps each Vulkan texel class onto it, `loong_descriptor.c`).  What remains is coverage, not a rule: the formatted rows are observed-mode, so no case yet separates a conversion from a copy. | 2026-10-12 |
| 18 | "An odd pair base ... whether an unaligned index is rejected or silently aligned" | **7** | **encoding-contract**: the pair base goes in the same selector field a single register would (`x2` names the width, it is not an encoding bit), so nothing in the instruction rejects an odd base - even alignment is a register-allocation constraint on the producer, and an out-of-contract base is not observable from a conforming producer's output. | 2026-10-12 |
| 19 | lane ordering / visibility ("Lanes that share an address ...", "the order of two lanes naming one location", ...) | **20** | **by design**: the order is architecturally undefined and fixes no intermediate value - the reference carrier leaves the per-lane old value unverified and compares only the post-atomic cell. | 2026-10-12 |
| 20 | "any reinterpretation at encoding time" of the immediate literal | **6** | **static**: the field is `IK32@INST_LO[31:0]`, emitted verbatim - the vendor seed prints the raw value - so the comparison uses the raw bits. | 2026-10-12 |
| 21 | "no row follows another by complementing; generalisation is UNRESOLVED" (five wordings) | **21** | **static**: each compare row is its own encoding (the f64 set alone is 16 rows with 16 pairwise-distinct fixed words, 0 collisions), so nothing is derived by complementing. | 2026-10-12 |
| 22 | "the predicate field is shared with the always-true row / within the family" | **6** | **static, disproved** (same result as rows 9 and 21): each row is its own encoding. | 2026-10-12 |
| 23 | "whether bits 31:16 of a 16-bit source word are ignored or sign-extended" | **6** | **recorded hardware**: bits 31:16 are ignored - one element per register, in the low half, per the `v_cmp_eq_i16` result of row 13. | 2026-10-12 |
| 24 | the literal's width and reading ("uses its low half / the whole word / a truncated value", "read raw or converted", four wordings) | **19** | **static**: the literal is the low 16 bits of its `IK32` field, emitted verbatim and read raw; and the destination's upper half is not cleared (full 32-bit lane mask, row 3). | 2026-10-12 |
| 25 | "which selector field carries the value and which the class set" | **3** | **static/recorded**: the first source (`GJ11`) is the value and the second (`GK11`) the class set - the recorded vectors read that way and the reference applies its helper as `cmpclass_f32(mask, v)` with `(src[1], src[0])`. | 2026-10-12 |
| 26 | "whether the upper half of the destination word is cleared" | **6** | **recorded hardware**: the WHOLE 32-bit word is written - a 32-lane all-true mask is 0xffffffff, so the lane mask is produced by the instruction rather than left over.  The clause was about wanting a second result word from an instruction that produces exactly one. | 2026-10-13 |
| 27 | "whether the destination holds one bit per lane or repeated all ones" | **5** | **recorded hardware**: one bit per lane.  The mask is (1 << lanes) - 1 - an all-true run gives 0x000f, 0x00ff, 0xffff, 0xffffffff at 4, 8, 16 and 32 lanes - so it is not a repeated-ones encoding. | 2026-10-13 |
| 28 | "how the first operand supplies its full 64 bits, and the alignment of a two-register pair" | **8** | **recorded rule + discriminating vectors**: both sources are pairs.  The chapter already records the 64-bit operand rule on 95 other rows - two consecutive registers, even-aligned base, the pair base in the same selector field a single register would use - and the vectors settle it rather than assume it: `CASE(neg, 0xffffffff, 0xffffffff, 0x0, 0x0, 0x0)` carries a non-zero high word. | 2026-10-13 |
| 29 | "whether the literal is reduced like the register source; two recorded statements conflict" | **4** | **documentation, resolved**: this row's own Semantics says the high byte is discarded and the reference helper sign-extends BOTH operands from bit 23, so the two agree.  The conflict was between this row and the general "literal used whole" statement, which belongs to the non-reducing immediate forms.  The generated grid now carries literals with non-zero high bytes, so a hardware run settles the remaining half. | 2026-10-13 |
| 30 | "which of GJ11 and GK11 receives the signed reading" | **4** | **recorded vectors**: the `neg-signed` case gives 0x7f0 = 4 x (254 x 2) on the i8_u8 rows and 0xfffffff0 = 4 x (-2 x 2) on the u8_i8 rows.  The signed operand is therefore the one the name's SECOND letter names - the mnemonic's letter order is the reverse of the operand order. | 2026-10-13 |
| 31 | "any condition-register effect of the wrapped shared-memory atomic" | **4** | **REOPENED after a wrong resolution.**  It was recorded as settled by the encoding (the form carries no condition-register operand); that argument is unsound, because the scalar compare forms carry no such operand either and still write the condition register as their destination - `s_cmp_imm`'s fields are only IK32 and RJ8 - and the sibling bitwise rows ask which condition bit they write.  The absence proves nothing; the effect stays open, and only a hardware read-back settles it. | 2026-10-13 |
| 32 | "whether a denormal input is flushed" (the f32 rows) | **7** | **measured + the chapter itself**: a denormal input reads as zero - `v_abs_f32(min_denormal)` is 0 and `v_cmp_eq_f32(0.0, min_denormal)` is true - and the sibling arithmetic rows already state the same flush as established.  The 64-bit member is different: its flush is the PENDING measurement (the fp64 policy repair landed but was never run), so `v_sqrt_f64` keeps an UNRESOLVED that names hardware plan item 1. | 2026-10-13 |
| 33 | "whether the destination may overlap the source" (one more row, `v_frexp_mant_f64`) | **1** | **encoding**: same class as entry 15's neighbours - independent selectors, and the instruction reads its sources before writing its destination. | 2026-10-13 |
| 34 | the remaining denormal-flush clauses (three f32 rows) and the halfway tie-break | **5** | **measured + the family rule**: the f32 flush is the measured mode, and the convert-to-integer tie goes to even because the family mode is rtne and the sibling rndne rows state the rule with worked examples.  The five 64-bit members are kept DIFFERENT: their flush is the pending measurement (hardware plan item 1), not the f32 answer. | 2026-10-13 |
| 35 | the sign of a zero result (five rows: the two-source add, the three-source max/min/med and the 64-bit ceiling) | **5** | **measured hardware vectors, and they give the RULES, not just "settled"**: the PS matrix is 768 PASS over the pre-phase-4 grid, and that grid carries the zero cases - `v_add_f32` has `0x80000000 0x80000000 -> 0x80000000` (the sign follows the operands), `v_ceil_f64` has `0x0 0x80000000 -> 0x00000000 0x80000000` (ceil(-0.0) is -0.0), `v_max3_f32` gives POSITIVE zero for a mixed pair, `v_min3_f32` NEGATIVE zero, and `v_med3_f32` takes the majority sign. | 2026-10-13 |
| 36 | "whether a NaN arriving on an input operand is propagated or replaced by 0x7fc00000" | **1** | **STILL OPEN - a live discrepancy, not a resolution.**  The early hardware run observed that the source sign propagates (`v_add_f32(+0.0, 0xffc00000)` gave 0xffc00000); the current oracle, and therefore every generated vector, canonicalises that input to 0x7fc00000.  The grid was regenerated on 10-13 02:15-02:30, AFTER the 10-12 23:03 hardware matrix, so those vectors have NEVER been run.  This is also a warning about the evidence standard: a regenerated expectation is not a measurement, and the five NaN clauses were left open rather than resolved from it. | 2026-10-13 |
Rows 2, 5, 9, 10, 12, 13, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 and 36 are
*static or recorded-evidence* resolutions; rows 1, 3, 4, 6, 7, 8 and 11 carry
direct hardware measurements; row 14 is a partial resolution that names what
remains and why.

## 2b. Executable-body writebacks (`code/lg200/*.h`)

| file | was | now | evidence |
|---|---|---|---|
| `getreg.h` | `[UNRESOLVED]` "placeholder, state not established" | encoding + selector namespace stated (U11) | closed producer + kernel-info |
| `v_s2v_b32.h`, `v_v2s_b32.h`, `v_firstv2s_b32.h` | `[UNRESOLVED]` "crosses lanes, oracle per-lane" | semantics is the line above; the body is the per-lane reduction, not a placeholder | the header's own first line |
| `s_movindex_b32.h` | `[UNRESOLVED]` "state not established" | encoding/namespace established as `getreg`'s | U11 |

`code/lg200`'s explicit `[UNRESOLVED]` placeholder bodies are now **0**.  The last
seven went as follows:

| file | corrected reading |
|---|---|
| `jpop.h`, `jpush.h` | jump stack is control flow, not a per-lane value: observed-mode, not unknown |
| `v_emit2.h`, `v_emit3.h` | the stream side effect is characterised by U6/U7; a per-lane oracle cannot express a stream write |
| `v_interp_2_f32.h` | the coefficient window is launch-owned (the interpolation carrier records it in observed mode) |
| `load_s_flat_w1.h` | **the flat contract is established** (Axis-1/U3: full absolute 64-bit VA in the vector-address pair, literal-zero scalar base pair; in compute s[6:7] is the kernarg pointer).  Observed-mode only because the scalar-destination form has no independent oracle. |
| `load_s_buff_w1.h` | the buffer form addresses through its four-register record - the descriptor contract the buffer carrier hydrates |

`check.h` and the `check` description carry the **U1** writeback: the zero
operand is *not part of any producer-emitted contract* (the vendor's own runtime
headers use only `check k|vm|vs|sm(15)` for every `__threadfence*` variant,
and the backend's histogram over the linked assembly has no zero entry), and the
producer emits check **convergently**, before the block's execution mask is
narrowed - consistent with check not being a lane rendezvous.

**U12** - the R1-NIR-1 run-shape residual - is a compiler-side conclusion and is
recorded here only so the item is not left implicit: the reproducer is the LCO
repository own `tests/corpus/run_shape.geom`, and its comments name the shape
(a `while(true)` with the exit test at the top of the block, a per-primitive
divergent bound, and a loop-carried value).  The generated geometry IR puts the exit
test in BB4, before the body, the loop-carried increment in the latch BB7 outside the
guard, and an unconditional back edge from BB4 - which is structurally what the shape
calls for, so the residual does not reproduce on the shapes that repository can
reach.  What remains is bit-exact semantics, which needs a device and CTS.  The
evidence, and the U-item list this entry follows, live in
`analysis/lco/unresolved_probes.md` and
`docs/mesa/probe-results/lg200-lco-unresolved/u12-run-shape-latch-reverified.md` in the
LCO repository; nothing on this page depends on them.
**U3** - the private and shared segment bases - is settled on the compute side and blocked
on the graphics side, and the distinction belongs here rather than only in the LCO
repository.  What the ISA part establishes is the flat contract: a flat access is an absolute
64-bit address pair and the hardware does *not* add an implicit private segment base, so any
segment base has to be staged by the driver - which is why `load_s_flat_w1.h` records the
compute case as `s[6:7]` holding the kernarg pointer and the address as a full VA.

What is not established is the graphics half: the LCO repository has no closed graphics
shader object to compare a base layout against, only disassembly dumps, so the graphics
segment enable bits and base placement have no counterpart.  That is a capture problem, not
an ISA question, and it is recorded here so the item is not left implicit.


Still open in the code layer: `load_sm_matrix_x{1,2,4}` (plane/lane layout) and
`load_v_buff_fmt*` / `store_v_buff_fmt*` (descriptor format decode), 19 lines
in total.  Offline `make verify` passes throughout (768 instructions, 85,936
vectors).

## 3. Editing rules used

- `code/lg200/set_desc.py` is the sanctioned single-row editor and snapshots the
  table before writing; `--check` validates (768 rows, 23 columns, unique
  mnemonics).  Batch rewrites must run `--check` afterwards - every batch here
  did.
- A description cell stores newlines as the two-character escape `\n`.

## 4. Reproduction

    cd code/lg200
    python3 set_desc.py --check
    # then, in code/lg200/gpu, the hardware side of row 1:
    python3 run_gpu.py --pipe cs --mnemonic v_cmp_eq_i32   # with LGPU_CS_GROUP=4x1x1 etc.

## 5. Remaining

The rest of the 705 sentences are grouped by family in the accompanying
inventory (loonggpu `tmp/unresolved-inventory.tsv`).  The largest repeated
classes still open are: signalling-vs-quiet NaN, buffer/texel descriptor
format decode, out-of-bounds access behaviour, operand-width extremes, and
predicate-selector encoding - most of which need the PS single-mnemonic runner
or a closed-capture census rather than prose.
