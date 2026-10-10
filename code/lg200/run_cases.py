#!/usr/bin/env python3
"""Run the LG200 oracle suite shipped in this directory.

Every <mnemonic>.cpp defines the executable reference pseudocode (the .h,
included by the oracle) and the deterministic case vectors (LG200_CASE_*
macros). This driver parses those vectors through cases.py, feeds them to
the compiled runner (build/lgpu-cases), and exits non-zero if any case fails.
"""
from __future__ import annotations

import subprocess
import sys
from pathlib import Path

from cases import parse_case_files

ROOT = Path(__file__).resolve().parent
RUNNER = ROOT / "build" / "lgpu-cases"
EXPECTED = 768


def fail(message: str) -> int:
    print(f"FAIL: {message}", file=sys.stderr)
    return 1


def main() -> int:
    try:
        mnemonics, cases = parse_case_files(ROOT, strict=True)
    except SystemExit as exc:
        print(str(exc), file=sys.stderr)
        return 1

    if len(mnemonics) != EXPECTED:
        return fail(f"expected {EXPECTED} mnemonics, found {len(mnemonics)}")
    if not RUNNER.is_file():
        return fail(f"runner missing; run 'make' first ({RUNNER})")

    rows = [
        f"{c.mnemonic}\t{c.fp_policy}\t{c.inputs}\t{c.expected}" for c in cases
    ]
    run = subprocess.run([str(RUNNER)], input="\n".join(rows) + "\n",
                         text=True, stdout=subprocess.PIPE,
                         stderr=subprocess.STDOUT)
    output = run.stdout
    if run.returncode != 0:
        print(output, end="")
        return fail("one or more CPU oracle cases failed")
    print(
        f"LG200 oracle cases: {len(mnemonics)} instructions, "
        f"{len(rows)} vectors: PASS")
    return 0


if __name__ == "__main__":
    sys.exit(main())
