#!/usr/bin/env python3
"""Do the pages and headers cite register names that no row actually has?

prose_audit.py checks a row's description against its own layout.  It reads neither the
family pages nor a header's hand-written comment lines, so both were surfaces no checker
covered - which is the failure the 2026-10-13 boundary-audit episode turned up: a checker
is only as good as what it reads, not how strictly it compares.

This closes the register-name half of that gap: it collects every register-shaped name
that any of the 768 rows uses in its layout, then reports backticked names in
docs/gpu/isa/*.md and in comment lines beyond a header's first (the first is the row's own
description, kept in step by header_sync.py).

Its BOUND is part of its definition, so it is stated here and printed: it catches a wrong
REGISTER NAME and nothing else.  The jump-stack paragraph that called a control-flow
record "a displacement" - while its rows said the resume consumes saved operand state - is
invisible to it, because both sentences are grammatical English.  That class still needs a
human reading the page against the rows.

Run 2026-10-13: 0 pages and 0 headers cite a register that no row has.
"""
from __future__ import annotations

import csv
import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
TSV = ROOT / "lg200-instructions.tsv"
DOCS = ROOT / "docs/gpu/isa"
HEADERS = ROOT / "code/lg200"
REG = re.compile(r"`([A-Z]{1,4}[0-9]{1,3}(?:x[0-9]+)?)`")


def main() -> int:
    fields = set()
    for r in csv.DictReader(TSV.open(), delimiter="\t"):
        for part in (r.get("operand_layout") or "").split(";"):
            if "@" in part:
                fields.add(re.sub(r"x[0-9]+$", "", part.split("@")[0].strip()))

    def unknown(text):
        out = []
        for name in REG.findall(text):
            base = re.sub(r"x[0-9]+$", "", name)
            if base in fields or base.startswith("M") or base in ("M", "Z"):
                continue
            out.append(name)
        return out

    pages = {p.name: unknown(p.read_text(encoding="utf-8", errors="replace"))
             for p in sorted(DOCS.glob("*.md"))}
    pages = {k: v for k, v in pages.items() if v}

    headers = {}
    for h in sorted(HEADERS.glob("*.h")):
        lines = h.read_text(encoding="utf-8", errors="replace").splitlines()
        rest = [l for i, l in enumerate(lines) if i and l.strip().startswith("//")]
        u = unknown("\n".join(rest))
        if u:
            headers[h.name] = u

    print("layout field names across all rows: %d" % len(fields))
    print("pages citing a register no row uses: %d" % len(pages))
    for k, v in pages.items():
        print("   %-26s %s" % (k, ",".join(v[:6])))
    print("headers citing a register no row uses (beyond line 1): %d" % len(headers))
    for k, v in headers.items():
        print("   %-26s %s" % (k, ",".join(v[:6])))
    print()
    print("bound: register NAMES only.  A wrong claim in prose - a record called a")
    print("displacement when its rows say otherwise - is not visible here.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
