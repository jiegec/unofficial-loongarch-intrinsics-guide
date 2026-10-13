#!/usr/bin/env python3
"""Do the pages' links lead anywhere?

A documentation set is navigated by its links, and a link rots the moment a page is renamed.
Nothing here read them, so the whole navigation surface was unchecked: over two hundred links across ninety-seven
pages, thirty-two of which name an anchor inside a page.

For each link this resolves the target relative to the page and then to the checkout, and for
a link that carries #anchor it requires a heading whose slug matches.  External links are left
alone."""
from __future__ import annotations

import pathlib
import re
import sys

HERE = pathlib.Path(__file__).resolve().parent
ROOT = HERE.parent.parent
LINK = re.compile(r"\]\(([^)\s]*?)(?:#([^)\s]+))?\)")
HEAD = re.compile(r"^(#+)\s+(.*)$", re.M)


def slug(heading):
    s = re.sub(r"[^a-z0-9 _-]", "", heading.strip().lower())
    return s.replace(" ", "-")


def pages():
    return (sorted((ROOT / "docs").rglob("*.md"))
            + sorted((HERE / "gpu").glob("*.md")))


def main() -> int:
    bad = 0
    seen = 0
    heads = {}
    for page in pages():
        text = page.read_text(encoding="utf-8", errors="replace")
        for m in LINK.finditer(text):
            target, anchor = m.group(1), m.group(2)
            if target.startswith(("http://", "https://", "mailto:")):
                continue
            seen += 1
            f = (page.parent / target).resolve() if target else page
            if not f.exists() and not (ROOT / target).exists():
                print("DEAD LINK  %s -> %s" % (page.relative_to(ROOT), target))
                bad += 1
                continue
            if anchor is None:
                continue
            if not target:
                f = page
            if f not in heads:
                heads[f] = {slug(h.group(2)) for h in HEAD.finditer(
                    f.read_text(encoding="utf-8", errors="replace"))}
            if anchor not in heads[f]:
                print("DEAD ANCHOR  %s -> #%s" % (page.relative_to(ROOT), anchor))
                bad += 1
    # The pages also tell a reader which commands to run, and a renamed script rots that
    # exactly as a renamed page rots a link.  Names are resolved by basename, because the
    # docs write them both with and without their directory.
    tools = {p.name for p in list((ROOT / "code" / "lg200").glob("*.py"))
             + list((ROOT / "code" / "lg200" / "gpu").glob("*.py"))
             + list(ROOT.glob("*.py"))}
    cmds = 0
    for page in pages():
        for m in re.finditer(r"python3\s+([A-Za-z0-9_./-]+[.]py)",
                             page.read_text(encoding="utf-8", errors="replace")):
            cmds += 1
            # A cited path is resolved as written; only a bare name falls back to the
            # basename, because two tools here share one (run_bench.py exists in code/lg200
            # and in code/lg200/gpu with different flags), and a basename-only test would
            # accept either.  That collision has now caught three of my own sweeps.
            cited = m.group(1)
            if "/" in cited:
                ok = (ROOT / cited).exists() or (ROOT / "code" / cited).exists()
            else:
                ok = cited in tools
            if not ok:
                print("DEAD COMMAND  %s -> python3 %s"
                      % (page.relative_to(ROOT), m.group(1)))
                bad += 1

    # The flags on those commands matter too: the hardware plan's preconditions were wrong
    # twice, once with a placeholder path and once with a missing environment variable, and
    # checking the script name alone would have caught neither.  Resolved the same way, by
    # path when the page writes one.
    tool_paths = {}
    for cand in (list((ROOT / "code" / "lg200").glob("*.py"))
                 + list((ROOT / "code" / "lg200" / "gpu").glob("*.py"))
                 + list(ROOT.glob("*.py"))):
        tool_paths[cand.name] = cand
    flag_n = 0
    for page in pages():
        text = page.read_text(encoding="utf-8", errors="replace")
        for m in re.finditer(r"python3\s+([A-Za-z0-9_./-]+[.]py)([^\n]*)", text):
            cited, rest = m.group(1), m.group(2)
            path = None
            for cand in (ROOT / cited, ROOT / "code" / cited):
                if cand.exists():
                    path = cand
                    break
            if path is None:
                path = tool_paths.get(cited)
            if path is None:
                continue          # the command check above already reported this one
            body = path.read_text(encoding="utf-8", errors="replace")
            for f in re.findall(r"--[a-z][a-z0-9-]+", rest):
                flag_n += 1
                if f not in body:
                    print("DEAD FLAG  %s -> python3 %s %s"
                          % (page.relative_to(ROOT), cited, f))
                    bad += 1
    if bad:
        return 1
    print("links: %d, commands: %d (%d flags), every one resolving"
          % (seen, cmds, flag_n))
    return 0


if __name__ == "__main__":
    sys.exit(main())