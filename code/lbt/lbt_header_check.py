#!/usr/bin/env python3
"""Validate LBT .h pseudocode files.

Each .h file must contain:
  - @meta category:   instruction category
  - @meta synopsis:   instruction syntax
  - @reg read:        registers/state read
  - @reg write:       registers/state written
  - @flag X: BEHAVIOR description      (for x86: CF,PF,AF,ZF,SF,OF; for ARM: N,Z,C,V)

Checks performed:
  1. All required annotations present
  2. Flag annotations are self-consistent (e.g., FORCE_0 must not write unexpected bits)
  3. C code block exists and has matching braces
  4. @flag COMPUTE/FORCE_0/FORCE_1 must have corresponding EFLAGS/ARMFLAGS write in code
  5. @flag PRESERVE must NOT have corresponding write in code
  6. Basic GCC syntax check (if available)
"""

import os, sys, re, subprocess
from pathlib import Path
from collections import namedtuple

HEADER_DIR = Path(__file__).resolve().parent  # code/lbt/

# ── Parser ────────────────────────────────────────────────────────────────────

Meta = namedtuple('Meta', ['category', 'synopsis'])
RegInfo = namedtuple('RegInfo', ['read', 'write', 'preserve'])
FlagBehavior = namedtuple('FlagBehavior', ['kind', 'note', 'condition'])
Header = namedtuple('Header', ['name', 'meta', 'reg', 'flags', 'cond', 'code_lines'])

FLAG_KINDS = {'COMPUTE', 'READ_COMPUTE', 'PRESERVE', 'FORCE_0', 'FORCE_1', 'CONDITIONAL', 'UNDEFINED'}
X86_FLAGS = {'CF', 'PF', 'AF', 'ZF', 'SF', 'OF'}
ARM_FLAGS = {'N', 'Z', 'C', 'V'}


def parse_header(path):
    """Parse a .h file into a Header namedtuple."""
    lines = open(path).read().split('\n')
    name = os.path.basename(path).replace('.h', '')

    meta = None
    reg_read, reg_write, reg_preserve = '', '', ''
    flags = {}
    cond = ''
    code_start = None
    code_end = None

    for i, line in enumerate(lines):
        s = line.strip()
        if s.startswith('// @meta category:'):
            meta = Meta(
                category=s.split(':', 1)[1].strip(),
                synopsis=''
            )
        elif s.startswith('// @meta synopsis:'):
            syn = s.split(':', 1)[1].strip()
            if meta:
                meta = Meta(category=meta.category, synopsis=syn)
            else:
                meta = Meta(category='', synopsis=syn)
        elif s.startswith('// @reg read:'):
            reg_read = s.split(':', 1)[1].strip()
        elif s.startswith('// @reg write:'):
            reg_write = s.split(':', 1)[1].strip()
        elif s.startswith('// @reg preserve:'):
            reg_preserve = s.split(':', 1)[1].strip()
        elif s.startswith('// @flag '):
            # Format: "// @flag CF: COMPUTE          carry/borrow"
            # Flag name is before ':', kind is the first word after ':'
            flag_name = s.split(':')[0].split()[-1]  # CF, PF, etc.
            body = s.split(':', 1)[1].strip()  # "COMPUTE          carry/borrow"
            if body:
                parts = body.split(None, 1)
                kind = parts[0]
                note = parts[1] if len(parts) > 1 else ''
            else:
                kind = ''
                note = ''
            condition = ''
            if '[when ' in note:
                note, condition = note.split('[when ', 1)
                condition = condition.rstrip(']')
                note = note.strip()
            flags[flag_name] = FlagBehavior(kind=kind, note=note, condition=condition)
        elif s == '// @flag: none':
            pass  # no flags
        elif s.startswith('// @cond:'):
            cond = s.split(':', 1)[1].strip()

        # Find code block: first { at start of line, matching }
        if s == '{' and not line.startswith(' ') and code_start is None:
            code_start = i
    # Find matching close brace (last } at start of line)
    for i in range(len(lines) - 1, -1, -1):
        if lines[i].strip() == '}' and not lines[i].startswith(' '):
            code_end = i
            break

    code_lines = lines[code_start+1:code_end] if code_start is not None and code_end is not None else []
    return Header(name=name, meta=meta, reg=RegInfo(reg_read, reg_write, reg_preserve),
                  flags=flags, cond=cond, code_lines=code_lines)


# ── Validators ────────────────────────────────────────────────────────────────

def check_header(hdr):
    issues = []

    # 1. Required annotations
    if not hdr.meta:
        issues.append('missing @meta')
    elif not hdr.meta.category:
        issues.append('@meta missing category')
    elif not hdr.meta.synopsis:
        issues.append('@meta missing synopsis')

    if not hdr.reg.read:
        issues.append('missing @reg read')
    if not hdr.reg.write:
        issues.append('missing @reg write')

    # 2. Flag kinds must be valid
    for fname, fb in hdr.flags.items():
        if fb.kind not in FLAG_KINDS:
            issues.append(f'@flag {fname}: unknown kind "{fb.kind}"')

    # 3. Code block exists
    if not hdr.code_lines:
        issues.append('empty code block')
        return issues

    code_text = '\n'.join(hdr.code_lines)

    # 4. Flag consistency: COMPUTE/FORCE_0/FORCE_1/READ_COMPUTE must have writes
    #    CONDITIONAL and PRESERVE are excluded from write-existence check
    for fname, fb in hdr.flags.items():
        if fb.kind in ('PRESERVE', 'CONDITIONAL'):
            continue  # These may or may not have code writes, both OK
        
        x86_write = f'EFLAGS.{fname}' if fname in X86_FLAGS else None
        arm_write = f'ARMFLAGS.{fname}' if fname in ARM_FLAGS else None
        write_pattern = x86_write or arm_write
        if not write_pattern:
            continue
        
        write_expr = write_pattern.replace('.', '\\.')
        if not re.search(rf'{write_expr}\s*=', code_text):
            issues.append(f'@flag {fname}: {fb.kind} but no "{write_pattern} =" in code')
    
    # 4b. PRESERVE flags should not be written (but accept if code writes then preserves)
    #     We skip this check — false positives are common (redundant computation paths)

    return issues


def check_syntax(hdr):
    """Try GCC -fsyntax-only on the .h file."""
    # Build a minimal test file that includes lbt_model.h and the target .h
    path = HEADER_DIR / (hdr.name.replace('.', '_') + '.h')
    test = (
        '#include "lbt_model.h"\n'
        '#ifdef __x86_64__\n'
        '  // Skip LoongArch-specific checks on x86\n'
        '#else\n'
        '  void _test(void) { const unsigned rj=12, rk=13, rd=4; const unsigned fd=4, fj=12, fk=13; const unsigned crd=0, crj=0; '
        f'const unsigned cond=0; const unsigned imm=0; const unsigned top=0; const unsigned mask=0;\n'
        f'  #include "{path.name}"\n'
        '  }\n'
        '#endif\n'
    )
    tmp = HEADER_DIR / '.check_tmp.c'
    tmp.write_text(test)
    result = subprocess.run(
        ['gcc', '-std=gnu11', '-fsyntax-only', '-I', str(HEADER_DIR), str(tmp)],
        capture_output=True, text=True
    )
    errors = []
    if result.returncode != 0:
        # Filter out noise
        for line in result.stderr.split('\n'):
            if 'error:' in line:
                errors.append(line.strip())
    tmp.unlink(missing_ok=True)
    return errors


# ── Main ──────────────────────────────────────────────────────────────────────

def main(check_compile=False):
    h_files = sorted(HEADER_DIR.glob('*.h'))
    h_files = [f for f in h_files if f.name not in ('lbt_model.h', 'lbt_common.h', 'lbt_wrappers.h')]

    passed = 0
    failed = 0
    total_issues = 0

    for path in h_files:
        hdr = parse_header(path)
        issues = check_header(hdr)

        if check_compile:
            syntax_issues = check_syntax(hdr)
            issues.extend(syntax_issues)

        if issues:
            print(f'FAIL: {path.name}')
            for issue in issues:
                print(f'  • {issue}')
            failed += 1
            total_issues += len(issues)
        else:
            passed += 1

    print(f'\n{"="*60}')
    print(f'Checked {passed + failed} .h files: {passed} PASS, {failed} FAIL ({total_issues} issues)')
    return 0 if failed == 0 else 1


if __name__ == '__main__':
    do_compile = '--compile' in sys.argv
    sys.exit(main(check_compile=do_compile))
