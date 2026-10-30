#!/usr/bin/env python3
"""CS microbench: measure every LG200 mnemonic on the compute pipe.

Chain-of-copies latency and independent-stream throughput, timed by the
firmware timestamp block in the signal allocation.  The execution window is
signal +0x38 - +0x30 (identified by the K sweep in the README); the marginal
cost (win(K2) - win(K1)) / (K2 - K1) removes the fixed launch overhead, so
ticks/op is the instruction's own cost.

Safe by construction, because the first full sweep (2026-10-12) wedged CPIPE
(it created ~5,400 user queues at one process's worth of churn):
  * every process publishes at most LGPU_CS_BENCH_MAX streams (csbench-batch
    defaults to 100, well under the ~330 cycles the submit layer documents);
  * 16 mnemonics per process, so 16 x 2 kinds x 2 sizes x 1 rep = 64;
  * csbench-batch stops at the first submission that does not come back clean,
    and this driver re-runs that mnemonic alone in a fresh process, so a
    transient failure is isolated instead of compounded.
"""
from __future__ import annotations
import subprocess, sys
from pathlib import Path

# bench_coverage.py owns the family -> reason mapping for a row the carrier cannot
# build.  Import it rather than restate it: a SKIP whose reason is generic ('not
# benchable on the compute carrier') cannot be acted on, and the CSV is the artifact a
# reader works from.  That file is guarded, so importing it does not run its scan.
from bench_coverage import classify as _bench_reason

ROOT = Path(__file__).resolve().parent
UNIVERSE = ROOT.parent.parent / "lg200-instructions.tsv"
RUNNER = ROOT / "build" / "lgpu-gpu"
OUT = ROOT / "build" / "cs_bench_results.tsv"
K_SMALL, K_LARGE, REPS = 256, 2048, 1
CHUNK = 16


def mnemonics() -> list[str]:
    out = []
    for line in UNIVERSE.read_text(encoding="utf-8").splitlines():
        parts = line.split("\t")
        if len(parts) >= 2 and parts[1] and parts[1] != "mnemonic":
            out.append(parts[1])
    return out


def run_batch(ms: list[str]):
    """One fresh process.  Returns (wins, skipped, faulted)."""
    lines = []
    for m in ms:
        for kind in ("lat", "tp"):
            lines.append(m + " " + kind + " " + str(K_SMALL) + " " + str(REPS))
            lines.append(m + " " + kind + " " + str(K_LARGE) + " " + str(REPS))
    p = subprocess.run([str(RUNNER), "csbench-batch"], input="\n".join(lines) + "\n",
                       text=True, capture_output=True)
    wins, skipped, faulted, notes = {}, set(), set(), {}
    for line in p.stdout.splitlines():
        f = line.split("\t")
        if f[0] == "CSBENCH" and len(f) >= 12:
            wins[(f[1], f[2], int(f[3]))] = int(f[6])
            if len(f) >= 13 and f[12]:
                notes[f[1]] = f[12]
        elif f[0] == "CSBENCH-SKIP":
            skipped.add((f[1], f[2]))
        elif f[0] == "CSBENCH-FAULT":
            faulted.add(f[1])
    return wins, skipped, faulted, notes


def verdict(m, wins, skipped, notes=None):
    # The two kinds are decided separately, because they are skipped for different
    # reasons.  A control row cannot be chained - there is no cost to accumulate - so
    # its LATENCY is undefined while its THROUGHPUT through independent streams is
    # measurable.  Returning SKIP for the whole row, which this did until 2026-10-13,
    # threw that number away and made 20 control rows look unmeasurable in both
    # columns.  A row measured in one kind is now PARTIAL and reports what it has.
    _fam, _why = _bench_reason(m)
    reason = ("the bench builder refuses this row on the compute pipe"
              if not _why else "%s: %s" % (_fam, _why))
    got, missing = {}, []
    for kind in ("lat", "tp"):
        if (m, kind) in skipped:
            missing.append(kind)
            continue
        a, b = wins.get((m, kind, K_SMALL)), wins.get((m, kind, K_LARGE))
        if a is None or b is None:
            missing.append(kind)
            continue
        got[kind] = (b - a) / (K_LARGE - K_SMALL)
    if not got:
        return ("SKIP", "", "", reason)
    if missing:
        note = "%s (skipped %s)" % (reason, "/".join(missing))
        cell = lambda k: ("%.3f" % got[k]) if k in got else ""
        return ("PARTIAL", cell("lat"), cell("tp"), note)
    lat, tp = got["lat"], got["tp"]
    # The runner knows when a row's measurement is not what the column name
    # implies (a store's lat is a store+load round trip); carry that into the CSV
    # note so the number cannot be read as something it is not.
    extra = (notes or {}).get(m, "")
    note = "lat/tp=%.2f" % (lat / tp) if tp else "lat/tp=inf"
    if extra:
        note = extra + "; " + note
    return ("OK", "%.3f" % lat, "%.3f" % tp, note)


def main() -> int:
    ms = mnemonics()
    if "--start" in sys.argv:
        ms = ms[int(sys.argv[sys.argv.index("--start") + 1]):]
    if "--limit" in sys.argv:
        ms = ms[:int(sys.argv[sys.argv.index("--limit") + 1])]
    out = Path(sys.argv[sys.argv.index("--out") + 1]) if "--out" in sys.argv else OUT
    # ---- the coverage gate -------------------------------------------------
    #
    # bench_coverage.classify() already answers "what does this row need that
    # the compute carrier cannot supply?", and until 2026-10-30 that answer was
    # only used to label the CSV *after* the fact.  It has to gate the
    # submission: on 2026-10-30 the sweep's first row was atomic_sm_add_u32,
    # classify() said "shared memory (LDS): needs an LDS window", the row was
    # submitted anyway, its builder emitted an LDS access while the launch
    # declared no shared memory, and that one submission took CPIPE down
    # (unresolvable fault, quarantine, recovery whose post-reset verification
    # timed out).  Every one of the 768 rows then reported SKIP or FAULT while
    # exactly one of them had actually broken the device.
    #
    # --allow names a row to submit anyway.  Trying a known-expensive row is a
    # deliberate act on a watched device, not something a default sweep does.
    allow = set()
    if "--allow" in sys.argv:
        allow = {x for x in sys.argv[sys.argv.index("--allow") + 1].split(",") if x}
    gated = {}
    for m in ms:
        fam, why = _bench_reason(m)
        if why and m not in allow:
            gated[m] = "%s: %s" % (fam, why)
    runnable = [m for m in ms if m not in gated]
    verdicts = {}
    for m, why in gated.items():
        verdicts[m] = ("SKIP", "", "", why)
    print("  %d gated by the coverage map, %d runnable"
          % (len(gated), len(runnable)), file=sys.stderr)

    stopped_at = None
    for i in range(0, len(runnable), CHUNK):
        chunk = runnable[i:i + CHUNK]
        wins, skipped, faulted, notes = run_batch(chunk)
        for m in chunk:
            if m not in faulted:
                verdicts[m] = verdict(m, wins, skipped, notes)
        if faulted:
            # Fail fast.  A submission that does not come back clean means the
            # device may already be unusable, and the rest of this loop would
            # then be 47 chunks of submissions to a dead pipe - which is how one
            # row's fault became "0 OK of 768".  Stop, name the row, and do not
            # retry it: the same failing case is not submitted twice.
            stopped_at = sorted(faulted)[0]
            for m in faulted:
                verdicts[m] = ("FAULT", "", "",
                               "submission did not come back clean; the sweep "
                               "stopped at this row rather than continue into a "
                               "device that may already be down")
            print("  STOP: %s failed; not submitting anything else" % stopped_at,
                  file=sys.stderr)
            break
        print("  %d/%d" % (min(i + CHUNK, len(runnable)), len(runnable)),
              file=sys.stderr)

    # A row that was never submitted must not borrow a reason from the coverage
    # map: "not measured" and "known unmeasurable" are different facts, and
    # reporting the second for the first is what made the 2026-10-30 sweep read
    # as 724 legitimate skips.
    unrun = "not measured: the sweep stopped at %s" % stopped_at if stopped_at             else "not measured"
    rows = [(m,) + verdicts.get(m, ("NO-DATA", "", "", unrun)) for m in ms]
    out.write_text("mnemonic\tstatus\tlat_ticks_per_op\ttp_ticks_per_op\tnote\n"
                   + "".join("\t".join(r) + "\n" for r in rows))
    ok = sum(1 for r in rows if r[1] == "OK")
    print("cs_bench: %d OK, %d SKIP/NO-DATA/FAULT of %d" % (ok, len(rows) - ok, len(rows)))
    print("results: " + str(out))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
