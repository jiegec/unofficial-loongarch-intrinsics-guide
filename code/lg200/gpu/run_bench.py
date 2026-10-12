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
    if any((m, k) in skipped for k in ("lat", "tp")):
        return ("SKIP", "", "", "not benchable on the compute carrier")
    got = {}
    for kind in ("lat", "tp"):
        a, b = wins.get((m, kind, K_SMALL)), wins.get((m, kind, K_LARGE))
        if a is None or b is None:
            return ("NO-DATA", "", "", "no timestamp line")
        got[kind] = (b - a) / (K_LARGE - K_SMALL)
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
    verdicts, retry = {}, []
    for i in range(0, len(ms), CHUNK):
        chunk = ms[i:i + CHUNK]
        wins, skipped, faulted, notes = run_batch(chunk)
        for m in chunk:
            if m not in faulted:
                verdicts[m] = verdict(m, wins, skipped, notes)
        if faulted:
            print("  chunk %d: transient fault on %s" % (i, sorted(faulted)),
                  file=sys.stderr)
            retry.extend(sorted(faulted))
        print("  %d/%d" % (min(i + CHUNK, len(ms)), len(ms)), file=sys.stderr)
    for m in retry:
        wins, skipped, faulted, notes = run_batch([m])
        if m in faulted:
            verdicts[m] = ("FAULT", "", "", "submission failed twice, in isolated processes")
        else:
            verdicts[m] = verdict(m, wins, skipped, notes)
        print("  retry %s -> %s" % (m, verdicts[m][0]), file=sys.stderr)
    rows = [(m,) + verdicts.get(m, ("NO-DATA", "", "", "not measured")) for m in ms]
    out.write_text("mnemonic\tstatus\tlat_ticks_per_op\ttp_ticks_per_op\tnote\n"
                   + "".join("\t".join(r) + "\n" for r in rows))
    ok = sum(1 for r in rows if r[1] == "OK")
    print("cs_bench: %d OK, %d SKIP/NO-DATA/FAULT of %d" % (ok, len(rows) - ok, len(rows)))
    print("results: " + str(out))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
