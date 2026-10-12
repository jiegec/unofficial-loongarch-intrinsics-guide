#!/usr/bin/env python3
"""Check markdown table integrity across the GPU pages.

The suite checks the TSV invariants, the register audit and the prose audit, but
it never read the markdown itself - so when an edit of mine stranded the
i8/i16/i32/i64 row of a class table after a paragraph, and then removed it,
nothing failed.  A table is a claim like any other and gets checked here.

A table is a CONTIGUOUS run of lines that start with a pipe.  That definition is
the whole check, and getting it wrong is instructive: the first version reset the
header only when a new separator appeared, so it compared each table's rows
against the previous table's header and reported 55 defects in a documentation
set with none.  A checker that does not scope what it checks does not check it.

Two defects, both seen in this repository:

* a row whose cell count differs from its header's;
* a run with no separator as its second line - either a stray row detached from
  any table, or a table whose header is malformed.
"""
import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent.parent


def cells(line):
    """Split a table row on UNESCAPED pipes only.

    Splitting on every pipe made this checker report four defects in a table whose
    pipes are escaped correctly: `\\|` inside a cell is content, not a divider.  This
    is the sixth time in this campaign that the instrument was the coarse one, and the
    pattern is consistent enough to be worth the comment - there is no such thing here
    as a checker whose own reading does not have to be checked.
    """
    s = line.strip()
    if not s.startswith("|"):
        return None
    body = s.strip("|")
    return [c.strip() for c in re.split(r"(?<!\\)\|", body)]


def is_sep(line):
    c = cells(line)
    if not c:
        return False
    return all(re.fullmatch(r":?-{2,}:?", x.strip()) for x in c)


def main() -> int:
    problems = []
    pages = sorted(ROOT.glob("docs/**/*.md"))
    for path in pages:
        lines = path.read_text(encoding="utf-8", errors="replace").splitlines()
        # Split into contiguous runs of pipe-starting lines, outside code fences.
        blocks, run = [], []
        fence = False
        for i, line in enumerate(lines):
            if line.strip().startswith("```"):
                fence = not fence
                if run:
                    blocks.append(run)
                    run = []
                continue
            if fence or cells(line) is None:
                if run:
                    blocks.append(run)
                    run = []
                continue
            run.append((i + 1, line))
        if run:
            blocks.append(run)

        for block in blocks:
            if len(block) < 2 or not is_sep(block[1][1]):
                problems.append((path, block[0][0],
                                 "a %d-line run with no separator under its header"
                                 % len(block)))
                continue
            width = len(cells(block[0][1]))
            for n, line in block[2:]:
                c = cells(line)
                if len(c) != width:
                    problems.append((path, n, "row has %d cells, header has %d"
                                     % (len(c), width)))
    for path, n, why in problems:
        print("%s:%d: %s" % (path.relative_to(ROOT), n, why))
    print("markdown tables: %d pages, %d problems" % (len(pages), len(problems)))
    return 1 if problems else 0


if __name__ == "__main__":
    sys.exit(main())
