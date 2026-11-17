#!/usr/bin/env python3
"""Regenerate `code/measure-<CPU>.csv` from LoongPerf measurement data.

LoongPerf (https://github.com/jiegec/LoongPerf) stores one JSON file per
instruction and microarchitecture under `docs/data/<uarch>/`.  This repository
stores one CSV per *machine* under `code/`.  The script maps each machine to
the LoongPerf microarchitecture it implements and rewrites the latency and
throughput columns of the CSV from the JSON data:

  * latency:     the min-max span over the recorded source operands.  LoongPerf
                 records one entry per source and stores the operand sweep as
                 `cycles_min`/`cycles_max` for data-dependent instructions, so
                 a single span keeps the "depends on the data" information that
                 a sample value loses (`vfsqrt.d` is 8-37 cycles, not 10).
  * throughput:  IPC and CPI of the recorded multi-lane throughput experiment.

Instructions that LoongPerf does not cover keep their existing row.  Those are
the undocumented encodings (`vfscaleb.{s,d}`, `mepatmsk.v`) and a few LBT
instructions; LA264 and LA364E machines have no LoongPerf data at all and are
therefore not touched.

Usage:
    python3 tools/import_loongperf.py                 # dry run, print a summary
    python3 tools/import_loongperf.py --write         # update the CSVs
    python3 tools/import_loongperf.py --loongperf ../LoongPerf --verbose
"""

from __future__ import annotations

import argparse
import csv
import glob
import json
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# Machine -> LoongPerf microarchitecture.  Machines without an entry keep
# their hand-measured CSV (LA264 / LA364E have no LoongPerf data yet).
MACHINES = {
    "3C5000": "la464",
    "3C6000": "la664",
}

FIELDS = ["name", "latency", "throughput(ipc)", "throughput(cpi)"]


def load_uarch(loongperf_dir: str, uarch: str) -> dict[str, dict]:
    """Return {instruction name: measurement} for one microarchitecture."""
    data_dir = os.path.join(loongperf_dir, "docs", "data", uarch)
    paths = sorted(glob.glob(os.path.join(data_dir, "*.json")))
    if not paths:
        raise SystemExit(f"no measurement JSON found in {data_dir}")
    entries = {}
    for path in paths:
        with open(path, encoding="utf-8") as f:
            data = json.load(f)
        entries[data["instruction"]["name"]] = data
    return entries


def fmt_cycles(value: float) -> str:
    """Format a cycle count like measure.cpp does (%.2f)."""
    return f"{value:.2f}"


def latency_field(entry: dict) -> str | None:
    """Format the latency cell as a single "min-max" span (or one value)."""
    spans = [
        (item.get("cycles_min", item["cycles"]), item.get("cycles_max", item["cycles"]))
        for item in entry.get("latencies", [])
    ]
    if not spans:
        return None
    low = min(low for low, _ in spans)
    high = max(high for _, high in spans)
    if abs(high - low) < 1e-9:
        return fmt_cycles(low)
    return f"{fmt_cycles(low)}-{fmt_cycles(high)}"


def throughput_field(entry: dict) -> tuple[str, str] | None:
    if not entry.get("throughput"):
        return None
    stats = entry["throughput"][0].get("throughput")
    if not stats or "ipc" not in stats or "cpi" not in stats:
        return None
    return f"{stats['ipc']:.2f}", f"{stats['cpi']:.2f}"


def import_machine(
    csv_path: str, entries: dict[str, dict], write: bool
) -> tuple[int, int, list[str], list[str], list[tuple[str, str, str, str]]]:
    with open(csv_path, newline="", encoding="utf-8") as f:
        rows = list(csv.DictReader(f))

    out: list[dict[str, str]] = []
    updated = 0
    unknown: list[str] = []  # not described by LoongPerf at all
    no_latency: list[str] = []  # described, but without a latency measurement
    changes: list[tuple[str, str, str, str]] = []

    for row in rows:
        # The CSV uses the assembler mnemonic with "_" in place of ".".
        entry = entries.get(row["name"].replace("_", "."))
        if entry is None:
            unknown.append(row["name"])
            out.append(row)
            continue

        new = dict(row)
        latency = latency_field(entry)
        if latency is None:
            no_latency.append(row["name"])
        else:
            new["latency"] = latency

        throughput = throughput_field(entry)
        if throughput is not None:
            new["throughput(ipc)"], new["throughput(cpi)"] = throughput

        if (new["latency"], new["throughput(ipc)"]) != (
            row["latency"],
            row["throughput(ipc)"],
        ):
            updated += 1
            changes.append(
                (
                    row["name"],
                    row["latency"],
                    new["latency"],
                    row["throughput(ipc)"],
                    new["throughput(ipc)"],
                )
            )
        out.append(new)

    if write:
        # Keep the file diff-able: one row per instruction, no quoting.
        with open(csv_path, "w", newline="", encoding="utf-8") as f:
            writer = csv.DictWriter(f, fieldnames=FIELDS, lineterminator="\n")
            writer.writeheader()
            writer.writerows(out)

    return len(rows), updated, unknown, no_latency, changes


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument(
        "--loongperf",
        default=os.path.join(ROOT, "..", "LoongPerf"),
        help="path to the LoongPerf checkout (default: ../LoongPerf)",
    )
    parser.add_argument(
        "--write", action="store_true", help="update the CSVs (default: dry run)"
    )
    parser.add_argument(
        "--verbose", action="store_true", help="print every changed instruction"
    )
    args = parser.parse_args()

    loongperf_dir = os.path.abspath(args.loongperf)
    if not os.path.isdir(os.path.join(loongperf_dir, "docs", "data")):
        raise SystemExit(f"{loongperf_dir} does not look like a LoongPerf checkout")

    total_updated = 0
    for machine, uarch in MACHINES.items():
        entries = load_uarch(loongperf_dir, uarch)
        csv_path = os.path.join(ROOT, "code", f"measure-{machine}.csv")
        rows, updated, unknown, no_latency, changes = import_machine(
            csv_path, entries, args.write
        )
        total_updated += updated
        print(f"{machine} ({uarch}): {rows} rows, {updated} changed")
        if unknown:
            print(f"    no LoongPerf entry (row kept): {', '.join(sorted(unknown))}")
        if no_latency:
            print(
                "    no LoongPerf latency (cell kept): "
                f"{', '.join(sorted(no_latency))}"
            )
        if args.verbose:
            for name, old_latency, new_latency, old_ipc, new_ipc in changes:
                print(
                    f"    {name}: latency {old_latency or '-'} -> "
                    f"{new_latency or '-'}, ipc {old_ipc} -> {new_ipc}"
                )

    print(f"{total_updated} rows {'written' if args.write else 'would change'}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
