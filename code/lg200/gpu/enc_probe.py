#!/usr/bin/env python3
"""Cross-check this tree's encoder against the vendor's own assembler.

Why: this tree's codec and Mesa's `lg200.xml` were synchronized from the same
artifact, so comparing them proves a faithful *transcription* and nothing more.
The vendor's shipped LLVM is an independent authority for the bytes, and it runs
CPU-side, so it works even when the GPU is wedged.

    python3 enc_probe.py            # build + compare every probe
    python3 enc_probe.py --first 5  # one probe, verbose

Each probe exists twice: as a `McInst` in enc_probe.cpp (this tree's encoder) and
as vendor assembly text below (the vendor's assembler).  A difference means one
of the two is wrong, and it is worth knowing which before an instruction reaches
hardware - the carriers emit scaffolding around every case, so a wrong
scaffolding encoding corrupts results silently.
"""
from __future__ import annotations

import os
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent
PAIRS = ROOT.parent
LOONGGPU = Path("/home/lin/work/src/loonggpu")
E = LOONGGPU / "analysis/lacm/examples/lacm-runtime/opt/lacm-0.1.0"
SHIM = LOONGGPU / "tmp/lacm/shim"
LLVM_MC = E / "lib/llvm/bin/llvm-mc"
PROBE = ROOT / "build" / "enc_probe"

# Probe id -> the vendor assembler's spelling of the same instruction.  Where the
# vendor text omits an operand this tree spells (the flat base pair defaults to
# zero on the vendor side), the comparison is made against the vendor's default
# and the probe's own spelling is noted in enc_probe.cpp.
VENDOR_TEXT = {
    "mov_imm_b32": "s_mov.imm.b32 s16, 0x0, scc",
    "mov_imm_b32_hi": "s_mov.imm.b32 s0, 0xb0000, scc",
    "mov_b32_v_v": "v_mov.b32 p0, v0, v1",
    "mov_b32_v_s": "v_mov.b32 p0, v0, s16",
    "mov_b32_v_zero": "v_mov.b32 p0, v0, zero",
    "mov_b32_s_s": "s_mov.b32 s0, s16, scc",
    "load_s_flat_w2": "load.s.flat.w2 s[0:1], s[6:7], zero, 0x0",
    "load_s_flat_w8": "load.s.flat.w8 s[8:15], s[6:7], zero, 0x0",
    "load_s_flat_w4_off": "load.s.flat.w4 s[4:7], s[0:1], zero, 0x200",
    "store_v_flat_b32": "store.v.flat.b32 p0, v10, v[0:1]",
    "store_v_flat_b32_alt": "store.v.flat.b32 p0, v0, v[4:5]",
    "load_v_flat_b32": "load.v.flat.b32 p0, v10, v[0:1]",
    "store_v_buff_b32": "store.v.buff.b32 p0, v10, v9, s[0:3], zero, offen, 0x20",
    "load_v_buff_b32": "load.v.buff.b32 p0, v10, v0, s[4:7], zero, offen, 0x0",
    "load_sm_b32": "load.sm.b32 p0, v10, v8",
    "store_sm_b32": "store.sm.b32 p0, v9, v8",
    "atomic_sm_add_u32": "atomic.sm.add.u32 p0, v9, v0, v8",  # our order: (pred, addr, data, dst)
    "check": "check k|vm|vs|sm(0xf)",
    "nop": "nop",
    "exit": "exit",
}


def vendor_encodings(texts: dict) -> dict:
    """Assemble every vendor text in one llvm-mc run, in the given order."""
    src = "\n".join(texts[k] for k in texts) + "\n"
    env = dict(os.environ)
    env["LD_LIBRARY_PATH"] = f"{SHIM}:{E}/lib/llvm/lib:{E}/lib"
    r = subprocess.run([str(LLVM_MC), "-arch=lggpu", "-mcpu=lg200",
                        "-show-encoding"],
                       input=src, text=True, capture_output=True, env=env)
    out = {}
    order = list(texts)
    lines = [l for l in r.stdout.splitlines() if "; encoding:" in l]
    if len(lines) != len(order):
        return {"__error__": (r.stdout + r.stderr)}
    for key, line in zip(order, lines):
        m = re.search(r"encoding: \[([^\]]+)\]", line)
        bs = [int(x, 16) for x in m.group(1).split(",")]
        if len(bs) != 8:
            out[key] = None
            continue
        lo = sum(b << (8 * i) for i, b in enumerate(bs[:4]))
        hi = sum(b << (8 * i) for i, b in enumerate(bs[4:]))
        out[key] = (lo, hi)
    return out


def main() -> int:
    subprocess.run(["make", "-s", "build/enc_probe"], cwd=ROOT, check=True)
    r = subprocess.run([str(PROBE)], capture_output=True, text=True)
    ours = {}
    for line in r.stdout.splitlines():
        parts = line.split()
        if len(parts) == 3 and parts[1] != "ENCODE-FAILED":
            ours[parts[0]] = (int(parts[1], 16), int(parts[2], 16))
        else:
            print(f"cannot encode: {line}")
    vend = vendor_encodings(VENDOR_TEXT)
    if "__error__" in vend:
        print(vend["__error__"])
        return 2

    bad = 0
    for key, text in VENDOR_TEXT.items():
        if key not in ours or vend.get(key) is None:
            print(f"SKIP {key:24s} (not assembled by one side)")
            bad += 1
            continue
        same = ours[key] == vend[key]
        if not same:
            bad += 1
        print(f"{'OK ' if same else 'DIFF'} {key:24s} "
              f"ours {ours[key][0]:08x} {ours[key][1]:08x}  "
              f"vendor {vend[key][0]:08x} {vend[key][1]:08x}  | {text}")
    print(f"\n{len(VENDOR_TEXT) - bad} of {len(VENDOR_TEXT)} agree")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
