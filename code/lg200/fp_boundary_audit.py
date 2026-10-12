#!/usr/bin/env python3
"""Which boundary classes does each floating-point row's case set actually contain?

Phase 4 asks for complete boundary coverage: every +-NaN (quiet and signalling), +-Inf,
+-0, the finite limits, and the subnormal range.  This reports what the vectors carry,
per row, so a gap can be worked rather than assumed.

It parses through cases.py, the one place that knows the case protocol.  Reading CASE_FP
lines with a local regex - which this audit did until 2026-10-13 - misses every plain
CASE(...) vector, and those are most of them: v_add_f32 alone has 385 input rows, of
which 22 are CASE_FP.  The audit therefore reported classes missing on rows that carried
them, and case_coverage.py, which uses the parser, was right all along.

A 64-bit operand is two little-endian words and is reassembled before its fields are
read; a 16-bit operand is one word.  Classifying a pair as two 32-bit words, which this
audit also did until 2026-10-13, reads a 16-bit NaN as an ordinary number.
"""
from __future__ import annotations

import collections
import pathlib
import re
import sys

from cases import parse_case_files

HERE = pathlib.Path(__file__).resolve().parent


def _classify(exp_bits: int, mant_bits: int, sign: int, exp: int, mant: int) -> str:
    top = 1 << (mant_bits - 1)
    if exp == (1 << exp_bits) - 1:
        if not mant:
            return 'inf'
        return 'nan-quiet' if mant & top else 'nan-signal'
    if exp == 0:
        if mant == 0:
            return 'negzero' if sign else 'poszero'
        return 'subnormal'
    if exp == (1 << exp_bits) - 2 and mant == (1 << mant_bits) - 1:
        return 'finite-max'
    if exp == 1 and mant == 0:
        return 'finite-min'
    return 'normal'


def _operand_width(mnemonic: str) -> int:
    if mnemonic.endswith('_f64'):
        return 64
    if mnemonic.endswith('_f16'):
        return 16
    return 32


def classify_words(words, width: int):
    per = 2 if width == 64 else 1
    out = set()
    i = 0
    while i + per <= len(words):
        v = words[i] & 0xFFFFFFFF
        if per == 2:
            v |= (words[i + 1] & 0xFFFFFFFF) << 32
            out.add(_classify(11, 52, (v >> 63) & 1, (v >> 52) & 0x7FF, v & ((1 << 52) - 1)))
        elif width == 16:
            out.add(_classify(5, 10, (v >> 15) & 1, (v >> 10) & 0x1F, v & 0x3FF))
        else:
            out.add(_classify(8, 23, (v >> 31) & 1, (v >> 23) & 0xFF, v & 0x7FFFFF))
        i += per
    return out


NAMES = {'nan-quiet', 'nan-signal', 'inf', 'poszero', 'negzero', 'subnormal',
         'finite-max', 'finite-min'}


def main() -> int:
    _mnemonics, case_list = parse_case_files(HERE)
    # The row SELECTION comes from CASE_FP; the input CLASSIFICATION uses every
    # vector the row has, plain CASE(...) included, because those are the majority
    # and they cover boundary values too.
    fp_rows = {c.mnemonic for c in case_list
               if re.search(r'_f(16|32|64)$', c.mnemonic)}
    rows = collections.defaultdict(set)
    for c in case_list:
        if c.mnemonic not in fp_rows:
            continue
        vals = [int(t, 16) for t in re.findall(r'0x[0-9a-fA-F]+', c.inputs or '')]
        if vals:
            rows[c.mnemonic] |= classify_words(vals, _operand_width(c.mnemonic))
    rows = {m: v for m, v in rows.items() if v}
    print('rows with input vectors: %d' % len(rows))
    thin = sorted(m for m in rows if NAMES - rows[m])
    print('rows missing at least one boundary class: %d' % len(thin))
    for m in thin[:16]:
        print('   %-28s missing %s' % (m, ','.join(sorted(NAMES - rows[m]))))
    print()
    # NOT 'every required class'.  This tool checks the eight BOUNDARY classes it names
    # above; the full required set is seventeen, and case_coverage.py is the gate that
    # checks it.  Saying 'required' here while checking a subset was the defect this
    # campaign keeps finding - a definition coarser than the thing it is measured
    # against - and the fix is to say what is checked, not to keep a second copy of the
    # requirement.
    print('rows covering every boundary class above: %d' % (len(rows) - len(thin)))
    print('  (the full required set is case_coverage.py\'s; this is the boundary subset)')
    floor = sorted(m for m in rows if not ({'nan-quiet', 'nan-signal'} & rows[m]))
    print('rows with NO NaN input at all (quiet or signalling): %d' % len(floor))
    for m in floor[:16]:
        print('   ' + m)
    return 0


if __name__ == '__main__':
    sys.exit(main())