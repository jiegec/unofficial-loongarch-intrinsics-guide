#!/usr/bin/env python3
"""Every observation-only row should say why in its own description, and every
reading it produces should land somewhere that can be diffed.

The runner prints the reason it set observe, so the results matrix carries it
next to the row instead of leaving a reader to infer one from the mnemonic.  The
first check compares the descriptions against that record: a row the matrix
marks observed must mention the mechanism the matrix names.

The second (--samples) checks build/gpu_obs/, where run_gpu.py persists the
read-back dwords of those rows.  The per-mnemonic log cannot serve as that
record: every run rewrites it, a --dry one included, so a reading printed there
survives only until the next dry sweep - which is how the values a description
quotes ("0x12345678 reads itself back", "1.0 stays 1.0") came to have nothing on
disk to check them against.  The record is a gate when one exists and a note
when none does, because it is device-produced and a checkout that has not run a
sweep has nothing to audit.
"""
from __future__ import annotations

import csv
import re
import sys
import time
from pathlib import Path

HERE = Path(__file__).resolve().parent
MATRIX = HERE / 'gpu' / 'build' / 'gpu_ps_generic_results.tsv'
TSV = HERE.parent.parent / 'code' / 'lg200-instructions.tsv'

# Where run_gpu.py persists the observe-mode read-back, and what the two files
# there look like.  The column lists are repeated here on purpose: a gate that
# imports the writer's parser cannot report that the writer wrote the wrong
# thing.
OBSDIR = HERE / 'gpu' / 'build' / 'gpu_obs'
RUNNER = HERE / 'gpu' / 'build' / 'lgpu-gpu'
MATRICES = {'ps': MATRIX,
            'cs': HERE / 'gpu' / 'build' / 'gpu_cs_generic_results.tsv'}
SAMPLE_COLS = ['case', 'offset', 'value', 'model', 'reason']
INDEX_COLS = ['mnemonic', 'pipe', 'status', 'pass', 'fail', 'rows', 'samples',
              'reason', 'stamp']
# The record is written by the same process, after the matrix it describes, so a
# record older than that matrix by more than this window describes an earlier
# run.  The window is for filesystem timestamp granularity, not for slack.
SLACK = 2.0

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
    'accumulator and destination tuples': ['tuples are not staged'],
    'scalar-compare destination': ['ordinary scalar register'],
    # [HW 2026-11-03] v_add_imm_i32 / v_sub_imm_i32: the instruction does not
    # write its VD8 destination on this silicon, so the graded cell holds the
    # previous dispatch's value.
    'leaves its VD8 destination unwritten': ['leaves its VD8 destination'],
    # [HW 2026-11-04] v_emit2/3: the operand the carrier grades is VJ8, and the
    # instruction's descriptor side effect overwrites it.
    "the emit's descriptor side effect overwrites the separator": ['separator VGPR'],
}


def read_tsv(path):
    with open(path, encoding='utf-8') as fh:
        reader = csv.DictReader(fh, delimiter='\t')
        return list(reader.fieldnames or []), list(reader)


def read_index(path):
    """The record's index as dicts, or [] when the file is not one of ours."""
    try:
        lines = path.read_text(encoding='utf-8').splitlines()
    except OSError:
        return []
    if not lines or lines[0].split('\t') != INDEX_COLS:
        return []
    rows = []
    for line in lines[1:]:
        cells = line.split('\t')
        if len(cells) == len(INDEX_COLS):
            rows.append(dict(zip(INDEX_COLS, cells)))
    return rows


def observed_verdict(detail):
    """(case id, reason) when the matrix detail records one case.

    A batch run's detail is the per-mnemonic BATCH summary line, which names no
    case and no reason; the record is then checked on its own, and that is a
    note rather than a pass, because the reason is what this record exists to
    keep.
    """
    m = re.search(r'^PASS (\S+) \(observed(?:: (.*))?\)$', detail)
    if m is None:
        return None, None
    return m.group(1), (m.group(2) or '')


def samples() -> int:
    """Is the observe-mode record present, consistent with the matrix, current?

    Three questions, in the order that fails fastest:

      present     every row the matrix marks observed has a record row and a
                  sample file, and the record holds no row the matrix does not
                  mark (a leftover from an older carrier)
      consistent  the record's status/pass/fail/rows match the matrix's, the
                  sample count matches the file, the reason is a known
                  mechanism, and the last sample is the case the matrix names
      current     no sample file is older than the matrix it belongs to, or than
                  build/lgpu-gpu - a reading that predates the binary in the tree
                  describes code that is no longer there
    """
    if not OBSDIR.is_dir():
        print('obs_samples: no build/gpu_obs; a non-dry run writes it')
        return 0
    runner = RUNNER.stat().st_mtime if RUNNER.is_file() else None
    bad = recorded = dwords = 0
    stamps, unaudited = set(), []
    for pipe in sorted(MATRICES):
        matrix = MATRICES[pipe]
        index_path = OBSDIR / pipe / 'index.tsv'
        marked, written = {}, None
        if matrix.is_file():
            written = matrix.stat().st_mtime
            marked = {r['mnemonic']: r for r in read_tsv(matrix)[1]
                      if '(observed' in (r.get('detail') or '')}
        index = {r['mnemonic']: r for r in read_index(index_path)}
        if not marked and not index:
            continue
        if not index_path.is_file():
            # A pipe whose record was never written is a note, not a failure: the
            # artifact arrived after the matrix it describes, so a matrix that
            # predates it cannot be required to have one, and demanding a sweep
            # to clear a gate would make the gate a device tax.  Half a record -
            # sample files with no index - is a truncated write and does fail.
            left = len(list((OBSDIR / pipe).glob('*.tsv')))
            if left:
                print('MISSING  %s: %d sample file(s) and no index to read them'
                      % (pipe, left))
                bad += 1
            else:
                print('NOTE     %s: no record for this pipe yet (the matrix marks '
                      '%d row(s) observed); a non-dry run of it writes one'
                      % (pipe, len(marked)))
                unaudited.append(pipe)
            continue
        if not marked and index:
            print('NOTE     %s: the matrix records no per-case verdict (a batch '
                  'sweep writes the summary line), so the record is checked on '
                  'its own' % pipe)
        if marked and written is not None:
            for m in sorted(set(index) - set(marked)):
                print('LEFTOVER %-26s the record holds it and the matrix does not '
                      'mark it observed' % m)
                bad += 1
        for m in (sorted(marked) if marked else sorted(index)):
            row = index.get(m)
            if row is None:
                print('MISSING  %-26s the matrix marks it observed and the record '
                      'does not hold it' % m)
                bad += 1
                continue
            if marked:
                for field in ('status', 'pass', 'fail', 'rows'):
                    if row.get(field) != marked[m].get(field):
                        print('DRIFT    %-26s record says %s=%s, matrix says %s'
                              % (m, field, row.get(field), marked[m].get(field)))
                        bad += 1
            if not any(k in (row.get('reason') or '') for k in PHRASES):
                print('REASON   %-26s record reason %r is not one of the known '
                      'mechanisms' % (m, row.get('reason')))
                bad += 1
            path = OBSDIR / pipe / (m + '.tsv')
            if not path.is_file():
                print('MISSING  %-26s record row with no sample file' % m)
                bad += 1
                continue
            cols, rows = read_tsv(path)
            if cols != SAMPLE_COLS:
                print('HEADER   %-26s sample file columns are %s' % (m, cols))
                bad += 1
                continue
            if not rows:
                print('EMPTY    %-26s record says it was read back and no dword is '
                      'stored' % m)
                bad += 1
                continue
            if len(rows) != int(row.get('samples') or -1):
                print('COUNT    %-26s record says %s sample(s), the file holds %d'
                      % (m, row.get('samples'), len(rows)))
                bad += 1
            recorded += 1
            dwords += len(rows)
            stamps.add(row.get('stamp') or '')
            if marked:
                case, why = observed_verdict(marked[m].get('detail') or '')
                if case is not None:
                    if rows[-1]['case'] != case:
                        print('DETAIL   %-26s matrix names case %s, the record '
                              'ends at %s' % (m, case, rows[-1]['case']))
                        bad += 1
                    elif rows[-1]['reason'] != why:
                        print('DETAIL   %-26s matrix reason %r, record reason %r'
                              % (m, why, rows[-1]['reason']))
                        bad += 1
            when = path.stat().st_mtime
            if written is not None and when + SLACK < written:
                print('STALE    %-26s recorded %s, the matrix is newer'
                      % (m, time.ctime(when)))
                bad += 1
            elif runner is not None and when + SLACK < runner:
                print('STALE    %-26s recorded %s, build/lgpu-gpu is newer: rerun '
                      'run_gpu.py, so the reading describes the binary in the tree'
                      % (m, time.ctime(when)))
                bad += 1
    print('obs_samples: %d row(s), %d dword(s), %d problem(s), record %s%s'
          % (recorded, dwords, bad,
             ', '.join(sorted(s for s in stamps if s)) or 'unstamped',
             ('; no record yet for ' + ', '.join(unaudited)) if unaudited else ''))
    return 1 if bad else 0


def reasons() -> int:
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


def main() -> int:
    if '--samples' in sys.argv[1:]:
        return samples()
    return reasons()


if __name__ == '__main__':
    sys.exit(main())
