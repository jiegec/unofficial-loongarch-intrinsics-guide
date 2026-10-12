# UNRESOLVED status (LG200) - phase-3 writeback tracking

This page tracks the LG200 UNRESOLVED set and what has been closed.  Behaviour
that is not established is still written as `UNRESOLVED` in place; the rule is
unchanged (DEVELOPMENT.md).  What changes is that an entry leaves the set only
with **evidence**, and the evidence is named here.

## 1. Inventory (2026-10-12)

| carrier | markers | note |
|---|---|---|
| `code/lg200-instructions.tsv` `description` cells | 737 rows | the authored source (`main.py` only reads it) |
| `code/lg200/*.h` / `*.cpp` | 87 | oracle headers and carrier notes |
| `docs/gpu/**` | ~40 | prose pages |
| other (`DEVELOPMENT.md`, `main.py`, ...) | ~24 | - |

Inside the 737 description cells there are **872 distinct UNRESOLVED sentences**
(1,511 occurrences), i.e. the same unknown is restated on every family member it
applies to.  Closing one sentence closes every row that carries it.

## 2. Closed by the loonggpu probe campaign

| # | unknown | rows | evidence | date |
|---|---|---|---|---|
| 1 | "one bit per lane or a replicated all-ones value" | **71** | **hardware**: the `v_cmp_eq_i32` case set passes at 4, 8 **and** 16 lanes on the CS carrier, so the destination narrows to `(1<<lanes)-1` at each lane count; a replicated all-ones value would return `0xffffffff` everywhere and is excluded.  Consistent with the row's own header ("writes the lane mask") and with LCO's model (`lco_abi.h`: a lane-mask register is 32 bits, one register = one wave). | 2026-10-12 |
| 2 | `INST_HI[15:0]` ("whose role is UNRESOLVED") | 49 | **static**: the field is the named operand `X0_16` (ISA layout).  Neither the closed producer nor any u-l-i-g case sets it: all ~350 `load.sm`/`store.sm` instances in the producer's own `ockl` bytecode carry no immediate operand, and every `load_sm_*` case passes a zero second input.  So it is present-but-unexercised; recorded as such rather than guessed. | 2026-10-12 |

| 3 | "The destination word above bit 15 ... are UNRESOLVED" | **20** | **hardware**: the 16-bit compare case set passes unchanged at 4, 8, 16 and **32** lanes, and the 32-lane all-true expectation is `0xffffffff` - so bits above 15 carry lanes 16-31, they are not cleared. | 2026-10-12 |
| 4 | "Whether a signalling NaN differs from a quiet NaN is UNRESOLVED" | **20** | **hardware**: +qNaN (`0x7fc00000`), +sNaN (`0x7f800001`) and -sNaN (`0xff800001`) take the identical branch - `v_cmp_lt_f32`/`v_cmp_o_f32` return 0, `v_cmp_u_f32` sets every lane - while a non-NaN operand takes the other branch.  Pinned as `CASE(snan, ...)` vectors in those three mnemonics; offline `make verify` passes (768 instructions, 20732 vectors). | 2026-10-12 |

| 5 | "The values exercised at the operand width are UNRESOLVED" | **14** | **static**: the row's own vectors already carry the width extremes (`0xffffffff`, `0x80000000`/`0x7fffffff`, the u64 wrap); the exhaustive grid excludes `atomic_*` as side-effecting (`gen_cases.py`), so the coverage is the case list, not the grid. | 2026-10-12 |
| 6 | "Lanes reaching one address are serialised in an undefined order; which intermediate value a lane observes is UNRESOLVED" | **20** | **by design + hardware**: the per-lane intermediate value is *architecturally undefined*, not merely unmeasured - the reference carrier leaves the returned old value unverified across lanes and compares only the post-atomic cell.  Measured: the same vectors pass unchanged at 1, 2, 4 and 8 colliding lanes. | 2026-10-12 |

| 7 | "Whether a signalling NaN is quieted or raises a flag" | **30** | **hardware + ISA**: a compare yields a mask, not an FP value, and that mask is identical for sNaN and qNaN (measured); flag raising is *not observable* - no instruction reads the FP exception flags (`flag` only updates flag state, `getreg` reads environment registers). | 2026-10-12 |

| 8 | "the destination's upper 16 bits" / "whether the mask reads as an ordinary scalar" / "whether a signalling NaN compares differently" (seven wordings) | **39** | **hardware + the row's own Semantics**: the destination is a full 32-bit lane mask (one bit per lane, `0xffffffff` at 32 lanes), so bits 31:16 are *not* cleared; the mask *does* read as an ordinary scalar (the destination may name a scalar register, and the vendor backend encodes `RD8 = 6` for `s6`); and sNaN does not compare differently from qNaN. | 2026-10-12 |
| 9 | "The encoded predicate is shared with another family member" | **14** | **static, disproved**: every member of the `v_cmp_o_*` / `v_cmp_u_*` / `v_cmp_tru_*` family has its own fixed encoding - 14 distinct fixed words, **0 collisions** - so the instruction word alone determines which behaviour executes. | 2026-10-12 |

Rows 2 and 5 are *scoping/coverage* resolutions; rows 1, 3, 4, 6, 7, 8 and 9 are behaviour
resolutions, all with hardware evidence except rows 2 (a closed-producer census) and 9
(a fixed-encoding uniqueness proof).

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

Still open in the code layer: `load_sm_matrix_x{1,2,4}` (plane/lane layout) and
`load_v_buff_fmt*` / `store_v_buff_fmt*` (descriptor format decode), 19 lines
in total.  Offline `make verify` passes throughout (768 instructions, 20732
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

The rest of the 872 sentences are grouped by family in the accompanying
inventory (loonggpu `tmp/unresolved-inventory.tsv`).  The largest repeated
classes still open are: signalling-vs-quiet NaN, buffer/texel descriptor
format decode, out-of-bounds access behaviour, operand-width extremes, and
predicate-selector encoding - most of which need the PS single-mnemonic runner
or a closed-capture census rather than prose.
