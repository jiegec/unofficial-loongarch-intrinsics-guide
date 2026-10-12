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

INT = [0x00000000, 0x00000001, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFE, 0xFFFFFFFF]

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
    return last if last in FLOAT_TAGS else ""

IMM_SLOT = re.compile(r"(?:X\d+|I[A-Z]?\d+)")


def nin_of(mnemonic: str) -> int:
    text = (ROOT / f"{mnemonic}.cpp").read_text(encoding="utf-8")
    m = re.search(r"LG200_ORACLE_BEGIN\(\s*\w+\s*,\s*(\d+)\s*\)", text)
    return int(m.group(1)) if m else -1


def grids(patterns, nin: int):
    if nin == 1:
        for a in patterns:
            yield [a]
        return
    if nin == 2:
        for a in patterns:
            for b in patterns:
                yield [a, b]
        return
    anchors = [0, 1, 0x80000000]
    if nin == 4:
        # Four sources (a binary f64 row is four dwords).  A full grid would be
        # |patterns|^4, so take the two natural pairs plus the all-equal
        # diagonal: enough to cross every boundary value with every other.
        for a in patterns:
            for b in patterns:
                yield [a, b, anchors[0], anchors[0]]
                yield [anchors[0], anchors[0], a, b]
        for x in patterns:
            yield [x, x, x, x]
        return
    # nin == 3
    for a in patterns:
        for b in patterns:
            yield [a, b, anchors[0]]
            yield [a, anchors[1], b]
            yield [anchors[0], a, b]
    for x in patterns:
        yield [x, x, x]


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
        # A four-dword row is a binary f64 (two pair operands); nothing else
        # has a four-source pure-register shape.
        if nin == 4 and input_format(mnemonic) != "f64":
            continue
        out.append(mnemonic)
    return out


# The case generator reaches further than the bench does.  The atomic families
# stage the cell and the operand straight from the case inputs - their address
# comes from the case index, not from a case value - so a boundary grid is
# stageable even though the bench cannot time them.
ATOMIC_OK = re.compile(r"^atomic_(sm|v_buff|v_flat)_")


def case_targets() -> list:
    out = list(targets())
    meta = {}
    with open(TSV, encoding="utf-8") as f:
        for row in csv.DictReader(f, delimiter="\t"):
            meta[row["mnemonic"]] = row
    for mnemonic in sorted(meta):
        if not ATOMIC_OK.match(mnemonic):
            continue
        if IMM_SLOT.search(meta[mnemonic].get("operand_signature", "")):
            continue
        if nin_of(mnemonic) in (2, 3, 4):
            out.append(mnemonic)
    return out


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
        fmt = input_format(mnemonic) if sem == "floating-point-value" else ""
        base = {"f64": F64, "f16": F16, "f32": F32}.get(fmt, INT)
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
        if fmt == "f64":
            # every source operand is a word pair, so the grid is over pairs
            for row in grids(F64, nin // 2):
                out.append(mnemonic + "\t" + FP_POLICY + "\t" +
                           " ".join(hex(v) for v in flat(row)))
            continue
        pol = FP_POLICY if sem == "floating-point-value" else ""
        for row in grids(base, nin):
            out.append(mnemonic + "\t" + pol + "\t" +
                       " ".join(hex(v) for v in row))
    print("\n".join(out))
    return 0


if __name__ == "__main__":
    sys.exit(main())
