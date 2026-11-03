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
            # The same set as the 32-bit rows, as word pairs.  It was five short until
            # 2026-10-13 - no negative max/min, no max_denorm, no negative NaN - so f64
            # rows were being judged against a weaker requirement than f32 and f16, which
            # is the defect this file exists to catch, one level up.
            for name, v in (("+0", 0), ("-0", 1 << 63), ("+1", 0x3ff0000000000000),
                            ("-1", 0xbff0000000000000), ("+0.5", 0x3fe0000000000000),
                            ("max_norm", 0x7fefffffffffffff),
                            ("-max_norm", 0xffefffffffffffff),
                            ("min_norm", 0x0010000000000000),
                            ("-min_norm", 0x8010000000000000),
                            ("max_denorm", 0x000fffffffffffff),
                            ("min_denorm", 1), ("+inf", 0x7ff0000000000000),
                            ("-inf", 0xfff0000000000000), ("qnan", 0x7ff8000000000000),
                            ("-qnan", 0xfff8000000000000),
                            ("snan", 0x7ff0000000000001),
                            ("-snan", 0xfff0000000000001)):
                req.append(("f64:" + name, v, 64))
        else:
            for name, v in tbl.items():
                req.append((tag + ":" + name, v, bits))
    return req


# --- payload rules: the 123 rows whose NAME carries no [uibf]NN tag ----------
# classes_for() reads the type tag out of the name, so a row called
# load_s_buff_w8 or samp_c_d is asked for nothing and its "0 gaps" says nothing.
# Those rows still move a value; what their classes are is read from the row's
# own .h (which columns the model reads) and from the width the model copies -
# a whole 32-bit word in every one of them.  The rule is applied ONLY when
# classes_for() came back empty, so no verdict on a tagged row can change.
PAYLOAD_RULES = (
    # Scalar wide loads: the size is a WORD COUNT (w1..w16), so no tag is in the
    # name.  The model copies the dwords (load_s_buff_w8.h:4-6), and the row
    # states that 0x80000000 keeps its set bit rather than being extended.
    (r"^load_s_(buff|flat)_w(1|2|4|8|16)$", "b32"),
    # Descriptor/flat block loads and stores: the size is a BIT BLOCK (b96/b128),
    # i.e. 3 or 4 raw dwords (load_v_flat_b96.h:4-7).
    (r"^(load|store)_v_(buff|flat)_b(96|128)$", "b32"),
    # Formatted loads/stores: the element format lives in the descriptor or in
    # the instruction field, and the carrier builds the descriptor, so the case
    # word is a whole dword the model copies (load_v_buff_fmtinst_c4.h:4-5).
    (r"^(load|store)_v_(buff|flat)_fmt(desc|inst)_c[1-4]$", "b32"),
    # Shared-memory reads: one word per lane per plane (family-memory.md:111).
    (r"^load_sm_(interp|matrix_x[124])$", "b32"),
    # Image fetches, gathers and samples: the texel word.
    (r"^(load|store)_v_tex(_mip)?(_pck)?(_sgn)?$", "b32"),
    (r"^(gather|samp)(_[a-z0-9]+)*$", "b32"),
    # The descriptor queries.
    (r"^(getlod|getresinfo)$", "b32"),
    # The scalar compares: s_cmp.h compares with cmp_eq_u32 and the row says
    # "two 32-bit scalar values", so both columns are 32-bit operands.
    (r"^s_cmp(_imm)?$", "b32"),
)


def payload_cols(mnemonic: str):
    """The input columns the row's own model reads - the payload, not the
    carrier's address/selector columns, which must stay inside the window."""
    body = (ROOT / (mnemonic + ".h")).read_text(encoding="utf-8")
    code = "\n".join(l for l in body.splitlines()
                     if l.strip() and not l.strip().startswith("//"))
    cols = {int(i) for i in re.findall(r"src\[(\d+)\]", code)}
    # src[0+1] is src[1]: the word-pair form the wide loads and the matrix x2
    # read.  A bare src[\d+] pattern does not see it (gen_cases.py:409 loses the
    # column for exactly this reason).
    for base, off in re.findall(r"src\[(\d+)\+(\d+)\]", code):
        cols |= {int(base), int(base) + int(off)}
    return sorted(c for c in cols if c >= 0)


def payload_for(mnemonic: str):
    """(name, value, bits, column) for the boundaries a tagless row admits."""
    tag = next((t for pat, t in PAYLOAD_RULES if re.match(pat, mnemonic)), None)
    if tag is None:
        return []
    cols = payload_cols(mnemonic)
    if not cols:
        return []          # the model reads no input: nothing to ask for
    tbl, bits = {"b32": (I32, 32), "b16": (I16, 16)}[tag]
    return [(tag + ":" + n, v, bits, c) for c in cols for n, v in tbl.items()]


def col_covered(cases, value, bits, col):
    """Covered in THIS column when one is named, in any word when col < 0.

    `cases` is the row's case list, one word list per case, so a column test
    asks whether SOME case puts the value in that column.
    """
    if col < 0:
        return any(covered(w, value, bits) for w in cases)
    return any(col < len(w) and covered([w[col]], value, bits) for w in cases)


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


# The class space is the claim that every boundary class is covered, and it was read
# from the tables below - so weakening a table weakened the requirement with it.
# Deleting -snan from F32 left the report at 738 of 738 with 0 gaps, because the
# cases no longer had to contain the class.  The names are pinned here instead.
REQUIRED_NAMES = (
    "+0", "+0.5", "+1", "+inf",
    "-0", "-1", "-inf", "-max_norm",
    "-min_norm", "-qnan", "-snan", "max_denorm",
    "max_norm", "min_denorm", "min_norm", "qnan",
    "snan",
)


def check_class_space():
    """Does each width still ask for the whole boundary class space?"""
    problems = []
    for m in ("v_add_f32", "v_add_f16", "v_add_f64"):
        # each entry is ("f64:+0", value, width), so compare the bare class name
        have = {n.split(":", 1)[1] for n, _v, _w in classes_for(m)}
        lost = [n for n in REQUIRED_NAMES if n not in have]
        if lost:
            problems.append("%s asks for %d classes and has lost %s" % (m, len(have), ", ".join(lost)))
    return problems

def main():
    lost = check_class_space()
    for p in lost:
        print(p)
    if lost:
        return 1
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
        ws = by.get(m, [])
        req = [(n, v, b, -1) for n, v, b in classes_for(m)]
        kind = "tagged"
        if not req:
            req = payload_for(m)
            kind = "payload" if req else "no-result"
        missing = [(name, col) for name, v, bits, col in req
                   if not col_covered(ws, v, bits, col)]
        shown = [n if c < 0 else "%s@c%d" % (n, c) for n, c in missing]
        rows.append((m, len(req), len(missing), ",".join(shown), kind))
        # one instruction counts once per class, however many of its columns
        # miss it
        for x in {n.split(":", 1)[1] if ":" in n else n for n, _c in missing}:
            gaps[x] += 1

    total = len(rows)
    # A row whose name carries no type tag is asked for NO class by
    # classes_for(), so "missing == 0" says nothing about it.  Counting those as
    # complete is how this gate reported "with gaps: 0" while load_s_buff_w8 and
    # 122 other payload/control rows had never been asked for anything.
    # The bucket is now named instead of counted as a hole.
    unasked = sum(1 for r in rows if r[1] == 0)
    full = sum(1 for r in rows if r[2] == 0 and r[1] > 0)
    payload = [r for r in rows if r[4] == "payload"]
    noresult = [r for r in rows if r[4] == "no-result"]
    print("instructions: %d value-expressible (%d launch-shape)"
          % (total, len(launch)))
    print("  with all required boundary classes: %d   with gaps: %d"
          % (full, total - full - unasked))
    print("  asked for NO class: %d  (%d payload-rule, %d no-result)"
          % (unasked, len(payload), len(noresult)))
    print("  launch-shape rows (need a launch state, not a case): %s"
          % " ".join(launch))
    print("  payload rows (name carries no [uibf]NN tag, model reads a dword):"
          " %d, all classes: %d, with gaps: %d"
          % (len(payload),
             sum(1 for r in payload if r[2] == 0),
             sum(1 for r in payload if r[2] > 0)))
    print("  no-result rows (oracle carries 0 inputs and 0 outputs): %d: %s"
          % (len(noresult), " ".join(r[0] for r in noresult)))

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
    for m, nreq, nmiss, miss, _kind in sorted(rows, key=lambda r: -r[2])[:12]:
        # The whole missing-class list, not the first 70 columns of it: WHICH classes
        # are missing is the actionable part.
        print("  %-26s %2d/%2d missing: %s" % (m, nmiss, nreq, miss))

    if args.tsv:
        with open(args.tsv, "w") as f:
            f.write("mnemonic\trequired\tmissing\tmissing_classes\tkind\n")
            for r in rows:
                f.write("%s\t%d\t%d\t%s\t%s\n" % r)
        print("\n-> %s" % args.tsv)


if __name__ == "__main__":
    sys.exit(main())
