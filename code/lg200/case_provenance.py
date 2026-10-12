#!/usr/bin/env python3
"""Was the boundary grid actually measured?

A row's UNRESOLVED clause may only be settled by evidence that was EXECUTED.
The tempting shortcut is to read the generated grid and treat its expectations
as measurements - but the grid can be regenerated at any time, and any vector
added after the last hardware run carries an ORACLE expectation, not a reading.

Round 86 found exactly that: the PS matrix ran at 2026-10-12 23:03 while the
grid was regenerated at 2026-10-13 02:15-02:30, so the NaN vectors it looked
like evidence for had never executed.

The measured baseline is pinned by the matrix itself: v_add_f32's row says
rows=111 and detail='PASS gen2510', and the 10-10 grid (4f26f81, 18,542
vectors) carries 100 generated vectors for that row against 11 hand cases -
111.  So the matrix measured THE 10-10 GRID, and everything the 10-13
regeneration added (about 65,000 vectors, three quarters of the current grid)
is oracle expectation, not reading.

A concrete consequence, checked by hand:

    v_add_f32 0x80000000 0x80000000 -> 0x80000000   IS in the 10-10 grid  -> measured
    v_add_f32 0x0 0xffc00000       -> 0x7fc00000   is NOT                  -> oracle only

which is why the zero-sign clauses could be settled and the NaN one could not.

This check compares what the matrix RECORDED (its per-row case count) against
what the grid HOLDS now, and reports the rows that grew.  A grown row is not a
disaster - it means the extra vectors are unmeasured, which is precisely what a
clause-settling round must not forget.
"""
from __future__ import annotations

import csv
import datetime
import re
import subprocess
import sys
from collections import Counter
from pathlib import Path

HERE = Path(__file__).resolve().parent
MATRIX = HERE / "gpu" / "build" / "gpu_ps_generic_results.tsv"
GRID = HERE / "generated_cases.tsv"


def matrix_counts() -> dict:
    out = {}
    if not MATRIX.exists():
        return out
    with MATRIX.open() as fh:
        for row in csv.DictReader(fh, delimiter="\t"):
            m = row.get("mnemonic") or row.get("name")
            n = row.get("cases") or row.get("rows")
            if m and str(n).strip().isdigit():
                out[m] = int(n)
    return out


def grid_counts() -> Counter:
    c = Counter()
    with GRID.open() as fh:
        for line in fh:
            f = line.split("\t")
            if f and f[0]:
                c[f[0]] += 1
    return c


def grid_commit_date() -> str:
    try:
        return subprocess.run(
            ["git", "log", "-1", "--format=%ad", "--date=format:%Y-%m-%d %H:%M",
             "--", str(GRID)],
            cwd=HERE, capture_output=True, text=True, check=True).stdout.strip()
    except Exception:
        return ""


PAGE = HERE.parent.parent / "docs" / "gpu" / "appendix" / "status.md"


def check_page(n_grown, extra) -> int:
    """Compare the provenance figures with what the status page states.

    The page calls this mechanically checkable and then quotes two of its numbers,
    but nothing ran the tool, so those numbers were checked by nothing.  This is the
    missing half of that sentence."""
    text = PAGE.read_text(encoding="utf-8")
    m = re.search(r"[*][*]([0-9]+) rows grew, [+]([0-9,]+) vectors", text)
    bad = 0
    if not m:
        print("MISSING  the page no longer states the provenance figures")
        bad += 1
    else:
        said_grown = int(m.group(1))
        said_extra = int(m.group(2).replace(",", ""))
        if said_grown != n_grown:
            print("DRIFT    rows grew: page says %d, the matrix says %d"
                  % (said_grown, n_grown))
            bad += 1
        if said_extra != extra:
            print("DRIFT    unmeasured vectors: page says %d, the matrix says %d"
                  % (said_extra, extra))
            bad += 1
    if bad:
        return 1
    print("case provenance: %d rows grew by %d unmeasured vectors, page agrees"
          % (n_grown, extra))
    return 0

def main() -> int:
    if not MATRIX.exists():
        print("case-provenance: no hardware matrix recorded, nothing to compare")
        return 0
    measured = matrix_counts()
    now = grid_counts()
    mtime = datetime.datetime.fromtimestamp(MATRIX.stat().st_mtime)
    gdate = grid_commit_date()
    print("case-provenance: matrix written %s; grid last changed %s"
          % (mtime.strftime("%Y-%m-%d %H:%M"), gdate or "(unknown)"))
    if gdate and mtime < datetime.datetime.strptime(gdate, "%Y-%m-%d %H:%M"):
        print("  NOTE: the grid is NEWER than the matrix, so vectors added since")
        print("        the matrix are oracle expectations, NOT measurements.")

    # Evidence STRENGTH per row, straight from the matrix: a row either compared
    # its vectors against the oracle (pass > 0), or only OBSERVED what the
    # hardware did (u-l-i-g's observed mode), or ran nothing at all.
    compared = observed = empty = 0
    with MATRIX.open() as fh:
        for row in csv.DictReader(fh, delimiter="\t"):
            npass = (row.get("pass") or "0").strip()
            detail = row.get("detail") or ""
            if npass not in ("0", ""):
                compared += 1
            elif "observed" in detail:
                observed += 1
            else:
                empty += 1
    print("  evidence strength: %d rows compared against the oracle, %d merely"
          % (compared, observed))
    print("                     OBSERVED (hardware value recorded, no independent"
          " comparison), %d with nothing run at all" % empty)

    # Which result artifacts are CLEAN?  The wedge at 2026-10-13 01:42 left
    # 'cannot open /dev/dri/renderD128' rows in everything written after it, and
    # a row whose only record says that is not evidence of anything.
    print("  result artifacts (wedge-contaminated lines / total):")
    build = HERE / "gpu" / "build"
    for art in sorted(build.glob("*.tsv")):
        try:
            body = art.read_text(errors="replace")
        except OSError:
            continue
        lines = body.count(chr(10))
        dirty = body.count("renderD128")
        mark = "CLEAN" if not dirty else ("ALL WEDGED" if dirty >= lines - 1 else "partly wedged")
        print("    %-32s %5d / %5d  %s" % (art.name, dirty, lines, mark))

    grown = sorted((m, measured[m], n) for m, n in now.items()
                   if m in measured and n > measured[m])
    extra = sum(b - a for _, a, b in grown)
    if "--check" in sys.argv:
        return check_page(len(grown), extra)
    print("  rows whose grid grew since the matrix: %d (+%d unmeasured vectors)"
          % (len(grown), extra))
    for m, a, b in grown[:8]:
        print("    %-28s measured %4d, grid now %4d" % (m, a, b))
    if len(grown) > 8:
        print("    ... and %d more" % (len(grown) - 8))
    return 0


if __name__ == "__main__":
    sys.exit(main())
