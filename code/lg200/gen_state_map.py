#!/usr/bin/env python3
"""Generate docs/gpu/assets/state-map.svg from the LG200 state-object block list.

The figure is a projection of `docs/gpu/graphics/state-object.md`'s state map, so
it is generated rather than drawn: a hand-drawn map silently dropped nine blocks
(the regions nothing writes) and merged five stage configs into one box, which is
exactly the information a driver author needs.  `check_gpu.py` verifies that the
block list below still agrees with the chapter's table.

Usage:
  python3 gen_state_map.py            # write the SVG
  python3 gen_state_map.py --check    # verify invariants only
"""
import pathlib
import sys

ROOT = pathlib.Path(__file__).resolve().parents[2]
OUT = ROOT / "docs" / "gpu" / "assets" / "state-map.svg"

# offset, size, label, category
#   full    a working path writes the whole block
#   partial a working path writes part of it
#   never   no established path writes it
#   open    written, but the field meanings are unresolved
BLOCKS = [
    (0x000, 0x24, "Draw Info", "full"),
    (0x024, 0x0c, "draw tail", "never"),
    (0x030, 0x0c, "Pipe Enables", "full"),
    (0x03c, 0x0c, "pipe tail", "never"),
    (0x048, 0x2c0, "Viewport[0..15]", "partial"),
    (0x308, 0x10, "Guardband", "full"),
    (0x318, 0x0c, "Line Raster", "full"),
    (0x324, 0x10, "Depth Control", "full"),
    (0x334, 0x04, "Stencil front", "full"),
    (0x338, 0x04, "Stencil back", "full"),
    (0x33c, 0x04, "Blend Control", "full"),
    (0x340, 0x20, "Blend RT[0..7]", "partial"),
    (0x360, 0x10, "Blend constants", "full"),
    (0x370, 0x100, "Color Buffer[0..7]", "partial"),
    (0x470, 0x20, "Depth Buffer", "partial"),
    (0x490, 0x30, "pipeline configuration", "never"),
    (0x4c0, 0x08, "Shader Config", "full"),
    (0x4c8, 0x08, "Vertex stage config", "never"),
    (0x4d0, 0x08, "Hull stage config", "never"),
    (0x4d8, 0x08, "Domain stage config", "full"),
    (0x4e0, 0x08, "Geometry stage config", "partial"),
    (0x4e8, 0x08, "Streamout stage config", "open"),
    (0x4f0, 0x40, "Fragment attribute map[32]", "full"),
    (0x530, 0x08, "Fragment input config", "full"),
    (0x538, 0x04, "Fragment output config", "full"),
    (0x53c, 0x04, "fragment tail", "never"),
    (0x540, 0x60, "Shader info: VS", "never"),
    (0x5a0, 0x60, "Shader info: HS", "never"),
    (0x600, 0x60, "Shader info: DS", "full"),
    (0x660, 0x60, "Shader info: GS", "partial"),
    (0x6c0, 0x60, "Shader info: SS", "partial"),
    (0x720, 0x60, "Shader info: PS", "full"),
    (0x780, 0x80, "state tail", "never"),
]

FILL = {
    "full":    "#c8e6c9",
    "partial": "#bbdefb",
    "never":   "#eeeeee",
    "open":    "#ffe0b2",
}
TOTAL = 0x800
FLOOR = 16.0      # px, so the 4-byte blocks can still carry a label
SCALE = 0.34      # px per byte for blocks above the floor


def check():
    problems = []
    expect = 0
    for off, size, label, cat in BLOCKS:
        if off != expect:
            problems.append(f"gap or overlap at 0x{off:03x} ({label}), expected 0x{expect:03x}")
        if size <= 0:
            problems.append(f"non-positive size at 0x{off:03x}")
        if cat not in FILL:
            problems.append(f"unknown category {cat!r} at 0x{off:03x}")
        expect = off + size
    if expect != TOTAL:
        problems.append(f"blocks cover 0x{expect:x}, expected 0x{TOTAL:x}")
    return problems


def render():
    heights = [max(FLOOR, s * SCALE) for _, s, _, _ in BLOCKS]
    top = 46.0
    bottom = top + sum(heights) + 10
    height = bottom + 46
    lines = [
        '<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 %d %d" width="%d" height="%d"'
        ' font-family="ui-monospace, SFMono-Regular, Menlo, monospace" font-size="11">' % (620, height, 620, height),
        '<text x="8" y="16" fill="#111" font-size="13" font-weight="bold">'
        'LG200 state object &#8212; 512 dwords (0x800), 33 blocks</text>',
        '<rect x="86" y="24" width="14" height="10" fill="%s" stroke="#666"/>' % FILL["full"],
        '<text x="104" y="33" fill="#333">written</text>',
        '<rect x="170" y="24" width="14" height="10" fill="%s" stroke="#666"/>' % FILL["partial"],
        '<text x="188" y="33" fill="#333">partly written</text>',
        '<rect x="290" y="24" width="14" height="10" fill="%s" stroke="#666"/>' % FILL["never"],
        '<text x="308" y="33" fill="#333">never written</text>',
        '<rect x="412" y="24" width="14" height="10" fill="%s" stroke="#666"/>' % FILL["open"],
        '<text x="430" y="33" fill="#333">meanings unresolved</text>',
    ]
    y = top
    for (off, size, label, cat), h in zip(BLOCKS, heights):
        cy = y + h / 2
        lines.append('<text x="80" y="%.1f" text-anchor="end" fill="#555">0x%03x</text>' % (cy + 3.5, off))
        lines.append('<rect x="86" y="%.1f" width="46" height="%.1f" fill="%s" stroke="#888"/>'
                     % (y, h, FILL[cat]))
        lines.append('<text x="140" y="%.1f" fill="#111">%s</text>' % (cy + 3.5, label))
        lines.append('<text x="470" y="%.1f" text-anchor="end" fill="#777">%d B</text>' % (cy + 3.5, size))
        y += h
    lines.append('<text x="8" y="%.1f" fill="#555" font-size="10">'
                 'Block heights are proportional to size above a %d px floor; the four-byte blocks are drawn at the floor.'
                 '</text>' % (height - 26, int(FLOOR)))
    lines.append('<text x="8" y="%.1f" fill="#555" font-size="10">'
                 'Offsets are relative to the first dword of the object; the packet byte offset adds the 0x0c-byte envelope header.'
                 '</text>' % (height - 12))
    lines.append('</svg>')
    return "\n".join(lines) + "\n"


def main(argv):
    problems = check()
    for p in problems:
        print("  " + p)
    if problems:
        return 1
    print("blocks=%d covered=0x%x ok" % (len(BLOCKS), TOTAL))
    if "--check" in argv:
        return 0
    OUT.write_text(render(), encoding="utf-8")
    print("wrote", OUT.relative_to(ROOT))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
