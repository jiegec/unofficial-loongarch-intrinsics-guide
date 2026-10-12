#!/usr/bin/env python3
"""Does a row's prose name a register its layout does not have?

Written after the 64-bit float cmpswap turned out to describe a VA8 operand that its
layout does not contain - and the description is what a reader implements from.

The check is deliberately simple and deliberately noisy.  It reports every backticked
register name in a description that the row's own layout does not list, after folding
the tuple suffix (the prose says VD8 where the layout says VD8x2, which is correct: the
x2 names the width, not an encoding bit).

THREE CLASSES OF FALSE POSITIVE are expected, and each was seen while writing it:

  * NEGATION - a row may say "no GJ11 or GK11 selector is present" to close a question;
  * COMPARISON - a row may cite a sibling's operand to contrast with its own;
  * the same sentence copied from a sibling TEMPLATE, which is not a false positive at
    all but a real defect: it is how copy_sm_from_global came to describe VJ8 and VD8
    while listing only PJ2.

So the tool's output is a queue to read, not a list of errors.
"""
from __future__ import annotations

import csv
import re
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
TSV = HERE.parent / "lg200-instructions.tsv"
REG = re.compile(r"`([A-Z]{1,4}[0-9]{1,3}(?:x[0-9]+)?)`")


def base(name: str) -> str:
    return re.sub(r"x[0-9]+$", "", name)


def main() -> int:
    rows = list(csv.DictReader(TSV.open(), delimiter="\t"))
    flagged = []
    for r in rows:
        layout = r.get("operand_layout") or ""
        fields = {base(p.split("@")[0].strip()) for p in layout.split(";") if "@" in p}
        if not fields:
            continue
        desc = (r.get("description") or "").replace("\\n", "\n")
        cited = {base(c) for c in REG.findall(desc)}
        unknown = sorted(c for c in cited if c not in fields and c not in ("M", "Z"))
        if unknown:
            flagged.append((r["mnemonic"], unknown, sorted(fields)))
    print("rows whose prose names a register the layout lacks: %d of %d"
          % (len(flagged), len(rows)))
    for m, u, f in flagged:
        print("  %-30s cites %-14s layout has %s" % (m, ",".join(u), ",".join(f)))
    print()
    print("read each one: negation and comparison are expected; a copied sibling")
    print("template is a real defect.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
