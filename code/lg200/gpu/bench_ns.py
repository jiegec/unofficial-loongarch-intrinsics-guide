#!/usr/bin/env python3
"""Add nanosecond columns to the CS microbench CSV.

The raw CSV is in firmware ticks; calibrate_ticks.py measured 1 tick =
NS_PER_TICK ns (~750 MHz counter).  This writes a derived file with lat_ns /
tp_ns alongside the ticks, so the timing can be quoted in physical units without
re-running the sweep.

    python3 bench_ns.py [in.tsv] [out.tsv]
"""
import csv
import sys
from pathlib import Path

NS_PER_TICK = 1.333   # calibrate_ticks.py: 1.3306 (30 s) / 1.3337 (10 s) ns


def main():
    src = Path(sys.argv[1]) if len(sys.argv) > 1 else Path("build/cs_bench_results.tsv")
    dst = Path(sys.argv[2]) if len(sys.argv) > 2 else src.with_name(src.stem + "_ns.tsv")
    rows, n = [], 0
    with open(src) as f:
        for r in csv.DictReader(f, delimiter="\t"):
            for ticks, ns in (("lat_ticks_per_op", "lat_ns"), ("tp_ticks_per_op", "tp_ns")):
                try:
                    r[ns] = "%.4f" % (float(r[ticks]) * NS_PER_TICK)
                except (KeyError, ValueError, TypeError):
                    r[ns] = ""
            rows.append(r)
            n += 1
    fields = list(rows[0].keys()) if rows else []
    with open(dst, "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=fields, delimiter="\t")
        w.writeheader()
        w.writerows(rows)
    ok = sum(1 for r in rows if r.get("status") == "OK")
    print("%d rows (%d OK) -> %s" % (n, ok, dst))


if __name__ == "__main__":
    main()
