#!/usr/bin/env python3
"""Which rounding and flush policy tokens does each floating-point row's cases use?

Phase 4 asks for the rounding and flush modes as well as the boundary values.  This
reports what the cases actually carry:

  2026-10-13: rtne 173 of 173, ftz 92, daz 91, rtz 1, rtup 1, rtdn 0.

Those missing tokens are deliberately NOT reported as gaps, and the reason is a carrier
limitation rather than an oversight: the PS carrier runs a fixed floating-point default
(RN + denormal-flush + positive NaN) and ignores the policy tokens, which is stated in
v_add_f32.cpp and v_mul_f32.cpp and now also in docs/gpu/appendix/case-boundaries.md.
Adding the remaining tokens would add cases that cannot differ on that carrier.  Where a
carrier CAN set the mode - the compute path - is an open question that needs a device.

Reported separately from fp_boundary_audit.py so that the value classes (which ARE
treated as gaps) and the mode classes (which are not) cannot be confused.
"""
from __future__ import annotations

import collections
import pathlib
import re
import sys

HERE = pathlib.Path(__file__).resolve().parent
CASE = re.compile(r"CASE_FP\(\s*([A-Za-z0-9_-]+)\s*,\s*([^,]+),")

ROUNDING = ("rtne", "rtz", "rtdn", "rtup")
FLUSH = ("ftz", "daz")


def main() -> int:
    per = collections.defaultdict(set)
    for path in sorted(HERE.glob("*.cpp")):
        for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
            m = CASE.search(line.strip())
            if not m:
                continue
            per[path.stem] |= {t for t in m.group(2).strip().split("|") if t}
    if not per:
        print("no CASE_FP rows found", file=sys.stderr)
        return 1
    counts = collections.Counter()
    for toks in per.values():
        for t in toks:
            counts[t] += 1
    print("fp rows: %d" % len(per))
    print("policy tokens (rows using each): "
          + ", ".join("%s %d" % (k, counts[k]) for k in sorted(counts)))
    for t in ROUNDING + FLUSH:
        miss = sorted(m for m in per if t not in per[m])
        print("   rows without %-5s %3d of %d" % (t, len(miss), len(per)))
    print()
    print("none of these are counted as gaps: the PS carrier's FP default is fixed and")
    print("ignores the tokens.  See docs/gpu/appendix/case-boundaries.md.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
