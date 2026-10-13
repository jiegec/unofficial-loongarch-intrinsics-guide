# Hardware session plan (after the wedge)

The device is parked: `dmesg` shows `GPU reset(2) failed` and
`ring gfx parked / xdma0 parked / bpipe parked`, with `the CP latch needs
platform power (S3/reboot)`.  Nothing here can run until the platform is power
cycled.  This page is the sequence to follow when it is, so the first run after
recovery is safe and informative rather than another sweep into a wedge.

## 0. What went wrong, so it is not repeated

A full CS matrix run wedged on the **`trap` control row**: the runner reported
`trap HANG/FAULT ... ring=0x100020000 wptr=192`, the device then vanished mid-run
(every later row got `@cannot open /dev/dri/renderD128`), and the kernel gave up
after two reset retries.

`trap` is a **terminating** control row - it "raises the encoded shader trap and
transfers control to the trap path".  It had run inside earlier sweeps, so this is
not a deterministic fail, but it is the only row in the universe that deliberately
diverts control, and it must not be treated like the others.

**Rules that follow:**

1. `trap` is never part of a batch again.  It runs last, as a single case, with
   the device state checked before and after.
2. No blind retry.  A row that faults is recorded and skipped, not re-submitted.
3. Every row gets a timeout; a timeout is a fault for scheduling purposes.
4. **Never pack more than a few dozen cases into one process.**  A 1102-case batch
   and then a 144-case batch both drew `program or result window exceeds the
   persistent cache size`.  After the second, the device was parked and even the
   one-case smoke test failed.  That message is a device-level warning, not a
   per-batch rejection: stop and re-run the preconditions, do not split the batch
   and retry.  That misreading is what wedged the device on 2026-10-13.

## 1. Preconditions before any submission

    dmesg | grep -c parked                                # must be 0
    ls /dev/dri/renderD128                                # must exist
    PKG_CONFIG_PATH=/home/lin/work/build/libdrm-loonggpu-prefix/lib/pkgconfig \
      make -C code/lg200/gpu build/lgpu-gpu                # must be up to date
    ./code/lg200/gpu/build/lgpu-gpu csverify              # host-only, must pass
    printf 'x\t\t0x1\t0x1\n' > /tmp/one.txt
    ./code/lg200/gpu/build/lgpu-gpu csrun s_abs_i32 < /tmp/one.txt   # must PASS first

If the one-case smoke test fails, stop - the device is not usable.

Two of those lines were wrong as first written, and both would have failed at the
session: the parked check pointed at `/proc/...`, a placeholder, and the build line
omitted the `PKG_CONFIG_PATH` that `code/lg200/gpu/README.md` records as necessary
(the deb-staged library is older and lacks `loonggpu_userq_free`, so a plain `make`
fails at link and removes the binary).

## 2. Recovery ladder

| step | run | stop if |
|---|---|---|
| 1 | one row, one case (`s_abs_i32`) | anything but PASS |
| 2 | one row's full vector (`v_cmp_eq_i16`) | any FAULT, or the device disappears |
| 3 | the non-control half of the matrix | `dmesg` gains a fault line |
| 4 | the control rows **except** `trap`, one process | any FAULT |
| 5 | `trap` alone, last | it is recorded and not retried |

## 3. What the first healthy session should measure

In priority order, each with its safety note.

| # | experiment | resolves | safety |
|---|---|---|---|
| 1 | re-run the CS matrix after the fp64 policy-column repair | the corrected fp64 compare expectations (the repair landed but was never measured) | ladder steps 1-3 |
| 2 | the boundary grid on the FP rows | the DAZ/FTZ convergence: 78 rows disagreed before the flush model, 54 after the f16/f32 compares converged; the arithmetic and bit-op paths are still open (`v_abs_f32(denormal)` is 0, `v_add_f32(+0.0, 0xffc00000)` keeps the NaN sign) | single rows, one process each |
| 3 | `M.mod2` / `M.clmp1` | the operation modifiers. They are emitted at fixed values today, so a case cannot drive the rounding direction; a `v_sqrt_f32` case works around it by choosing the policy whose reference equals the observed value | single rows; changing a fixed field is the riskiest item here, so give it its own process and an explicit before/after device check |
| 4 | `v_fcvt_f16_i16` / `v_fcvt_f16_u16` | whether the destination is written at all - the encodings decode and occupy their slot, and the register has never been read back | single rows; the carrier already runs them observed |
| 5 | **the microbench re-run** | the phase-2 numbers for every template added since the last CSV: **626 rows are buildable now against 497 in it**, so 129 rows would produce a timing for the first time.  That figure was 605 here until the host-only section below was corrected: two piles closed after this table was written, and the count is kept by `bench_doc_check.py` in all three places it appears.  The runner also emits a per-row `note` now, which the CSV carries - the store rows will label their lat column as the store+load round trip it is | the ordinary batch path (bounded streams, per-row timeout); no control rows, no `trap` |
| 6 | a misaligned flat/buffer access | the alignment clause phase 3 left open | **single case, its own process, and expect a fault** - the most likely wedge; run it only after the ladder is proven |
| 7 | two-lane ordering / fence-less visibility | the visibility clauses | needs a two-instruction program the carrier cannot build today |
| 8 | texture out-of-bounds and wrap | the gather/samp boundary class | single rows |
| 9 | the atomic counter bound | where the increment/decrement forms read their bound - no encoded field carries it in the narrow form | single rows |
| 10 | the buffer index/offset registers with `idxen`/`offen` live | whether the buffer form addresses the way the descriptor stride implies.  The correctness carrier exercises neither live: its load keeps the index and offset registers at zero and puts the offset in the immediate.  The remaining memory bench rows (buffer loads, scalar buffer loads) cannot be chained without this - and a wrong index register is an out-of-window access, so it is also a wedge risk | single rows, its own process, one access before any chain |
| 11 | `load_sm_matrix_x{1,2,4}` plane/lane layout | the last three code-layer UNRESOLVED markers | observed mode today; a real measurement needs the matrix file staged |

Items 6 and 7 must be designed as a **new carrier shape** first (the current
carriers deliberately cannot stage them), and reviewed for ring risk before they
are submitted.

## 4. Host-only work that does not wait

The boundary-case authoring is **done**: `case_coverage.py` reports
**738 of 738 value-expressible rows** with every required class and no gaps, plus
30 rows listed as launch-shape (their result is launch state - the environment and
SIO reads, the stream emits, the interpolators and the index read - so they need a
launch state, not a case).

The microbench templates are done for every family this side of the hardware
questions: **626 of 768 rows build now** (`bench_buildable.py`), against 497 in the
last hardware CSV and 605 when this section was first written.  The 605 was stale by
the time it was read, because two piles on it were closed after it was written, and
the texture pile was undercounted - it listed `samp` and `gather` only, leaving out
`atomic_v_tex_*`, `store_v_tex*`, `load_v_tex*` and `image_*`, which the same refusal
covers.  What is left is four piles rather than five:

| pile | rows | why it waits |
|---|---|---|
| texture (`samp`, `gather`, `*_v_tex_*`, `image_*`) | 79 | the correctness carrier refuses them (SKIP-ENVELOPE); a bench must not exercise a stream the validation path rejects |
| SIO and GS-prefix | 20 | their correctness carrier runs them OBSERVED and their chain is not a chain of independent work; 13 of the 17 SIO rows still carry UNRESOLVED |
| buffer and scalar-buffer loads | 16 | the vector forms' chain needs a live index whose next value comes from the loaded result; the scalar forms are staged |
| control, environment, sm load, copy, flat cmpswap, trap, movindex | 27 | four of these need nothing (control, environment) and the rest need semantics or a device |

Two piles from the previous version are **closed**: the scalar buffer loads (all five
widths chain through the loaded word the way the flat scalar loads do) and the
sub-word and formatted buffer stores (b8, b16, and the eight fmtdesc/fmtinst forms,
which stage the compare pair the way the correctness carrier does).

`check_doc_numbers.py` keeps the pages' figures honest in the meantime, and
`bench_doc_check.py` compares this section and the bench page against
`bench_buildable.py`, because this is the second copy of one number.
