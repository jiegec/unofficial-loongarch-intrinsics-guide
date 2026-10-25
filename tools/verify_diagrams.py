#!/usr/bin/env python3
"""Cross-check docs/diagram/*.svg against the reference implementations in code/.

Every diagram draws, for each destination element, the source element it comes
from.  This script rebuilds that mapping from the SVG, then computes the same
mapping by compiling and running code/<name>.h (the fuzz-tested reference
implementation) with tagged inputs, and reports any difference.

Usage:
    python3 tools/verify_diagrams.py [-v] [name ...]

Diagrams whose instruction has no reference implementation (`code/<name>.h`)
are reported as skipped.
"""

import argparse
import os
import re
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DIAGRAM_DIR = os.path.join(ROOT, "docs", "diagram")

WIDTH = {
    "b": 8, "bu": 8, "h": 16, "hu": 16, "w": 32, "wu": 32,
    "d": 64, "du": 64, "q": 128, "qu": 128,
}

# value tags used for the inputs
A_TAG = 0x00  # a[i] = i
B_TAG = 0x40  # b[i] = 0x40 + i
SCALAR_TAG = 0x5B  # value coming from a GPR
MEM_TAG = 0x5A  # value coming from memory
LIMIT = 0x20  # tags stay below this / above B_TAG

RECT = re.compile(r'<rect x="(-?\d+)" y="(-?\d+)" width="(-?\d+)" height="(\d+)"')
TEXT = re.compile(
    r'<text x="(-?\d+)" y="(-?\d+)" text-anchor="middle" '
    r'dominant-baseline="middle">([^<]*)</text>'
)
LINE = re.compile(r'<line x1="(-?\d+)" y1="(-?\d+)" x2="(-?\d+)" y2="(-?\d+)"')
NOTE = re.compile(
    r'<text x="10" y="(\d+)" font-family="monospace">(?:imm|lane) = 0x([0-9a-f]+)'
)

SCALAR_WIDTH = 90  # width of the GPR / memory boxes drawn by gen.py


# --------------------------------------------------------------------------
# SVG parsing
# --------------------------------------------------------------------------
def parse_svg(path):
    rects, labels, lines, notes = [], [], [], []
    with open(path) as f:
        for line in f:
            m = NOTE.search(line)
            if m:
                notes.append((int(m.group(1)), int(m.group(2), 16)))
                continue
            m = RECT.search(line)
            if m:
                rects.append((int(m.group(1)), int(m.group(2)), int(m.group(3))))
                continue
            m = TEXT.search(line)
            if m:
                labels.append((int(m.group(1)), int(m.group(2)), m.group(3)))
                continue
            m = LINE.search(line)
            if m:
                lines.append(tuple(int(m.group(i)) for i in range(1, 5)))
    boxes = []
    for (x, y, w) in rects:
        label = None
        for (tx, ty, text) in labels:
            if tx == x + w // 2 and ty == y + 13:
                label = text
                break
        boxes.append({"x": x, "y": y, "w": w, "label": label})
    return boxes, lines, notes


def svg_mapping(path, vlen, pair=0):
    """Return {dst_label: (source_kind, source_label)} for one row pair."""
    boxes, lines, notes = parse_svg(path)
    row_in = 100 * (2 * pair + 1)
    row_out = 100 * (2 * pair + 2)
    r1 = [b for b in boxes if b["y"] == row_in - 10]
    r2 = [b for b in boxes if b["y"] == row_out - 10]
    boundary = 100 + (575 if vlen == 256 else 300)

    def box_at(row, x):
        found = [b for b in row if b["x"] <= x <= b["x"] + b["w"]]
        return found[0] if len(found) == 1 else None

    def kind_of(box):
        if box["w"] == SCALAR_WIDTH:
            return "mem" if re.fullmatch(r"\d+-bit", box["label"] or "") else "gpr"
        return "a" if box["x"] < boundary else "b"

    mapping = {}
    for (x1, y1, x2, y2) in lines:
        if y1 != row_in + 16 or y2 != row_out - 10:
            continue
        src, dst = box_at(r1, x1), box_at(r2, x2)
        if src is None or dst is None:
            return None, notes, f"ambiguous line at x={x1},{x2}"
        if src["label"] is None or dst["label"] is None:
            continue
        kind = kind_of(src)
        src_label = "0" if kind in ("gpr", "mem") else src["label"]
        key = dst["label"]
        if key in mapping:
            return None, notes, f"destination {key} has several sources"
        mapping[key] = (kind, src_label)
    return mapping, notes, None


# --------------------------------------------------------------------------
# reference implementation harness
# --------------------------------------------------------------------------
TYPES = r"""
#include <cstdint>
#include <cstdio>
#include <cstring>

typedef int8_t s8; typedef uint8_t u8;
typedef int16_t s16; typedef uint16_t u16;
typedef int32_t s32; typedef uint32_t u32;
typedef int64_t s64; typedef uint64_t u64;
typedef __int128 s128; typedef unsigned __int128 u128;
typedef float f32; typedef double f64;

union v128 {
  u8 byte[16]; u16 half[8]; u32 word[4]; u64 dword[2]; u128 qword[1];
};
union v256 {
  u8 byte[32]; u16 half[16]; u32 word[8]; u64 dword[4]; u128 qword[2];
};

#define UARCH_LA264 0
#define UARCH_LA464 0

static u64 mem_tag = MEM_TAG;
static u64 stored_value;
static int stored_width;
static inline u64 memory_load(int width, const void *addr) {
  (void)width; (void)addr;
  return mem_tag;
}
static inline void memory_store(int width, u64 value, const void *addr) {
  (void)addr;
  stored_width = width;
  stored_value = value;
}
"""

HARNESS_VEC = r"""
#ifdef USE_V256
typedef v256 VT;
static const int VLEN = 256;
#else
typedef v128 VT;
static const int VLEN = 128;
#endif

static VT a, b, c, dst;
static int imm = IMMV;
static int idx = 1;
static int lane = 1;

static u128 get(const VT &v, int w, int i) {
  switch (w) {
  case 8: return v.byte[i];
  case 16: return v.half[i];
  case 32: return v.word[i];
  case 64: return v.dword[i];
  case 128: return v.qword[i];
  }
  return 0;
}
static void put(VT &v, int w, int i, u128 x) {
  switch (w) {
  case 8: v.byte[i] = (u8)x; break;
  case 16: v.half[i] = (u16)x; break;
  case 32: v.word[i] = (u32)x; break;
  case 64: v.dword[i] = (u64)x; break;
  case 128: v.qword[i] = x; break;
  }
}

int main() {
  const int SW = SRCW, DW = DSTW;
  memset(&a, 0, sizeof(a));
  memset(&b, 0, sizeof(b));
  memset(&dst, 0xff, sizeof(dst));
  for (int i = 0; i < VLEN / SW; i++) {
    put(a, SW, i, i + 1);
    put(b, SW, i, B_TAG + i);
  }

  /* reference implementation */
  {
#include CODE
  }

  for (int j = 0; j < VLEN / DW; j++) {
    u128 v = get(dst, DW, j);
    if (v >= 1 && v <= LIMIT)
      printf("%d a %llu\n", j, (unsigned long long)(v - 1));
    else if (v >= B_TAG && v < B_TAG + LIMIT)
      printf("%d b %llu\n", j, (unsigned long long)(v - B_TAG));
    else if (v == SCALAR_TAG)
      printf("%d gpr 0\n", j);
    else if (v == MEM_TAG)
      printf("%d mem 0\n", j);
    else
      printf("%d ? %llu\n", j, (unsigned long long)v);
  }
  return 0;
}
"""

HARNESS_GPR_IN = r"""
#ifdef USE_V256
typedef v256 VT;
static const int VLEN = 256;
#else
typedef v128 VT;
static const int VLEN = 128;
#endif

static VT a, dst;
static int imm = IMMV;
static long long b = SCALAR_TAG;
static long long val = SCALAR_TAG;

static u128 get(const VT &v, int w, int i) {
  switch (w) {
  case 8: return v.byte[i];
  case 16: return v.half[i];
  case 32: return v.word[i];
  case 64: return v.dword[i];
  }
  return 0;
}
static void put(VT &v, int w, int i, u128 x) {
  switch (w) {
  case 8: v.byte[i] = (u8)x; break;
  case 16: v.half[i] = (u16)x; break;
  case 32: v.word[i] = (u32)x; break;
  case 64: v.dword[i] = (u64)x; break;
  }
}

int main() {
  const int SW = SRCW;
  memset(&a, 0, sizeof(a));
  memset(&dst, 0xff, sizeof(dst));
  for (int i = 0; i < VLEN / SW; i++) put(a, SW, i, i + 1);

  /* reference implementation */
  {
#include CODE
  }

  for (int j = 0; j < VLEN / SW; j++) {
    u128 v = get(dst, SW, j);
    if (v >= 1 && v <= LIMIT)
      printf("%d a %llu\n", j, (unsigned long long)(v - 1));
    else if (v == SCALAR_TAG)
      printf("%d gpr 0\n", j);
    else
      printf("%d ? %llu\n", j, (unsigned long long)v);
  }
  return 0;
}
"""

HARNESS_GPR_OUT = r"""
#ifdef USE_V256
typedef v256 VT;
static const int VLEN = 256;
#else
typedef v128 VT;
static const int VLEN = 128;
#endif

static VT a;
static long long dst = -1;
static int imm = IMMV;
static int idx = 1;

static void put(VT &v, int w, int i, u128 x) {
  switch (w) {
  case 8: v.byte[i] = (u8)x; break;
  case 16: v.half[i] = (u16)x; break;
  case 32: v.word[i] = (u32)x; break;
  case 64: v.dword[i] = (u64)x; break;
  }
}

int main() {
  const int SW = SRCW;
  memset(&a, 0, sizeof(a));
  for (int i = 0; i < VLEN / SW; i++) put(a, SW, i, i + 1);

  /* reference implementation */
  {
#include CODE
  }

  if (dst >= 1 && dst <= LIMIT)
    printf("0 a %lld\n", dst - 1);
  else
    printf("0 ? %lld\n", dst);
  return 0;
}
"""

HARNESS_MEM_LOAD = r"""
#ifdef USE_V256
typedef v256 VT;
static const int VLEN = 256;
#else
typedef v128 VT;
static const int VLEN = 128;
#endif

static VT dst;
static int imm = IMMV;
static char mem_buf[64];
static char *addr = mem_buf;
static int offset = 0;

static u128 get(const VT &v, int w, int i) {
  switch (w) {
  case 8: return v.byte[i];
  case 16: return v.half[i];
  case 32: return v.word[i];
  case 64: return v.dword[i];
  }
  return 0;
}

int main() {
  const int DW = DSTW;
  memset(&dst, 0xff, sizeof(dst));

  /* reference implementation */
  {
#include CODE
  }

  for (int j = 0; j < VLEN / DW; j++) {
    u128 v = get(dst, DW, j);
    if (v == MEM_TAG)
      printf("%d mem 0\n", j);
    else
      printf("%d ? %llu\n", j, (unsigned long long)v);
  }
  return 0;
}
"""

HARNESS_MEM_STORE = r"""
#ifdef USE_V256
typedef v256 VT;
static const int VLEN = 256;
#else
typedef v128 VT;
static const int VLEN = 128;
#endif

static VT data;
static int imm = IMMV;
static int lane = 1;
static char mem_buf[64];
static char *addr = mem_buf;
static int offset = 0;

static void put(VT &v, int w, int i, u128 x) {
  switch (w) {
  case 8: v.byte[i] = (u8)x; break;
  case 16: v.half[i] = (u16)x; break;
  case 32: v.word[i] = (u32)x; break;
  case 64: v.dword[i] = (u64)x; break;
  }
}

int main() {
  const int SW = SRCW;
  memset(&data, 0, sizeof(data));
  for (int i = 0; i < VLEN / SW; i++) put(data, SW, i, i + 1);
  stored_value = 0xffff;
  stored_width = 0;

  /* reference implementation */
  {
#include CODE
  }

  if (stored_value >= 1 && stored_value <= LIMIT)
    printf("0 a %llu\n", (unsigned long long)(stored_value - 1));
  else
    printf("0 ? %llu\n", (unsigned long long)stored_value);
  return 0;
}
"""


def classify(name):
    """Return (kind, vlen, src_width, dst_width, default_imm)."""
    vlen = 256 if name.startswith("x") else 128
    rest = name[1:] if name.startswith("x") else name
    if rest.startswith("v"):
        rest = rest[1:]
    toks = rest.split("_")
    op = toks[0]
    if op == "ext2xv":
        return "vec", 256, WIDTH[toks[2]], WIDTH[toks[1]], 1
    if op in ("sllwil", "extl", "exth"):
        return "vec", vlen, WIDTH[toks[2]], WIDTH[toks[1]], 0 if op == "sllwil" else 1
    w = WIDTH[toks[1]]
    if op == "insgr2vr":
        return "gpr_in", vlen, w, w, 1
    if op == "replgr2vr":
        return "gpr_in", vlen, w, w, 1  # uses `val`, same tagged scalar
    if op == "pickve2gr":
        return "gpr_out", vlen, w, w, 1
    if op == "ldrepl":
        return "mem_load", vlen, w, w, 1
    if op == "stelm":
        return "mem_store", vlen, w, w, 1
    return "vec", vlen, w, w, 1


def build_and_run(name, kind, vlen, srcw, dstw, imm):
    harness = {
        "vec": HARNESS_VEC,
        "gpr_in": HARNESS_GPR_IN,
        "gpr_out": HARNESS_GPR_OUT,
        "mem_load": HARNESS_MEM_LOAD,
        "mem_store": HARNESS_MEM_STORE,
    }[kind]
    defines = (
        f"#define A_TAG {A_TAG:#x}\n"
        f"#define B_TAG {B_TAG:#x}\n"
        f"#define SCALAR_TAG {SCALAR_TAG:#x}\n"
        f"#define MEM_TAG {MEM_TAG:#x}\n"
        f"#define LIMIT {LIMIT:#x}\n"
    )
    source = defines + TYPES + harness
    with tempfile.TemporaryDirectory() as tmp:
        cpp = os.path.join(tmp, "harness.cpp")
        exe = os.path.join(tmp, "harness")
        with open(cpp, "w") as f:
            f.write(source)
        cmd = [
            os.environ.get("CXX", "g++"), "-std=c++17", "-w",
            "-DUSE_V256" if vlen == 256 else "-DNO_V256",
            f"-DSRCW={srcw}", f"-DDSTW={dstw}", f"-DIMMV={imm}",
            f'-DCODE="{ROOT}/code/{name}.h"',
            "-o", exe, cpp,
        ]
        result = subprocess.run(cmd, capture_output=True, text=True)
        if result.returncode != 0:
            return None, result.stderr.strip().splitlines()[-1] if result.stderr else "compile error"
        result = subprocess.run([exe], capture_output=True, text=True)
        if result.returncode != 0:
            return None, "runtime error"
    mapping = {}
    for line in result.stdout.split("\n"):
        if not line:
            continue
        dst, kind_name, src = line.split()
        mapping[dst] = (kind_name, src)
    return mapping, None


FANIN = ("vshuf", "xvshuf", "vpermi", "xvpermi", "xvhseli")


def verify(path, verbose=False):
    base = os.path.splitext(os.path.basename(path))[0]
    if base.startswith(FANIN):
        return "skipped", "fan-in / index-selection diagram, not a 1:1 mapping"
    if not os.path.exists(os.path.join(ROOT, "code", base + ".h")):
        return "skipped", "no reference implementation in code/"
    kind, vlen, srcw, dstw, default_imm = classify(base)
    boxes, lines, notes = parse_svg(path)
    npairs = max(1, len(notes))

    problems = []
    for pair in range(npairs):
        note_imm = None
        if notes:
            candidates = [n for n in notes if n[0] < 100 * (2 * pair + 1)]
            if candidates:
                note_imm = max(candidates, key=lambda n: n[0])[1]
        imm = note_imm if note_imm is not None else default_imm
        expected, err = build_and_run(base, kind, vlen, srcw, dstw, imm)
        if expected is None:
            return "error", err
        actual, _, err = svg_mapping(path, vlen, pair)
        if actual is None:
            problems.append(f"pair {pair}: {err}")
            continue
        if kind in ("gpr_out", "mem_store"):
            exp = expected.get("0")
            if exp is None or exp[0] == "?":
                problems.append("reference produced no single result")
            elif not actual:
                problems.append("result cell is not connected in the diagram")
            else:
                got = next(iter(actual.values()))
                if got != exp:
                    problems.append(
                        f"svg={got[0]}[{got[1]}] reference={exp[0]}[{exp[1]}]"
                    )
        else:
            for dst, exp in expected.items():
                if exp[0] == "?":
                    continue
                got = actual.get(dst)
                if got is None:
                    problems.append(f"dst {dst}: missing (expected {exp[0]}[{exp[1]}])")
                elif got != exp:
                    problems.append(
                        f"dst {dst}: svg={got[0]}[{got[1]}] reference={exp[0]}[{exp[1]}]"
                    )
            for dst in actual:
                if dst not in expected:
                    problems.append(f"dst {dst}: drawn but not produced by reference")
        if verbose and not problems:
            print(f"  ok {base} (imm=0x{imm:02x})")

    if problems:
        return "fail", "; ".join(problems[:6])
    return "ok", ""


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("names", nargs="*", help="diagram names to check")
    parser.add_argument("-v", "--verbose", action="store_true")
    args = parser.parse_args()

    if args.names:
        paths = [os.path.join(DIAGRAM_DIR, n + ".svg") for n in args.names]
    else:
        paths = sorted(
            os.path.join(DIAGRAM_DIR, f)
            for f in os.listdir(DIAGRAM_DIR)
            if f.endswith(".svg")
        )

    counts = {"ok": 0, "fail": 0, "skipped": 0, "error": 0}
    failures = []
    for path in paths:
        status, detail = verify(path, args.verbose)
        counts[status] += 1
        if status in ("fail", "error"):
            failures.append((os.path.basename(path), detail))
        elif status == "skipped" and args.verbose:
            print(f"  skipped {os.path.basename(path)}: {detail}")

    print(
        f"checked {counts['ok']} diagram(s): {counts['ok']} ok, "
        f"{counts['fail']} mismatched, {counts['error']} error(s), "
        f"{counts['skipped']} skipped"
    )
    for name, detail in failures:
        print(f"- {name}: {detail}")
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
