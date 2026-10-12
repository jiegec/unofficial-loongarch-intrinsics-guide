#!/usr/bin/env python3
"""The result store's address register must never hold case data.

Address = descriptor_base + addr_reg + imm, so if addr_reg holds a case input
the store lands at (base + data + imm).  That is exactly the fault both hardware
suites hit - a GPU protection fault at kArgVa + 0x12345678 + imm, where
0x12345678 is the corpus' passthrough datum - and each one quarantined the CPIPE
and needed a platform power cycle.

Why this check exists rather than a comment: the first instance was fixed by
moving the address register from v9 to v15, which fixed that instance and
created the next one - v15 is the second dword of the wide atomic read-back
(`load_v_flat_b64 ... v[14:15]`).  A register-by-register fix cannot see that;
this walks every carrier program on both pipes, tracks which VGPRs hold loaded or
staged case data (INCLUDING multi-register destinations such as `v[14:15]`,
which a naive operand scan misses), and flags any result store whose address
register is one of them.

    python3 addr_scan.py        # expects "0 mnemonics" on both pipes
"""
import re, subprocess, sys
from pathlib import Path
GPU = Path(__file__).resolve().parent          # code/lg200/gpu
ROOT = GPU.parent                              # code/lg200
R = str(GPU / 'build/lgpu-gpu')
DATA = 0x12345678
rows = []
for line in (ROOT.parent / 'lg200-instructions.tsv').read_text().splitlines()[1:]:
    p = line.split('\t')
    if len(p) > 1 and p[1] and p[1] not in rows: rows.append(p[1])

def regs(spec):
    """'v8' -> [8]; 'v[14:15]' -> [14,15]; 'v[10:13]' -> [10,11,12,13]"""
    m = re.match(r'v\[(\d+):(\d+)\]$', spec)
    if m: return list(range(int(m.group(1)), int(m.group(2)) + 1))
    m = re.match(r'v(\d+)$', spec)
    return [int(m.group(1))] if m else []

total = 0
for pipe in ('ps', 'cs'):
    hits = {}
    for m in rows:
        cmd = [R,'run',m,'--dry-run'] if pipe=='ps' else [R,'csrun',m,'--dry-run']
        r = subprocess.run(cmd, input='x\t\t0x12345678 0x12345678 0x12345678 0x12345678\t0x12345678\n',
                           text=True, capture_output=True)
        imm, tainted = {}, set()
        for line in r.stdout.splitlines():
            line = line.strip()
            mm = re.match(r's_mov_imm_b32 \(scc\) s(\d+), 0x([0-9a-f]+)$', line)
            if mm: imm['s'+mm.group(1)] = int(mm.group(2), 16); continue
            # a move decides the register's value outright - check it FIRST, or
            # the zeroing that clears a staging register looks like a producer
            mm = re.match(r'v_mov_b32 p0, (v\[?\d+[:\d]*\]?), (zero|s\d+|v\[?\d+[:\d]*\]?)$', line)
            if mm:
                dst = regs(mm.group(1)); src = mm.group(2)
                if src == 'zero':
                    for v in dst: tainted.discard(v)
                elif src.startswith('s') and imm.get(src) != DATA:
                    for v in dst: tainted.discard(v)
                continue
            mm = re.match(r'(?:load_v_\w+|getreg|v_\w+) p0, (v\[?\d+[:\d]*\]?),', line)
            # any load/multi-register producer makes its destination pair data
            if mm:
                for v in regs(mm.group(1)): tainted.add(v)
                continue
            mm = re.match(r'store_v_buff_b32 p0, [^,]+, v(\d+),', line)
            if mm is None:
                mm2 = re.match(r'store_v_flat_b32 p0, [^,]+, v\[(\d+):\d+\],', line)
                addr = int(mm2.group(1)) if mm2 else None
            else:
                addr = int(mm.group(1))
            if addr is not None and addr in tainted:
                hits.setdefault(m, []).append(line)
    total += len(hits)
    print(f"=== {pipe}: {len(hits)} mnemonics store results through a register holding case data")
    for m in sorted(hits)[:6]: print(f"    {m:24s} {hits[m][0][:80]}")
print(f"addr scan: {total} mnemonics store results through a register holding case data")
# This guarded the fault that wedged the platform, and it could not fail: it
# printed a count and exited 0 for every count, so wiring it into the suite
# would have been theatre.  The exit code is the gate.
sys.exit(1 if total else 0)

