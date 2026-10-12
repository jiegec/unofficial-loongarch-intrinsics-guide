#!/usr/bin/env python3
"""Propose boundary cases for floating-point rows, with expected values from the model.

The expected value of a case must come from the row's own model, never from a guess.  The
compiled runner prints the model's answer when the expected value is wrong
("expected X got Y"), so a deliberately wrong expected turns it into an oracle: feed the
boundary inputs, read back what the model says, and emit a correct CASE_FP line.

This is the 32-bit first pass.  Rows whose operands are 64-bit take two words per operand
and are skipped here rather than described wrongly.

  python3 fp_bounds_gen.py            report only
  python3 fp_bounds_gen.py --emit     print the CASE_FP lines to add
"""
from __future__ import annotations

import re
import subprocess
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
RUNNER = HERE / "build" / "lgpu-cases"

# one word per class, 32-bit
BOUNDS = [
    ("nan-pos", 0x7FC00000),
    ("nan-neg", 0xFFC00000),
    ("inf-pos", 0x7F800000),
    ("inf-neg", 0xFF800000),
    ("zero-neg", 0x80000000),
    ("subnormal-min", 0x00000001),
    ("subnormal-neg", 0x807FFFFF),
]
FILLER = 0x3F800000  # 1.0f, the neutral element for the comparison-shaped rows too
GOT = re.compile(r"got (0x[0-9a-fA-F]+)")


def main(argv) -> int:
    emit = "--emit" in argv
    rows = []
    for path in sorted(HERE.glob("*.cpp")):
        text = path.read_text(encoding="utf-8", errors="replace")
        m = re.search(r"LG200_ORACLE_BEGIN\(\s*(\w+)\s*,\s*(\d+)\s*\)", text)
        if not m:
            continue
        name, nin = m.group(1), int(m.group(2))
        if "_f64" in name or "_f16" in name:
            continue  # multi-word operands: a later pass
        if "CASE_FP(" not in text:
            continue
        rows.append((name, nin, path))
    print("32-bit floating-point rows considered: %d" % len(rows))
    total = 0
    for name, nin, path in rows:
        for cid, word in BOUNDS:
            inputs = [FILLER] * nin
            inputs[0] = word
            ins = " ".join(hex(v) for v in inputs)
            line = "\t".join([name, "rtne", ins, "0x1"])
            run = subprocess.run([str(RUNNER)], input=line + "\n", text=True,
                                 stdout=subprocess.PIPE,
                                 stderr=subprocess.STDOUT, timeout=60)
            got = GOT.search(run.stdout)
            if run.returncode == 0:
                continue  # already that value; nothing to propose
            if not got:
                continue
            total += 1
            if emit:
                print("CASE_FP(%s-%s, rtne, %s, %s)"
                      % (name, cid, ", ".join(hex(v) for v in inputs), got.group(1)))
    print("boundary cases the model would answer (%d rows): %d" % (len(rows), total))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
