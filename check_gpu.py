#!/usr/bin/env python3
"""Structural checks for the LG200 GPU chapter.

`check.py` and `check_instr.py` cover LSX/LASX and need an external binutils
tree; nothing validated the GPU chapter, which is how a mask transcription defect
survived in nine rows of the instruction table and a hand-drawn state map came to
disagree with its own table.

This script asserts the invariants the documentation states about itself:

  1. the five masks partition all 64 bits of every machine row;
  2. `opcode` is a subset of `fixed`;
  3. every base word satisfies `base & fixed == base` and `base & parameter == 0`;
  4. `parameter`, `ignored` and `reserved` are pairwise disjoint;
  5. the twelve machine formats' row counts sum to 768 and match the encoding
     chapter's table;
  6. the seven families' row counts sum to 768 and match the profile and the
     overview;
  7. every mnemonic is referenced by exactly one instruction index, and no index
     references a mnemonic that is absent;
  8. the state map's blocks are contiguous and cover 0x800, and the block list
     still agrees with the table in the state-object chapter;
  9. every nav target exists, and every page is reachable from the nav;
 10. every relative link resolves to a file, and every fragment to a heading.

With `--mesa PATH` it additionally compares the five masks and the base word
against the open driver's machine table for all 768 rows.  That comparison is not
redundant: four of the nine mask defects found by review satisfied every internal
invariant above, because the wrong bit was inside the mask the row declared as
well as inside `fixed`.  Only the second, independently maintained transcription
of the same decoder artifact exposes that.

Note what is deliberately *not* checked: every field in `operand_layout` need not
lie inside `parameter_mask`.  The layout is the declared operand template and the
mask is the encoded operand set, and they differ in 17 rows where a row inherits a
slot from its format without encoding it.  The row pages print those as inactive
layout fields.

Usage:
  python3 check_gpu.py
  python3 check_gpu.py --mesa ../loonggpu/src/mesa/src/loonggpu/isa/lg200.xml
"""
import csv
import os
import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parent
DOCS = ROOT / "docs"
GPU = DOCS / "gpu"
TSV = ROOT / "code" / "lg200-instructions.tsv"
MARKER = chr(92) + "n"          # the table stores newlines as the escape \n

problems = []


def fail(where, msg):
    problems.append("%s: %s" % (where, msg))


def load_rows():
    with open(TSV, encoding="utf-8") as f:
        return list(csv.DictReader(f, delimiter="\t"))


def table_after(text, heading):
    """Return the first Markdown table that follows `heading`, as cell lists."""
    i = text.find(heading)
    if i < 0:
        return None
    lines = text[i:].split("\n")
    rows, started = [], False
    for line in lines[1:]:
        s = line.strip()
        if s.startswith("|"):
            cells = [c.strip() for c in re.split(r"(?<!\\)\|", s)[1:-1]]
            if set("".join(cells)) <= set("-: "):
                started = True
                continue
            if started:
                rows.append(cells)
        elif started:
            break
    return rows


def check_masks(rows):
    for r in rows:
        m = r["mnemonic"]
        lo, hi = int(r["inst_lo"], 16), int(r["inst_hi"], 16)
        f = (int(r["fixed_mask_lo"], 16), int(r["fixed_mask_hi"], 16))
        o = (int(r["opcode_mask_lo"], 16), int(r["opcode_mask_hi"], 16))
        p = (int(r["parameter_mask_lo"], 16), int(r["parameter_mask_hi"], 16))
        g = (int(r["ignored_mask_lo"], 16), int(r["ignored_mask_hi"], 16))
        v = (int(r["reserved_mask_lo"], 16), int(r["reserved_mask_hi"], 16))
        for w, name in ((0, "LO"), (1, "HI")):
            if f[w] | p[w] | g[w] | v[w] != 0xFFFFFFFF:
                fail(m, "%s: the four masks do not cover all 32 bits" % name)
            if o[w] & ~f[w]:
                fail(m, "%s: opcode is not a subset of fixed" % name)
            if p[w] & g[w] or p[w] & v[w] or g[w] & v[w]:
                fail(m, "%s: parameter/ignored/reserved are not disjoint" % name)
        if (lo & f[0]) != lo or (hi & f[1]) != hi:
            fail(m, "base & fixed != base")
        if (lo & p[0]) or (hi & p[1]):
            fail(m, "base carries operand bits (base & parameter != 0)")


def check_formats(rows):
    by_format = {}
    for r in rows:
        by_format.setdefault(r["format"], []).append(r)
    if sum(len(v) for v in by_format.values()) != 768:
        fail("formats", "row counts do not sum to 768")
    text = (GPU / "isa" / "encoding.md").read_text(encoding="utf-8")
    table = table_after(text, "## Machine formats")
    if not table:
        fail("encoding.md", "the machine-format table was not found")
        return
    declared = {}
    for cells in table:
        if len(cells) < 5:
            continue
        name = cells[1].strip("`")
        declared[name] = int(cells[4])
    if set(declared) != set(by_format):
        fail("encoding.md", "format names differ from the table: %s"
             % sorted(set(declared) ^ set(by_format)))
    for name, n in declared.items():
        got = len(by_format.get(name, []))
        if n != got:
            fail("encoding.md", "format %s declares %d rows, the table has %d" % (name, n, got))


def check_families(rows):
    by_cat = {}
    for r in rows:
        by_cat.setdefault(r["category"], []).append(r)
    if sum(len(v) for v in by_cat.values()) != 768:
        fail("families", "category counts do not sum to 768")
    for page, heading, col in (("chips/lg200.md", "## Instruction families", 1),
                               ("index.md", "## Instruction reference", 1)):
        text = (GPU / page).read_text(encoding="utf-8")
        table = table_after(text, heading)
        if not table:
            fail(page, "the family table was not found")
            continue
        seen = 0
        for cells in table:
            if len(cells) <= col:
                continue
            try:
                n = int(cells[col].replace(",", ""))
            except ValueError:
                continue
            seen += n
        if seen != 768:
            fail(page, "family row counts sum to %d, not 768" % seen)


def check_index_coverage(rows):
    refs = {}
    for path in sorted(GPU.rglob("*.md")):
        for m in re.finditer(r"\{\{\s*lg200\('([^']+)'\)\s*\}\}", path.read_text(encoding="utf-8")):
            refs.setdefault(m.group(1), []).append(path)
    names = {r["mnemonic"] for r in rows}
    for name, where in refs.items():
        if len(where) > 1:
            fail("index", "%s is referenced %d times (%s)" % (name, len(where), where[0].name))
        if name not in names:
            fail("index", "%s is referenced but absent from the table" % name)
    for name in sorted(names - set(refs)):
        fail("index", "%s is in the table but has no index reference" % name)


def check_state_map():
    text = (GPU / "graphics" / "state-object.md").read_text(encoding="utf-8")
    table = table_after(text, "## State map")
    if not table:
        fail("state-object.md", "the state map table was not found")
        return
    blocks, expect, sizes = [], 0, {}
    for cells in table:
        if len(cells) < 2 or not cells[0].startswith("`0x"):
            continue
        off = int(cells[0].strip("`"), 16)
        m = re.match(r"^(?:(\d+)\s*x\s*)?(0x[0-9a-f]+)$", cells[1])
        if not m:
            fail("state-object.md", "unparsable size %r at 0x%03x" % (cells[1], off))
            continue
        size = (int(m.group(1)) if m.group(1) else 1) * int(m.group(2), 16)
        blocks.append((off, size, cells[2]))
        sizes[off] = size
        if off != expect:
            fail("state-object.md", "gap or overlap at 0x%03x, expected 0x%03x" % (off, expect))
        expect = off + size
    if expect != 0x800:
        fail("state-object.md", "blocks cover 0x%x, not 0x800" % expect)
    sys.path.insert(0, str(ROOT / "code" / "lg200"))
    import gen_state_map
    gen = {off: (size, label) for off, size, label, _ in gen_state_map.BLOCKS}
    if set(gen) != set(sizes):
        fail("gen_state_map.py", "offsets differ from the chapter: %s"
             % sorted(hex(o) for o in set(gen) ^ set(sizes)))
    for off in set(gen) & set(sizes):
        if gen[off][0] != sizes[off]:
            fail("gen_state_map.py", "size at 0x%03x is 0x%x, the chapter says 0x%x"
                 % (off, gen[off][0], sizes[off]))
    for p in gen_state_map.check():
        fail("gen_state_map.py", p)


def check_nav_and_links():
    mk = (ROOT / "mkdocs.yml").read_text(encoding="utf-8")
    nav = re.findall(r"(gpu/[^\s]+\.md)", mk)
    for target in nav:
        if not (DOCS / target).exists():
            fail("mkdocs.yml", "nav target %s does not exist" % target)
    pages = {os.path.relpath(p, DOCS) for p in GPU.rglob("*.md")}
    for p in sorted(pages - set(nav)):
        fail("mkdocs.yml", "%s is not reachable from the nav" % p)

    anchors = {}
    for path in DOCS.rglob("*.md"):
        got = set()
        for m in re.finditer(r"^#{1,6}\s+(.*)", path.read_text(encoding="utf-8"), re.M):
            got.add(re.sub(r"[^\w\s-]", "", m.group(1).lower()).strip().replace(" ", "-"))
        anchors[path] = got
    for path in sorted(GPU.rglob("*.md")):
        text = path.read_text(encoding="utf-8")
        for m in re.finditer(r"\[([^\]]*)\]\(([^)]+)\)", text):
            target = m.group(2)
            if target.startswith(("http", "mailto:", "/")):
                continue
            rel, _, frag = target.partition("#")
            full = pathlib.Path(os.path.normpath(path.parent / rel)) if rel else path
            if rel and not full.exists():
                fail(str(path.relative_to(ROOT)), "link to missing file: %s" % target)
                continue
            if frag and full.suffix == ".md" and frag not in anchors.get(full, set()):
                fail(str(path.relative_to(ROOT)), "link to missing anchor: %s" % target)


def check_figures():
    text = (GPU / "graphics" / "state-object.md").read_text(encoding="utf-8")
    for path in GPU.rglob("*.md"):
        body = path.read_text(encoding="utf-8")
        for m in re.finditer(r"!\[[^\]]*\]\(([^)]+\.svg)\)", body):
            target = os.path.normpath(path.parent / m.group(1))
            if not os.path.exists(target):
                fail(str(path.relative_to(ROOT)), "figure missing: %s" % m.group(1))
    for svg in sorted((GPU / "assets").glob("*.svg")):
        try:
            import xml.etree.ElementTree as ET
            ET.parse(svg)
        except Exception as exc:                     # noqa: BLE001
            fail(str(svg.relative_to(ROOT)), "not well-formed XML: %s" % exc)


# Rows whose u-l-i-g encoding deliberately differs from the driver table.
#
# The driver's lg200.xml and this table were both synchronized from the same
# artifact, so agreement proves transcription and nothing more.  For the flat
# memory forms the artifact itself was wrong, and the vendor's own assembler
# says so: every flat form it assembles carries lo[30] (`bl1`) and hi[28]
# (hi[29] too for load_v_flat_i16/u16) set, while the artifact classified those
# bits as "not part of the encoding" (parameter / reserved), so this table and
# the driver both left them clear and produced words the vendor's own
# MCSDisassembler rejects (decoded as PHI).
#
#   python3 enc_probe.py        # the vendor assembler comparison that fixes this
#
# The divergence is expected until the driver table is corrected the same way;
# anything NOT in this set is a real disagreement and still fails.
MESA_KNOWN_DIVERGENCE = {
    "atomic_v_flat_add_i32", "atomic_v_flat_add_i64", "atomic_v_flat_and_b32",
    "atomic_v_flat_and_b64", "atomic_v_flat_cmpswap_b32",
    "atomic_v_flat_cmpswap_b64", "atomic_v_flat_dec_u32",
    "atomic_v_flat_dec_u64", "atomic_v_flat_inc_u32", "atomic_v_flat_inc_u64",
    "atomic_v_flat_max_i32", "atomic_v_flat_max_i64", "atomic_v_flat_max_u32",
    "atomic_v_flat_max_u64", "atomic_v_flat_min_i32", "atomic_v_flat_min_i64",
    "atomic_v_flat_min_u32", "atomic_v_flat_min_u64", "atomic_v_flat_or_b32",
    "atomic_v_flat_or_b64", "atomic_v_flat_sub_i32", "atomic_v_flat_sub_i64",
    "atomic_v_flat_swap_b32", "atomic_v_flat_swap_b64",
    "atomic_v_flat_xor_b32", "atomic_v_flat_xor_b64",
    "load_v_flat_b128", "load_v_flat_b32", "load_v_flat_b64", "load_v_flat_b96",
    "load_v_flat_i16", "load_v_flat_i8", "load_v_flat_u16", "load_v_flat_u8",
    "store_v_flat_b128", "store_v_flat_b16", "store_v_flat_b32",
    "store_v_flat_b64", "store_v_flat_b8", "store_v_flat_b96",
}


def check_codec_consistency(rows):
    """The C++ codec's generated table must agree with this catalog.

    `codec/generated/lg200_codec.inc` says it is generated by
    `tools/gen_codec.py` from the LG200 catalog, and that generator is not in
    the repository - so the two copies are synchronized by hand, which is how
    a mask fix can land in one and not the other.  This check compares the
    fields that are fully derivable from the catalog:

      inst_lo/hi      == the catalog's base word
      fixed_lo/hi     == catalog fixed | (catalog reserved & catalog inst)

    The second line is not a guess: CodecRow has no reserved field, so the
    generator folds into `fixed` exactly those reserved bits the base word
    sets, which are the bits the encoder must not clear.  A reserved bit the
    base leaves clear stays free, so it is not folded.

    `template_lo/hi` is deliberately NOT checked: it is the canonical seed's
    encoding, which needs the operand rules to reproduce, and those live in
    semantics.cpp rather than in the catalog.
    """
    inc = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                       "code", "lg200", "gpu", "codec", "generated",
                       "lg200_codec.inc")
    if not os.path.exists(inc):
        fail("codec", "%s not found" % inc)
        return
    import re
    pat = re.compile(
        r'^  \{"([a-z0-9_]+)", "[^"]*", "[^"]*", "[^"]*", '
        r'(0x[0-9a-f]{8})u, (0x[0-9a-f]{8})u, (0x[0-9a-f]{8})u, (0x[0-9a-f]{8})u, ',
        re.M)
    seen = {}
    for m in pat.finditer(open(inc).read()):
        seen[m.group(1)] = tuple(int(m.group(i), 16) for i in range(2, 6))
    for r in rows:
        got = seen.get(r["mnemonic"])
        if got is None:
            fail("codec", "%s is absent from codec/generated/lg200_codec.inc"
                 % r["mnemonic"])
            continue
        want_inst = (int(r["inst_lo"], 16), int(r["inst_hi"], 16))
        want_fixed = (
            int(r["fixed_mask_lo"], 16) |
                (int(r["reserved_mask_lo"], 16) & int(r["inst_lo"], 16)),
            int(r["fixed_mask_hi"], 16) |
                (int(r["reserved_mask_hi"], 16) & int(r["inst_hi"], 16)))
        if got[0:2] != want_inst:
            fail("codec", "%s: the generated table's base word is %08x/%08x, the "
                 "catalog says %08x/%08x" % ((r["mnemonic"],) + got[0:2] + want_inst))
        if got[2:4] != want_fixed:
            fail("codec", "%s: the generated table's fixed mask is %08x/%08x, the "
                 "catalog says %08x/%08x (fixed | reserved&inst)"
                 % ((r["mnemonic"],) + got[2:4] + want_fixed))


def check_mesa(rows, path):
    """Compare the masks and the base word against the driver's machine table."""
    if not os.path.exists(path):
        print("check_gpu: --mesa %s not found, skipping the cross-check" % path)
        return
    known = 0
    import xml.etree.ElementTree as ET
    mesa = {}
    for ins in ET.parse(path).getroot().iter("instruction"):
        e = ins.find("encoding")
        if e is not None:
            mesa[ins.get("name")] = e.attrib
    if not mesa:
        fail("mesa", "no <encoding> elements found in %s" % path)
        return
    def g(d, k):
        return int(d.get(k, "0") or "0", 16)
    pairs = [("opcode", "opcode_mask_lo", "opcode_mask_hi", "opcode-mask-inst-lo", "opcode-mask-inst-hi"),
             ("parameter", "parameter_mask_lo", "parameter_mask_hi", "parameter-mask-inst-lo", "parameter-mask-inst-hi"),
             ("ignored", "ignored_mask_lo", "ignored_mask_hi", "print-ignored-mask-inst-lo", "print-ignored-mask-inst-hi"),
             ("fixed", "fixed_mask_lo", "fixed_mask_hi", "fixed-mask-inst-lo", "fixed-mask-inst-hi")]
    for r in rows:
        m = mesa.get(r["mnemonic"])
        if m is None:
            fail("mesa", "%s is absent from the driver table" % r["mnemonic"])
            continue
        differs = any((int(r[ulo], 16), int(r[uhi], 16)) != (g(m, mlo), g(m, mhi))
                      for label, ulo, uhi, mlo, mhi in pairs) or \
            (int(r["inst_lo"], 16), int(r["inst_hi"], 16)) != (g(m, "value-inst-lo"), g(m, "value-inst-hi"))
        if differs and r["mnemonic"] in MESA_KNOWN_DIVERGENCE:
            known += 1
            continue
        for label, ulo, uhi, mlo, mhi in pairs:
            if (int(r[ulo], 16), int(r[uhi], 16)) != (g(m, mlo), g(m, mhi)):
                fail("mesa", "%s: %s mask differs (u-l-i-g %s/%s, driver %s/%s)"
                     % (r["mnemonic"], label, r[ulo], r[uhi], m[mlo], m[mhi]))
        if (int(r["inst_lo"], 16), int(r["inst_hi"], 16)) != (g(m, "value-inst-lo"), g(m, "value-inst-hi")):
            fail("mesa", "%s: base word differs (u-l-i-g %s/%s, driver %s/%s)"
                 % (r["mnemonic"], r["inst_lo"], r["inst_hi"],
                    m["value-inst-lo"], m["value-inst-hi"]))
    if known:
        print("check_gpu: %d flat rows differ from the driver table by design - the "
              "driver carries the same artifact defect and must be corrected the "
              "same way (see MESA_KNOWN_DIVERGENCE and enc_probe.py)" % known)


def check_generator_links():
    """Relative links emitted by main.py must resolve from every macro page.

    A link written in a page is checked above; a link the *generator* writes is
    in no page's source, so it has to be checked against every directory whose
    pages include generated content.  This caught a hardcoded `measurements.md`
    that went stale when the instruction indexes moved into their own directory.
    """
    text = (ROOT / "main.py").read_text(encoding="utf-8")
    macro_dirs = {p.parent for p in GPU.rglob("*.md")
                  if "{{" in p.read_text(encoding="utf-8")}
    if not macro_dirs:
        fail("main.py", "no page references a macro, so generated links are unchecked")
        return
    for m in re.finditer(r"\]\(([^)#\s]+\.md)(?:#[^)]*)?\)", text):
        target = m.group(1)
        if target.startswith(("http", "/")):
            continue
        for d in sorted(macro_dirs):
            if not (d / target).exists():
                fail("main.py", "generated link %r does not resolve from %s"
                     % (target, os.path.relpath(d, ROOT)))


def main(argv):
    rows = load_rows()
    if len(rows) != 768:
        fail("table", "expected 768 rows, found %d" % len(rows))
    check_codec_consistency(rows)
    check_masks(rows)
    check_formats(rows)
    check_families(rows)
    check_index_coverage(rows)
    check_state_map()
    check_nav_and_links()
    check_figures()
    check_generator_links()
    if "--mesa" in argv:
        check_mesa(rows, argv[argv.index("--mesa") + 1])
    if problems:
        print("check_gpu: %d problem(s)" % len(problems))
        for p in problems[:40]:
            print("  " + p)
        if len(problems) > 40:
            print("  ... and %d more" % (len(problems) - 40))
        return 1
    print("check_gpu: 768 rows, 12 formats, 7 families, 33 state blocks - all invariants hold")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
