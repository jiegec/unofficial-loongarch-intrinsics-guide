widths_all = ["b", "bu", "h", "hu", "w", "wu", "d", "du"]

widths = {
    "b": 8,
    "bu": 8,
    "h": 16,
    "hu": 16,
    "w": 32,
    "wu": 32,
    "d": 64,
    "du": 64,
    "q": 128,
}

members = {
    "b": "byte",
    "bu": "byte",
    "h": "half",
    "hu": "half",
    "w": "word",
    "wu": "word",
    "d": "dword",
    "du": "dword",
}

for width in widths_all:
    w = widths[width]
    m = members[width]
    if "u" in width:
        sign = "u"
    else:
        sign = "s"
    with open(f"vavg_{width}.h", 'w') as f:
        print(f'for (int i = 0;i < {128 // w};i++) {{', file=f)
        print(f'  dst.{m}[i] = (({sign}{w})a.{m}[i] >> 1) + (({sign}{w})b.{m}[i] >> 1) + (a.{m}[i] & b.{m}[i] & 1);', file=f)
        print(f'}}', file=f)