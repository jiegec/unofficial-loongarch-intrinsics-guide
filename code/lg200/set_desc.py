#!/usr/bin/env python3
"""Safe single-row editor for the LG200 instruction table.

The table (code/lg200-instructions.tsv) is the vendored source of truth for the
instruction pages.  Phase-C documentation work rewrites the `description` cell
of individual rows, so this helper enforces the house rules:

  * read the whole table, then write the whole table (never truncate in place);
  * keep the header and the row count identical;
  * a cell may not contain a literal tab or newline: newlines are stored as the
    two-character escape `\n` and expanded by main.py at render time;
  * leave a snapshot under /tmp before writing.

Usage:
  python3 set_desc.py <mnemonic> <markdown-file>   # set one description
  python3 set_desc.py --show <mnemonic>            # print the current value
  python3 set_desc.py --check                      # integrity check
"""
import csv
import os
import pathlib
import shutil
import sys

ROOT = pathlib.Path(__file__).resolve().parent
TSV = ROOT.parent / "lg200-instructions.tsv"
SNAPSHOT = pathlib.Path("/tmp/lg200-instructions.tsv.bak")


def load():
    # Plain tab-separated text: cells may contain any character except a tab or a
    # newline, and are never quoted, so the file is split by hand rather than
    # through the csv module (which would treat a leading quote specially).
    with open(TSV, newline="", encoding="utf-8") as f:
        lines = f.read().split("\n")
    while lines and lines[-1] == "":
        lines.pop()
    fields = lines[0].split("\t")
    rows = []
    for line in lines[1:]:
        cells = line.split("\t")
        if len(cells) != len(fields):
            raise SystemExit("row has %d cells, expected %d: %s" % (len(cells), len(fields), line[:60]))
        rows.append(dict(zip(fields, cells)))
    return fields, rows


def save(fields, rows):
    if not SNAPSHOT.exists():
        shutil.copyfile(TSV, SNAPSHOT)
    tmp = TSV.with_suffix(".tsv.tmp")
    with open(tmp, "w", newline="", encoding="utf-8") as f:
        f.write("\t".join(fields) + "\n")
        for row in rows:
            f.write("\t".join(row.get(k, "") or "" for k in fields) + "\n")
    os.replace(tmp, TSV)


def check():
    fields, rows = load()
    problems = []
    seen = set()
    for i, row in enumerate(rows, start=2):
        name = row.get("mnemonic") or ""
        if not name:
            problems.append("row %d: empty mnemonic" % i)
        if name in seen:
            problems.append("row %d: duplicate mnemonic %s" % (i, name))
        seen.add(name)
        for key, value in row.items():
            if value is None:
                problems.append("row %d (%s): missing column %s" % (i, name, key))
        desc = row.get("description") or ""
        if "\t" in desc or "\n" in desc:
            problems.append("row %d (%s): raw tab/newline in description" % (i, name))
    print("rows=%d columns=%d unique=%d" % (len(rows), len(fields), len(seen)))
    for p in problems[:20]:
        print("  " + p)
    return 1 if problems else 0


def main(argv):
    if len(argv) == 1 and argv[0] == "--check":
        return check()
    if len(argv) == 2 and argv[0] == "--show":
        _, rows = load()
        for row in rows:
            if row["mnemonic"] == argv[1]:
                print(row["description"])
                return 0
        print("no such mnemonic: " + argv[1], file=sys.stderr)
        return 1
    if len(argv) != 2:
        print(__doc__)
        return 2
    mnemonic, path = argv
    text = pathlib.Path(path).read_text(encoding="utf-8").strip()
    if "\t" in text:
        print("description must not contain a tab", file=sys.stderr)
        return 1
    cell = text.replace("\n", "\\n")
    fields, rows = load()
    hits = 0
    for row in rows:
        if row["mnemonic"] == mnemonic:
            row["description"] = cell
            hits += 1
    if hits != 1:
        print("expected exactly one row for %s, found %d" % (mnemonic, hits), file=sys.stderr)
        return 1
    save(fields, rows)
    print("updated %s (%d chars)" % (mnemonic, len(cell)))
    return check()


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
