#!/usr/bin/env python3
"""Driver for the LG200 hardware runners.

Runs every instruction of the universe through build/lgpu-gpu on the real GPU,
one process per (instruction, chunk), and reports a PASS/FAIL matrix.  Two
transports consume the same case rows:

    --pipe ps   the PS-generic carrier on the GFX ring (default)
    --pipe cs   the CPIPE compute carrier on the MCS user queue

    python3 run_gpu.py [--dry] [--mnemonic m] [--timeout S] [--only-ok]
                       [--pipe ps|cs] [--lane-sweep [MAX]]
                       [--batch [--batch-size N]]
       --dry        only assemble the envelopes (no DRM submission)
       --mnemonic m run one instruction
       --only-ok    stop after the first hardware-defect instruction
       --timeout S  per-process timeout in seconds (default 30)
       --pipe       which launch engine to run through
       --lane-sweep re-run the per-lane families at several lane counts
                    (default 1,2,4,8,16,32; pass MAX for 1..MAX); compute only
       --batch      one DRM session per group of mnemonics (compute only)

Results: build/gpu_ps_generic_results.tsv and
build/gpu_cs_generic_results.tsv (full runs; mnemonic runs land in
build/gpu_last_run.tsv) plus a per-instruction log in build/gpu-run-logs/.

The log cannot double as the record of the observe-mode rows: every run
rewrites it, a --dry one included, so a reading printed only there survives
until the next dry sweep.  Those readings are also written to
build/gpu_obs/<pipe>/, a record only a run that actually submitted touches, and
observed_reasons.py --samples checks it against the matrix and against its own
age.
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
OUT = {
    "ps": ROOT / "build" / "gpu_ps_generic_results.tsv",
    "cs": ROOT / "build" / "gpu_cs_generic_results.tsv",
}
OUT_LAST = ROOT / "build" / "gpu_last_run.tsv"            # every invocation
LOGDIR = ROOT / "build" / "gpu-run-logs"

# The observe-mode readback, kept apart from the per-mnemonic log on purpose: a
# log is rewritten by every run that touches the mnemonic, a --dry one
# included, so a reading printed there survives only until the next dry sweep.
# These files are written by non-dry runs only, so each one is a statement of
# the run that made it rather than a leftover of that run.
OBSDIR = ROOT / "build" / "gpu_obs"
OBS_COLS = ("case", "offset", "value", "model", "reason")
# The matrix's own column names first, so a reader that already parses the matrix
# parses this, and then what only a record of samples can say: how many dwords
# the row produced, why the row is observed, and when it was read.
OBS_INDEX_COLS = ("mnemonic", "pipe", "status", "pass", "fail", "rows",
                  "samples", "reason", "stamp")
# One read-back dword as main.cpp's compare_cases prints it, and the verdict line
# that follows the case's dwords and carries the carrier's reason.  The model
# field came with this record; a binary built before it printed the line without
# one, and such a sample is stored as "-" rather than dropped.
OBS_LINE = re.compile(r"^OBS (\S+): offset 0x([0-9a-f]+) = 0x([0-9a-f]+)"
                      r"(?: model (0x[0-9a-f]+|-))?$")
OBS_VERDICT = re.compile(r"^PASS (\S+) \(observed(?:: (.*))?\)$")

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


def run_one(mnemonic, rows, dry, timeout_s, pipe, env=None):
    chunks = [rows[i:i + CHUNK] for i in range(0, len(rows), CHUNK)] or [[]]
    merged = {"status": "PASS", "pass": 0, "fail": 0, "rows": len(rows),
              "detail": "", "out": ""}
    for chunk in chunks:
        argv = [str(RUNNER), "run" if pipe == "ps" else "csrun", mnemonic]
        if dry:
            argv.append("--dry-run")
        try:
            r = subprocess.run(argv, input="\n".join(chunk) + "\n", text=True,
                               stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                               timeout=timeout_s, env=env)
            rc = r.returncode
            out = r.stdout
        except subprocess.TimeoutExpired:
            merged.update({"status": "TIMEOUT",
                           "detail": f"timeout {timeout_s}s"})
            break
        # Count the per-case verdict line ("PASS <id>" / "FAIL <id>") only.
        # A failing case also prints one "FAIL <id>: offset ..." line per
        # mismatching word, so counting "FAIL " prefixes counted a case twice
        # or more; the batch path already counted cases, and the two paths must
        # agree.
        merged["pass"] += len(re.findall(r"^PASS \S+$", out, re.M))
        merged["fail"] += len(re.findall(r"^FAIL \S+$", out, re.M))
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
        if rc == 5:
            # The host-side preflight refused the stream.  That is a defect in
            # this tool, not a hardware result: nothing was submitted.
            merged.update({"status": "PREFLIGHT-FAIL", "detail": last})
            break
        if rc == 4:
            # The host-side LDS-window preflight refused the stream: nothing
            # was submitted, so this is a defect in the carrier declaration and
            # not a hardware result.
            merged.update({"status": "PREFLIGHT-FAIL", "detail": last})
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


# Lane counts the sweep re-runs the per-lane families at.  1 is in the list on
# purpose: it is the shape u-l-i-g's oracle vectors describe (dst[0] = old,
# dst[1] = old <op> src is one lane's view), so it is the only count that is an
# ORACLE comparison.  The larger counts compare against this repo's per-lane
# model, so they are a weaker evidence class and are labelled as such - but
# they are what tests the law rather than a point, and what turns a declared
# group size into a verified executed-lane count.
LANE_COUNTS = (1, 2, 3, 4, 5, 6, 7, 8, 16, 32)

# Statuses that mean nothing reached the hardware, or that the host refused the
# stream: a sweep cannot fail on these, but it must report them.
LANE_SWEEP_OK = ("PASS", "SKIP-ENVELOPE", "NO-VECTORS")
# Statuses that mean the device may be wedged: stop the sweep immediately
# rather than spending the rest of the matrix on a dead pipe.
LANE_SWEEP_FATAL = ("HANG/FAULT", "TIMEOUT", "ERROR", "PREFLIGHT-FAIL")


def run_batch(mnemonics, by, dry, timeout_s, env=None):
    """Submit many mnemonics through ONE process and ONE DRM session.

    Returns {mnemonic: result}.  The session is the point rather than the
    parsing: `csrun` opens a DRM file and creates a user queue per call, and a
    few hundred of those create/destroy cycles wedge the pipe (see README, "the
    churn hazard").  One batch pays one pair for the whole group.
    """
    argv = [str(RUNNER), "csrun-batch"]
    if dry:
        argv.append("--dry-run")
    # Same chunking as the single-mnemonic path: one oversized program is not
    # equivalent to several smaller ones.  v_fma_f32 carries 312 rows, and
    # sending them as a single program stalled the pipe.
    stdin = []
    for m in mnemonics:
        rows = by.get(m, [])
        for i in range(0, max(len(rows), 1), CHUNK):
            stdin.append("@" + m)
            stdin.extend(rows[i:i + CHUNK])
    blank = {m: {"status": "ERROR", "pass": 0, "fail": 0,
                 "rows": len(by.get(m, [])), "detail": "no BATCH line"}
             for m in mnemonics}
    try:
        r = subprocess.run(argv, input="\n".join(stdin) + "\n", text=True,
                           stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                           timeout=timeout_s, env=env)
    except subprocess.TimeoutExpired:
        for m in blank:
            blank[m].update({"status": "TIMEOUT",
                             "detail": f"batch timeout {timeout_s}s"})
        return blank
    res = {}
    # Per-block payloads: a dry run emits the same listing the single-mnemonic
    # path does, wrapped in BATCH-BLOCK markers.  Keep them so the caller can
    # write the same per-mnemonic logs - otherwise a --batch dry run leaves the
    # previous run's listings behind and audit.py reads stale programs.
    payload = {}
    current = None
    for raw in r.stdout.splitlines(True):
        stripped = raw.rstrip("\n")
        if stripped.startswith("BATCH-BLOCK "):
            current = stripped[len("BATCH-BLOCK "):].strip()
            payload.setdefault(current, [])
            continue
        if stripped.startswith("BATCH-BLOCK-END "):
            current = None
            continue
        if current is not None:
            payload[current].append(raw)
    for line in r.stdout.splitlines():
        if not line.startswith("BATCH "):
            continue
        parts = line.split()
        if len(parts) < 4:
            continue
        counts = {}
        for field in parts[3:]:
            if "=" in field:
                key, _, val = field.partition("=")
                counts[key] = int(val) if val.isdigit() else 0
        # A mnemonic can appear once per chunk, so accumulate rather than
        # overwrite and keep the worst status.
        m = parts[1]
        rank = {st: i for i, st in enumerate(
            ("NOT-RUN", "SKIP-ENVELOPE", "NO-VECTORS", "PASS", "FAIL",
             "ERROR", "PREFLIGHT-FAIL", "TIMEOUT", "HANG/FAULT"))}
        if m in res:
            prev = res[m]
            prev["pass"] += counts.get("pass", 0)
            prev["fail"] += counts.get("fail", 0)
            prev["rows"] += counts.get("rows", 0)
            prev["detail"] = line[:120]
            if rank.get(parts[2], 0) > rank.get(prev["status"], 0):
                prev["status"] = parts[2]
        else:
            res[m] = {"status": parts[2], "pass": counts.get("pass", 0),
                      "fail": counts.get("fail", 0),
                      "rows": counts.get("rows", 0),
                      "detail": line[:120]}
    for m in mnemonics:
        res.setdefault(m, blank[m])
        res[m]["out"] = "".join(payload.get(m, []))
    return res


def obs_samples(out):
    """The read-back dwords in one mnemonic's output, as sample rows.

    compare_cases prints one "OBS <case>: offset 0x.. = 0x.. model 0x.." line per
    staged dword of an observe-mode case and then that case's verdict,
    "PASS <case> (observed: <why>)".  The reason belongs to the case, not to the
    dword, so the dwords are held until the verdict arrives and a block whose
    verdict never came contributed no reading rather than being attributed to
    the next case.
    """
    rows, pending = [], []
    for line in out.splitlines():
        m = OBS_LINE.match(line.strip())
        if m:
            pending.append(m.groups())
            continue
        v = OBS_VERDICT.match(line.strip())
        if v is None:
            continue
        why = (v.group(2) or "").strip()
        for case, off, val, model in pending:
            rows.append((case, "0x" + off, "0x" + val, model or "-", why))
        pending = []
    return rows


def read_index(path):
    """Rows of an existing index.tsv as dicts; empty when the file is foreign."""
    try:
        lines = path.read_text(encoding="utf-8").splitlines()
    except OSError:
        return []
    if not lines or lines[0].split("\t") != list(OBS_INDEX_COLS):
        return []
    out = []
    for line in lines[1:]:
        cells = line.split("\t")
        if len(cells) == len(OBS_INDEX_COLS):
            out.append(dict(zip(OBS_INDEX_COLS, cells)))
    return out


def write_obs(pipe, results, full):
    """Persist this run's observe-mode readings under build/gpu_obs/<pipe>/.

    Two tab-separated artifacts, both rewritten whole:

        <mnemonic>.tsv  case  offset  value  model  reason   - one row per
                        read-back dword, in the order the runner printed it
        index.tsv       mnemonic pipe status pass fail rows samples reason
                        stamp    - one row per observed mnemonic, carrying the
                        same status/pass/fail/rows as the results matrix

    The index repeats the matrix's column names so a reader that parses the
    matrix parses this, and so observed_reasons.py --samples can check the two
    records against each other without knowing how either was made.  The model
    column is the case's expected dword: it is recorded, not compared - the row
    is observation-only, which is the whole reason its reading needs a record of
    its own.

    Only a run that submitted writes here.  A --dry run assembles and compares
    nothing, so it has no reading to record and must not touch the last one.

    full says the run covered the whole universe: then the index is this run's
    complete set and a mnemonic it does not list is a leftover from an older
    carrier, dropped so the record cannot describe two models at once.  A
    --mnemonic run leaves the rest of the session's record alone.
    """
    d = OBSDIR / pipe
    d.mkdir(parents=True, exist_ok=True)
    stamp = time.strftime("%Y%m%dT%H%M%SZ", time.gmtime())
    fresh = [r for r in results if r.get("obs")]
    for r in fresh:
        lines = ["\t".join(OBS_COLS)]
        for case, off, val, model, why in r["obs"]:
            lines.append("\t".join((case, off, val, model, why)))
        (d / (r["mnemonic"] + ".tsv")).write_text(
            "\n".join(lines) + "\n", encoding="utf-8")
    # The previous index is read even for a full run: the file is rebuilt from
    # this run alone, but what it used to hold is what has to be cleaned up.
    previous = read_index(d / "index.tsv")
    idx = {} if full else {row["mnemonic"]: row for row in previous}
    for r in fresh:
        idx[r["mnemonic"]] = {
            "mnemonic": r["mnemonic"], "pipe": pipe, "status": r["status"],
            "pass": r["pass"], "fail": r["fail"], "rows": r["rows"],
            "samples": len(r["obs"]), "reason": r["obs"][-1][4],
            "stamp": stamp}
    if full:
        for row in previous:
            m = row["mnemonic"]
            if m not in idx:
                (d / (m + ".tsv")).unlink(missing_ok=True)
    lines = ["\t".join(OBS_INDEX_COLS)]
    for m in sorted(idx):
        lines.append("\t".join(str(idx[m][c]) for c in OBS_INDEX_COLS))
    (d / "index.tsv").write_text("\n".join(lines) + "\n", encoding="utf-8")
    return d


# A representative subset of the v_cmp family, whose mask output is a function
# of the active lane count (envelope.cpp is_lane_mask_family).  One all-true row
# pins the width for the whole family; sweeping all ~163 v_cmp rows would
# multiply a hardware campaign that costs a cold boot per fault for one number.
# v_cmp_eq_f32 and v_cmpclass_f32 add a mixed and a partly-set case.
LANE_MASK_PROBES = ("v_cmp_tru_f32", "v_cmp_f_f32", "v_cmp_eq_f32",
                    "v_cmpclass_f32", "v_cmp_eq_i32", "v_cmp_eq_u32",
                    "v_cmp_f_i32", "v_cmp_gt_u64")


def lane_sensitive(universe) -> list:
    """Mnemonics whose expected value is a function of the lane count.

    These are the rows routed through the per-lane atomic model
    (envelope.cpp `atomic_final_n` / `sm_final_n`): atomic_sm_* on the LDS
    carrier and atomic_v_buff_* on the buffer carrier, plus atomic_v_tex_*
    which only the graphics transport can run.  atomic_v_flat_* is deliberately
    included by prefix even though it is observation-only on both transports
    (build_carrier_env marks the flat family `observe`), so the sweep reports
    it as an observation rather than silently dropping it.

    LANE_MASK_PROBES also sweeps the v_cmp lane-mask width - the one number
    that decides whether envelope.cpp's (1<<lanes)-1 expectation is the
    hardware's actual mask.
    """
    return sorted(m for m in universe
                  if m.startswith("atomic_") or m in LANE_MASK_PROBES)


def lane_sweep(pipe, dry, timeout_s, counts, batch_timeout=900) -> int:
    import os

    if pipe != "cs":
        print("lane-sweep: the graphics launch's lane count is not host "
              "settable (kGraphicsLanes), so there is nothing to sweep; "
              "run with --pipe cs", file=sys.stderr)
        return 2
    universe = read_universe()
    targets = lane_sensitive(universe)
    by = case_rows_by_mnemonic()
    print(f"lane sweep: {len(targets)} per-lane mnemonics x "
          f"{len(counts)} lane counts")

    rows_out = []
    fatal = 0
    for n in counts:
        env = dict(os.environ)
        env["LGPU_CS_GROUP"] = f"{n},1,1"
        n_pass = n_fail = n_skip = n_rows = 0
        # One batch per lane count: the lane count is per-process environment,
        # so it is the natural batch boundary, and it turns ~97 sessions per
        # count into one.
        results = run_batch(targets, by, dry, batch_timeout, env=env)
        for m in targets:
            rows = by.get(m, [])
            if not rows:
                n_skip += 1
                continue
            res = results[m]
            n_rows += res["rows"]
            if res["status"] == "PASS":
                n_pass += 1
            elif res["status"] in LANE_SWEEP_OK:
                n_skip += 1
            else:
                n_fail += 1
                print(f"  lane={n:2d} {m:34s} {res['status']:12s} "
                      f"{res['detail'][:52]}")
                rows_out.append((n, m, res["status"], res["detail"]))
                if res["status"] in LANE_SWEEP_FATAL:
                    fatal = 1
        print(f"lane={n:2d}: pass={n_pass:3d} not-compared={n_skip:3d} "
              f"FAIL={n_fail:3d} rows={n_rows}")
        if fatal:
            print(f"lane sweep stopped at lane={n}: device-threatening status")
            break
    target = ROOT / "build" / "gpu_cs_lanesweep.tsv"
    with open(target, "w", encoding="utf-8") as f:
        f.write("lane\tmnemonic\tstatus\tdetail\n")
        for n, m, st, d in rows_out:
            f.write(f"{n}\t{m}\t{st}\t{d}\n")
    print("results:", target)
    return 1 if rows_out else 0


def main() -> int:
    if len(sys.argv) > 1 and sys.argv[1] == "golden":
        sys.argv.pop(1)
        return golden()
    only = None
    pipe = "ps"
    for i, a in enumerate(sys.argv):
        if a == "--pipe" and i + 1 < len(sys.argv):
            pipe = sys.argv[i + 1]
    if pipe not in ("ps", "cs"):
        print(f"run_gpu: --pipe must be ps or cs, got {pipe}", file=sys.stderr)
        return 2
    dry = "--dry" in sys.argv[1:]
    only_ok = "--only-ok" in sys.argv[1:]
    # [HW 2026-11-04] The compute pipe runs in batch mode by DEFAULT, because the
    # per-mnemonic path is the one that parks the device.  Measured on
    # load_s_buff_w16: through `csrun` the dispatch completes ("signal=0") and then
    # the queue teardown fails - "the dispatch completed but the queue teardown
    # failed; the ring is left mapped for the DRM file teardown" - the kernel logs
    # "CP microcode re-pushed after reset", and the device stops admitting
    # submissions for every row after it.  The SAME fifteen cases through
    # `csrun-batch` pass twice, with the default 3000 ms timeout.  The difference
    # is the session: batch pays one create/destroy pair for a whole group, the
    # default path pays one per mnemonic.
    batch = ("--batch" in sys.argv[1:] or
             (pipe == "cs" and "--no-batch" not in sys.argv[1:]))
    batch_size = 48
    for i, a in enumerate(sys.argv):
        if a == "--batch-size" and i + 1 < len(sys.argv):
            batch_size = max(1, int(sys.argv[i + 1]))
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

    if "--lane-sweep" in sys.argv[1:]:
        counts = LANE_COUNTS
        for i, a in enumerate(sys.argv):
            if a == "--lane-sweep" and i + 1 < len(sys.argv):
                try:
                    counts = tuple(range(1, int(sys.argv[i + 1]) + 1))
                except ValueError:
                    pass
        return lane_sweep(pipe, dry, timeout_s, counts)

    LOGDIR.mkdir(parents=True, exist_ok=True)
    universe = read_universe()
    print(f"running all {len(universe)} mnemonics through the "
          f"{'PS (GFX ring)' if pipe == 'ps' else 'compute (CPIPE user queue)'} "
          f"transport")
    avail = list(universe.keys())
    by = case_rows_by_mnemonic()

    targets = [only] if only else avail
    results = []
    t0 = time.time()
    if batch:
        # One DRM session per group of mnemonics instead of one per mnemonic.
        #
        # A batch can stop early: a submission in a fresh process occasionally
        # does not come back clean even though the same case submitted alone
        # passes (measured: s_sub_i32 reported HANG/FAULT as the first
        # submission of a new process and then passed 38/38 on its own, with no
        # GPU fault in the kernel log).  That is a stale-queue/teardown artefact
        # of the previous session, not a defect in the case, so a stall is
        # retried in a fresh process rather than ending the sweep.  A retry that
        # also stalls is treated as real and stops the run.
        from collections import Counter
        ordered = sorted(targets)
        i = 0
        stalls = 0
        while i < len(ordered):
            group = ordered[i:i + batch_size]
            res_map = run_batch(group, by, dry, 900)
            done = 0
            stalled = None
            for m in group:
                res = dict(res_map[m])
                res["mnemonic"] = m
                if res["status"] in LANE_SWEEP_FATAL:
                    stalled = m
                    break
                if res["status"] == "NOT-RUN":
                    break          # never attempted; leave it for the next batch
                # Parsed before the log write, and from the same text: the log
                # is the next run's to overwrite, the sample record is this
                # run's to keep.
                out = res.pop("out", "") or ""
                res["obs"] = obs_samples(out)
                results.append(res)
                done += 1
                (LOGDIR / f"{m}.log").write_text(out, encoding="utf-8")
                print(f"[{len(results):3d}/{len(targets)}] {m:36s} "
                      f"{res['status']:14s} pass={res['pass']:3d} "
                      f"fail={res['fail']:3d} rows={res['rows']:3d} "
                      f"{res['detail'][:40]}")
            if stalled is None:
                i += max(done, 1)
                continue
            stalls += 1
            print(f"  batch stopped at {stalled}; retrying it in a fresh "
                  f"process (stall {stalls})")
            # [HW 2026-11-04] The retry goes through the BATCH path too.  It used
            # to call run_one, i.e. the per-mnemonic `csrun` path - the one whose
            # queue teardown is what parks the device - so a stall was retried by
            # repeating the operation that caused it.
            retry = dict(run_batch([stalled], by, dry, 900)[stalled])
            retry["mnemonic"] = stalled
            retry["detail"] = "retry after batch stall: " + retry["detail"]
            retry_out = retry.pop("out", "") or ""
            retry["obs"] = obs_samples(retry_out)
            (LOGDIR / f"{stalled}.retry.log").write_text(retry_out,
                                                         encoding="utf-8")
            (LOGDIR / f"{stalled}.log").write_text(retry_out, encoding="utf-8")
            results.append(retry)
            done += 1
            print(f"[{len(results):3d}/{len(targets)}] {stalled:36s} "
                  f"{retry['status']:14s} pass={retry['pass']:3d} "
                  f"fail={retry['fail']:3d} rows={retry['rows']:3d} "
                  f"{retry['detail'][:40]}")
            if retry["status"] in LANE_SWEEP_FATAL:
                print(f"stopping: {stalled} did not recover in a fresh process")
                break
            i += done
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
            with open(OUT[pipe], "w", encoding="utf-8") as f:
                f.write("mnemonic\tstatus\tpass\tfail\trows\trun\tdetail\n")
                for r in results:
                    f.write(f"{r['mnemonic']}\t{r['status']}\t{r['pass']}\t"
                            f"{r['fail']}\t{r['rows']}\t"
                            f"{r['pass'] + r['fail']}\t{r['detail']}\n")
            print("results:", OUT[pipe])
            # After the matrix, so the record can be checked against it by age
            # as well as by content: a record older than the matrix it belongs
            # to is the stale case, and this order makes that visible.
            print("obs:", write_obs(pipe, results, full=True))
        return 0
    for i, m in enumerate(sorted(targets), 1):
        rows = by.get(m, [])
        if not rows:
            res = {"mnemonic": m, "status": "NO-VECTORS", "pass": 0, "fail": 0,
                   "rows": 0, "out": "", "detail": ""}
        else:
            res = run_one(m, rows, dry, timeout_s, pipe)
            res["mnemonic"] = m
            out = res.pop("out", "") or ""
            res["obs"] = obs_samples(out)
            (LOGDIR / f"{m}.log").write_text(out, encoding="utf-8")
        results.append(res)
        print(f"[{i:3d}/{len(targets)}] {m:36s} {res['status']:14s} "
              f"pass={res['pass']:3d} fail={res['fail']:3d} "
              f"rows={res['rows']:3d} {res['detail'][:40]}")
        # SKIP-ENVELOPE and NO-VECTORS never reach the hardware: the host
        # refused the stream, or there was nothing to submit.  They are
        # expected outcomes of the coverage split (see README), so a campaign
        # looking for the first *hardware* defect must run through them.
        if only_ok and res["status"] not in ("PASS", "SKIP-ENVELOPE",
                                             "NO-VECTORS"):
            print(f"stopping at first hardware defect: {m} ({res['status']})")
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
        target = OUT[pipe] if full else OUT_LAST
        with open(target, "w", encoding="utf-8") as f:
            f.write(
                "mnemonic\tstatus\tpass\tfail\trows\trun\tdetail\n")
            for r in results:
                f.write(f"{r['mnemonic']}\t{r['status']}\t{r['pass']}\t"
                        f"{r['fail']}\t{r['rows']}\t"
                        f"{r['pass'] + r['fail']}\t{r['detail']}\n")
        print("results:", target)
        print("obs:", write_obs(pipe, results, full=full))
    return 0


if __name__ == "__main__":
    sys.exit(main())
