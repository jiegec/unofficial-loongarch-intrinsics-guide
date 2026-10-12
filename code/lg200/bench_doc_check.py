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
    (HERE / "gpu" / "MICROBENCH.md", r"buildable for both kinds \| \*\*(\d+)\*\*"),
    (HERE / "gpu" / "HARDWARE-SESSION-PLAN.md",
     r"\*\*(\d+) of 768 rows build now\*\*"),
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
    for path, pat in PAGES:
        d = re.search(pat, path.read_text(encoding="utf-8"))
        if not d:
            print("MISSING  %s does not state a buildable count" % path.name)
            bad += 1
            continue
        doc = int(d.group(1))
        if doc != live:
            print("DRIFT  %s says %d, bench_buildable.py says %d"
                  % (path.name, doc, live))
            bad += 1
    if bad:
        return 1
    print("%d buildable for both kinds, and %d pages agree with the tool"
          % (live, len(PAGES)))
    return 0


if __name__ == "__main__":
    sys.exit(main())