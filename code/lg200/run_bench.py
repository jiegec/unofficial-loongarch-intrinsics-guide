#!/usr/bin/env python3
"""Differential latency/throughput microbench driver for the PS carrier.

Per-op cost = slope of wall-clock vs copies (least squares over the copy
sweep); the fixed submission/readback overhead cancels.  This is the LSX
measure.cpp differential philosophy adapted to the PS carrier.

Measurement floor (measured, 2026-08-22/23): one submission costs ~16.6-18.5ms
fixed and the per-op signal is ~50-200ns, so without GPU timestamps only the
rows whose fitted slope is positive with a sane R^2 are meaningful; the
per-op values of those are RELATIVE wall-clock numbers, not architectural
cycles.  min-of-reps is used for the fit (least-noise estimator, matching
the LSX measure.cpp convention); med is kept in the row for reference.

    python3 run_bench.py            # full sweep -> measure-lg200.csv
    python3 run_bench.py --first N  # first N benchable targets only
    python3 run_bench.py --only m1,m2  # selected targets
"""
from __future__ import annotations

import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent
RUNNER = ROOT / "gpu" / "build" / "lgpu-gpu"
OUTCSV = ROOT / "measure-lg200.csv"
# 1536 copies = ~12.5KB of FS, comfortably under the 16KB single-page BO;
# 2040+ hits the "payload exceeds one GPU page" limit.
COPIES = [128, 256, 512, 1024, 1536]
REPS = 31
NL = chr(10)

sys.path.insert(0, str(ROOT))
from gen_cases import targets  # noqa: E402


def one_run(mnemonic: str, kind: str, copies: int):
    r = subprocess.run([str(RUNNER), "bench", mnemonic, kind, str(copies),
                        str(REPS)], text=True, stdout=subprocess.PIPE,
                       stderr=subprocess.STDOUT)
    if r.returncode != 0:
        return None
    parts = r.stdout.strip().split(chr(9))
    if len(parts) < 7:
        return None
    # columns: name kind copies reps wall_ms_med wall_ms_min wall_ms_max
    #          gpu_ticks_med gpu_ticks_per_op
    try:
        return (float(parts[5]), float(parts[4]))  # (min, med) ms
    except ValueError:
        return None


def median_ms(mnemonic: str, kind: str, copies: int):
    """Legacy helper: median wall-clock ms (kept for compatibility)."""
    r = one_run(mnemonic, kind, copies)
    return r[1] if r else None


def slope(points: list):
    n = len(points)
    sx = sum(c for c, _ in points)
    sy = sum(m for _, m in points)
    sxx = sum(c * c for c, _ in points)
    sxy = sum(c * m for c, m in points)
    den = n * sxx - sx * sx
    if den == 0:
        return 0.0, 0.0, 0.0
    sl = (n * sxy - sx * sy) / den
    ic = (sy - sl * sx) / n
    ss_tot = sum((m - sy / n) ** 2 for _, m in points)
    ss_res = sum((m - (sl * c + ic)) ** 2 for c, m in points)
    r2 = 1.0 if ss_tot == 0 else 1.0 - ss_res / ss_tot
    return sl, ic, r2


def one_target(m: str):
    """Fit lat and tp slopes from min-of-reps; return row dict or None."""
    lat = []
    tp = []
    for c in COPIES:
        lr = one_run(m, "lat", c)
        tr = one_run(m, "tp", c)
        if lr is not None:
            lat.append((c, lr[0]))
        if tr is not None:
            tp.append((c, tr[0]))
    if len(lat) < 3 or len(tp) < 3:
        return None
    lsl, lic, lr2 = slope(lat)
    tsl, tic, tr2 = slope(tp)
    return {
        "name": m,
        "lat_ns_per_op": lsl * 1e6,
        "tp_ns_per_op": tsl * 1e6,
        "lat_intercept_ms": lic,
        "tp_intercept_ms": tic,
        "lat_r2": lr2,
        "tp_r2": tr2,
        "reliable": 1 if (lsl > 0 and tsl > 0 and lr2 > 0.5 and tr2 > 0.5) else 0,
    }


def main() -> int:
    first = None
    if len(sys.argv) > 2 and sys.argv[1] == "--first":
        first = int(sys.argv[2])
    only = None
    if len(sys.argv) > 2 and sys.argv[1] == "--only":
        only = set(sys.argv[2].split(","))
    tl = targets()[:first] if first else targets()
    if only:
        tl = [m for m in tl if m in only]
    print("bench: {} targets x {} copies x lat/tp x {} reps".format(
        len(tl), len(COPIES), REPS))
    rows = []
    unreliable = []
    t0 = time.time()
    for i, m in enumerate(sorted(tl), 1):
        row = one_target(m)
        if row is None:
            unreliable.append(m)
            print("[{:3d}/{:3d}] {:36s} skipped (not benchable)".format(
                i, len(tl), m))
            continue
        if not row["reliable"]:
            unreliable.append(m)
            print("[{:3d}/{:3d}] {:36s} unreliable lat={:8.1f} tp={:8.1f} "
                  "r2=({:.2f},{:.2f})".format(
                      i, len(tl), m, row["lat_ns_per_op"],
                      row["tp_ns_per_op"], row["lat_r2"], row["tp_r2"]))
            rows.append(row)
            continue
        rows.append(row)
        print("[{:3d}/{:3d}] {:36s} lat={:8.2f}ns tp={:8.2f}ns".format(
            i, len(tl), m, row["lat_ns_per_op"], row["tp_ns_per_op"]))
    with open(OUTCSV, "w", encoding="utf-8") as f:
        f.write("name,lat_ns_per_op,tp_ns_per_op,lat_intercept_ms,"
                "tp_intercept_ms,lat_r2,tp_r2,reliable" + NL)
        for row in rows:
            f.write("{},{:.2f},{:.2f},{:.4f},{:.4f},{:.4f},{:.4f},{}".format(
                row["name"], row["lat_ns_per_op"], row["tp_ns_per_op"],
                row["lat_intercept_ms"], row["tp_intercept_ms"],
                row["lat_r2"], row["tp_r2"], row["reliable"]) + NL)
    nrel = sum(1 for r in rows if r["reliable"])
    print("bench: {} rows ({} reliable) -> {}; {} unreliable/skipped"
          .format(len(rows), nrel, OUTCSV, len(unreliable)))
    print("elapsed={:.1f}s".format(time.time() - t0))
    return 0


if __name__ == "__main__":
    sys.exit(main())
