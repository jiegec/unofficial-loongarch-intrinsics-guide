#!/usr/bin/env python3
"""Keep each header's first comment line in step with its row's description.

Every code/lg200/<mnemonic>.h opens with

    // <mnemonic> -- <the row's first description line>
    //

and that line is hand-maintained: set_desc.py writes in the other direction, from a file
INTO the tsv, so nothing regenerates the headers when a description changes.  Five had
drifted by 2026-10-13, all of them rows whose text this campaign had corrected -
s_addc_u32 and the four v_dp4a_* rows.  The bodies had been updated correctly; only the
comment lagged, which is exactly the failure mode that makes a header untrustworthy.

  python3 header_sync.py --check     report drift (exit 1 if any)
  python3 header_sync.py             rewrite the drifting lines from the rows

Only lines beginning "// <mnemonic> --" are ever touched, so a comment written in some
other shape is left alone rather than clobbered.
"""
from __future__ import annotations

import csv
import pathlib
import sys

HERE = pathlib.Path(__file__).resolve().parent
TSV = HERE.parent / "lg200-instructions.tsv"


def load():
    rows = {}
    for r in csv.DictReader(TSV.open(), delimiter="\t"):
        rows[r["mnemonic"]] = r
    return rows


def first_line(desc: str) -> str:
    # the header quotes exactly the tsv's first line; strip the literal \n markers
    return desc.replace("\\n", "\n").split("\n")[0].strip()


def main(argv) -> int:
    check_only = argv == ["--check"]
    rows = load()
    drift = []
    for mnemonic, r in rows.items():
        path = HERE / (mnemonic + ".h")
        if not path.exists():
            continue
        lines = path.read_text(encoding="utf-8").splitlines(keepends=True)
        if not lines:
            continue
        prefix = "// " + mnemonic + " -- "
        if not lines[0].startswith(prefix):
            continue
        want = prefix + first_line(r.get("description") or "")
        have = lines[0].rstrip("\n")
        if have == want:
            continue
        drift.append(mnemonic)
        if not check_only:
            lines[0] = want + "\n"
            path.write_text("".join(lines), encoding="utf-8")
    if check_only:
        print("headers out of step with their rows: %d" % len(drift))
        for m in drift:
            print("   " + m)
        return 1 if drift else 0
    print("headers rewritten: %d" % len(drift))
    for m in drift:
        print("   " + m)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
