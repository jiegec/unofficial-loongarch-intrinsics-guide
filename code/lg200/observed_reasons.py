#!/usr/bin/env python3
"""Every observation-only row should say why in its own description.

The runner now prints the reason it set observe, so the results matrix carries it
next to the row instead of leaving a reader to infer one from the mnemonic.  This
checks the descriptions against that record: a row the matrix marks observed must
mention the mechanism the matrix names.
"""
from __future__ import annotations

import csv
import re
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
MATRIX = HERE / 'gpu' / 'build' / 'gpu_ps_generic_results.tsv'
TSV = HERE.parent.parent / 'code' / 'lg200-instructions.tsv'

# Each record the runner can write, and the phrases a description may use for it.
PHRASES = {
    'flat input-plane': ['input-plane'],
    'persistent matrix file': ['matrix file'],
    'fixed plane patterns': ['plane pattern'],
    'window state rather than': ['window state'],
    'SIO channel-file': ['channel-file'],
    'emit-index VGPR': ['emit-index'],
    'LG210-legacy no-op': ['executes it empty'],
    'lane-id read': ['last lane'],
    'coefficient window': ['coefficient window'],
    'launch-state group': ['launch-state group', 'launch state'],
}

def main() -> int:
    if not MATRIX.exists():
        print('observed_reasons: no PS matrix; run the PS sweep first')
        return 1
    desc = {}
    with open(TSV, encoding='utf-8') as fh:
        for row in csv.DictReader(fh, delimiter='\t'):
            desc[row['mnemonic']] = row['description']
    checked = bad = 0
    with open(MATRIX, encoding='utf-8') as fh:
        for row in csv.DictReader(fh, delimiter='\t'):
            detail = row.get('detail') or ''
            m = re.search(r'observed: ([^)]*)', detail)
            if not m:
                continue
            why = m.group(1)
            phrases = next((v for k, v in PHRASES.items() if k in why), None)
            if phrases is None:
                print('UNKNOWN reason %r on %s' % (why, row['mnemonic']))
                bad += 1
                continue
            checked += 1
            text = desc.get(row['mnemonic'], '')
            if not any(p in text for p in phrases):
                print('MISSING  %-26s matrix says %r, description never mentions %s'
                      % (row['mnemonic'], why, phrases[0]))
                bad += 1
    print('observed_reasons: %d row(s) checked, %d without a matching reason' % (checked, bad))
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())