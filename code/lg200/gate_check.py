#!/usr/bin/env python3
"""Is every check the suite runs able to fail?

A gate that cannot fail is worse than no gate: it is reported as a pass.  addr_scan.py was
one - it printed a count and exited 0 for every count - and it guarded the fault that wedged
the platform, so it would have been cited as protection it did not provide.  It was found by
reading it, which does not scale and does not stop the next one.

This reads the scripts check_all.sh actually invokes and requires each to contain a failure
path, or to be named here as a report whose findings are not pass/fail.  A new report-only
tool wired into the suite therefore fails this check until someone says which it is."""
from __future__ import annotations

import pathlib
import re
import sys

HERE = pathlib.Path(__file__).resolve().parent
SUITE = HERE / "check_all.sh"

# These two report findings that are expected, so they have no failing mode to have.  The
# list is deliberately short and named: anything else in the suite has to be able to fail.
REPORTS = {"prose_audit", "doc_register_audit"}


def main() -> int:
    text = SUITE.read_text(encoding="utf-8")
    # Comments are not checks.  The suite quotes `python3 x.py | tail` in a comment
    # explaining pipefail, and a scanner that reads it reports a script that cannot
    # exist - the same mistake, one level down, as the gate it is looking for.
    text = chr(10).join(ln for ln in text.splitlines()
                      if not ln.strip().startswith("#"))
    names = sorted(set(re.findall(r"python3 (?:code/lg200/)?(?:gpu/)?([a-z_]+)[.]py", text)))
    if not names:
        print("no python checks found in check_all.sh")
        return 1
    # Having a failure path is not the same as the failure triggering.  Every gate
    # also needs a case in mutation_check, which is what actually runs it broken.
    mc = (HERE / "mutation_check.py").read_text(encoding="utf-8")
    cased = set()
    for nm in re.findall(r"[(][\"]([a-z_]+[.]py[^\"]*)[\"]", mc):
        cased.add(nm.split()[0][:-3])
    bad = 0
    for name in names:
        if name in REPORTS:
            continue
        for d in (HERE, HERE / "gpu", HERE.parent.parent):
            p = d / (name + ".py")
            if p.exists():
                break
        else:
            print("MISSING  %s is invoked by the suite but not found beside it" % name)
            bad += 1
            continue
        body = p.read_text(encoding="utf-8", errors="replace")
        if not re.search(r"return [1-9]|sys[.]exit\([1-9]|exit\([1-9]", body):
            print("CANNOT FAIL  %s is a gate in the suite and has no failure path" % name)
            bad += 1
        if name not in cased:
            print("UNPROVEN  %s is a gate and no mutation case ever breaks it"
                  % name)
            bad += 1
    if bad:
        return 1
    print("gate check: %d suite scripts, %d named as reports, the rest can fail and are all broken by a case"
          % (len(names), len(REPORTS & set(names))))
    return 0


if __name__ == "__main__":
    sys.exit(main())