#!/usr/bin/env python3
"""Exhaustive-ish case generator for the LG200 oracle suite (u-l-i-g style).

For every instruction with 1-3 pure-register dword inputs (no immediate
slots in its operand signature, integer or floating-point semantics), a
boundary pattern grid is generated per input width:

  nin=1:  one pattern per value
  nin=2:  full pairwise grid
  nin=3:  three pairwise grids (a x b, b x c, a x c) with the remaining
          operand anchored, plus the all-equal diagonal

The generated rows carry inputs only; run through `build/lgpu-cases --fill`
to attach oracle-computed expected dwords:

  python3 gen_cases.py > raw.tsv
  ./build/lgpu-cases --fill < raw.tsv > generated_cases.tsv

Hand-written cases in the .cpp files are untouched; generated_cases.tsv is
committed next to them and merged by cases.py.
"""
from __future__ import annotations

import csv
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent
TSV = ROOT.parent / "lg200-instructions.tsv"

# Integer boundary sets, one per source width.  A 16-bit row's classes are the
# 16-bit extremes, not the 32-bit ones, so a single INT list leaves every narrow
# row short of the values its operands actually admit.
INT8 = [0x00, 0x01, 0x7F, 0x80, 0xFE, 0xFF]
INT16 = [0x0000, 0x0001, 0x7FFF, 0x8000, 0xFFFE, 0xFFFF]
INT = [0x00000000, 0x00000001, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFE, 0xFFFFFFFF]
# A narrow row still wants the wider patterns too (a row's neighbours and its
# destination may be wider), so the narrow set is a UNION, not a replacement.
INT_TBL = {"i8": INT8 + INT, "u8": INT8 + INT,
           "i16": INT16 + INT, "u16": INT16 + INT}

# 64-bit integer operands are word pairs; these cover the six classes the audit
# asks of a u64/i64 (0, 1, max, min, -2, -1) without a 36-pair expansion.
INT_PAIRS = [(0x00000000, 0x00000000), (0x00000001, 0x00000000),
             (0xffffffff, 0x7fffffff), (0x00000000, 0x80000000),
             (0xfffffffe, 0xffffffff), (0xffffffff, 0xffffffff)]

# Boundary classes required of every floating-point operand.  The set must name
# the whole signed space the format admits: both zeros, both infinities, quiet
# AND signalling NaN in both signs, the finite limits and the denormal range, so
# a row's cases cannot pin the ordinary values while leaving NaN, infinity or
# the subnormal boundary unexercised.
F32 = [0x00000000, 0x80000000, 0x3f000000, 0x3f800000, 0xbf800000,
       0x00800000, 0x80800000, 0x7f7fffff, 0xff7fffff,
       0x00000001, 0x007fffff, 0x80000001, 0x807fffff,
       0x7f800000, 0xff800000, 0x7fc00000, 0xffc00000,
       0x7f800001, 0xff800001]
# f16 operands sit in the low half of a word (the collected-vector convention).
F16 = [0x0000, 0x8000, 0x3800, 0x3c00, 0xbc00,
       0x0400, 0x8400, 0x7bff, 0xfbff,
       0x0001, 0x03ff, 0x8001, 0x83ff,
       0x7c00, 0xfc00, 0x7e00, 0xfe00,
       0x7c01, 0xfc01]
# f64 operands are word pairs, low word first.
F64 = [(0, 0), (0, 0x80000000), (0, 0x3fe00000), (0, 0x3ff00000),
       (0, 0xbff00000), (0, 0x00100000), (0, 0x80100000),
       (0xffffffff, 0x7fefffff), (0xffffffff, 0xffefffff),
       (1, 0), (0xffffffff, 0x000fffff), (1, 0x80000000),
       (0xffffffff, 0x800fffff), (0, 0x7ff00000), (0, 0xfff00000),
       (0, 0x7ff80000), (0, 0xfff80000), (1, 0x7ff00000), (1, 0xfff00000)]

# Backwards-compatible alias: the f32 set is what "the float grid" meant.
FLOAT = F32

# The hardware's floating-point mode, measured (2026-10-12): denormal INPUTS
# read as zero (v_cmp_eq_f32(0.0, min_denormal) is true) and denormal OUTPUTS
# are flushed (v_mul_f32(min_normal, 0.5) is 0, not the denormal 2^-127), i.e.
# daz + ftz around a round-to-nearest-even core.  The oracle's default policy is
# plain rtne, so every denormal case disagreed until this was carried.
FP_POLICY = "rtne|daz|ftz"

FLOAT_TAGS = ("f64", "f32", "f16")
# Every type token a name can end with; the LAST one names the source operand
# (names read v_<op>_<dst>_<src>), so v_fcvt_f64_i32 reads an integer.
TYPE_TOKENS = ("f64", "f32", "f16", "i64", "u64", "i32", "u32",
               "i16", "u16", "i8", "u8")


# A row whose SOURCE is 64-bit may still name 32-bit tags (v_fcvt_f32_f64).  The
# pair list then also carries those tags' boundaries as a pair's low word, so the
# 32-bit classes are exercised on a whole input word.
PAIR32 = {"f32": F32, "i32": INT, "u32": INT, "i16": INT16, "u16": INT16,
          "i8": INT8, "u8": INT8}


def pair_list(mnemonic: str):
    out, seen = [], set()
    cand = list(F64)
    for t, lst in PAIR32.items():
        if t in mnemonic:
            cand += [(v, 0) for v in lst]
    for p in cand:
        if p not in seen:
            seen.add(p)
            out.append(p)
    return out


def union_base(mnemonic: str, fmt: str):
    """The boundary list for a row, unioned over every type tag its name carries."""
    seen, out = set(), []
    for t in TYPE_TOKENS:
        if t not in mnemonic:
            continue
        lst = {"f64": F64, "f32": F32, "f16": F16}.get(t) or INT_TBL.get(t, INT)
        for v in lst:
            if v not in seen:
                seen.add(v)
                out.append(v)
    return out or INT_TBL.get(fmt, INT)


def input_format(mnemonic: str) -> str:
    """The format of the mnemonic's SOURCE operands.

    Names read v_<op>_<dst>_<src>..., so the last float tag names the input; a
    single-tag name has that tag on both sides.
    """
    last, pos = "", -1
    for t in TYPE_TOKENS:
        i = mnemonic.rfind(t)
        if i > pos:
            last, pos = t, i
    return last

# X0_16 is the unexercised modifier field the atomic layouts carry, not an
# operand slot: a case supplies no value for it (see the resolved note in
# code/lg200-instructions.tsv), so it must not disqualify a row from the grid.
IMM_SLOT = re.compile(r"I[A-Z]?\d+|X\d+(?!_16)")


def nin_of(mnemonic: str) -> int:
    text = (ROOT / f"{mnemonic}.cpp").read_text(encoding="utf-8")
    m = re.search(r"LG200_ORACLE_BEGIN\(\s*\w+\s*,\s*(\d+)\s*\)", text)
    return int(m.group(1)) if m else -1


def grid_cols(cols):
    """Boundary grid over per-column pattern lists.

    cols[i] is the list of values column i takes, so a row whose last input is an
    immediate gets register boundaries crossed with immediate boundaries rather
    than the immediate being gridded as if it were a value.
    """
    n = len(cols)
    if n == 1:
        for a in cols[0]:
            yield [a]
        return
    if n == 2:
        for a in cols[0]:
            for b in cols[1]:
                yield [a, b]
        return
    # A tuple-valued column list means the grid is over PAIRS (a 64-bit operand),
    # so the anchors must be pairs too.
    pairwise = bool(cols[0]) and isinstance(cols[0][0], tuple)
    anchors = ([(0, 0), (1, 0), (0, 0x80000000)] if pairwise
               else [0, 1, 0x80000000])
    if n == 4:
        # Four sources (a binary f64 row, or a 64-bit atomic pair): a full grid
        # would be |cols|^4, so take the two natural pairs plus the diagonal.
        for a in cols[0]:
            for b in cols[1]:
                yield [a, b, anchors[0], anchors[0]]
                yield [anchors[0], anchors[0], a, b]
        for x in cols[0]:
            yield [x, x, x, x]
        return
    # n == 3
    for a in cols[0]:
        for b in cols[1]:
            yield [a, b, anchors[0]]
            yield [a, anchors[1], b]
            yield [anchors[0], a, b]
    for x in cols[0]:
        yield [x, x, x]


def grids(patterns, nin: int):
    """The single-pattern-list case of grid_cols (every column the same)."""
    return grid_cols([patterns] * nin)


def targets() -> list:
    """Benchable/exhaustive-target mnemonics: pure-register 1-3 dword-input
    integer/float instructions (shared by the case generator and the bench
    driver)."""
    meta = {}
    with open(TSV, encoding="utf-8") as f:
        for row in csv.DictReader(f, delimiter="\t"):
            meta[row["mnemonic"]] = row
    out = []
    for mnemonic in sorted(meta):
        info = meta[mnemonic]
        sem = info.get("semantic_model", "")
        sig = info.get("operand_signature", "")
        if sem not in ("integer-value", "floating-point-value"):
            continue
        if IMM_SLOT.search(sig):
            continue
        # Memory/texture/control families stage operands through carrier
        # conventions (texel windows, LDS cells, sentinels, ...), so their
        # rows stay hand-written.
        if mnemonic.startswith(("load_", "store_", "atomic_", "samp",
                                "gather", "getlod", "getresinfo",
                                "copy_sm_", "getpc", "getreg", "jirl",
                                "jpush", "jpop", "nop", "exit", "trap",
                                "barrier", "cb", "czb", "flag", "prefix_")):
            continue
        nin = nin_of(mnemonic)
        if nin not in (1, 2, 3, 4):
            continue
        # A four-dword row is two pair operands (a binary f64; the bench's own
        # set stays f64-only).  The case generator admits the 64-bit integer
        # compares too, in case_targets below.
        if nin == 4 and input_format(mnemonic) != "f64":
            continue
        out.append(mnemonic)
    return out


# The case generator reaches further than the bench does.  The atomic families
# stage the cell and the operand straight from the case inputs - their address
# comes from the case index, not from a case value - so a boundary grid is
# stageable even though the bench cannot time them.
ATOMIC_OK = re.compile(r"^atomic_(sm|v_buff|v_flat|v_tex)_")
# The sub-word loads stage their memory content from the case's FIRST input (the
# carriers store it before loading), so a boundary grid exercises exactly the
# classes the row's element width admits.  Their second input is the address
# offset, which must stay inside the carrier's window and so is held at zero.
LOAD_OK = re.compile(r"^load_(sm|v_buff|v_flat)_[iu](8|16)$")


def case_targets() -> list:
    out = list(targets())
    meta = {}
    with open(TSV, encoding="utf-8") as f:
        for row in csv.DictReader(f, delimiter="\t"):
            meta[row["mnemonic"]] = row
    for mnemonic in sorted(meta):
        sig = meta[mnemonic].get("operand_signature", "")
        if ATOMIC_OK.match(mnemonic):
            if not IMM_SLOT.search(sig) and nin_of(mnemonic) in (2, 3, 4, 6):
                out.append(mnemonic)
            continue
        if LOAD_OK.match(mnemonic):
            if nin_of(mnemonic) in (1, 2):
                out.append(mnemonic)
            continue
        # Rows whose last input is an immediate: the register columns keep the
        # value grid and the immediate column gets its own, which is what the
        # bench's target set has never been able to express.
        if not IMM_SLOT.search(sig):
            continue
        # (the i64/u64 pair rows are admitted by the dedicated pass below)
        if mnemonic.startswith(("load_", "store_", "samp", "gather", "get",
                                "copy_sm_", "jirl", "jpush", "jpop", "nop",
                                "exit", "trap", "barrier", "cb", "czb", "flag",
                                "prefix_", "sio")):
            continue
        if meta[mnemonic].get("semantic_model") not in ("integer-value",
                                                        "floating-point-value"):
            continue
        if nin_of(mnemonic) in (1, 2, 3):
            out.append(mnemonic)
    # The 64-bit integer compares are two pair operands, like a binary f64; they
    # are outside the bench's set but want the same pair grid.
    for mnemonic in sorted(meta):
        if mnemonic.startswith(("load_", "store_", "atomic_", "samp", "gather")):
            continue
        if nin_of(mnemonic) in (4, 5, 6) and input_format(mnemonic) in ("i64", "u64", "f64"):
            out.append(mnemonic)
    return list(dict.fromkeys(out))


def main() -> int:
    meta = {}
    with open(TSV, encoding="utf-8") as f:
        for row in csv.DictReader(f, delimiter="\t"):
            meta[row["mnemonic"]] = row
    out = []

    def flat(row):
        """Expand a row of f64 pairs into the dword stream the table stores."""
        if row and isinstance(row[0], tuple):
            vals = []
            for pair in row:
                vals += [pair[0], pair[1]]
            return vals
        return list(row)

    for mnemonic in case_targets():
        sem = meta[mnemonic]["semantic_model"]
        nin = nin_of(mnemonic)
        # The name's type token decides the boundary set even when the row's
        # semantic model is not a value class (an f32 compare-and-swap is an
        # atomic, but its operands are still f32 and want the f32 boundaries).
        fmt = input_format(mnemonic)
        # A row may name more than one width (v_fcvt_f32_f16, v_dp4a_i8_i8_i32):
        # the audit asks for every tag's classes, and a union list carries them
        # all in the same column, since a 32-bit class is matched on the whole
        # word and a 16-bit one on either half.
        base = union_base(mnemonic, fmt)
        # [HW 2026-08-24] ldexp carries an integer exponent in the last
        # slot: bit-pattern floats there are meaningless (and the generic
        # grid made most v_ldexp rows unverifiable).  Use the INT grid
        # for the exponent column.
        if mnemonic in ("v_ldexp_f64", "v_ldexp_f32"):
            if fmt == "f64":
                for row in grids(F64, (nin - 1) // 2):
                    for e in INT:
                        out.append(mnemonic + "\t" + FP_POLICY + "\t" +
                                   " ".join(hex(v) for v in flat(row)) + " " +
                                   hex(e))
            else:  # f32: (mantissa, exponent)
                for a in base:
                    for e in INT:
                        out.append(mnemonic + "\t" + FP_POLICY + "\t" +
                                   hex(a) + " " + hex(e))
            continue
        # A 64-bit operand is a word pair in every family, including the b64
        # atomics whose names carry no type token.
        pfmt = fmt if fmt in ("f64", "i64", "u64") else ("b64" if "b64" in mnemonic
                                                         and fmt == "" else "")
        if pfmt and nin % 2 == 0 and nin > 0:
            plist = pair_list(mnemonic) if pfmt == "f64" else INT_PAIRS
            for row in grids(plist, nin // 2):
                out.append(mnemonic + "\t" + FP_POLICY + "\t" +
                           " ".join(hex(v) for v in flat(row)))
            continue
        pol = FP_POLICY if sem == "floating-point-value" else ""
        if IMM_SLOT.search(meta[mnemonic].get("operand_signature", "")) and nin > 1:
            # The immediate is the last input (the ISA prints it last and the
            # seed reads v_cmp_imm_eq_f16 p0, s0, v0, 0x00000000).
            cols = [base] * (nin - 1) + [base]
            gen = grid_cols(cols)
        elif LOAD_OK.match(mnemonic) and nin == 2:
            # content | address offset (held in-window)
            gen = grid_cols([base, [0]])
        else:
            gen = grids(base, nin)
        for row in gen:
            out.append(mnemonic + "\t" + pol + "\t" +
                       " ".join(hex(v) for v in flat(row)))
    print("\n".join(out))
    return 0


if __name__ == "__main__":
    sys.exit(main())
