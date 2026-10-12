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
| 1 | re-run the CS matrix after the fp64 policy-column repair | the corrected fp64 compare expectations (the repair landed but was never measured) | step 1-3 of the ladder |
| 2 | DAZ/FTZ on the arithmetic and bit-op paths | the ~40 rows still failing: `v_abs_f32(denormal)` is 0 and `v_add_f32(+0.0, 0xffc00000)` keeps the NaN sign, so the flush applies beyond the softfloat path | single rows, one process each |
| 3 | a misaligned flat/buffer access | the alignment clause phase 3 left open | **single case, its own process, and expect a fault** - a misaligned access is the most likely wedge; run it with the recovery ladder already proven |
| 4 | two-lane ordering / fence-less visibility | the visibility clauses | needs a two-instruction program the carrier cannot build today |
| 5 | texture out-of-bounds and wrap | the gather/samp boundary class | single rows |
| 6 | `load_sm_matrix_x{1,2,4}` plane/lane layout | the last three code-layer UNRESOLVED markers | observed mode today; a real measurement needs the matrix file staged |

Anything in row 3 or 4 must be designed as a **new carrier shape** first (the
current carriers deliberately cannot stage it), and reviewed for ring risk before
it is submitted.

## 4. Host-only work that does not wait

- the boundary-case authoring for the 269 hand-written rows (`case_coverage.py`
  lists exactly which classes each one is missing);
- the compiled audit (`audit.py`) and the microbench templates for the families
  still at SKIP (buffer 61, flat 45, LDS 44, SIO 17).
