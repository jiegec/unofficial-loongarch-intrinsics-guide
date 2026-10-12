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
DOC = HERE / "gpu" / "MICROBENCH.md"
TOOL = HERE / "gpu" / "bench_buildable.py"


def main() -> int:
    run = subprocess.run([sys.executable, str(TOOL)], cwd=str(HERE / "gpu"),
                         capture_output=True, text=True, timeout=1800)
    m = re.search(r"buildable for both kinds: (\d+)", run.stdout)
    if not m:
        print("could not read the tool:"); print(run.stdout[-300:]); return 2
    live = int(m.group(1))
    text = DOC.read_text(encoding="utf-8")
    start = text.index("### 5.2 Buildable now")
    block = text[start:start + 3000]
    d = re.search(r"buildable for both kinds \| \*\*(\d+)\*\*", block)
    if not d:
        print("the page does not state a buildable-for-both-kinds number")
        return 2
    doc = int(d.group(1))
    if doc != live:
        print("DRIFT  MICROBENCH.md says %d, bench_buildable.py says %d" % (doc, live))
        return 1
    print("bench page: %d buildable for both kinds, agrees with the tool" % live)
    return 0


if __name__ == "__main__":
    sys.exit(main())