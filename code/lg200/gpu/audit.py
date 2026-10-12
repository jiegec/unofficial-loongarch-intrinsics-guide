#!/usr/bin/env python3
"""Static safety audit of every carrier program, read from the runner's own
listings (build/gpu-run-logs/<mnemonic>.log, written by `run_gpu.py --dry`).

Three defect classes have each wedged this GPU and cost a platform power cycle.
All three are visible in the emitted program:

  A  control flow   a linear carrier may only contain transfers whose target it
                    controls: `b` with zero displacement, plus the terminator.
                    Measured: jirl with an uninitialised source pair fetched at
                    address 0x8 and quarantined the CPIPE.
  B  address regs   a memory operand is added to a descriptor base, so a
                    register holding case data puts the access at (base + data +
                    imm).  Measured twice: kArgVa + 0x12345678 + imm.  Multi-
                    register destinations like v[14:15] count - missing them is
                    why an earlier scan passed a broken carrier.
  C  window         an immediate that names a VA must name the carrier's own
                    argument BO (0x100000000 / 0x1000b0000, one page each).

Usage: regenerate the listings first, then audit them.

    python3 run_gpu.py --dry            # writes the PS listings
    python3 audit.py ps
    python3 run_gpu.py --dry --pipe cs  # writes the CS listings
    python3 audit.py cs
"""
import collections
import csv
import re
import subprocess
import sys
from pathlib import Path

GPU = Path(__file__).resolve().parent
LOGS = GPU / 'build' / 'gpu-run-logs'
DATA = 0x12345678
ARG_LO, ARG_HI = 0x100000000, 0x100000000 + 0x2000
CS_ARG_LO, CS_ARG_HI = 0x1000b0000, 0x1000b0000 + 0x2000
# A balanced jump-stack pair with a zero encoded position is a controlled round
# trip (the branch unit's zero-displacement rule continues at the next
# instruction), so it is not a transfer out of the program.  Everything else
# that moves control must not appear.
# jirl is emitted with a staged target: the stub at a fixed VA.  It is only
# safe while that staging is present and correct, so the check verifies the
# staging rather than the mnemonic.
# jirl/trap/rfe all take their transfer position from a register pair, so each
# must be preceded by staging that pair with the stub address; a pair left zero
# transfers to 0 (measured: rfe, instruction fetch at 0x8).  Any other transfer
# instruction is a problem.
STAGED = {'jirl': 10, 'trap': 10, 'rfe': 18}
TRAMPOLINES = (0x100040000, 0x1000d0000)   # carrier::kTrampolineVa / kCsTrampolineVa
JIRL = re.compile(r'^jirl s\[\d+:\d+\], s\[(\d+):\d+\], 0x0+$')
PAIR = re.compile(r'^(jpush|jpop) p0, s\d+, s\d+, 0x0+, 0x0+$')

# ---- rule E: operand roles must cover the driver row's canonical form ----
#
# The carrier that wedged and faulted most often was the one whose emitted
# operand list did not match the machine row: store_v_buff_b16 went out with
# the 8-bit store's five-operand shape while its row declares the address VGPR
# (RA8 RK8x4 VQ8 VJ8 UO12 - six printed fields), so the descriptor landed in
# the VQ8 slot and the hardware addressed through it.  Rules A-D look at
# control flow, address registers, VA windows and the result pair; none of them
# looks at the operands themselves, so it passed.
#
# The driver's own seed assembly (code/lg200-instructions.tsv, seed_assembly)
# is the vendor's canonical print of that row, so it names the roles an emitter
# must supply - not the bit order of the layout column, which is a different
# order.  A coarse role (predicate / register / immediate) per printed operand
# is enough to catch a missing or doubled operand without needing to know where
# each field sits; a register field can legitimately print zero or a vector, so
# both classes collapse to REG.
#
# Scope, stated: a field the seed itself omits (buffer atomics print no VQ8
# while the codec encodes it as the zero selector) is outside this check, and a
# swap between two same-class registers is invisible to it.  It is a
# lower-bound coverage check, not an encoding oracle.
DATAROW = GPU.parent.parent / 'lg200-instructions.tsv'
MOD_TOKENS = {'offen', 'idxen', 'bl1', 'bl2', 'un', 'na', 'rsz', 'vm', 'mod2',
              'clmp1', 'dmask', 'sio', 'channum', 'chan'}
# Rows whose driver seed disagrees with the catalog's established roles and
# that the catalog itself writes UNRESOLVED.  They are recorded, not failed:
# guessing either reading is exactly what the project forbids.
E_ALLOWED = {
    ('jpush', ('REG',)): 'X2_23 prints as a register in the driver seed; the catalog marks its role UNRESOLVED',
    ('jpop', ('REG',)): 'X2_23 prints as a register in the driver seed; the catalog marks its role UNRESOLVED',
}


def _kind(tok, seed=False):
    tok = tok.strip()
    if not tok:
        return None
    if seed and tok in ('(scc)', 'SCC'):
        return None                      # the "(writes SCC)" canonical marker
    if tok.split(':')[0] in MOD_TOKENS:
        return None                      # modifier, not a positional operand
    if re.match(r'^p\d+$', tok):
        return 'P'
    if re.match(r'^v(\[|\d)', tok) or tok.startswith('vcc('):
        return 'REG'
    if re.match(r'^s(\[|\d)', tok) or tok in ('zero', 'SCC'):
        return 'REG'
    if re.match(r'^0x', tok) or re.match(r'^[if]\(', tok):
        return 'IMM'
    return None                          # BB_n / (n) branch position


def _kind_counter(tokens, seed=False):
    c = collections.Counter()
    for t in tokens:
        k = _kind(t, seed)
        if k:
            c[k] += 1
    return c


def _driver_rows():
    if not DATAROW.exists():
        return {}
    with open(DATAROW, newline='') as fh:
        return {r['mnemonic']: r for r in csv.DictReader(fh, delimiter='\t')}


DRIVER_ROWS = _driver_rows()


def _load_specs():
    """The codec's own rule set, dumped by the built runner.

    Rule F compares the driver row's declared fields against this, so the
    codec is the authority for what an emitter must cover rather than a second
    transcription of its class tables.  Returns None (and rule F reports that
    it could not run) when the runner is absent or refuses the dump.
    """
    exe = GPU / 'build' / 'lgpu-gpu'
    if not exe.exists():
        return None
    try:
        proc = subprocess.run([str(exe), '--specs'], text=True,
                              stdout=subprocess.PIPE, stderr=subprocess.DEVNULL,
                              timeout=30)
    except (OSError, subprocess.TimeoutExpired):
        return None
    if proc.returncode != 0:
        return None
    specs = collections.defaultdict(dict)
    for line in proc.stdout.splitlines():
        parts = line.split('\t')
        if len(parts) != 5:
            continue
        mnemonic, field, tuple_width, optional, is_output = parts
        specs[mnemonic][field] = (int(tuple_width), optional == '1',
                                  is_output == '1')
    return dict(specs)


SPECS = _load_specs()


def regs(spec):
    m = re.match(r'v\[(\d+):(\d+)\]$', spec)
    if m:
        return list(range(int(m.group(1)), int(m.group(2)) + 1))
    m = re.match(r'v(\d+)$', spec)
    return [int(m.group(1))] if m else []


def audit(path, pipe):
    problems, refused, built = [], 0, 0
    built_names = []
    for log in sorted(LOGS.glob('*.log')):
        if log.name.endswith('.retry.log'):
            continue   # the retry's own <mnemonic>.log is the audited copy
        text = log.read_text()
        if 'not run by this carrier' in text or 'envelope listing' not in text:
            refused += 1
            continue
        built += 1
        built_names.append(log.stem)
        m = log.stem
        tainted, imm, pending, result_pairs = set(), {}, '', []
        for raw in text.splitlines():
            line = raw.strip()
            if not line or line.startswith('--'):
                continue
            head = line.split()[0]
            if head in STAGED:
                # jirl/trap take the transfer position from their SECOND pair
                # (rd, rj, disp); rfe from its only pair.
                pairs = re.findall(r's\[(\d+):\d+\]', line)
                base = int(pairs[-1]) if pairs else STAGED[head]
                lo = imm.get('s%d' % base)
                hi = imm.get('s%d' % (base + 1))
                target = None if lo is None or hi is None else lo | (hi << 32)
                if target not in TRAMPOLINES:
                    problems.append(
                        (m, 'A', '%s without a staged stub target: %s' % (head, line)))
                continue
            if head in ('jpush', 'jpop'):
                if not PAIR.match(line) or pending != 'jpush' and head == 'jpop' and not pending:
                    problems.append((m, 'A', 'unbalanced jump-stack pair: ' + line))
                elif head == 'jpush':
                    pending = 'jpush'
                else:
                    pending = ''
                continue
            if head == 'b' and not re.match(r'^b\s+0x0+$', line):
                problems.append((m, 'A', line))
            mm = re.match(r's_mov_imm_b32 \(scc\) s(\d+), 0x([0-9a-f]+)$', line)
            if mm:
                imm['s' + mm.group(1)] = int(mm.group(2), 16)
                continue
            mm = re.match(r'v_mov_b32 p0, (v\[?\d+[:\d]*\]?), (zero|s\d+|v\[?\d+[:\d]*\]?)$', line)
            if mm:
                dst, src = regs(mm.group(1)), mm.group(2)
                val = imm.get(src) if src.startswith('s') else None
                if src == 'zero' or (val is not None and val != DATA):
                    for v in dst:
                        tainted.discard(v)
                # Any immediate at or above 4 GiB is this driver's VA space, so
                # it must name the carrier's own argument BO.  The first version
                # of this check compared against 0x10000000, which no carrier VA
                # can equal - it never fired.
                if val is not None and val >= 0x100000000:
                    if not (ARG_LO <= val < ARG_HI or CS_ARG_LO <= val < CS_ARG_HI):
                        problems.append((m, 'C', line))
                continue
            mm = re.match(r'(?:load_[vs]_\w+|getreg|v_\w+) p0, (v\[?\d+[:\d]*\]?),', line)
            if mm:
                for v in regs(mm.group(1)):
                    tainted.add(v)
                continue
            mm = re.match(r'(?:store|load|atomic)_[vs]_\w+ p0, [^,]+, v(\d+)', line) or \
                 re.match(r'(?:store|load|atomic)_v_\w+ p0, [^,]+, v\[(\d+):\d+\]', line)
            if mm and int(mm.group(1)) in tainted:
                problems.append((m, 'B', line))
            # D: the compute result store names an address PAIR, and no other
            # instruction in the program may read it.  It runs between case rows,
            # so a carrier that uses the pair as its memory index picks the
            # address up as data in the next row: the buff family faulted
            # (0x100160300 = buff base + arg_va + off) and the shared-memory
            # family read the wrong cell because an LDS offset is 16 bits and the
            # value wrapped instead of faulting.
            mm = re.match(r'store_v_flat_b32 p0, [^,]+, v\[(\d+):(\d+)\]', line)
            if mm:
                result_pairs.append((int(mm.group(1)), int(mm.group(2)), line))
    # D (applied after the walk, once the result pair is known)
    for log in sorted(LOGS.glob('*.log')):
        if log.name.endswith('.retry.log'):
            continue   # the retry's own <mnemonic>.log is the audited copy
        text = log.read_text()
        if 'envelope listing' not in text:
            continue
        m = log.stem
        lines = [l.strip() for l in text.splitlines() if l.strip() and not l.startswith('--')]
        pairs = set()
        for line in lines:
            mm = re.match(r'store_v_flat_b32 p0, [^,]+, v\[(\d+):(\d+)\]', line)
            if mm:
                pairs.add((int(mm.group(1)), int(mm.group(2))))
        # v0:v1 is the carriers' own flat-addressing pair (carrier_map.hpp), so
        # it is legitimately read by every flat access; the invariant is that the
        # RESULT store uses some other, dedicated pair, which is exactly what
        # this rule then enforces for every other pair.
        pairs.discard((0, 1))
        for lo, hi in pairs:
            # the result store's own staging is the four lines before it: two
            # s_mov_imm/v_mov pairs that build the absolute address
            own = set()
            for i, line in enumerate(lines):
                if line.startswith('store_v_flat_b32 p0, ') and ('v[%d:%d]' % (lo, hi)) in line:
                    own.update(range(max(0, i - 5), i + 1))
            for i, line in enumerate(lines):
                if i in own:
                    continue
                if re.search(r'\bv(%d|%d)\b' % (lo, hi), line) or \
                   re.search(r'\bv\[%d:' % lo, line):
                    problems.append((m, 'D', 'the result-store pair v[%d:%d] is also read: %s'
                                     % (lo, hi, line)))
                    break
    # E: every emitted target instruction must cover the roles the driver row
    # prints.  The row is looked up per mnemonic from the tracked instruction
    # table; the emitted line is the one whose head is that mnemonic.
    notes = []
    for log in sorted(LOGS.glob('*.log')):
        if log.name.endswith('.retry.log'):
            continue   # the retry's own <mnemonic>.log is the audited copy
        text = log.read_text()
        if 'not run by this carrier' in text or 'envelope listing' not in text:
            continue
        m = log.stem
        row = DRIVER_ROWS.get(m)
        if row is None:
            continue
        seed = row.get('seed_assembly') or ''
        if not seed.startswith(m):
            continue
        need = _kind_counter(re.split(r'[,\s]+', seed[len(m):]), seed=True)
        if not need:
            continue
        for raw in text.splitlines():
            line = raw.strip()
            if not line or line.startswith('--'):
                continue
            line = re.sub(r'^(\S+)\s+\(scc\)\s*', r'\1 ', line)
            head, _, rest = line.partition(' ')
            if head != m:
                continue
            got = _kind_counter(t.strip() for t in rest.split(',') if t.strip())
            missing = need - got
            if not missing:
                continue
            key = (m, tuple(sorted(missing.elements())))
            if key in E_ALLOWED:
                notes.append(('E', m, E_ALLOWED[key]))
                break
            problems.append((m, 'E', 'driver row prints %s, emitted %s: %s'
                             % (dict(need), dict(got), line)))
            break

    # F: every field the driver row declares must be modeled by the codec's
    # semantic rule class, and the class must not invent a field the row does
    # not declare.  Rule E reads what the row prints; F reads what it declares,
    # so a field the canonical form omits from text (buffer atomics print no
    # VQ8 while the class still encodes it as the zero selector) is covered
    # too.  It runs against the built programs, so it audits the rows this run
    # actually emits.  Fields the row marks X are parameter bits the canonical
    # form may leave at the template, so an unmodeled X is recorded, not failed.
    if SPECS is None:
        print("    [F] codec --specs unavailable; declared-field coverage not checked")
    else:
        for m in sorted(set(built_names)):
            row = DRIVER_ROWS.get(m)
            if row is None:
                continue
            layout = row.get('operand_layout') or ''
            if not layout or layout == '-':
                continue
            spec = SPECS.get(m)
            if spec is None:
                continue          # dedicated codec (no rule class)
            declared = [p.split('@')[0].strip()
                        for p in layout.split(';') if p.strip()]
            spec_base = {re.sub(r'x\d+$', '', f) for f in spec}
            declared_base = {re.sub(r'x\d+$', '', f) for f in declared}
            for f in declared:
                b = re.sub(r'x\d+$', '', f)
                if b.startswith('M.') or b in spec_base:
                    continue
                if b.startswith('X'):
                    notes.append(('F', m, 'declared parameter bit %s is not '
                                          'modeled (kept at the template)' % f))
                else:
                    problems.append((m, 'F', 'driver row declares %s but the codec '
                                     'class does not model it' % f))
            for f in sorted(spec_base - declared_base):
                problems.append((m, 'F', 'codec class models %s but the driver '
                                 'row does not declare it' % f))
            # Tuple widths are part of a field.  Where the driver spells one
            # (VJ8x4, VA8x2, RD8x16) and the codec's effective tuple differs,
            # the encoder/renderer disagree about how many registers the
            # operand names.  Recorded, not failed: the width is a per-mnemonic
            # override and the generator that carries it is not in this tree,
            # so the fix belongs with the catalog.  The overrides that are
            # deliberate (gather reads 3 of its 8 coordinate slots) and the
            # gaps that are not (load_s_flat_w* addresses a 2-register pair
            # while the shared sbuff-load class says 4) both show up here.
            for f in declared:
                n = re.search(r'x(\d+)$', f)
                if n is None or f.startswith('M.'):
                    continue
                b = re.sub(r'x\d+$', '', f)
                match = [v for k, v in spec.items()
                         if re.sub(r'x\d+$', '', k) == b]
                if match and match[0][0] != int(n.group(1)):
                    notes.append(('F', m, 'driver %sx%d vs codec tuple %d'
                                  % (b, int(n.group(1)), match[0][0])))

    if built == 0:
        # The trap this exists for: a batch or real-hardware run leaves a log set
        # with no "envelope listing", and a silent "0 programs audited, N
        # refused" reads like a clean result.  Fail loudly instead.
        print(f"=== {pipe}: STALE LOGS: no auditable programs under {LOGS} "
              f"({refused} unusable) - re-run run_gpu.py --dry first")
        return 1
    print(f"=== {pipe}: {built} programs audited, {refused} refused, {len(problems)} problems")
    for m, kind, detail in problems[:10]:
        print(f"    [{kind}] {m:24s} {detail[:74]}")
    agg = collections.Counter((tag, reason) for tag, _m, reason in notes)
    for (tag, reason), count in sorted(agg.items()):
        print(f"    [{tag} note] {count:3d} rows: {reason[:66]}")
    return len(problems)


if __name__ == '__main__':
    sys.exit(1 if audit(LOGS, sys.argv[1] if len(sys.argv) > 1 else '?') else 0)
