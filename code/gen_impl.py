import os

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
    "qu": 128,
}

double_widths = {
    "b": "h",
    "bu": "h",
    "h": "w",
    "hu": "w",
    "w": "d",
    "wu": "d",
    "d": "q",
    "du": "q",
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
    "q": "qword",
    "qu": "qword",
}

for width in ["b", "bu", "h", "hu", "w", "wu", "d", "du"]:
    w = widths[width]
    m = members[width]
    if "u" in width:
        sign = "u"
    else:
        sign = "s"
    with open(f"vavg_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = (({sign}{w})a.{m}[i] >> 1) + (({sign}{w})b.{m}[i] >> 1) + (a.{m}[i] & b.{m}[i] & 1);",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vavgr_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = (({sign}{w})a.{m}[i] >> 1) + (({sign}{w})b.{m}[i] >> 1) + ((a.{m}[i] | b.{m}[i]) & 1);",
            file=f,
        )
        print(f"}}", file=f)

for width in ["b", "bu", "h", "hu", "w", "wu", "d", "du"]:
    double_width = double_widths[width]
    w = widths[width]
    m = members[width]
    double_w = widths[double_width]
    double_m = members[double_width]
    if "u" in width:
        sign = "u"
    else:
        sign = "s"
    for signed2 in [False, True]:
        if "u" not in width and not signed2:
            continue
        if "u" in width and signed2:
            suffix = f"_{width[:-1]}"
        else:
            suffix = ""
        if signed2:
            sign2 = "s"
        else:
            sign2 = "u"
        with open(f"vaddwev_{double_width}_{width}{suffix}.h", "w") as f:
            print(f"for (int i = 0;i < {128 // double_w};i++) {{", file=f)
            print(
                f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})a.{m}[2 * i] + ({sign2}{double_w})({sign2}{w})b.{m}[2 * i];",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"vaddwod_{double_width}_{width}{suffix}.h", "w") as f:
            print(f"for (int i = 0;i < {128 // double_w};i++) {{", file=f)
            print(
                f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})a.{m}[2 * i + 1] + ({sign2}{double_w})({sign2}{w})b.{m}[2 * i + 1];",
                file=f,
            )
            print(f"}}", file=f)

for width in ["b", "h", "w", "d"]:
    w = widths[width]
    m = members[width]
    with open(f"vbitclr_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = a.{m}[i] & (~((u{w})1 << (b.{m}[i] % {w})));",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vbitclri_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = a.{m}[i] & (~((u{w})1 << imm));",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vbitset_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = a.{m}[i] | ((u{w})1 << (b.{m}[i] % {w}));",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vbitseti_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = a.{m}[i] | ((u{w})1 << imm);",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vbitrev_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = a.{m}[i] ^ ((u{w})1 << (b.{m}[i] % {w}));",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vbitrevi_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = a.{m}[i] ^ ((u{w})1 << imm);",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vclo_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = clo(a.{m}[i]);",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vclz_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = clz(a.{m}[i]);",
            file=f,
        )
        print(f"}}", file=f)

os.system("clang-format -i *.cpp *.h")
