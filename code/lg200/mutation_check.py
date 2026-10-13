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

  proven in this tool (19): set_desc, check_doc_numbers, header_sync, check_gpu,
                            run_cases, format_audit, bench_doc_check,
                            rule_dupe_check, md_table_check, check_all.sh,
                            fp_mode_audit, case_provenance, addr_scan, gate_check,
                            jumpstack_check, gen_state_map, link_check,
                            case_coverage
  note: addr_scan has two cases.  One forces a hit and proves the EXIT CODE; the
        other breaks the detector, which the tool's --selftest catches over a
        crafted listing, including the v[14:15] shape a register scan misses.
  reports by design (2):    prose_audit, doc_register_audit
  session-time (2):        audit - reads a runner listing, which is not tracked, so it
                           runs only when one is present and no case can break its input;
                           enc_probe - compares against a cross-repo Mesa tree and builds
                           a probe first, so there is nothing here to break
                           runs only when one is present and no case can break its input

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
    # The runner is mutated by a case of its own.  It runs, but it does not announce
    # itself the way the checks it drives do, so it is counted on the ran side.
    ran.add('check_all.sh')
    # rule_dupe_check and md_table_check were proven by one-off injection
    # until they joined CASES; the union below no longer needs them named twice.
    accounted |= {"prose_audit", "doc_register_audit"}       # reports by design
    # Session-time gates read a device-produced artifact that is not tracked, so they
    # run only when there is something to audit and no case here can break their input.
    accounted |= {"audit", "enc_probe"}
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

def mutate_size_rule(text):
    """Break one page's copy of a size rule, leaving the other page's copy alone."""
    return text.replace("at least 16 and at most 102",
                        "at least 16 and at most 101", 1)


def mutate_table(text):
    """Detach a row from every table by starting a new one at the end."""
    return text.rstrip(chr(10)) + chr(10) + chr(10) + "| stranded | row | And text that no table wants. |" + chr(10)

def mutate_suite(text):
    """Make the last pipeline in the suite fail, which is the shape it once hid.

    Without set -o pipefail a failed command whose output is piped to tail returns tail's
    status, and the suite printed ALL CHECKS PASSED while a check had not run.  This
    mutation reproduces exactly that shape, so the fix stays fixed."""
    return text.replace('echo "ALL CHECKS PASSED"',
                        'false | tail -1' + chr(10) + 'echo "ALL CHECKS PASSED"', 1)

def mutate_fp_mode(text):
    """Change a policy-token count on the page, leaving the cases alone."""
    return text.replace("`ftz` in 92", "`ftz` in 93", 1)

def mutate_provenance(text):
    """Change the unmeasured-vector count on the page, leaving the grid alone."""
    return text.replace('+62,969 vectors', '+62,970 vectors', 1)

def mutate_addr_scan(text):
    """Force a hit, which tests the exit code rather than the detection."""
    return text.replace('total += len(hits)', 'total += len(hits) + 1', 1)

def mutate_gate_file(text):
    """Take a gate's failure path away, so gate_check should report it."""
    return text.replace('return 1', 'return 0', 1)

def mutate_addr_detect(text):
    """Break the detector rather than the exit code, so the selftest should notice."""
    return text.replace('    return hits', '    return []', 1)

def mutate_case_list(text):
    '''Rename a case's gate, leaving that gate in the suite but unproven.

    The target is assembled rather than written out.  Spelled literally, the first
    occurrence in the file is this function's own pattern, so the mutation renamed
    itself instead of the case - and the check passed, which is how it was found.
    '''
    q = chr(34)
    old = '(' + q + 'run_cases.py' + q + ', ' + q + 'code/lg200/atomic_sm_add_u32.cpp' + q
    new = '(' + q + 'run_cases_renamed.py' + q + ', ' + q + 'code/lg200/atomic_sm_add_u32.cpp' + q
    return text.replace(old, new, 1)

def mutate_jumpstack(text):
    '''Break the check's own expectation, which proves the verdict path and not the
    comparison: the data it compares against is the vendor's ISA row, in another
    repository, so there is nothing here to break that would change the answer.''',
    return text.replace('bad += 0 if d == 1 << 18 else 1',
                        'bad += 0 if d == 1 << 19 else 1', 1)

def mutate_state_map(text):
    '''Change a label, which alters what is rendered but not the block table.

    So it leaves check() passing and the asset comparison to catch it - the half that
    did not exist until this round.''',
    return text.replace('"Draw Info"', '"Draw Info!"', 1)

def mutate_link(text):
    '''Point a page's link at a file that does not exist.'''
    return text.replace('(isa/family-vector.md)', '(isa/family-vector-renamed.md)', 1)

def mutate_class_space(text):
    '''Remove a boundary class from a width's table, which must no longer be allowed.'''
    q = chr(34)
    old = '    ' + q + 'snan' + q + ': 0x7f800001, ' + q + '-snan' + q + ': 0xff800001,'
    return text.replace(old, '    ' + q + 'snan' + q + ': 0x7f800001,', 1)

def mutate_taxonomy(text):
    '''Rename a tool in the taxonomy, so it no longer accounts for that tool.'''
    q = chr(96)
    return text.replace(q + 'fp_bounds_gen.py' + q, q + 'fp_bounds_gen-renamed.py' + q, 1)

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
     "cd code/lg200 && python3 bench_doc_check.py"),
    ("rule_dupe_check.py", "docs/gpu/shader/registers.md", mutate_size_rule,
     "cd code/lg200 && python3 rule_dupe_check.py"),
    ("md_table_check.py", "docs/gpu/appendix/status.md", mutate_table,
     "cd code/lg200 && python3 md_table_check.py"),
    ("check_all.sh", "code/lg200/check_all.sh", mutate_suite,
     "sh code/lg200/check_all.sh"),
    ("fp_mode_audit.py", "docs/gpu/appendix/case-boundaries.md", mutate_fp_mode,
     "cd code/lg200 && python3 fp_mode_audit.py --check"),
    ("case_provenance.py", "docs/gpu/appendix/status.md", mutate_provenance,
     "cd code/lg200 && python3 case_provenance.py --check"),
    ("addr_scan.py", "code/lg200/gpu/addr_scan.py", mutate_addr_scan,
     "python3 code/lg200/gpu/addr_scan.py"),
    ("gate_check.py", "code/lg200/md_table_check.py", mutate_gate_file,
     "python3 code/lg200/gate_check.py"),
    ("addr_scan.py", "code/lg200/gpu/addr_scan.py", mutate_addr_detect,
     "python3 code/lg200/gpu/addr_scan.py --selftest"),
    ("gate_check.py", "code/lg200/mutation_check.py", mutate_case_list,
     "python3 code/lg200/gate_check.py"),
    ("jumpstack_check.py", "code/lg200/gpu/jumpstack_check.py", mutate_jumpstack,
     "python3 code/lg200/gpu/jumpstack_check.py"),
    ("gen_state_map.py", "code/lg200/gen_state_map.py", mutate_state_map,
     "python3 code/lg200/gen_state_map.py --check"),
    ("link_check.py", "docs/gpu/index.md", mutate_link,
     "python3 code/lg200/link_check.py"),
    ("case_coverage.py", "code/lg200/case_coverage.py", mutate_class_space,
     "python3 code/lg200/case_coverage.py"),
    ("gate_check.py taxonomy", "docs/gpu/appendix/status.md", mutate_taxonomy,
     "python3 code/lg200/gate_check.py"),]


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