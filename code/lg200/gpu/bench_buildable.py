#!/usr/bin/env python3
"""Which rows the bench can BUILD, host-only.

The bench's own coverage report joins a hardware CSV, so a template that was
just written cannot show up until the device comes back.  This one asks the
runner to build each row for both kinds in dry-run mode - no device, no
submission - and reports the outcome and the reason, so a template's effect is
measurable the moment it compiles.

    python3 bench_buildable.py [--tsv out.tsv]
"""
from __future__ import annotations

import argparse
import csv
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent
RUNNER = ROOT / "build" / "lgpu-gpu"
TSV = ROOT.parent.parent / "lg200-instructions.tsv"
KINDS = ("lat", "tp")


def mnemonics() -> list:
    with open(TSV, encoding="utf-8") as f:
        return [r["mnemonic"] for r in csv.DictReader(f, delimiter="\t")]


def build_one(mnemonic: str, kind: str):
    p = subprocess.run([str(RUNNER), "csbench", mnemonic, kind, "32", "1",
                        "--dry-run"], text=True, capture_output=True,
                       timeout=120)
    if p.returncode == 0 and p.stdout.startswith("--"):
        return True, ""
    line = (p.stderr.strip().splitlines() or [""])[-1]
    line = re.sub(r"^csbench:\s*\S+:\s*", "", line)
    return False, line


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--tsv")
    args = ap.parse_args()
    rows, built, why = [], 0, {}
    for m in mnemonics():
        kinds = {}
        first_reason = ""
        for k in KINDS:
            ok, reason = build_one(m, k)
            kinds[k] = (ok, reason)
            if not ok and reason and not first_reason:
                first_reason = reason
        if first_reason:
            why[first_reason] = why.get(first_reason, 0) + 1
        both = all(v[0] for v in kinds.values())
        built += 1 if both else 0
        rows.append((m, kinds["lat"][0], kinds["tp"][0],
                     kinds["lat"][1] or kinds["tp"][1]))
    print("rows: %d   buildable for both kinds: %d   not: %d"
          % (len(rows), built, len(rows) - built))
    print()
    print("%-42s %s" % ("reason (rows)", "count"))
    for reason, n in sorted(why.items(), key=lambda kv: -kv[1]):
        print("  %-40s %d" % (reason[:40], n))
    if args.tsv:
        with open(args.tsv, "w", encoding="utf-8") as f:
            f.write("mnemonic\tlat\ttp\treason\n")
            for m, la, tp, reason in rows:
                f.write("%s\t%d\t%d\t%s\n" % (m, la, tp, reason))
        print("\n->", args.tsv)
    return 0


if __name__ == "__main__":
    sys.exit(main())
