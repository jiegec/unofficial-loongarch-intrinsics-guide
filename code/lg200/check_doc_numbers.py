#!/usr/bin/env python3
"""Check the pages' headline numbers against the live tools.

The same figures are quoted in status.md, unresolved.md, case-boundaries.md and
MICROBENCH.md, and they move whenever the generator, the audit or the bench
changes.  This recomputes each one and fails loudly on a mismatch, so a stale
number cannot survive a round.

    python3 check_doc_numbers.py
"""
from __future__ import annotations

import csv
import re
import subprocess
import sys
from pathlib import Path
import sys as _sys
_sys.path.insert(0, str(Path(__file__).resolve().parent))
import gen_cases

ROOT = Path(__file__).resolve().parent
DOCS = ROOT.parent.parent / 'docs' / 'gpu' / 'appendix'
TSV = ROOT.parent / 'lg200-instructions.tsv'


def table():
    with open(TSV, encoding='utf-8') as f:
        return list(csv.DictReader(f, delimiter=chr(9)))


def unresolved_counts():
    rows = table()
    s = set()
    occ = 0
    for r in rows:
        d = r.get('description', '')
        if 'UNRESOLVED' not in d:
            continue
        for x in re.split(r'(?<=[.!?])\s+', d):
            if 'UNRESOLVED' in x:
                s.add(x.strip())
                occ += 1
    return sum(1 for r in rows if 'UNRESOLVED' in r.get('description', '')), len(s), occ



def bold_census(live):
    """Every bold 3+ digit number near a measure keyword must be a live figure.

    This replaces the older one-match-per-pattern shape, which is how four copies of
    the same number drifted in turn: each sweep found the next copy, because the check
    named one occurrence.  The convention here is that the pages mark a CURRENT figure
    in bold and keep older ones plain, so bold is the set that has to be live - and a
    new bold figure fails rather than waiting to be noticed.  Plain numbers are left
    alone, which is what lets the historical snapshots stay on the pages."""
    keys = ["UNRESOLVED", "value-expressible", "launch-shape", "buildable",
            "occurrences", "vectors"]
    ok = {"%d" % v for v in live.values()}
    bad = 0
    docs = Path(__file__).resolve().parent.parent.parent / "docs" / "gpu"
    for path in sorted(docs.rglob("*.md")):
        for n, line in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
            if not any(k in line for k in keys):
                continue
            for num in re.findall(r"\*\*([0-9][0-9,]{2,})\*\*", line):
                if num not in ok and num.replace(",", "") not in ok:
                    print("DRIFT    %s:%d: bold figure %s is not a live value"
                          % (path.name, n, num))
                    bad += 1
    return bad

def run(script, cwd):
    p = subprocess.run([sys.executable, script], cwd=cwd, text=True,
                       capture_output=True, timeout=1800)
    return p.stdout + p.stderr


def main() -> int:
    rows, sents, occ = unresolved_counts()
    cov = run('case_coverage.py', ROOT)
    m = re.search(r'with all required boundary classes: (\d+)\s+with gaps: (\d+)', cov)
    launch = re.search(r'\((\d+) launch-shape\)', cov)
    vec = run('run_cases.py', ROOT)
    nvec = re.search(r'\d+ instructions, (\d+) vectors', vec)
    if not (m and launch and nvec):
        print('could not read the live tools:'); print(cov[-300:]); print(vec[-300:])
        return 2
    live = {
        'rows-unresolved': rows,
        'sentences': sents,
        'occurrences': occ,
        'covered': int(m.group(1)),
        'gaps': int(m.group(2)),
        'launch-shape': int(launch.group(1)),
        'vectors': int(nvec.group(1)),
        # gen_cases.targets() is the row set the boundary grid is built for.  The
        # page states its size, so it is read from the generator rather than typed.
        'targets': len(gen_cases.targets()),
    }
    checks = [
        ('unresolved.md', r'(\d+) of the 768', 'rows-unresolved'),
        ('status.md', r'\\?\| rows stating an .UNRESOLVED. \| \d+ \| \*\*(\d+)\*\*', 'rows-unresolved'),
        ('status.md', r'distinct UNRESOLVED sentences \| \d+ \| \*\*(\d+)\*\*', 'sentences'),
        ('status.md', r'(1,\d+) occurrences', 'occurrences'),
        ('case-boundaries.md', r'\*\*(\d+) of \d+ value-expressible', 'covered'),
        ('status.md', r'(\d+) of \d+ value-expressible', 'covered'),
        # unresolved-status.md keeps a dated snapshot, so its current figures carry a
        # date and are anchored by it.  This file had no anchor at all until 2026-10-13:
        # it is the fourth page with count claims and the only one the check did not read.
        ('unresolved-status.md', r'\*\*(\d+) as of 2026-10-13\*\*', 'rows-unresolved'),
        ('case-boundaries.md', r'still \*\*(\d+)\*\*', 'targets'),
        ('unresolved-status.md', r'is (\d+) distinct sentences', 'sentences'),
        ('unresolved-status.md', r'distinct sentences and ([\d,]+) occurrences', 'occurrences'),
    ]
    bad = 0
    for doc, pat, key in checks:
        text = (DOCS / doc).read_text()
        got = re.search(pat, text)
        if not got:
            print('MISSING  %-22s %s' % (doc, pat)); bad += 1; continue
        value = got.group(1).replace(',', '')
        if int(value) != live[key]:
            print('DRIFT    %-22s %s: doc %s vs live %d' % (doc, key, value, live[key]))
            bad += 1
        else:
            print('ok       %-22s %s = %d' % (doc, key, live[key]))
    for k, v in live.items():
        print('  live %-16s %d' % (k, v))
    bad += bold_census(live)
    print('drift: %d' % bad)
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())