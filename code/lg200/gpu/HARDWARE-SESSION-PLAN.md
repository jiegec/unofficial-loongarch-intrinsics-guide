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

## 1. Preconditions before any submission

    grep -c parked /proc/... (or dmesg | grep -c parked)   # must be 0
    ls /dev/dri/renderD128                                  # must exist
    make -C code/lg200/gpu build/lgpu-gpu                   # must be up to date
    ./build/lgpu-gpu csverify                               # host-only, must pass
    printf 'x\t\t0x1\t0x1\n' > /tmp/one.txt
    ./build/lgpu-gpu csrun s_abs_i32 < /tmp/one.txt         # must PASS first

If the one-case smoke test fails, stop - the device is not usable.

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
| 5 | a misaligned flat/buffer access | the alignment clause phase 3 left open | **single case, its own process, and expect a fault** - the most likely wedge; run it only after the ladder is proven |
| 6 | two-lane ordering / fence-less visibility | the visibility clauses | needs a two-instruction program the carrier cannot build today |
| 7 | texture out-of-bounds and wrap | the gather/samp boundary class | single rows |
| 8 | the atomic counter bound | where the increment/decrement forms read their bound - no encoded field carries it in the narrow form | single rows |
| 10 | the buffer index/offset registers with `idxen`/`offen` live | whether the buffer form addresses the way the descriptor stride implies.  The correctness carrier never exercises these: its load keeps the index and offset registers at zero and puts the offset in the immediate.  The memory bench templates cannot be built without this - and a wrong index register is an out-of-window access, so it is also a wedge risk | single rows, its own process, one access before any chain |
| 9 | `load_sm_matrix_x{1,2,4}` plane/lane layout | the last three code-layer UNRESOLVED markers | observed mode today; a real measurement needs the matrix file staged |

Anything in row 5 or 6 must be designed as a **new carrier shape** first (the
current carriers deliberately cannot stage it), and reviewed for ring risk before
it is submitted.

## 4. Host-only work that does not wait

The boundary-case authoring is **done**: `case_coverage.py` reports
**738 of 738 value-expressible rows** with every required class and no gaps, plus
30 rows listed as launch-shape (their result is launch state - the environment and
SIO reads, the stream emits, the interpolators and the index read - so they need a
launch state, not a case).  What remains host-side is the microbench templates for
the families still at SKIP (buffer 61, flat 45, LDS 44, SIO 17), and the launch
shapes themselves.
