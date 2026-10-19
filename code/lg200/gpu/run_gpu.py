#!/usr/bin/env python3
"""Driver for the LG200 PS-generic hardware runner.

Runs every instruction of the universe through build/lgpu-gpu on the real
GPU, one process per (instruction, chunk), and reports a PASS/FAIL matrix.

    python3 run_gpu.py [--dry] [--mnemonic m] [--timeout S] [--only-ok]
       --dry        only assemble the envelopes (no DRM submission)
       --mnemonic m run one instruction
       --only-ok    stop after the first non-PASS instruction
       --timeout S  per-process timeout in seconds (default 30)

Results: build/gpu_ps_generic_results.tsv (full runs; mnemonic runs land in
build/gpu_last_run.tsv) plus a per-instruction log in build/gpu-run-logs/.
"""
from __future__ import annotations

import re
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent
PAIRS = ROOT.parent  # code/lg200
# The 768-mnemonic universe: the tracked instruction table's mnemonic column.
UNIVERSE = PAIRS.parent / "lg200-instructions.tsv"
RUNNER = ROOT / "build" / "lgpu-gpu"
OUT = ROOT / "build" / "gpu_ps_generic_results.tsv"       # full-matrix runs only
OUT_LAST = ROOT / "build" / "gpu_last_run.tsv"            # every invocation
LOGDIR = ROOT / "build" / "gpu-run-logs"

sys.path.insert(0, str(PAIRS))
from cases import parse_case_files  # noqa: E402


def case_rows_by_mnemonic() -> dict:
    """Case rows (id/fp/inputs/expected) grouped per mnemonic, parsed once."""
    try:
        _, cases = parse_case_files(PAIRS)
    except SystemExit:
        return {}
    by: dict = {}
    for c in cases:
        by.setdefault(c.mnemonic, []).append(
            f"{c.id}\t{c.fp_policy}\t{c.inputs}\t{c.expected}")
    return by


def materializable_matrix() -> dict:
    sub = subprocess.run([str(RUNNER), "--matrix"], text=True,
                         stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    out = {}
    for line in sub.stdout.splitlines():
        parts = line.split("\t")
        if len(parts) >= 2:
            out[parts[0]] = parts[1]
    return out


def read_universe() -> dict:
    """The 768-mnemonic universe from the tracked instruction table's
    mnemonic column (col 2); no local copy is maintained."""
    out = {}
    if UNIVERSE.is_file():
        for line in UNIVERSE.read_text(encoding="utf-8").splitlines():
            parts = line.split("\t")
            if len(parts) >= 2 and parts[1] and parts[1] != "mnemonic":
                out[parts[1]] = parts[0] if parts[0] != "generation" else ""
    return out


# Case rows per PS program: larger sets are chunked so the envelope and the
# result region stay bounded (one submission + one process per chunk).
CHUNK = 64


def run_one(mnemonic, rows, dry, timeout_s):
    chunks = [rows[i:i + CHUNK] for i in range(0, len(rows), CHUNK)] or [[]]
    merged = {"status": "PASS", "pass": 0, "fail": 0, "rows": len(rows),
              "detail": "", "out": ""}
    for chunk in chunks:
        argv = [str(RUNNER), "run", mnemonic]
        if dry:
            argv.append("--dry-run")
        try:
            r = subprocess.run(argv, input="\n".join(chunk) + "\n", text=True,
                               stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                               timeout=timeout_s)
            rc = r.returncode
            out = r.stdout
        except subprocess.TimeoutExpired:
            merged.update({"status": "TIMEOUT",
                           "detail": f"timeout {timeout_s}s"})
            break
        merged["pass"] += len(re.findall(r"^PASS ", out, re.M))
        merged["fail"] += len(re.findall(r"^FAIL ", out, re.M))
        merged["out"] += out
        last = (out.strip().splitlines() or [""])[-1] if out.strip() else ""
        if "PASS " in last or "FAIL " in last:
            merged["detail"] = last
        if rc == 2:
            merged.update({"status": "SKIP-ENVELOPE", "detail": last})
            break
        if rc == 3:
            merged.update({"status": "HANG/FAULT", "detail": last})
            break
        if rc not in (0, 1):
            merged.update({"status": "ERROR", "detail": last})
            break
        if rc == 1 and merged["status"] == "PASS":
            merged["status"] = "FAIL"
    return merged


GOLDEN = ROOT / "build" / "golden"

# One representative envelope per carrier shape.
GOLDEN_TARGETS = [
    "v_add_u32", "load_v_buff_b32", "atomic_v_buff_cmpswap_b32",
    "load_sm_b32", "atomic_sm_add_u64", "load_v_flat_b32",
    "load_v_tex_mip", "samp", "samp_c_b_cl_o", "gather_c_l_o",
    "atomic_v_tex_swap_b32", "store_v_tex", "getresinfo",
    "s_cmp_eq_u64",
]


def golden() -> int:
    """Regenerate (or --check to verify) golden envelope listings per carrier.

    Protects refactors: any assembler-visible change must be a conscious
    golden update, not an accident.
    """
    check = "--check" in sys.argv
    GOLDEN.mkdir(parents=True, exist_ok=True)
    failures = 0
    by = case_rows_by_mnemonic()
    for mnemonic in GOLDEN_TARGETS:
        rows = by.get(mnemonic) or [f"{mnemonic}\t\t0x12345678\t0x12345678"]
        r = subprocess.run(
            [str(RUNNER), "run", mnemonic, "--dry-run"],
            input="\n".join(rows) + "\n", text=True,
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        listing = "\n".join(
            l for l in r.stdout.splitlines()
            if not l.startswith("-- envelope"))
        want = GOLDEN / f"{mnemonic}.listing"
        if check:
            if not want.is_file() or want.read_text() != listing:
                print(f"GOLDEN MISMATCH: {mnemonic}")
                failures += 1
        else:
            want.write_text(listing)
            print(f"golden written: {mnemonic}")
    if check and failures:
        print(f"golden: {failures} mismatches")
        return 1
    print(f"golden checked/written:", len(GOLDEN_TARGETS), "carriers")
    return 0


def main() -> int:
    if len(sys.argv) > 1 and sys.argv[1] == "golden":
        sys.argv.pop(1)
        return golden()
    only = None
    dry = "--dry" in sys.argv[1:]
    only_ok = "--only-ok" in sys.argv[1:]
    timeout_s = 30
    for i, a in enumerate(sys.argv):
        if a == "--mnemonic" and i + 1 < len(sys.argv):
            only = sys.argv[i + 1]
        if a == "--timeout" and i + 1 < len(sys.argv):
            timeout_s = int(sys.argv[i + 1])
    if not RUNNER.is_file():
        print("run_gpu: build/lgpu-gpu missing; run 'make build/lgpu-gpu'",
              file=sys.stderr)
        return 1

    LOGDIR.mkdir(parents=True, exist_ok=True)
    universe = read_universe()
    print(f"running all {len(universe)} mnemonics")
    avail = list(universe.keys())
    by = case_rows_by_mnemonic()

    targets = [only] if only else avail
    results = []
    t0 = time.time()
    for i, m in enumerate(sorted(targets), 1):
        rows = by.get(m, [])
        if not rows:
            res = {"mnemonic": m, "status": "NO-VECTORS", "pass": 0, "fail": 0,
                   "rows": 0, "out": "", "detail": ""}
        else:
            res = run_one(m, rows, dry, timeout_s)
            res["mnemonic"] = m
            (LOGDIR / f"{m}.log").write_text(res.pop("out", "") or "",
                                             encoding="utf-8")
        results.append(res)
        print(f"[{i:3d}/{len(targets)}] {m:36s} {res['status']:14s} "
              f"pass={res['pass']:3d} fail={res['fail']:3d} "
              f"rows={res['rows']:3d} {res['detail'][:40]}")
        if only_ok and res["status"] != "PASS":
            print(f"stopping at first non-PASS: {m} ({res['status']})")
            break

    from collections import Counter
    counts = Counter(r["status"] for r in results)
    print()
    print("==== summary ====")
    for k, n in sorted(counts.items()):
        print(f"{k:16s} {n}")
    tot_pass = sum(r["pass"] for r in results)
    tot_fail = sum(r["fail"] for r in results)
    tot_rows = sum(r["rows"] for r in results)
    print(f"cases: pass={tot_pass} fail={tot_fail} rows={tot_rows} "
          f"elapsed={time.time() - t0:.1f}s")

    if not dry:
        # The authoritative matrix tsv is only rewritten by full runs;
        # single-mnemonic invocations land in OUT_LAST so they can never
        # clobber it.
        full = only is None
        target = OUT if full else OUT_LAST
        with open(target, "w", encoding="utf-8") as f:
            f.write(
                "mnemonic\tstatus\tpass\tfail\trows\trun\tdetail\n")
            for r in results:
                f.write(f"{r['mnemonic']}\t{r['status']}\t{r['pass']}\t"
                        f"{r['fail']}\t{r['rows']}\t"
                        f"{r['pass'] + r['fail']}\t{r['detail']}\n")
        print("results:", target)
    return 0


if __name__ == "__main__":
    sys.exit(main())
