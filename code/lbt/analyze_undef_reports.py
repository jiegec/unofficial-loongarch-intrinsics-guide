#!/usr/bin/env python3
"""Summarize native x86 and LBT undefined-flag probe reports."""

from __future__ import annotations

import argparse
import re
from collections import defaultdict
from pathlib import Path

FLAG_BITS = {"CF": 0x001, "PF": 0x004, "AF": 0x010, "ZF": 0x040, "SF": 0x080, "OF": 0x800}
FLAGS = ("CF", "PF", "AF", "ZF", "SF", "OF")
WIDTHS = {"b": 8, "h": 16, "w": 32, "d": 64, "bu": 8, "hu": 16, "wu": 32, "du": 64}
UNDEF_SCOPE = {
    "x86and": ("AF",),
    "x86or": ("AF",),
    "x86test": ("AF",),
    "x86xor": ("AF",),
    "x86mul": ("PF", "AF", "ZF", "SF"),
    "x86div": ("CF", "PF", "AF", "ZF", "SF", "OF"),
    "x86idiv": ("CF", "PF", "AF", "ZF", "SF", "OF"),
    "x86sll": ("AF", "OF"),
    "x86slli": ("AF", "OF"),
    "x86srl": ("AF", "OF"),
    "x86srli": ("AF", "OF"),
    "x86sra": ("AF", "OF"),
    "x86srai": ("AF", "OF"),
    "x86rotl": ("OF",),
    "x86rotli": ("OF",),
    "x86rotr": ("OF",),
    "x86rotri": ("OF",),
    "x86rcl": ("OF",),
    "x86rcli": ("OF",),
    "x86rcr": ("OF",),
    "x86rcri": ("OF",),
    "x86bt": ("PF", "AF", "ZF", "SF", "OF"),
    "x86btc": ("PF", "AF", "ZF", "SF", "OF"),
    "x86btr": ("PF", "AF", "ZF", "SF", "OF"),
    "x86bts": ("PF", "AF", "ZF", "SF", "OF"),
    "x86bsf": ("CF", "PF", "AF", "SF", "OF"),
    "x86bsr": ("CF", "PF", "AF", "SF", "OF"),
    "x86shld": ("CF", "PF", "AF", "ZF", "SF", "OF"),
    "x86shrd": ("CF", "PF", "AF", "ZF", "SF", "OF"),
}
ALIASES = {
    "x86slli": "x86sll",
    "x86srli": "x86srl",
    "x86srai": "x86sra",
    "x86rotli": "x86rotl",
    "x86rotri": "x86rotr",
    "x86rcli": "x86rcl",
    "x86rcri": "x86rcr",
    "x86btc": "x86bt",
    "x86btr": "x86bt",
    "x86bts": "x86bt",
}


def stem(instr: str) -> str:
    return instr.split(".", 1)[0]


def canonical(instr: str) -> str:
    return ALIASES.get(stem(instr), stem(instr))


def suffix(instr: str) -> str:
    return instr.split(".")[-1] if "." in instr else ""


def flag(raw: int, name: str) -> int:
    return 1 if raw & FLAG_BITS[name] else 0


def effective_count(case: dict[str, object]) -> int | None:
    if "c" not in case:
        return None
    bits = WIDTHS.get(suffix(str(case["instr"])), 32)
    c = int(case["c"]) & (0x3f if bits == 64 else 0x1f)
    st = str(case["stem"])
    if st in ("x86rotl", "x86rotli", "x86rotr", "x86rotri"):
        return c % bits
    if st in ("x86rcl", "x86rcli", "x86rcr", "x86rcri") and bits in (8, 16):
        return c % (bits + 1)
    return c


def parse_label(label: str) -> dict[str, object]:
    parts = label.split()
    instr = parts[0]
    text = " ".join(parts[1:])
    data: dict[str, object] = {"instr": instr, "stem": stem(instr), "canon": canonical(instr), "text": text}
    for key in ("a", "b", "c"):
        m = re.search(rf"{key}=([^ ]+)", text)
        if m:
            s = m.group(1).replace("ULL", "")
            data[key] = int(s, 0)
    return data


def parse_report(path: Path):
    cases = []
    checks = {"total": 0, "failed": 0}
    for line in path.read_text(errors="replace").splitlines():
        m = re.match(r"\s*(\d+) tests, (\d+) passed, (\d+) FAILED", line)
        if m:
            checks["total"] = int(m.group(1))
            checks["failed"] = int(m.group(3))
            continue
        if line.startswith("SUMMARY|"):
            m = re.search(r"tests=(\d+)\|passed=(\d+)\|failed=(\d+)", line)
            if m:
                checks["total"] = int(m.group(1))
                checks["failed"] = int(m.group(3))
            continue
        if line.startswith("PROBE|"):
            parts = line.split("|")
            if len(parts) < 4:
                continue
            label = parts[1]
            init = int(parts[2].split("=", 1)[1], 16)
            hw = int(parts[3].split("=", 1)[1], 16)
            meta = parse_label(label)
            meta["init"] = init
            meta["hw"] = hw
            cases.append(meta)
    return checks, cases


def classify_basic(cases, flag_name: str) -> str:
    vals = [flag(int(c["hw"]), flag_name) for c in cases]
    inits = [flag(int(c["init"]), flag_name) for c in cases]
    if all(v == 0 for v in vals):
        return "0"
    if all(v == 1 for v in vals):
        return "1"
    if all(v == i for v, i in zip(vals, inits)):
        return "P"
    return "M"


def classify(cases, flag_name: str) -> str:
    if not cases:
        return "?"
    overall = classify_basic(cases, flag_name)
    if overall != "M" or not any("c" in c for c in cases):
        return overall

    by_count = defaultdict(list)
    for c in cases:
        by_count[effective_count(c)].append(c)
    count_codes = {count: classify_basic(group, flag_name) for count, group in by_count.items()}
    if len(set(count_codes.values())) <= 1:
        return next(iter(count_codes.values()))
    zero = count_codes.get(0)
    nonzero = {code for count, code in count_codes.items() if count not in (None, 0)}
    if zero == "P" and len(nonzero) == 1:
        nz = next(iter(nonzero))
        if nz == "0":
            return "P0"
        if nz == "1":
            return "P1"
        if nz == "M":
            return "PM"
    return "C"


def summarize(report_paths: list[Path]):
    for path in report_paths:
        checks, cases = parse_report(path)
        if checks["total"]:
            print(f"# {path.name}: VERIFY {checks['total'] - checks['failed']}/{checks['total']} passed")
        else:
            print(f"# {path.name}: VERIFY n/a")
        grouped = defaultdict(list)
        for c in cases:
            grouped[c["canon"]].append(c)
        for st in sorted(grouped):
            scoped = set()
            for raw_st, flags in UNDEF_SCOPE.items():
                if ALIASES.get(raw_st, raw_st) == st:
                    scoped.update(flags)
            if not scoped:
                continue
            fields = []
            for fn in FLAGS:
                if fn in scoped:
                    fields.append(f"{fn}={classify(grouped[st], fn)}")
            print(f"{st}: " + ", ".join(fields))
        print()


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("reports", nargs="+", type=Path)
    args = ap.parse_args()
    summarize(args.reports)


if __name__ == "__main__":
    main()
