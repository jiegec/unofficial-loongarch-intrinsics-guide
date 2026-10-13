#!/usr/bin/env python3
"""Compare the bench page buildable numbers with the tool that produces them.

The 5.2 table in MICROBENCH.md sat at 610 through sixteen rows of template work -
the page is updated by hand and the tool is not, which is the same drift this
campaign keeps finding between a claim and its source.  So the two are compared
mechanically: run bench_buildable.py in dry-run mode and check that the page
agrees with it on the headline count.

    python3 bench_doc_check.py
"""
import pathlib
import re
import subprocess
import sys

HERE = pathlib.Path(__file__).resolve().parent
# Two pages state this number: the bench page and the hardware-session plan, which
# said 605 for two weeks of work after that was no longer true.  A check over one
# copy is how the other drifts, so both are read.
PAGES = [
    (HERE / "gpu" / "MICROBENCH.md", [
        r"\| yes \| yes \| yes \| \*\*(\d+)\*\*",
        r"buildable for both kinds \| \*\*(\d+)\*\*",
    ]),
    (HERE / "gpu" / "HARDWARE-SESSION-PLAN.md", [
        r"\*\*(\d+) of 768 rows build now\*\*",
        r"\*\*(\d+) rows are buildable now against",
    ]),
]
# The last hardware run's result, which the pages quote in three places.  Its source is a
# device-produced CSV that is not tracked, so it is checked only when that file is present.
MEASURED = [
    (HERE / "gpu" / "MICROBENCH.md", r"\*\*(\d+) OK / (\d+) SKIP"),
    (HERE / "gpu" / "README.md", r"\*\*(\d+) OK / (\d+) SKIP"),
    (HERE.parent.parent / "docs" / "gpu" / "appendix" / "status.md",
     r"\| CS microbench over the universe \| (\d+) OK / (\d+) SKIP"),
]

TOOL = HERE / "gpu" / "bench_buildable.py"


def main() -> int:
    run = subprocess.run([sys.executable, str(TOOL)], cwd=str(HERE / "gpu"),
                         capture_output=True, text=True, timeout=1800)
    m = re.search(r"buildable for both kinds: (\d+)", run.stdout)
    if not m:
        print("could not read the tool:"); print(run.stdout[-300:]); return 2
    live = int(m.group(1))
    bad = 0
    n = 0
    for path, pats in PAGES:
        text = path.read_text(encoding="utf-8")
        for pat in pats:
            d = re.search(pat, text)
            if not d:
                print("MISSING  %s: no match for %s" % (path.name, pat[:40]))
                bad += 1
                continue
            n += 1
            doc = int(d.group(1))
            if doc != live:
                print("DRIFT  %s says %d, bench_buildable.py says %d"
                      % (path.name, doc, live))
                bad += 1
    # A second number in the same pages, kept by a different tool.  Without this it is the
    # next one found stale by hand, in as many places as the first.
    results = HERE / "gpu" / "build" / "cs_bench_results.tsv"
    if results.exists():
        lines = results.read_text(errors="replace").splitlines()
        head = lines[0].split(chr(9))
        idx = head.index("status") if "status" in head else 1
        statuses = [ln.split(chr(9))[idx] for ln in lines[1:]
                    if len(ln.split(chr(9))) > idx]
        ok, skip = statuses.count("OK"), statuses.count("SKIP")
        for path, pat in MEASURED:
            for m in re.finditer(pat, path.read_text(encoding="utf-8")):
                got = (int(m.group(1)), int(m.group(2)))
                if got != (ok, skip):
                    print("DRIFT  %s says %d OK / %d SKIP, the results file says %d / %d"
                          % (path.name, got[0], got[1], ok, skip))
                    bad += 1
    else:
        print("no bench results file; the measured OK/SKIP pair is not checked")
    if bad:
        return 1
    print("%d buildable for both kinds, and %d statements on %d pages agree"
          % (live, n, len(PAGES)))
    return 0


if __name__ == "__main__":
    sys.exit(main())