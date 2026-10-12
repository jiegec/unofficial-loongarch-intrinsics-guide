#!/usr/bin/env python3
"""Check that the shader-info size rules say the same thing on both pages.

The rules live in the Shader ABI chapter, with the record they belong to, and are
restated in Registers and Allocation so that chapter reads on its own.  Two copies
of one requirement is this campaign's most reliable source of drift - the wrap
difference between these two blocks is proof they are maintained by hand - so the
copies are compared mechanically instead of being trusted to stay in step.

Either make them agree, or delete one and point at the other.
"""
import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent.parent
PAGES = [ROOT / "docs/gpu/shader/abi.md", ROOT / "docs/gpu/shader/registers.md"]


def rules(path):
    text = path.read_text(encoding="utf-8")
    m = re.search(r"## Size rules\n(.*?)(?=\n## |\Z)", text, re.S)
    if not m:
        return None
    bullets, cur = [], None
    for line in m.group(1).splitlines():
        if line.startswith("* "):
            if cur:
                bullets.append(cur)
            cur = line[2:]
        elif line.startswith("  ") and cur is not None:
            cur += " " + line.strip()
        elif not line.strip():
            continue
        else:
            # A non-bullet, non-continuation line ends the list.  This used to do
            # `cur = None` without appending, which DROPPED the last bullet of every
            # list: the checker then reported a difference between two pages whose rules
            # agree.  A checker that manufactures the drift it looks for is worse than no
            # checker, and this is the third time in this campaign that the instrument was
            # the coarse one - the value of a checker really is bounded by what it reads.
            if cur:
                bullets.append(cur)
            cur = None
    if cur:
        bullets.append(cur)
    return [" ".join(b.split()) for b in bullets]


def main() -> int:
    got = [(p, rules(p)) for p in PAGES]
    missing = [str(p) for p, r in got if r is None]
    if missing:
        print("no '## Size rules' section in: %s" % ", ".join(missing))
        return 1
    (pa, ra), (pb, rb) = got
    if ra != rb:
        print("the size rules disagree between %s and %s" % (pa.name, pb.name))
        for a in ra:
            if a not in rb:
                print("    only in %s: %s" % (pa.name, a))
        for b in rb:
            if b not in ra:
                print("    only in %s: %s" % (pb.name, b))
        return 1
    print("size rules: %d rules agree on both pages" % len(ra))
    return 0


if __name__ == "__main__":
    sys.exit(main())
