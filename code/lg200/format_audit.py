#!/usr/bin/env python3
"""Does the encoding table describe the fields the rows actually carry?

The encoding page groups the 768 rows into 12 formats and lists each format's fields.
That table and the rows' operand_layout columns are two statements of the same thing, so
they can be compared mechanically.  Either direction of disagreement matters:

  * a field the PAGE lists but no row of that format carries is an over-claim - the
    reader is told to expect an encoding field that does not exist;
  * a field the ROWS carry but the page does not list is an under-claim - the table is
    incomplete.

Run 2026-10-13: ALL TWELVE formats show an empty page-only set.  The table never
over-claims.  The row-only side is non-empty for flat (RA8), shared-memory (RJ8, UO8,
X0_16), sio-id (IK16, VD8) and sio-imm, among others, so those lists are incomplete -
a real but milder defect, and one a reader can work around by consulting the rows.

The M.* modifier names are filtered out: the page lists them in a separate table, so
comparing them here would report noise.
"""
from __future__ import annotations

import collections
import csv
import re
import sys
from pathlib import Path

# the TSV lives beside code/lg200/, the docs at the repository root
CODE = Path(__file__).resolve().parent.parent
REPO = CODE.parent
TSV = CODE / "lg200-instructions.tsv"
ENCODING = REPO / "docs/gpu/isa/encoding.md"


def base(name: str) -> str:
    return re.sub(r"x[0-9]+$", "", name.strip())


def main() -> int:
    rows = list(csv.DictReader(TSV.open(), delimiter="\t"))
    by_format: dict = collections.defaultdict(set)
    counts: collections.Counter = collections.Counter()
    for r in rows:
        f = (r.get("format") or "").strip()
        counts[f] += 1
        for part in (r.get("operand_layout") or "").split(";"):
            if "@" in part:
                by_format[f].add(base(part.split("@")[0]))

    page: dict = {}
    for line in ENCODING.read_text().splitlines():
        if not line.startswith("|"):
            continue
        cells = [c.strip() for c in line.strip("|").split("|")]
        if len(cells) < 6:
            continue
        m = re.search(r"`([a-z0-9-]+)`", cells[1])
        if not m:
            continue
        names = {base(x) for x in re.findall(r"`([A-Za-z0-9_.]+)`", cells[-1])}
        names = {n for n in names
                 if not n.startswith("M.") and re.fullmatch(r"[A-Z]{1,4}[0-9]{1,3}", n)}
        page[m.group(1)] = names

    print("formats: %d in the rows, %d in the page table" % (len(by_format), len(page)))
    over = 0
    for f in sorted(set(page) & set(by_format)):
        only_page = sorted(page[f] - by_format[f])
        only_rows = sorted(by_format[f] - page[f])
        if only_page:
            over += 1
        if only_page or only_rows:
            print("%-16s rows=%3d  PAGE-ONLY: %-24s row-only: %s"
                  % (f, counts[f], ",".join(only_page) or "-", ",".join(only_rows)))
    print()
    print("formats where the table over-claims: %d" % over)
    # An over-claim is a real defect - the page promises an encoding field the rows do
    # not carry - so this is a gate and returns non-zero on one.  It returned 0
    # unconditionally until 2026-10-13, which meant its place in check_all.sh was a
    # report: the suite could not fail on the thing this file exists to catch.  The
    # count is 0 today, so nothing changes now; what changes is that it would.
    return 1 if over else 0


if __name__ == "__main__":
    sys.exit(main())
