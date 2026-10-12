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


def _classify(width: int, exp_bits: int, mant_bits: int, w: int) -> str:
    """The class of one operand.

    Quiet and signalling NaN are separate: the mantissa's top bit tells them apart, and
    the per-row requirement names both.  So are the finite limits - the largest finite
    value and the smallest normal one - because 'the bounds' is a class, not a value.
    Collapsing any of these, as this audit did until 2026-10-13, reports a gap as closed
    while the thing the page asks for has never been exercised.
    """
    sign = (w >> (width - 1)) & 1
    exp = (w >> mant_bits) & ((1 << exp_bits) - 1)
    mant = w & ((1 << mant_bits) - 1)
    top = 1 << (mant_bits - 1)
    if exp == (1 << exp_bits) - 1:
        if not mant:
            return "inf"
        return "nan-quiet" if mant & top else "nan-signal"
    if exp == 0:
        if mant == 0:
            return "negzero" if sign else "poszero"
        return "subnormal"
    if exp == (1 << exp_bits) - 2 and mant == (1 << mant_bits) - 1:
        return "finite-max"
    if exp == 1 and mant == 0:
        return "finite-min"
    return "normal"


def _operand_width(mnemonic: str) -> int:
    """The width of ONE operand, in bits, from the mnemonic's suffix."""
    if mnemonic.endswith("_f64"):
        return 64
    if mnemonic.endswith("_f16"):
        return 16
    return 32


def classify_words(words, width: int):
    """Classify a whole operand, however many words it spans.

    A 64-bit operand is two little-endian words, so it is reassembled before its fields
    are read.  Classifying such a pair as two 32-bit words - which this audit did until
    2026-10-13 - reads a 16-bit NaN as a normal number and a 64-bit one almost by luck,
    and it made the audit report missing classes on rows that already carried them.
    """
    per = width // 32 if width >= 32 else 1
    out = set()
    i = 0
    while i + per <= len(words):
        v = 0
        for k in range(per):
            v |= (words[i + k] & 0xFFFFFFFF) << (32 * k)
        if width == 64:
            out.add(_classify(64, 11, 52, v))
        elif width == 16:
            out.add(_classify(16, 5, 10, v & 0xFFFF))
        else:
            out.add(_classify(32, 8, 23, v & 0xFFFFFFFF))
        i += per
    return out


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
            rows[path.stem] |= classify_words(vals[:-1], _operand_width(path.stem))
            for f in re.split(r"[|\s]+", fl.strip()):
                if f and f != "rtne":
                    flags[path.stem].add(f)
    names = {"nan-quiet", "nan-signal", "inf", "poszero", "negzero", "subnormal",
             "finite-max", "finite-min"}
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
    print("rows covering every required class: %d" % len(named))
    floor = [m for m in sorted(rows) if not ({"nan-quiet", "nan-signal"} & rows[m])]
    print("rows with NO NaN input at all (quiet or signalling): %d" % len(floor))
    for m in floor[:24]:
        print("   " + m)
    return 0


if __name__ == "__main__":
    sys.exit(main())
