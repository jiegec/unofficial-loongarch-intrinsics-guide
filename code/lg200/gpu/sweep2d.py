#!/usr/bin/env python3
"""Two-dimensional operand sweep for one mnemonic, with the batch cap enforced.

Why this exists
---------------
A hand-rolled sweep packed 144 cases into one `csrun` process and drew
"program or result window exceeds the persistent cache size".  That message is a
device-level warning, not a per-batch rejection, and retrying after it left the
device parked.  run_gpu.py already caps a batch at CHUNK = 64; a hand-rolled
batch simply ignored that.  This tool imports the cap instead of restating it,
so a sweep cannot exceed it, and it refuses to submit at all unless the
preconditions pass.

Usage
-----
    sweep2d.py --mnemonic v_cmp_nle_f64 --dry-run
    sweep2d.py --mnemonic v_cmp_nle_f64            # submits, prints a table
"""
import argparse
import pathlib
import re
import subprocess
import sys

HERE = pathlib.Path(__file__).resolve().parent
ROOT = HERE.parents[1]
G = HERE / "build" / "lgpu-gpu"

# One batch is never larger than the runner's own chunk: the cap is imported,
# not repeated, so the two cannot drift apart.
sys.path.insert(0, str(HERE))
from run_gpu import CHUNK  # noqa: E402

CLASSES64 = {
    "p0": "0x0 0x0", "n0": "0x0 0x80000000",
    "pden": "0x1 0x0", "nden": "0x1 0x80000000",
    "p1": "0x0 0x3ff00000", "n1": "0x0 0xbff00000",
    "pinf": "0x0 0x7ff00000", "ninf": "0x0 0xfff00000",
    "pmax": "0xffffffff 0x7fefffff", "nmax": "0xffffffff 0xffefffff",
    "pqnan": "0x0 0x7ff80000", "nqnan": "0x0 0xfff80000",
    # [HW 2026-10-13] the first grid gave every class a zero or a large half, so a
    # rule that flushed only denormal halves fitted all 144 cells and still failed on
    # finite-min-pos, whose high half is 0x00100000 - the smallest NORMAL f32.  These
    # half-fill each side: small normals and denormals in either half.
    # Names carry no underscore: the result regex splits a "FAIL a_b:" line on the
    # single underscore, so a class name containing one is silently unparsed.
    "hismallnorm": "0x0 0x00100000", "hismallden": "0x0 0x1",
    "losmallnorm": "0x00800000 0x0", "losmallden": "0x1 0x0",
    "hitwo": "0x0 0x40000000", "bothone": "0x3f800000 0x3f800000",
}
CLASSES32 = {
    "p0": "0x0", "n0": "0x80000000",
    "pden": "0x1", "nden": "0x80000001",
    "p1": "0x3f800000", "n1": "0xbf800000",
    "pinf": "0x7f800000", "ninf": "0xff800000",
    "pmax": "0x7f7fffff", "nmax": "0xff7fffff",
    "pqnan": "0x7fc00000", "nqnan": "0xffc00000",
}


def parked():
    try:
        out = subprocess.run(["dmesg"], text=True, capture_output=True).stdout
    except OSError:
        return None
    return sum(1 for line in out.splitlines() if "parked" in line)


def preconditions():
    """Returns None when the device is usable, else the reason to stop."""
    n = parked()
    if n:
        return f"dmesg shows {n} parked line(s)"
    if not pathlib.Path("/dev/dri/renderD128").exists():
        return "the node /dev/dri/renderD128 is missing"
    one = pathlib.Path("/tmp/sweep2d-one.txt")
    one.write_text("x\t\t0x1\t0x1\n")
    r = subprocess.run([str(G), "csrun", "s_abs_i32"], stdin=one.open(),
                       text=True, capture_output=True, cwd=str(ROOT))
    if "cannot open" in (r.stdout + r.stderr):
        return "the one-case smoke test cannot open the device"
    return None


def build(mnemonic, classes):
    rows = []
    for a, av in classes.items():
        for b, bv in classes.items():
            rows.append(f"{a}_{b}\trtne\t{av} {bv}\t0xdeadbeef")
    return rows


def batches(rows):
    return [rows[i:i + CHUNK] for i in range(0, len(rows), CHUNK)]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--mnemonic", required=True)
    ap.add_argument("--width", choices=["32", "64"], default="64")
    ap.add_argument("--dry-run", action="store_true")
    args = ap.parse_args()
    classes = CLASSES64 if args.width == "64" else CLASSES32
    rows = build(args.mnemonic, classes)
    bs = batches(rows)
    print(f"{args.mnemonic}: {len(rows)} probes in {len(bs)} batch(es), "
          f"cap CHUNK={CHUNK} -> largest {max(len(b) for b in bs)}")
    if args.dry_run:
        for i, b in enumerate(bs):
            print(f"  batch {i}: {len(b)} rows, first={b[0].split(chr(9))[0]}")
        return
    reason = preconditions()
    if reason:
        print(f"REFUSING to submit: {reason}")
        return
    res = {}
    for i, b in enumerate(bs):
        r = subprocess.run([str(G), "csrun", args.mnemonic],
                           input="\n".join(b) + "\n", text=True,
                           capture_output=True, cwd=str(ROOT))
        out = r.stdout + r.stderr
        if "cannot open" in out:
            print(f"batch {i}: device lost - stopping, not retrying")
            return
        for m in re.finditer(r"^FAIL ([a-z0-9]+)_([a-z0-9]+): .*got (0x[0-9a-f]+)",
                             out, re.M):
            res[(m.group(1), m.group(2))] = int(m.group(3), 16)
        print(f"  batch {i}: {len(b)} rows probed")
    names = list(classes)
    print("a\\b    " + " ".join(f"{n:<5}" for n in names))
    for a in names:
        cells = []
        for b in names:
            v = res.get((a, b))
            cells.append(f"{'T' if v else 'F' if (a, b) in res else '?':<5}")
        print(f"{a:<6} " + " ".join(cells))


if __name__ == "__main__":
    main()
