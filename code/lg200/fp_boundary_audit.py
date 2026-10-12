#!/usr/bin/env python3
"""Which floating-point boundary classes does each row's case set actually contain?

Phase 4 asks for complete boundary coverage: every +-NaN, +-Inf, +-0, subnormal, the
rounding modes and the flush modes.  This reports what the CASE_FP vectors really carry,
per row, so the gap can be worked rather than assumed.

A 32-bit word is classified by its fields: exp all ones with a non-zero mantissa is NaN,
exp all ones with a zero mantissa is Inf, exp zero with a non-zero mantissa is subnormal,
exp zero with a zero mantissa is zero (the sign bit distinguishes +0 from -0).

Only inputs are classified; the last argument of CASE_FP is the expected value.
"""
from __future__ import annotations

import collections
import pathlib
import re
import sys

HERE = pathlib.Path(__file__).resolve().parent
CASE = re.compile(r"CASE_FP\(\s*([A-Za-z0-9_-]+)\s*,\s*([^,]+),\s*(.+?)\)\s*$")


def classify(w: int) -> str:
    exp = (w >> 23) & 0xFF
    mant = w & 0x7FFFFF
    sign = (w >> 31) & 1
    if exp == 0xFF:
        return "nan" if mant else "inf"
    if exp == 0:
        if mant == 0:
            return "negzero" if sign else "poszero"
        return "subnormal"
    return "normal"


def main() -> int:
    rows = collections.defaultdict(set)
    flags = collections.defaultdict(set)
    for path in sorted(HERE.glob("*.cpp")):
        text = path.read_text(encoding="utf-8", errors="replace")
        for line in text.splitlines():
            m = CASE.search(line.strip())
            if not m:
                continue
            name, fl, rest = m.group(1), m.group(2), m.group(3)
            toks = re.findall(r"0x[0-9a-fA-F]+|0b[01]+", rest)
            if len(toks) < 2:
                continue
            vals = []
            for t in toks:
                try:
                    vals.append(int(t, 16 if t.startswith("0x") else 2))
                except ValueError:
                    pass
            for v in vals[:-1]:
                if v <= 0xFFFFFFFF:
                    rows[path.stem].add(classify(v))
            for f in re.split(r"[|\s]+", fl.strip()):
                if f and f != "rtne":
                    flags[path.stem].add(f)
    names = {"nan", "inf", "poszero", "negzero", "subnormal"}
    print("fp rows with CASE_FP vectors: %d" % len(rows))
    gaps = []
    for m in sorted(rows):
        missing = sorted(names - rows[m])
        if missing or flags.get(m):
            gaps.append((m, missing, sorted(flags.get(m, ()))))
    thin = [g for g in gaps if g[1]]
    print("rows missing at least one boundary class: %d" % len(thin))
    for m, missing, fl in thin[:24]:
        print("   %-28s missing %-34s modes %s" % (m, ",".join(missing), ",".join(fl) or "-"))
    print()
    named = [m for m in sorted(rows) if not (names - rows[m])]
    print("rows covering all five classes: %d" % len(named))
    floor = [m for m in sorted(rows) if "nan" not in rows[m]]
    print("rows with NO NaN input at all: %d" % len(floor))
    for m in floor[:24]:
        print("   " + m)
    return 0


if __name__ == "__main__":
    sys.exit(main())
