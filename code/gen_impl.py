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

double_widths_same_signedness = {
    "b": "h",
    "bu": "hu",
    "h": "w",
    "hu": "wu",
    "w": "d",
    "wu": "du",
    "d": "q",
    "du": "qu",
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

members_fp = {
    "s": "fp32",
    "d": "fp64",
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
    with open(f"vdiv_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = (b.{m}[i] == 0) ? 0 : (({sign}{w})a.{m}[i] / ({sign}{w})b.{m}[i]);",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vmod_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = (b.{m}[i] == 0) ? 0 : (({sign}{w})a.{m}[i] % ({sign}{w})b.{m}[i]);",
            file=f,
        )
        print(f"}}", file=f)

    double_width = double_widths_same_signedness[width]
    double_w = widths[double_width]
    double_m = members[double_width]
    with open(f"vexth_{double_width}_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // double_w};i++) {{", file=f)
        print(
            f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})a.{m}[{128 // double_w} + i];",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vhaddw_{double_width}_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // double_w};i++) {{", file=f)
        print(
            f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})a.{m}[2 * i + 1] + ({sign}{double_w})({sign}{w})b.{m}[2 * i];",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vhsubw_{double_width}_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // double_w};i++) {{", file=f)
        print(
            f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})a.{m}[2 * i + 1] - ({sign}{double_w})({sign}{w})b.{m}[2 * i];",
            file=f,
        )
        print(f"}}", file=f)

    if width == "d" or width == "du":
        with open(f"vextl_{double_width}_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {128 // double_w};i++) {{", file=f)
            print(
                f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})a.{m}[i];",
                file=f,
            )
            print(f"}}", file=f)
    with open(f"vmax_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = max(({sign}{w})a.{m}[i], ({sign}{w})b.{m}[i]);",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vmaxi_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = max(({sign}{w})a.{m}[i], ({sign}{w})imm);",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vmin_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = min(({sign}{w})a.{m}[i], ({sign}{w})b.{m}[i]);",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vmini_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = min(({sign}{w})a.{m}[i], ({sign}{w})imm);",
            file=f,
        )
        print(f"}}", file=f)
    for name, op in [("lt", "<"), ("le", "<=")]:
        with open(f"vs{name}_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = (({sign}{w})a.{m}[i] {op} ({sign}{w})b.{m}[i]) ? 0x{((1 << w) - 1):X} : 0;",
                file=f,
            )
            print(f"}}", file=f)
    for op in ["sadd", "ssub"]:
        with open(f"v{op}_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = ({sign}{w}){op}(({sign}{w})a.{m}[i], ({sign}{w})b.{m}[i]);",
                file=f,
            )
            print(f"}}", file=f)
    with open(f"vmuh_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = ((({sign}{w * 2})({sign}{w})a.{m}[i] * ({sign}{w * 2})({sign}{w})b.{m}[i])) >> {w};",
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
        for arith, op in [("add", "+"), ("sub", "-"), ("mul", "*")]:
            if suffix != "" and arith == "sub":
                continue
            with open(f"v{arith}wev_{double_width}_{width}{suffix}.h", "w") as f:
                print(f"for (int i = 0;i < {128 // double_w};i++) {{", file=f)
                print(
                    f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})a.{m}[2 * i] {op} ({sign2}{double_w})({sign2}{w})b.{m}[2 * i];",
                    file=f,
                )
                print(f"}}", file=f)
            with open(f"v{arith}wod_{double_width}_{width}{suffix}.h", "w") as f:
                print(f"for (int i = 0;i < {128 // double_w};i++) {{", file=f)
                print(
                    f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})a.{m}[2 * i + 1] {op} ({sign2}{double_w})({sign2}{w})b.{m}[2 * i + 1];",
                    file=f,
                )
                print(f"}}", file=f)
        with open(f"vmaddwev_{double_width}_{width}{suffix}.h", "w") as f:
            print(f"for (int i = 0;i < {128 // double_w};i++) {{", file=f)
            print(
                f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})b.{m}[2 * i] * ({sign2}{double_w})({sign2}{w})c.{m}[2 * i] + ({sign2}{double_w})a.{double_m}[i];",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"vmaddwod_{double_width}_{width}{suffix}.h", "w") as f:
            print(f"for (int i = 0;i < {128 // double_w};i++) {{", file=f)
            print(
                f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})b.{m}[2 * i + 1] * ({sign2}{double_w})({sign2}{w})c.{m}[2 * i + 1] + ({sign2}{double_w})a.{double_m}[i];",
                file=f,
            )
            print(f"}}", file=f)

for width in ["b", "h", "w", "d"]:
    w = widths[width]
    m = members[width]
    for name, op in [("add", "+"), ("sub", "-"), ("mul", "*")]:
        with open(f"v{name}_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = a.{m}[i] {op} b.{m}[i];",
                file=f,
            )
            print(f"}}", file=f)
    with open(f"vmadd_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = b.{m}[i] * c.{m}[i] + a.{m}[i];",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vmsub_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = - b.{m}[i] * c.{m}[i] + a.{m}[i];",
            file=f,
        )
        print(f"}}", file=f)
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
    with open(f"vpcnt_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = popcount(a.{m}[i]);",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vextrins_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        mask = 128 // w - 1
        print(
            f"  dst.{m}[i] = (i == ((imm >> 4) & {mask})) ? b.{m}[imm & {mask}] : a.{m}[i];",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vilvh_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = (i % 2 == 1) ? a.{m}[i / 2 + {64 // w}] : b.{m}[i / 2 + {64 // w}];",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vilvl_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = (i % 2 == 1) ? a.{m}[i / 2] : b.{m}[i / 2];",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vinsgr2vr_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = (i == imm) ? b : a.{m}[i];",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vseq_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = (a.{m}[i] == b.{m}[i]) ? 0x{((1 << w) - 1):X} : 0;",
            file=f,
        )
        print(f"}}", file=f)

for width in ["s", "d"]:
    m = members_fp[width]
    with open(f"vfdiv_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = a.{m}[i] / b.{m}[i];",
            file=f,
        )
        print(f"}}", file=f)

os.system("clang-format -i *.cpp *.h")
