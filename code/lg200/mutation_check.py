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

PROVEN BOTH WAYS (this tool): set_desc, check_doc_numbers, header_sync, check_gpu.
PROVEN BY A ONE-OFF INJECTION ELSEWHERE: rule_dupe_check and md_table_check, both on
2026-10-13, by changing a size rule and by stranding a table row.

REPORTS BY DESIGN, so they have no failing mode to prove: prose_audit (its findings
include three expected classes of false positive) and doc_register_audit (its bound -
a wrong register name and nothing else - is stated in its own docstring).  format_audit
was in this group until 2026-10-13 and is now a gate: it returned 0 unconditionally,
so it could not fail on an over-claim, which is the defect it exists to find.

THE ELEVEN GATES, ACCOUNTED FOR (2026-10-13):

  proven in this tool (7):  set_desc, check_doc_numbers, header_sync, check_gpu,
                            run_cases, format_audit, bench_doc_check
  proven by injection (2):  rule_dupe_check, md_table_check
  reports by design (2):    prose_audit, doc_register_audit

The last two have no failing mode to prove: prose_audit findings include three expected
classes of false positive, and doc_register_audit states in its own docstring that it
catches a wrong register name and nothing else.  Every other gate has been shown to fail
on something it actually reads, which is what makes the suite mean anything: all checks
pass is worth as much as the number of them shown to be able to fail.

Getting a mutation right is most of the work.  Three times in this exercise the first
attempt changed something the check does not read - a generic 768, the generation cell of
the TSV, an early cell of the format table when format_audit reads only the last - and
each looked like a weak check until the mutation was aimed at the check own anchor.
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


def mutate_header(text):
    # header_sync compares the .h first line against the row's description
    lines = text.splitlines(keepends=True)
    lines[0] = lines[0].rstrip(chr(10)) + 'X' + chr(10)
    return ''.join(lines)


def mutate_row_count(text):
    # check_gpu asserts the documented row count
    lines = text.splitlines(keepends=True)
    return ''.join(lines[:1] + lines[2:])


def mutate_case(text):
    # run_cases compares each vector against the compiled runner: flip an expected value
    m = re.search(r"CASE\(add, 0x5, 0x3, 0x5, 0x8\)", text)
    return text[:m.start()] + "CASE(add, 0x5, 0x3, 0x5, 0x9)" + text[m.end():]


def mutate_format_claim(text):
    # format_audit compares the page table against the rows, and reads the LAST cell of
    # each row - so the field has to go in the operand-positions column.  The first
    # version of this inserted early in the row and was invisible: the third time in
    # this exercise that a mutation was not something the check reads.
    # Add a field the rows never
    # carry, which is the over-claim it exists to catch
    m = re.search(r"^\| 0 \| `sop` \|[^\n]*\|", text, re.M)
    return text[:m.end() - 1] + ", `VD99` " + text[m.end() - 1:]


def mutate_bench_doc(text):
    # bench_doc_check compares the page's headline with the tool's own output
    return text.replace('| buildable for both kinds | **626** |',
                        '| buildable for both kinds | **620** |', 1)



def suite_accounting():
    """Check that the suite's checks and this file's accounting agree.

    The accounting above is prose.  A twelfth check could be added to check_all.sh and
    the prose would quietly be wrong - the same shape as every drift this campaign has
    found, one level up from the numbers.  So the suite's script names are read and
    compared with the three lists the accounting describes."""
    here = pathlib.Path(__file__).resolve().parent
    script = here / "check_all.sh"
    text = script.read_text(encoding="utf-8")
    ran = set()
    # The suite announces every check as  echo "== name" .  Reading those is exact:
    # it needs no path resolution, and unlike a filename scan it still catches a check
    # whose script name is a typo, which is the failure the scan was meant to find.
    ran = set()
    for line in text.splitlines():
        line = line.strip()
        if line.startswith('echo "== ') and line.endswith('"'):
            ran.add(line[len('echo "== '):-1].split()[0])
    # both sides are reduced to the bare script name: the CASES entries carry
    # arguments and an extension, and the suite greps carry the extension
    def bare(name):
        return name.split()[0].rsplit('/', 1)[-1][:-3] if name.endswith('.py') \
            else name.split()[0].rsplit('/', 1)[-1].replace('.py', '')
    accounted = {bare(name) for name, _path, _mut, _cmd in CASES}
    accounted |= {"rule_dupe_check", "md_table_check"}      # proven by injection
    accounted |= {"prose_audit", "doc_register_audit"}       # reports by design
    missing = sorted(ran - accounted)
    extra_names = sorted(accounted - ran)
    bad = 0
    for name in missing:
        print("UNACCOUNTED  %s runs in check_all.sh but is in none of the three lists"
              % name)
        bad += 1
    for name in extra_names:
        print("STALE        %s is accounted for but no longer runs" % name)
        bad += 1
    return bad

CASES = [
    ("set_desc.py --check", "code/lg200-instructions.tsv", mutate_tsv,
     "cd code/lg200 && python3 set_desc.py --check"),
    ("check_doc_numbers.py", "docs/gpu/appendix/status.md", mutate_doc,
     "cd code/lg200 && python3 check_doc_numbers.py"),
    ("header_sync.py --check", "code/lg200/v_add_f32.h", mutate_header,
     "cd code/lg200 && python3 header_sync.py --check"),
    ("check_gpu.py", "code/lg200-instructions.tsv", mutate_row_count,
     "python3 check_gpu.py"),
    ("run_cases.py", "code/lg200/atomic_sm_add_u32.cpp", mutate_case,
     "cd code/lg200 && python3 run_cases.py"),
    ("format_audit.py", "docs/gpu/isa/encoding.md", mutate_format_claim,
     "cd code/lg200 && python3 format_audit.py"),
    ("bench_doc_check.py", "code/lg200/gpu/MICROBENCH.md", mutate_bench_doc,
     "cd code/lg200 && python3 bench_doc_check.py"),]


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
    bad += suite_accounting()
    print("mutation check: %d checks, %d problems" % (len(CASES), bad))
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())