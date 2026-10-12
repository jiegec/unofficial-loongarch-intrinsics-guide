#!/usr/bin/env python3
"""Boundary-coverage audit for the LG200 case vectors (phase 4).

The question this answers: for every instruction, does its case set actually
exercise the boundary values its operand types admit?  A row whose cases all sit
at 1.0 and 2.0 pins nothing about NaN, infinity, signed zero, the finite limits
or the denormal range, and neither does an integer row that never sees 0, -1 or
the width extremes.

The required classes are derived from the mnemonic's own type suffixes, so a
mixed row (v_cvt_f32_i32) is asked for both sides.  32-bit classes are matched by
whole input word; 16-bit classes by either half of the word (the collected
vectors put one element in a word's low half, so the audit accepts either); 64-bit
classes by an input word pair.

    python3 case_coverage.py                 # summary + gap list
    python3 case_coverage.py --tsv out.tsv   # per-instruction detail
"""
from __future__ import annotations

import argparse
import collections
import csv
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from cases import parse_case_files  # noqa: E402

ROOT = Path(__file__).resolve().parent

F32 = {
    "+0": 0x00000000, "-0": 0x80000000, "+1": 0x3f800000, "-1": 0xbf800000,
    "+0.5": 0x3f000000, "max_norm": 0x7f7fffff, "-max_norm": 0xff7fffff,
    "min_norm": 0x00800000, "-min_norm": 0x80800000,
    "max_denorm": 0x007fffff, "min_denorm": 0x00000001,
    "+inf": 0x7f800000, "-inf": 0xff800000,
    "qnan": 0x7fc00000, "-qnan": 0xffc00000,
    "snan": 0x7f800001, "-snan": 0xff800001,
}
F16 = {
    "+0": 0x0000, "-0": 0x8000, "+1": 0x3c00, "-1": 0xbc00, "+0.5": 0x3800,
    "max_norm": 0x7bff, "-max_norm": 0xfbff, "min_norm": 0x0400,
    "-min_norm": 0x8400, "max_denorm": 0x03ff, "min_denorm": 0x0001,
    "+inf": 0x7c00, "-inf": 0xfc00, "qnan": 0x7e00, "-qnan": 0xfe00,
    "snan": 0x7c01, "-snan": 0xfc01,
}
I32 = {"0": 0, "1": 1, "max": 0x7fffffff, "min": 0x80000000,
       "-2": 0xfffffffe, "-1": 0xffffffff}
I16 = {"0": 0, "1": 1, "max": 0x7fff, "min": 0x8000, "-2": 0xfffe, "-1": 0xffff}
I8 = {"0": 0, "1": 1, "max": 0x7f, "min": 0x80, "-2": 0xfe, "-1": 0xff}
I64 = {"0": 0, "1": 1, "max": 0x7fffffffffffffff, "min": 0x8000000000000000,
       "-2": 0xfffffffffffffffe, "-1": 0xffffffffffffffff}


def classes_for(mnemonic: str):
    """(name, value, width) triples required by the mnemonic's suffixes."""
    req = []
    # b8/b16/b32/b64 name a width without a signedness: the raw bit patterns an
    # operand of that width admits are the same boundary set.
    for tag, tbl, bits in (("f32", F32, 32), ("f16", F16, 16),
                           ("f64", None, 64), ("i8", I8, 8), ("u8", I8, 8),
                           ("b8", I8, 8),
                           ("i16", I16, 16), ("u16", I16, 16), ("b16", I16, 16),
                           ("i32", I32, 32), ("u32", I32, 32), ("b32", I32, 32),
                           ("i64", I64, 64), ("u64", I64, 64), ("b64", I64, 64)):
        if tag not in mnemonic:
            continue
        if tag == "f64":
            # f64 boundary set, expressed as (low, high) pairs
            for name, v in (("+0", 0), ("-0", 1 << 63), ("+1", 0x3ff0000000000000),
                            ("-1", 0xbff0000000000000), ("+0.5", 0x3fe0000000000000),
                            ("max_norm", 0x7fefffffffffffff), ("min_norm", 0x0010000000000000),
                            ("min_denorm", 1), ("+inf", 0x7ff0000000000000),
                            ("-inf", 0xfff0000000000000), ("qnan", 0x7ff8000000000000),
                            ("snan", 0x7ff0000000000001)):
                req.append(("f64:" + name, v, 64))
        else:
            for name, v in tbl.items():
                req.append((tag + ":" + name, v, bits))
    return req


def words_of(s: str):
    return [int(w, 16) for w in re.findall(r"0[xX][0-9a-fA-F]+", s or "")]


def value_at(words, i, bits):
    if bits == 64:
        if i + 1 >= len(words):
            return None
        return words[i] | (words[i + 1] << 32)
    if bits == 32:
        return words[i] if i < len(words) else None
    # 16/8-bit: accept the value in either half of the word
    if i >= len(words):
        return None
    w = words[i]
    return w & ((1 << bits) - 1) if bits <= 16 else w


def covered(words, value, bits):
    if bits == 64:
        for i in range(len(words) - 1):
            if value_at(words, i, 64) == value:
                return True
        return False
    for i in range(len(words)):
        w = words[i]
        if bits == 32:
            if w == value:
                return True
        elif value == 0 and (w & 0xffff) == 0:
            return True  # any zero half counts
        else:
            if (w & 0xffff) == value or (w >> 16) == value:
                return True
    return False


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--tsv")
    args = ap.parse_args()

    mnemonics, cases = parse_case_files(ROOT)
    by = collections.defaultdict(list)
    for c in cases:
        by[c.mnemonic].append(words_of(c.inputs))

    # A row whose result is not an operand value cannot be asked for input
    # classes: an execution-environment read returns launch state, and the
    # interpolator returns the coefficient window.  Those are LAUNCH-SHAPE rows -
    # they need a launch state, not a case - and are reported separately rather
    # than counted as gaps the grid could close.
    sem = {}
    with open(Path(__file__).resolve().parent.parent /
              "lg200-instructions.tsv", encoding="utf-8") as f:
        for row in csv.DictReader(f, delimiter="\t"):
            sem[row["mnemonic"]] = row.get("semantic_model", "")
    # Rows whose result is launch state, not an operand: the environment and SIO
    # reads, the geometry-stream emits, the interpolators (a launch-owned
    # coefficient window) and the index read.  The vector<->scalar moves are NOT
    # here: their carrier stages the value, so their classes are ordinary input
    # coverage.
    LAUNCH_SEM = ("execution-environment", "graphics-sio")
    LAUNCH_EXTRA = ("v_v2s_b32", "v_s2v_b32", "v_firstv2s_b32")
    launch_skip = set(LAUNCH_EXTRA)

    rows, gaps, launch = [], collections.Counter(), []
    for m in sorted(mnemonics):
        if sem.get(m) in LAUNCH_SEM and m not in launch_skip:
            launch.append(m)
            continue
        req = classes_for(m)
        ws = by.get(m, [])
        missing = [name for name, v, bits in req
                   if not any(covered(w, v, bits) for w in ws)]
        rows.append((m, len(req), len(missing), ",".join(missing)))
        for x in missing:
            gaps[x.split(":", 1)[1] if ":" in x else x] += 1

    total = len(rows)
    full = sum(1 for r in rows if r[2] == 0)
    print("instructions: %d value-expressible (%d launch-shape)"
          % (total, len(launch)))
    print("  with all required boundary classes: %d   with gaps: %d"
          % (full, total - full))
    print("  launch-shape rows (need a launch state, not a case): %s"
          % " ".join(launch))

    # A floating-point case with no declared policy is silently evaluated as
    # "round to nearest, no flush" - which is NOT the measured hardware mode, so
    # such a row would disagree with the device on any denormal.  Report them.
    fp_rows = {m for m in mnemonics if sem.get(m) == "floating-point-value"}
    undeclared = sorted({c.mnemonic for c in cases
                         if c.mnemonic in fp_rows and not c.fp_policy})
    declared = sorted(fp_rows - set(undeclared))
    print("  FP rows: %d declared a policy, %d did NOT: %s"
          % (len(declared), len(undeclared), " ".join(undeclared) or "-"))
    print()
    print("%-28s %s" % ("class space", "instructions missing it"))
    for k, n in gaps.most_common():
        print("  %-26s %d" % (k, n))
    print()
    print("worst rows:")
    for m, nreq, nmiss, miss in sorted(rows, key=lambda r: -r[2])[:12]:
        print("  %-26s %2d/%2d missing: %s" % (m, nmiss, nreq, miss[:70]))

    if args.tsv:
        with open(args.tsv, "w") as f:
            f.write("mnemonic\trequired\tmissing\tmissing_classes\n")
            for r in rows:
                f.write("%s\t%d\t%d\t%s\n" % r)
        print("\n-> %s" % args.tsv)


if __name__ == "__main__":
    main()
