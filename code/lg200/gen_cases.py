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
FLOAT = [0x00000000, 0x80000000, 0x3f000000, 0x3f800000, 0xbf800000,
         0x00800000, 0x7f7fffff, 0x7f800000, 0xff800000, 0x7fc00000]

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
    # nin == 3
    anchors = [0, 1, 0x80000000]
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
        if nin not in (1, 2, 3):
            continue
        out.append(mnemonic)
    return out


def main() -> int:
    meta = {}
    with open(TSV, encoding="utf-8") as f:
        for row in csv.DictReader(f, delimiter="\t"):
            meta[row["mnemonic"]] = row
    out = []
    for mnemonic in targets():
        sem = meta[mnemonic]["semantic_model"]
        patterns = FLOAT if sem == "floating-point-value" else INT
        nin = nin_of(mnemonic)
        # [HW 2026-08-24] ldexp carries an integer exponent in the last
        # slot: bit-pattern floats there are meaningless (and the generic
        # grid made most v_ldexp rows unverifiable).  Use the INT grid
        # for the exponent column.
        if mnemonic in ("v_ldexp_f64", "v_ldexp_f32"):
            if nin == 3:
                for row in grids(patterns, 2):
                    for e in INT:
                        out.append(mnemonic + "\t\t" +
                                   " ".join(hex(v) for v in row) + " " +
                                   hex(e))
            else:  # f32: (mantissa, exponent)
                for a in patterns:
                    for e in INT:
                        out.append(mnemonic + "\t\t" +
                                   hex(a) + " " + hex(e))
            continue
        for row in grids(patterns, nin):
            out.append(mnemonic + "\t\t" + " ".join(hex(v) for v in row))
    print("\n".join(out))
    return 0


if __name__ == "__main__":
    sys.exit(main())
