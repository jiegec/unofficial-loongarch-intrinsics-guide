#!/usr/bin/env python3
"""Verify the jump-stack carriers against the closed driver's own ISA row.

jpush/jpop have no counterpart in the vendor's LLVM: `llvm-mc -arch=lggpu`
rejects both spellings, so the assembler that validates every other family
cannot be the oracle here.  The closed driver's machine description can be:
src/mesa/src/loonggpu/isa/lg200.xml states the value, all five mask classes, the
operand signature and the bit layout for both rows, and this catalog's rows are
byte-identical to it.

What is checked, per pipe:

  1. the catalog row equals the driver's row field for field;
  2. the emitted word places every operand exactly where that layout says -
     RA8@INST_LO[7:0], X2_23@INST_LO[30:8], X0_1@INST_LO[31],
     X1_8@INST_HI[15:8], PJ2@INST_HI[17:16] - by recomposing the word from the
     operands the carrier passes and comparing bit for bit;
  3. the two opcodes differ only in the bit the driver declares (INST_HI[18]);
  4. the carrier emits a BALANCED pair with a zero encoded position, which is
     what keeps the program linear: the branch unit's documented rule is that a
     zero displacement continues at the next instruction, so the record a pop
     consumes describes the instruction the push is followed by.

Usage: python3 jumpstack_check.py
"""
import csv
import re
import subprocess
import sys
import xml.etree.ElementTree as ET
from pathlib import Path

GPU = Path(__file__).resolve().parent
ROOT = GPU.parent
R = str(GPU / 'build/lgpu-gpu')
MESA = Path('/home/lin/work/src/loonggpu/src/mesa/src/loonggpu/isa/lg200.xml')
FIELDS = {'RA8': (0, 0, 8), 'X2_23': (0, 8, 23), 'X0_1': (0, 31, 1),
          'X1_8': (1, 8, 8), 'PJ2': (1, 16, 2)}
# operands the carrier passes, in the row's rule order (PJ2, RA8, X1_8, X2_23, X0_1)
OPS = {'PJ2': 0, 'RA8': 4, 'X1_8': 5, 'X2_23': 0, 'X0_1': 0}


def mesa_rows():
    out = {}
    for ins in ET.parse(MESA).getroot().iter('instruction'):
        if ins.get('name') in ('jpush', 'jpop'):
            e = ins.find('encoding').attrib
            o = ins.find('operands')
            out[ins.get('name')] = (e, o.get('layout'), o.get('signature'))
    return out


def catalog_rows():
    out = {}
    for r in csv.DictReader(open(ROOT.parent / 'lg200-instructions.tsv'),
                            delimiter='\t'):
        if r['mnemonic'] in ('jpush', 'jpop'):
            out[r['mnemonic']] = r
    return out


def emitted(mnemonic, pipe):
    cmd = ([R, 'run'] if pipe == 'ps' else [R, 'csrun']) + [mnemonic, '--dry-run']
    r = subprocess.run(cmd, input='x\t\t0x1\t0x1\n', text=True,
                       capture_output=True, env={'LGPU_DUMP_WORDS': '1', 'PATH': '/usr/bin'})
    listing = [l.strip() for l in r.stdout.splitlines()
               if re.match(r'^(jpush|jpop)\b', l.strip())]
    words = [(int(a, 16), int(b, 16)) for a, b in
             re.findall(r'word\s+\d+: lo=0x([0-9a-f]+) hi=0x([0-9a-f]+)', r.stdout)]
    # the two jump-stack instructions are the only ones with these opcodes
    js = [(lo, hi) for lo, hi in words if (hi & 0x0ffc0000) in (0x08000000, 0x08040000)]
    return listing, js


def main():
    mesa, cat = mesa_rows(), catalog_rows()
    bad = 0
    print("1. catalog vs the driver's ISA row")
    for m in ('jpush', 'jpop'):
        e, layout, sig = mesa[m]
        c = cat[m]
        same = (int(c['inst_lo'], 16) == int(e['value-inst-lo'], 16) and
                int(c['inst_hi'], 16) == int(e['value-inst-hi'], 16) and
                int(c['fixed_mask_lo'], 16) == int(e['fixed-mask-inst-lo'], 16) and
                int(c['fixed_mask_hi'], 16) == int(e['fixed-mask-inst-hi'], 16) and
                int(c['parameter_mask_lo'], 16) == int(e['parameter-mask-inst-lo'], 16) and
                int(c['parameter_mask_hi'], 16) == int(e['parameter-mask-inst-hi'], 16) and
                int(c['ignored_mask_hi'], 16) == int(e['print-ignored-mask-inst-hi'], 16) and
                int(c['reserved_mask_hi'], 16) == int(e['unresolved-mask-inst-hi'], 16) and
                c['operand_layout'] == layout and c['operand_signature'] == sig)
        print(f"   {'OK ' if same else 'DIFF'} {m:6s} value={c['inst_hi']} layout={layout[:46]}")
        bad += 0 if same else 1

    print("2. emitted field placement (recomposed from the operands passed)")
    for pipe in ('ps', 'cs'):
        for m in ('jpush', 'jpop'):
            listing, js = emitted(m, pipe)
            want_lo = want_hi = 0
            for f, (word, shift, bits) in FIELDS.items():
                v = OPS[f]
                if word == 0:
                    want_lo |= (v & ((1 << bits) - 1)) << shift
                else:
                    want_hi |= (v & ((1 << bits) - 1)) << shift
            want_lo |= int(cat[m]['inst_lo'], 16)
            want_hi |= int(cat[m]['inst_hi'], 16)
            ok_shape = all(len(l.split(',')) == 5 for l in listing) and len(listing) == 2
            ok_word = (want_lo, want_hi) in js
            ok_bal = [l.split()[0] for l in listing] == ['jpush', 'jpop']
            print(f"   {'OK ' if (ok_shape and ok_word and ok_bal) else 'DIFF'} "
                  f"{pipe} {m:6s} want {want_lo:08x} {want_hi:08x} "
                  f"emitted {[f'{a:08x} {b:08x}' for a, b in js]} "
                  f"operands={[len(l.split(',')) for l in listing]}")
            bad += 0 if (ok_shape and ok_word and ok_bal) else 1

    print("3. the opcodes differ only in the bit the driver declares")
    d = int(cat['jpush']['inst_hi'], 16) ^ int(cat['jpop']['inst_hi'], 16)
    print(f"   {'OK ' if d == 1 << 18 else 'DIFF'} jpush^jpop = 0x{d:08x} (expected 0x40000)")
    bad += 0 if d == 1 << 18 else 1
    print("problems:", bad)
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
