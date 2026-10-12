#!/usr/bin/env python3
"""Check that the suite has teeth, by breaking what each check looks at.

A check that passes is evidence only if it can fail.  This mutates a TRACKED file in a
way the check is supposed to notice, runs the check, and asserts it fails; then it restores
the file and asserts the check passes again, comparing hashes so the restore is verified
rather than assumed.  Only tracked files are touched, so git is a real safety net.

The first version of this test mutated a generic "768" in a document and a generation cell
in the TSV, and both checks "missed" - not because the checks are weak, but because neither
looks at those things.  A mutation test whose mutation the check does not read proves
nothing about the check.  These two mutations are taken from the checks own anchors:
set_desc forbids a literal newline in a cell, and check_doc_numbers reads a specific
numbered row of status.md.

Usage:  python3 mutation_check.py
"""
import hashlib
import pathlib
import re
import subprocess
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent.parent


def run(cmd):
    p = subprocess.run(cmd, shell=True, cwd=ROOT, capture_output=True, text=True)
    return p.returncode, (p.stdout + p.stderr)


def sha(path):
    return hashlib.sha256(pathlib.Path(path).read_bytes()).hexdigest()[:12]


def mutate_tsv(text):
    i = text.index("\\n")
    return text[:i] + chr(10) + text[i + 2:]


def mutate_doc(text):
    m = re.search(r"(rows stating an .UNRESOLVED. \| \d+ \| \*\*)(\d+)(\*\*)", text)
    return text[:m.start(2)] + "999" + text[m.end(2):]


CASES = [
    ("set_desc.py --check", "code/lg200-instructions.tsv", mutate_tsv,
     "cd code/lg200 && python3 set_desc.py --check"),
    ("check_doc_numbers.py", "docs/gpu/appendix/status.md", mutate_doc,
     "cd code/lg200 && python3 check_doc_numbers.py"),
]


def main() -> int:
    bad = 0
    for name, rel, mutate, check in CASES:
        full = ROOT / rel
        rc, _ = run("git ls-files --error-unmatch " + rel)
        if rc != 0:
            print("%-24s SKIPPED (not tracked, no safety net)" % name)
            bad += 1
            continue
        before = sha(full)
        original = full.read_text()
        mutated = mutate(original)
        if mutated == original:
            print("%-24s SKIPPED (mutation did not apply)" % name)
            bad += 1
            continue
        full.write_text(mutated)
        caught, _out = run(check)
        full.write_text(original)
        after = sha(full)
        clean, _out = run(check)
        ok = caught != 0 and clean == 0 and before == after
        print("%-24s fault exit=%d, restored %s, clean=%s  %s"
              % (name, caught, "ok" if before == after else "MISMATCH",
                 clean == 0, "OK" if ok else "PROBLEM"))
        if not ok:
            bad += 1
    print("mutation check: %d checks, %d problems" % (len(CASES), bad))
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())