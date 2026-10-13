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
            if m.group(1).split("/")[-1] not in tools:
                print("DEAD COMMAND  %s -> python3 %s"
                      % (page.relative_to(ROOT), m.group(1)))
                bad += 1
    if bad:
        return 1
    print("links: %d checked and %d commands cited, every target, anchor and script resolves"
          % (seen, cmds))
    return 0


if __name__ == "__main__":
    sys.exit(main())