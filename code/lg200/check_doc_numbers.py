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



WHY = ('no case', 'no vector', 'cannot', 'not observable', 'carrier', 'not reachable',
       'observation-only', 'launch state', 'encoding', 'not settable', 'no experiment',
       'the protocol', 'not answerable', 'does not stage', 'decoder census',
       'decoder should reject', 'belong to the launch', 'reaches that address',
       'pair base of its own', 'are not staged', 'no vector varies them',
       'answers rather than a run', 'reports a flag', 'case protocol')


def explained_rows():
    """Rows whose every UNRESOLVED sentence states a reason, in itself or the next one."""
    total = 0
    path = TSV
    for row in csv.DictReader(open(path), delimiter=chr(9)):
        text = row['description'].replace(chr(92) + 'n', ' ')
        sents = [s.strip() for s in re.split(r'(?<=[.])\s+', text) if s.strip()]

        def why(s):
            return any(w in s.lower() for w in WHY)
        bare = [s for i, s in enumerate(sents)
                if 'UNRESOLVED' in s and not why(s)
                and not (i + 1 < len(sents) and why(sents[i + 1]))]
        if any('UNRESOLVED' in s for s in sents) and not bare:
            total += 1
    return total


def main() -> int:
    rows, sents, occ = unresolved_counts()
    cov = run('case_coverage.py', ROOT)
    m = re.search(r'with all required boundary classes: ([0-9]+)\s+with gaps: ([0-9]+)', cov)
    launch = re.search(r'\(([0-9]+) launch-shape\)', cov)
    vec = run('run_cases.py', ROOT)
    nvec = re.search(r'[0-9]+ instructions, ([0-9]+) vectors', vec)
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
        # Prose figures nothing watched until 2026-10-22: the vector total was stated in
        # plain text on two pages and the split as an independent sum on a third.
        'generated-rows': len((ROOT / 'generated_cases.tsv').read_text().splitlines()),
        # A row whose every open clause names the layer that cannot settle it is
        # explained even though it is not resolved.  This is the number the
        # campaign's objective turns on now, so it is read from the table rather
        # than typed into prose.
        'explained-rows': explained_rows(),
        'hand-cases': sum(len(re.findall(r'^\s*CASE(?:_FP)?\(', f.read_text(), re.M))
                           for f in sorted(ROOT.glob('*.cpp'))),
    }
    checks = [
        ('unresolved.md', r'([0-9]+) of the 768', 'rows-unresolved'),
        ('status.md', r'\\?\| rows stating an .UNRESOLVED. \| [0-9]+ \| \*\*([0-9]+)\*\*', 'rows-unresolved'),
        ('status.md', r'distinct UNRESOLVED sentences \| [0-9]+ \| \*\*([0-9]+)\*\*', 'sentences'),
        ('status.md', r'rows whose every open clause states a reason \| \u2014 \| \*\*([0-9]+)\*\*', 'explained-rows'),
        # The pattern used to be (1,[0-9]+), which assumed a thousands separator and
        # could not match once the count fell below a thousand on 2026-10-13.  The
        # status.md copy and the unresolved-status.md one now accept both forms.
        ('status.md', r'([0-9,]+) occurrences', 'occurrences'),
        ('case-boundaries.md', r'\*\*([0-9]+) of [0-9]+ value-expressible', 'covered'),
        ('status.md', r'([0-9]+) of [0-9]+ value-expressible', 'covered'),
        # unresolved-status.md keeps a dated snapshot, so its current figures carry a
        # date and are anchored by it.  This file had no anchor at all until 2026-10-13:
        # it is the fourth page with count claims and the only one the check did not read.
        ('case-boundaries.md', r'rows still with gaps \| \*\*([0-9]+)\*\*', 'gaps'),
        ('status.md', r'768 PASS / \*\*([0-9,]+)\*\* rows', 'vectors'),
        ('case-boundaries.md', r'still \*\*([0-9]+)\*\*', 'targets'),
        ('case-boundaries.md', r'The \*\*([0-9]+)\*\* that remain are ' +
                                r'\*\*launch-shape\*\*', 'launch-shape'),
        ('unresolved-status.md', r'\*\*([0-9]+) as of 2026-10-13\*\*', 'rows-unresolved'),
        ('unresolved-status.md', r'is ([0-9]+) distinct sentences', 'sentences'),
        ('unresolved-status.md', r'The rest of the ([0-9]+) sentences', 'sentences'),
        ('status.md', r'oracle passes \(\*\*768 instructions, ([0-9,]+)', 'vectors'),
        ('unresolved-status.md', r'passes throughout \(768 instructions, ([0-9,]+)', 'vectors'),
        ('case-boundaries.md', r'files plus ([0-9,]+) rows in `generated_cases.tsv`', 'generated-rows'),
        ('case-boundaries.md', r'([0-9,]+) hand-written `CASE`', 'hand-cases'),
        ('unresolved-status.md', r'distinct sentences and ([0-9,]+) occurrences', 'occurrences'),
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

    # Every live measure must either be anchored above or be listed here as one that
    # no page states.  The previous round claimed the anchored set was complete and it
    # was not - gaps and vectors were computed and never compared with anything - which
    # is the same failure as the missing anchor itself, one level up.  So the claim is
    # checked rather than asserted.
    # Empty, and it should stay that way: the two entries it first held were wrong.
    # vectors is stated on three pages and gaps on one, and neither was anchored, so
    # listing them as unstated was a way of not looking rather than a finding.
    UNSTATED = set()
    anchored_keys = {key for _doc, _pat, key in checks}
    for key in live:
        if key not in anchored_keys and key not in UNSTATED:
            print('UNANCHORED  live measure %s has no anchor and is not listed as unstated'
                  % key)
            bad += 1
    bad += bold_census(live)
    print('drift: %d' % bad)
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())