#!/usr/bin/env python3
"""Documentation gates for docs/gpu.

The manual is a view of the machine-readable sources, not a second copy of
them.  This script checks four things:

  coverage  every genxml struct, UAPI ioctl, ISA format and ISA operand field
            has a home in the manual;
  bits      every field of the state, shader and descriptor tables has the
            bit range the genxml declares;
  clean     no provenance, date, experiment or repository path survives in the
            hand-written pages;
  links     every relative link resolves to a file, and every local anchor
            resolves to a heading.

Run from anywhere:

    python3 code/lg200/doc_gate.py --all
"""
from __future__ import annotations

import argparse
import os
import re
import sys
import xml.etree.ElementTree as ET
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
DOCS = ROOT / "docs" / "gpu"
WORKSPACE = ROOT.parent
MESA = WORKSPACE / "loonggpu" / "src" / "mesa" / "src" / "loonggpu"
LIBDRM = WORKSPACE / "loonggpu" / "src" / "libdrm" / "include" / "drm" / "loonggpu_drm.h"


def norm(text: str) -> str:
    text = text.lower().replace("colour", "color")
    text = text.replace("minimum", "min").replace("maximum", "max")
    text = text.replace("destination", "dest").replace("operation", "op")
    return re.sub(r"[^a-z0-9]", "", text)


def md_files():
    return sorted(DOCS.rglob("*.md"))


def read(path: Path) -> str:
    return path.read_text(encoding="utf-8")


# ---------------------------------------------------------------------------
# coverage
# ---------------------------------------------------------------------------

STRUCT_ALIASES = {
    "Full State Header": "state-record header",
    "Stencil Face Control": "stencil face record",
    "Blend RT": "Blend RT",
    "Color Buffer": "colour buffer record",
}

# ioctls outside the manual's scope: display and metadata are documented by the
# display driver, not by the graphics/compute manual.
DISPLAY_IOCTLS = {
    "DRM_IOCTL_LOONGGPU_LAYER_DISPLAY",
    "DRM_IOCTL_LOONGGPU_LAYER_ZOOM",
    "DRM_IOCTL_LOONGGPU_LAYER_TILE",
    "DRM_IOCTL_LOONGGPU_LAYER_ROTATE",
    "DRM_IOCTL_LOONGGPU_VIDEO_GAMMA",
    "DRM_IOCTL_LOONGGPU_GET_META",
    "DRM_IOCTL_LOONGGPU_SET_META",
}


def coverage() -> int:
    bad = 0
    all_text = "\n".join(read(p) for p in md_files())
    normalized = norm(all_text)

    genxml = MESA / "genxml" / "lg200.xml"
    if not genxml.exists():
        print("coverage: SKIP genxml (source tree not present)")
    else:
        root = ET.parse(genxml).getroot()
        for struct in root.findall("struct"):
            name = struct.get("name")
            probe = norm(STRUCT_ALIASES.get(name, name))
            if probe not in normalized:
                print("coverage: genxml struct has no home: %s" % name)
                bad += 1
        print("coverage: genxml structs checked: %d" % len(root.findall("struct")))

    if not LIBDRM.exists():
        print("coverage: SKIP UAPI (source tree not present)")
    else:
        header = read(LIBDRM)
        ioctls = sorted(set(re.findall(r"#define\s+(DRM_IOCTL_LOONGGPU_[A-Z0-9_]+)", header)))
        uapi = read(DOCS / "host" / "uapi.md")
        for name in ioctls:
            if name in DISPLAY_IOCTLS:
                continue
            if name not in uapi:
                print("coverage: UAPI ioctl missing from host/uapi.md: %s" % name)
                bad += 1
        print("coverage: UAPI ioctls checked: %d (display/meta excluded)" % (len(ioctls) - len(DISPLAY_IOCTLS)))

    isa = MESA / "isa" / "lg200.xml"
    if not isa.exists():
        print("coverage: SKIP ISA XML")
    else:
        tree = ET.parse(isa).getroot()
        encoding = read(DOCS / "isa" / "encoding.md")
        formats = [f.get("name") for f in tree.findall("./formats/format")]
        for name in formats:
            if name not in encoding:
                print("coverage: ISA format missing from isa/encoding.md: %s" % name)
                bad += 1
        fields = set()
        for layout in re.findall(r'layout="([^"]+)"', read(isa)):
            for token in layout.split(";"):
                if "@" in token:
                    base = re.sub(r"x[0-9]+$", "", token.split("@")[0])
                    fields.add(base)
        isa_text = "\n".join(read(p) for p in (DOCS / "isa").rglob("*.md"))
        for name in sorted(fields):
            if name.startswith("X"):
                continue  # row-defined literals; described by family
            if name not in isa_text:
                print("coverage: ISA operand field missing from isa/: %s" % name)
                bad += 1
        print("coverage: ISA formats checked: %d, operand fields: %d" % (len(formats), len(fields)))

    return bad


# ---------------------------------------------------------------------------
# bits
# ---------------------------------------------------------------------------

# (genxml struct, doc file, section heading, field-name aliases)
SECTIONS = {
    "Draw Info": ("graphics/draw.md", "## Draw Info", {}),
    "Pipe Enables": ("graphics/state-object.md", "## Pipe Enables", {
        "Fixed Point Size Enable": "Fixed point size",
    }),
    "Depth Control": ("graphics/depth-stencil.md", "## Depth Control", {
        "Offset Units Bits": "Offset units",
        "Offset Factor Bits": "Offset factor",
        "Offset Clamp Bits": "Offset clamp",
        "Depth Clamp Near": "Clamp near",
        "Depth Clamp Far": "Clamp far",
    }),
    "Stencil Face Control": ("graphics/depth-stencil.md", "## Stencil face record", {}),
    "Blend RT": ("graphics/blending.md", "## Per-target blend word", {
        "RGB Function": "RGB equation",
        "Alpha Function": "Alpha equation",
        "RGB Destination Factor": "RGB destination factor",
        "Alpha Destination Factor": "Alpha destination factor",
    }),
    "Shader Config": ("graphics/state-object.md", "## Shader config", {
        "VS Enable": "VS enable (vertex)",
        "HS Enable": "HS enable (hull)",
        "DS Enable": "DS enable (domain)",
        "GS Enable": "GS enable (geometry)",
        "SS Enable": "SS enable (streamout)",
        "DS As VS": "DS as VS (domain as vertex)",
        "DS Includes GS": "DS includes GS",
        "VS Includes HS": "VS includes HS",
        "GS Primitive Type": "GS primitive type",
        "DS Primitive Type": "DS primitive type",
        "DS Point Mode": "DS point mode",
        "DS Clockwise": "DS clockwise",
        "DS Spacing": "DS spacing",
    }),
    "Shader Info": ("shaders/shader-abi.md", "## Shader-info record", {
        "RW Buffer": "Root 1 (RW Buffer)",
        "Const Buffer": "Root 2 (Const Buffer)",
        "Constant": "Root 3 (Constant)",
        "Resource Sampler Descriptors": "Root 4 (Resource Sampler Descriptors)",
        "Attributes": "Root 5 (Attributes)",
    }),
    "Buffer Descriptor": ("shaders/descriptors.md", "## Buffer descriptor", {}),
    "Image Descriptor": ("graphics/texture.md", "## Image descriptor", {
        "LOD Hardware Count Enable": "LOD hardware count",
        "Alpha Is On MSB": "Alpha is MSB",
        "Lost Color Bits": "Lost colour bits",
    }),
    "Sampler Descriptor": ("graphics/texture.md", "## Sampler descriptor", {
        "Maximum Anisotropy Ratio": "Maximum anisotropy",
        "Border Color Pointer": "Border colour pointer",
        "Border Color Type": "Border colour type",
    }),
    "Blend Control": ("graphics/blending.md", "## Blend Control", {}),
    "Blend Constants": ("graphics/blending.md", "## Blend constants", {}),
    "Color Buffer": ("graphics/state-object.md", "## Colour buffer", {}),
    "Depth Buffer": ("graphics/state-object.md", "## Depth buffer", {}),
    "Fragment Attribute Map Entry": ("graphics/state-object.md", "## Fragment state", {}),
    "Fragment Input Config": ("graphics/state-object.md", "## Fragment state", {}),
    "Fragment Output Config": ("graphics/state-object.md", "## Fragment state", {}),
    "Shader Output Sizes": ("graphics/state-object.md", "## Shader config", {}),
    "Vertex Stage Config": ("graphics/state-object.md", "## Vertex stage config", {}),
    "Domain Stage Config": ("graphics/state-object.md", "## Domain stage config", {}),
    "Geometry Stage Config": ("graphics/state-object.md", "## Geometry stage config", {}),
    "Compute Local Size": ("host/compute-submission.md", "## Local-size and dispatch records", {}),
    "Compute Dispatch": ("host/compute-submission.md", "## Local-size and dispatch records", {}),
}


def section(path: Path, heading: str) -> str:
    text = read(path)
    start = text.find(heading)
    if start < 0:
        return ""
    end = text.find("\n## ", start + 1)
    return text[start:end if end >= 0 else len(text)]


def table_rows(text: str):
    rows = []
    for line in text.splitlines():
        if line.startswith("|") and "[" in line:
            cells = [c.strip() for c in line.strip("|").split("|")]
            if len(cells) >= 2:
                rows.append(cells)
    return rows


def bit_range(cell: str):
    match = re.fullmatch(r"\`?\[(\d+)(?::(\d+))?\]\`?", cell.strip())
    if not match:
        return None
    hi = int(match.group(1))
    lo = int(match.group(2)) if match.group(2) else hi
    return hi, lo


def bits() -> int:
    genxml = MESA / "genxml" / "lg200.xml"
    if not genxml.exists():
        print("bits: SKIP genxml")
        return 0
    root = ET.parse(genxml).getroot()
    structs = {s.get("name"): s for s in root.findall("struct")}
    bad = 0
    checked = 0
    for name, (rel, heading, aliases) in SECTIONS.items():
        struct = structs.get(name)
        if struct is None:
            print("bits: genxml struct missing: %s" % name)
            bad += 1
            continue
        rows = table_rows(section(DOCS / rel, heading))
        if not rows:
            print("bits: no table found for %s in %s %s" % (name, rel, heading))
            bad += 1
            continue
        by_name = {norm(r[1]): r for r in rows}
        for field in struct.findall("field"):
            fname = field.get("name")
            key = norm(aliases.get(fname, fname))
            row = by_name.get(key)
            if row is None:
                print("bits: %s.%s has no row in %s %s" % (name, fname, rel, heading))
                bad += 1
                continue
            found = bit_range(row[0])
            if found is None:
                print("bits: %s.%s row has no bit range: %s" % (name, fname, row[0]))
                bad += 1
                continue
            hi = int(field.get("start")) + int(field.get("size")) - 1
            lo = int(field.get("start"))
            if found != (hi, lo):
                print("bits: %s.%s is [%d:%d] in genxml, [%d:%d] in %s"
                      % (name, fname, hi, lo, found[0], found[1], rel))
                bad += 1
            checked += 1
    print("bits: fields checked: %d" % checked)
    return bad


# ---------------------------------------------------------------------------
# clean
# ---------------------------------------------------------------------------

BANNED = re.compile(
    r"20\d\d-\d\d-\d\d"
    r"|\bcarrier\b|\bvendor\b|\bprobe\b|\bobserved\b|\bworktree\b"
    r"|code/lg200|src/mesa|\bCTS\b|\bMesa\b"
    r"|\bclosed\s+(producer|driver|backend)\b|\bestablished producer\b"
    r"|HARDWARE-SESSION|MICROBENCH",
    re.I,
)


def clean() -> int:
    bad = 0
    for path in md_files():
        if "instructions" in path.parts:
            continue
        for n, line in enumerate(read(path).splitlines(), 1):
            match = BANNED.search(line)
            if match:
                print("clean: %s:%d: %s" % (path.relative_to(ROOT), n, match.group(0)))
                bad += 1
    print("clean: files checked: %d" % sum(1 for p in md_files() if "instructions" not in p.parts))
    return bad


# ---------------------------------------------------------------------------
# links
# ---------------------------------------------------------------------------

def slug(text: str) -> str:
    text = re.sub(r"\`[^\`]*\`", "", text)
    text = re.sub(r"\[([^\]]*)\]\([^)]*\)", r"\1", text)
    text = text.lower()
    text = re.sub(r"[^a-z0-9 _-]", "", text)
    text = re.sub(r"[ _]+", "-", text).strip("-")
    return text


def anchors(path: Path):
    out = set()
    for line in read(path).splitlines():
        if line.startswith("#"):
            out.add(slug(line.lstrip("#").strip()))
    return out


def links() -> int:
    bad = 0
    cache = {}
    for path in md_files():
        text = read(path)
        for target in re.findall(r"\]\(([^)]+)\)", text):
            if target.startswith(("http:", "https:", "mailto:")):
                continue
            file_part, _, anchor = target.partition("#")
            if not file_part:
                dest = path
            else:
                dest = (path.parent / file_part).resolve()
            if not dest.exists():
                print("links: %s -> missing %s" % (path.relative_to(ROOT), target))
                bad += 1
                continue
            if anchor:
                if dest not in cache:
                    cache[dest] = anchors(dest)
                if anchor not in cache[dest]:
                    print("links: %s -> missing anchor %s" % (path.relative_to(ROOT), target))
                    bad += 1
    print("links: files checked: %d" % len(md_files()))
    return bad


# ---------------------------------------------------------------------------

def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--coverage", action="store_true")
    parser.add_argument("--bits", action="store_true")
    parser.add_argument("--clean", action="store_true")
    parser.add_argument("--links", action="store_true")
    parser.add_argument("--all", action="store_true")
    args = parser.parse_args()
    if not any((args.coverage, args.bits, args.clean, args.links, args.all)):
        parser.print_help()
        return 2
    total = 0
    if args.all or args.coverage:
        total += coverage()
    if args.all or args.bits:
        total += bits()
    if args.all or args.clean:
        total += clean()
    if args.all or args.links:
        total += links()
    if total:
        print("doc_gate: %d finding(s)" % total)
        return 1
    print("doc_gate: ALL CHECKS PASSED")
    return 0


if __name__ == "__main__":
    sys.exit(main())
