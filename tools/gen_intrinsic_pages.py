#!/usr/bin/env python3
"""Generate one page per intrinsic under docs/intrinsic/.

The pages are plain, pre-rendered markdown: they reuse the output of the
mkdocs-macros functions defined in main.py, so running this script after
changing a description keeps the alphabetical pages in sync.

Usage:
    python3 tools/gen_intrinsic_pages.py
"""

import glob
import json
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT_DIR = os.path.join(ROOT, "docs", "intrinsic")
EXTENSIONS = ("lsx", "lasx", "lbt")


def load_intrinsics():
    sys.path.insert(0, ROOT)
    import main

    class Env:
        def __init__(self):
            self.macros = {}

        def macro(self, fn):
            self.macros[fn.__name__] = fn

    env = Env()
    main.define_env(env)
    return json.loads(env.macros["all_intrinsics"](render=True))


def group_pages():
    """Map (extension, group title) to the markdown file that holds it."""
    pages = {}
    for ext in EXTENSIONS:
        for path in sorted(glob.glob(os.path.join(ROOT, "docs", ext, "*.md"))):
            title = None
            with open(path) as f:
                for line in f:
                    if line.startswith("# "):
                        title = line[2:].strip()
                        break
            if title is not None:
                pages[(ext, title)] = f"{ext}/{os.path.basename(path)}"
    return pages


def short_name(heading):
    """`## __m128i __lsx_vadd_b (__m128i a, __m128i b)` -> `__lsx_vadd_b`."""
    return heading.split("(")[0].strip().split()[-1]


def section_of(name):
    """Section of the alphabetical index an intrinsic belongs to."""
    for prefix, label, skip in (
        ("__lsx_", "LSX_", "v"),
        ("__lasx_", "LASX_", "xv"),
    ):
        if name.startswith(prefix):
            rest = name[len(prefix):]
            for token in (skip, skip[:1]):
                if token and rest.startswith(token):
                    rest = rest[len(token):]
                    break
            if rest:
                return label + rest[0].upper()
            return label + "_"
    return name.lstrip("_")[:1].upper() or "_"


def section_key(name):
    section = section_of(name)
    return (0 if section.startswith("LSX_") else
            1 if section.startswith("LASX_") else 2, section, name.lower())


DESCRIPTION_RE = re.compile(r"### Description\s*\n+(.+?)(?:\n|$)")


def first_paragraph(markdown):
    """First line of the Description section, used in the alphabetical index."""
    match = DESCRIPTION_RE.search(markdown)
    if match is None:
        return ""
    text = match.group(1).strip()
    if text.startswith(("```", "|", "-", "#")):
        return ""
    return text


def slug(name):
    """File name for an intrinsic; intrinsic names are already filename safe."""
    return re.sub(r"[^A-Za-z0-9_.-]", "_", name)


def main():
    intrinsics = load_intrinsics()
    pages = group_pages()

    os.makedirs(OUT_DIR, exist_ok=True)
    for stale in glob.glob(os.path.join(OUT_DIR, "*.md")):
        os.remove(stale)

    written = set()
    descriptions = {}
    for entry in intrinsics:
        name = short_name(entry["name"])
        filename = slug(name) + ".md"
        if filename in written:
            continue
        written.add(filename)
        descriptions[filename] = first_paragraph(entry["markdown"])

        ext = entry["extension"].lower()
        group = entry["group"]
        source = pages.get((ext, group))

        lines = [f"# {name}", ""]
        lines.append(
            f"{entry['extension']} intrinsic"
            + (f" · group: [{group}](../{source})" if source else f" · group: {group}")
        )
        lines += ["", entry["markdown"].strip(), ""]
        lines += [
            "---",
            "",
            f"[All intrinsics in alphabetical order](index.md)",
            "",
        ]
        with open(os.path.join(OUT_DIR, filename), "w") as f:
            f.write("\n".join(lines))

    # index page: LSX and LASX get one section per mnemonic letter, so that the
    # list does not end up with a single huge "L" section
    entries = sorted(written, key=lambda f: section_key(f[: -len(".md")]))
    sections = {}
    for filename in entries:
        name = filename[: -len(".md")]
        sections.setdefault(section_of(name), []).append((name, filename))

    order = [f"LSX_{c}" for c in "ABCDEFGHIJKLMNOPQRSTUVWXYZ"]
    order += [f"LASX_{c}" for c in "ABCDEFGHIJKLMNOPQRSTUVWXYZ"]
    order += list("ABCDEFGHIJKLMNOPQRSTUVWXYZ")
    order += sorted(set(sections) - set(order))

    lines = [
        "# Intrinsics in Alphabetical Order",
        "",
        f"{len(entries)} intrinsics, one page each. "
        "See also the grouped pages linked from the home page.",
        "",
        "LSX and LASX intrinsics are grouped by the first letter of the "
        "operation name (the letter after the `v`/`xv` prefix), for example "
        "`__lsx_vadd_b` is listed under LSX_A.",
    ]
    for section in order:
        if section not in sections:
            continue
        lines += ["", f"## {section} ({len(sections[section])})", ""]
        for name, filename in sections[section]:
            entry = f"- [{name}]({filename})"
            description = descriptions.get(filename)
            if description:
                entry += f" — {description}"
            lines.append(entry)
    lines.append("")

    with open(os.path.join(OUT_DIR, "index.md"), "w") as f:
        f.write("\n".join(lines))

    print(f"wrote {len(entries)} intrinsic pages and index.md to docs/intrinsic/")


if __name__ == "__main__":
    main()
