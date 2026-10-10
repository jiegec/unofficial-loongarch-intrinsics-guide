#!/usr/bin/env python3
"""Parser for the LG200 case-file protocol (the one place that knows it).

Every code/lg200/<mnemonic>.cpp defines:
  LG200_ORACLE_BEGIN(mnemonic, nin) / LG200_ORACLE_END(nout)
  LG200_CASE_BEGIN(mnemonic)
    CASE_FP(id, policy, in-words..., exp-words...)   (fp policy)
    CASE(id, in-words..., exp-words...)              (default policy)
  LG200_CASE_END

Both consumers (run_cases.py for the CPU oracle runner and
run_gpu.py for the hardware runner) get the parsed cases from here, so the
protocol has exactly one definition.
"""
from __future__ import annotations

import re
import sys
from dataclasses import dataclass
from pathlib import Path

HEX = re.compile(r"0[xX][0-9a-fA-F]+")


@dataclass
class Case:
    id: str
    mnemonic: str
    fp_policy: str  # "" = default policy
    inputs: str
    expected: str


def _case_rows(mnemonic: str, text: str, nout: int, strict: bool,
               path: Path):
    block = re.search(
        r"LG200_CASE_BEGIN\(" + re.escape(mnemonic) + r"\).*?LG200_CASE_END",
        text, re.S)
    if block is None:
        if strict:
            raise SystemExit(
                f"FAIL: {path.name}: missing LG200_CASE_BEGIN")
        return []
    out = []
    for case in re.finditer(r"CASE_FP\(([\w-]+)\s*,\s*([\w|]+)\s*,(.*?)\)",
                            block.group(0)):
        words = _words(case.group(3), nout, strict, path, case.group(1))
        if words is None:
            continue
        ins = " ".join(hex(v) for v in words[:len(words) - nout])
        exp = " ".join(hex(v) for v in words[len(words) - nout:])
        out.append(Case(case.group(1), mnemonic, case.group(2), ins, exp))
    for case in re.finditer(r"CASE\(([\w-]+)(?:\s*,(.*?))?\)",
                            block.group(0)):
        words = _words(case.group(2) or "", nout, strict, path, case.group(1))
        if words is None:
            continue
        ins = " ".join(hex(v) for v in words[:len(words) - nout])
        exp = " ".join(hex(v) for v in words[len(words) - nout:])
        out.append(Case(case.group(1), mnemonic, "", ins, exp))
    return out


def _words(text: str, nout: int, strict: bool, path: Path, case_id: str):
    words = [int(x, 16) for x in HEX.findall(text)]
    if len(words) < nout:
        if strict:
            raise SystemExit(
                f"FAIL: {path.name}: case {case_id} has {len(words)} words, "
                f"needs >= {nout}")
        return None
    return words


def parse_case_files(root: Path, strict: bool = False):
    """(mnemonics, cases) parsed from every LG200_CASE_* vector in root."""
    mnemonics: list[str] = []
    cases: list[Case] = []
    for path in sorted(root.glob("*.cpp")):
        if path.name == "main.cpp":
            continue
        text = path.read_text(encoding="utf-8")
        head = re.search(r"LG200_ORACLE_BEGIN\(\s*(\w+)\s*,\s*(\d+)\s*\)",
                         text)
        if head is None:
            if strict:
                raise SystemExit(
                    f"FAIL: {path.name}: missing LG200_ORACLE_BEGIN")
            continue
        tail = re.search(r"LG200_ORACLE_END\((\d+)\s*\)", text)
        mnemonic = head.group(1)
        nout = int(tail.group(1)) if tail else 0
        mnemonics.append(mnemonic)
        cases.extend(_case_rows(mnemonic, text, nout, strict, path))
    # Committed generator output, same 4-tab protocol, merged last.
    gen = root / "generated_cases.tsv"
    if gen.is_file():
      known = set(mnemonics)
      for i, line in enumerate(gen.read_text(encoding="utf-8").splitlines()):
        if not line or line.startswith("#"):
          continue
        parts = line.split("\t")
        if len(parts) != 4:
          raise SystemExit(
              f"FAIL: {gen.name}:{i + 1}: expected 4 tab fields")
        if parts[0] not in known:
          continue  # stale generated row for a removed mnemonic
        cases.append(Case(f"gen{i + 1}", parts[0], parts[1], parts[2],
                          parts[3]))
    return mnemonics, cases
