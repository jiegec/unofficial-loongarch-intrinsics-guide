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

    # fp to int
    "s": "word",
}

members_fp = {
    "s": "fp32",
    "d": "fp64",
}

widths_fp = {
    "s": 32,
    "d": 64,
}

for width in ["b", "bu", "h", "hu", "w", "wu", "d", "du"]:
    w = widths[width]
    m = members[width]
    if "u" in width:
        sign = "u"
    else:
        sign = "s"
    double_width = double_widths_same_signedness[width]
    double_w = widths[double_width]
    double_m = members[double_width]

    for name, op in [("avg", "&"), ("avgr", "|")]:
        with open(f"v{name}_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = (({sign}{w})a.{m}[i] >> 1) + (({sign}{w})b.{m}[i] >> 1) + ((a.{m}[i] {op} b.{m}[i]) & 1);",
                file=f,
            )
            print(f"}}", file=f)
    for name, op in [("div", "/"), ("mod", "%")]:
        with open(f"v{name}_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = (b.{m}[i] == 0) ? 0 : (({sign}{w})a.{m}[i] {op} ({sign}{w})b.{m}[i]);",
                file=f,
            )
            print(f"}}", file=f)
    with open(f"vexth_{double_width}_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // double_w};i++) {{", file=f)
        print(
            f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})a.{m}[{128 // double_w} + i];",
            file=f,
        )
        print(f"}}", file=f)
    for name, op in [("add", "+"), ("sub", "-")]:
        with open(f"vh{name}w_{double_width}_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {128 // double_w};i++) {{", file=f)
            print(
                f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})a.{m}[2 * i + 1] {op} ({sign}{double_w})({sign}{w})b.{m}[2 * i];",
                file=f,
            )
            print(f"}}", file=f)

    if width != "d" and width != "du":
        with open(f"vsllwil_{double_width}_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {128 // double_w};i++) {{", file=f)
            print(
                f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})a.{m}[i] << imm;",
                file=f,
            )
            print(f"}}", file=f)
        if sign == "s":
            for name, sign in [("srl", "u"), ("sra", "s")]:
                with open(f"v{name}n_{width}_{double_width}.h", "w") as f:
                    print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
                    print(
                        f"  dst.{m}[i] = (i < {64 // w}) ? ({sign}{w})(({sign}{double_w})a.{double_m}[i] >> (b.{double_m}[i] & {double_w-1})) : 0;",
                        file=f,
                    )
                    print(f"}}", file=f)
                with open(f"v{name}rn_{width}_{double_width}.h", "w") as f:
                    print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
                    print(f"if (i < {64 // w}) {{", file=f)
                    print(f"u8 shift = (b.{double_m}[i] & {double_w-1});", file=f)
                    print(f"if (shift == 0) {{", file=f)
                    print(
                        f"  dst.{m}[i] = ({sign}{w})({sign}{double_w})a.{double_m}[i];",
                        file=f,
                    )
                    print(f"}} else {{", file=f)
                    print(
                        f"  dst.{m}[i] = ({sign}{w})((({sign}{double_w})a.{double_m}[i] >> shift) + ((({sign}{double_w})a.{double_m}[i] >> (shift - 1)) & 0x1));",
                        file=f,
                    )
                    print(f"}}", file=f)
                    print(f"}} else {{", file=f)
                    print(f"  dst.{m}[i] = 0;", file=f)
                    print(f"}}", file=f)
                    print(f"}}", file=f)
        for name, shift_sign in [("srl", "u"), ("sra", "s")]:
            double_width_signed = double_width[:1]
            if shift_sign == "u":
                min = 0
                if sign == "u":
                    max = (2**w) - 1
                else:
                    max = (2 ** (w - 1)) - 1
            else:
                if sign == "u":
                    min = 0
                    max = (2**w) - 1
                else:
                    min = -(2 ** (w - 1))
                    max = (2 ** (w - 1)) - 1
            with open(f"vs{name}n_{width}_{double_width_signed}.h", "w") as f:
                print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
                print(f"if (i < {64 // w}) {{", file=f)
                print(
                    f"  {shift_sign}{double_w} temp = ({shift_sign}{double_w})a.{double_m}[i] >> (b.{double_m}[i] & {double_w-1});",
                    file=f,
                )
                print(
                    f"  dst.{m}[i] = clamp<{shift_sign}{double_w}>(temp, {min}, {max});",
                    file=f,
                )
                print(f"}} else {{", file=f)
                print(
                    f"  dst.{m}[i] = 0;",
                    file=f,
                )
                print(f"}}", file=f)
                print(f"}}", file=f)
            with open(f"vs{name}rn_{width}_{double_width_signed}.h", "w") as f:
                print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
                print(f"if (i < {64 // w}) {{", file=f)
                print(f"{shift_sign}{double_w} temp;", file=f)
                print(f"if ((b.{double_m}[i] & {double_w-1}) == 0) {{", file=f)
                print(
                    f"  temp = ({shift_sign}{double_w})a.{double_m}[i];",
                    file=f,
                )
                print(f"}} else {{", file=f)
                print(
                    f"  temp = (({shift_sign}{double_w})a.{double_m}[i] >> (b.{double_m}[i] & {double_w-1})) + ((({shift_sign}{double_w})a.{double_m}[i] >> ((b.{double_m}[i] & {double_w-1}) - 1)) & 1);",
                    file=f,
                )
                print(f"}}", file=f)
                print(
                    f"  dst.{m}[i] = clamp<{shift_sign}{double_w}>(temp, {min}, {max});",
                    file=f,
                )
                print(f"}} else {{", file=f)
                print(
                    f"  dst.{m}[i] = 0;",
                    file=f,
                )
                print(f"}}", file=f)
                print(f"}}", file=f)

    if sign == "s":
        for name, sign in [("srl", "u"), ("sra", "s")]:
            with open(f"v{name}ni_{width}_{double_width}.h", "w") as f:
                print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = (i < {64 // w}) ? ({sign}{w})(({sign}{double_w})b.{double_m}[i] >> imm) : ({sign}{w})(({sign}{double_w})a.{double_m}[i - {64 // w}] >> imm);",
                    file=f,
                )
                print(f"}}", file=f)
            with open(f"v{name}rni_{width}_{double_width}.h", "w") as f:
                print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
                print(f"if (i < {64 // w}) {{", file=f)
                print(f"if (imm == 0) {{", file=f)
                print(
                    f"  dst.{m}[i] = ({sign}{w})({sign}{double_w})b.{double_m}[i];",
                    file=f,
                )
                print(f"}} else {{", file=f)
                print(
                    f"  dst.{m}[i] = ({sign}{w})((({sign}{double_w})b.{double_m}[i] >> imm) + ((({sign}{double_w})b.{double_m}[i] >> (imm - 1)) & 0x1));",
                    file=f,
                )
                print(f"}}", file=f)
                print(f"}} else {{", file=f)
                print(f"if (imm == 0) {{", file=f)
                print(
                    f"  dst.{m}[i] = ({sign}{w})({sign}{double_w})a.{double_m}[i - {64 // w}];",
                    file=f,
                )
                print(f"}} else {{", file=f)
                print(
                    f"  dst.{m}[i] = ({sign}{w})((({sign}{double_w})a.{double_m}[i - {64 // w}] >> imm) + ((({sign}{double_w})a.{double_m}[i - {64 // w}] >> (imm - 1)) & 0x1));",
                    file=f,
                )
                print(f"}}", file=f)
                print(f"}}", file=f)
                print(f"}}", file=f)

    for name, shift_sign in [("srl", "u"), ("sra", "s")]:
        double_width_signed = double_width[:1]
        if shift_sign == "u":
            min = 0
            if sign == "u":
                max = (2**w) - 1
            else:
                max = (2 ** (w - 1)) - 1
        else:
            if sign == "u":
                min = 0
                max = (2**w) - 1
            else:
                min = -(2 ** (w - 1))
                max = (2 ** (w - 1)) - 1
        with open(f"vs{name}ni_{width}_{double_width_signed}.h", "w") as f:
            print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
            print(f"if (i < {64 // w}) {{", file=f)
            print(
                f"  {shift_sign}{double_w} temp = ({shift_sign}{double_w})b.{double_m}[i] >> imm;",
                file=f,
            )
            print(
                f"  dst.{m}[i] = clamp<{shift_sign}{double_w}>(temp, {min}, {max});",
                file=f,
            )
            print(f"}} else {{", file=f)
            print(
                f"  {shift_sign}{double_w} temp = ({shift_sign}{double_w})a.{double_m}[i - {64 // w}] >> imm;",
                file=f,
            )
            print(
                f"  dst.{m}[i] = clamp<{shift_sign}{double_w}>(temp, {min}, {max});",
                file=f,
            )
            print(f"}}", file=f)
            print(f"}}", file=f)
        with open(f"vs{name}rni_{width}_{double_width_signed}.h", "w") as f:
            print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
            print(f"if (i < {64 // w}) {{", file=f)
            print(f"{shift_sign}{double_w} temp;", file=f)
            print(f"if (imm == 0) {{", file=f)
            print(
                f"  temp = ({shift_sign}{double_w})b.{double_m}[i];",
                file=f,
            )
            print(f"}} else {{", file=f)
            print(
                f"  temp = (({shift_sign}{double_w})b.{double_m}[i] >> imm) + ((({shift_sign}{double_w})b.{double_m}[i] >> (imm - 1)) & 1);",
                file=f,
            )
            print(f"}}", file=f)
            print(
                f"  dst.{m}[i] = clamp<{shift_sign}{double_w}>(temp, {min}, {max});",
                file=f,
            )
            print(f"}} else {{", file=f)
            print(f"{shift_sign}{double_w} temp;", file=f)
            print(f"if (imm == 0) {{", file=f)
            print(
                f"  temp = ({shift_sign}{double_w})a.{double_m}[i - {64 // w}];",
                file=f,
            )
            print(f"}} else {{", file=f)
            print(
                f"  temp = (({shift_sign}{double_w})a.{double_m}[i - {64 // w}] >> imm) + ((({shift_sign}{double_w})a.{double_m}[i - {64 // w}] >> (imm - 1)) & 1);",
                file=f,
            )
            print(f"}}", file=f)
            print(
                f"  dst.{m}[i] = clamp<{shift_sign}{double_w}>(temp, {min}, {max});",
                file=f,
            )
            print(f"}}", file=f)
            print(f"}}", file=f)

    if width == "d" or width == "du":
        with open(f"vextl_{double_width}_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {128 // double_w};i++) {{", file=f)
            print(
                f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})a.{m}[i];",
                file=f,
            )
            print(f"}}", file=f)

    for op in ["max", "min"]:
        with open(f"v{op}_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = {op}(({sign}{w})a.{m}[i], ({sign}{w})b.{m}[i]);",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"v{op}i_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = {op}(({sign}{w})a.{m}[i], ({sign}{w})imm);",
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
        with open(f"vs{name}i_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = (({sign}{w})a.{m}[i] {op} imm) ? 0x{((1 << w) - 1):X} : 0;",
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
    with open(f"vpickve2gr_{width}.h", "w") as f:
        print(f"dst = a.{m}[idx];", file=f)

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
    with open(f"vsat_{width}.h", "w") as f:
        if sign == "s":
            min = "-(1 << imm)"
            max = "(1 << imm) - 1"
        else:
            min = "0"
            max = "(1 << (imm+1)) - 1"
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = clamp<{sign}{w}>(a.{m}[i], {min}, {max});",
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
    for name, op in [("add", "+"), ("sub", "-")]:
        with open(f"v{name}i_{width}u.h", "w") as f:
            print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = a.{m}[i] {op} imm;",
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
    with open(f"vseqi_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = ((s{w})a.{m}[i] == imm) ? 0x{((1 << w) - 1):X} : 0;",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vsll_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = a.{m}[i] << (b.{m}[i] & 0x{w-1:x});",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vslli_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = a.{m}[i] << imm;",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vsrl_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = a.{m}[i] >> (b.{m}[i] & 0x{w-1:x});",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vsrli_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = a.{m}[i] >> imm;",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vsra_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = ((s{w})a.{m}[i]) >> (b.{m}[i] & 0x{w-1:x});",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vsrai_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = ((s{w})a.{m}[i]) >> imm;",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vrotr_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = (a.{m}[i] >> (b.{m}[i] & 0x{w-1:x})) | (a.{m}[i] << ({w} - (b.{m}[i] & 0x{w-1:x})));",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vrotri_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = (a.{m}[i] >> imm) | (a.{m}[i] << ({w} - imm));",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vsrlr_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(f"  if ((b.{m}[i] & 0x{w-1:x}) == 0) {{", file=f)
        print(
            f"    dst.{m}[i] = a.{m}[i];",
            file=f,
        )
        print(f"  }} else {{", file=f)
        print(
            f"    dst.{m}[i] = (a.{m}[i] >> (b.{m}[i] & 0x{w-1:x})) + ((a.{m}[i] >> ((b.{m}[i] & 0x{w-1:x}) - 1)) & 0x1);",
            file=f,
        )
        print(f"  }}", file=f)
        print(f"}}", file=f)
    with open(f"vsrlri_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(f"  if (imm == 0) {{", file=f)
        print(
            f"    dst.{m}[i] = a.{m}[i];",
            file=f,
        )
        print(f"  }} else {{", file=f)
        print(
            f"    dst.{m}[i] = (a.{m}[i] >> imm) + ((a.{m}[i] >> (imm - 1)) & 0x1);",
            file=f,
        )
        print(f"  }}", file=f)
        print(f"}}", file=f)
    with open(f"vsrar_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(f"  if ((b.{m}[i] & 0x{w-1:x}) == 0) {{", file=f)
        print(
            f"    dst.{m}[i] = a.{m}[i];",
            file=f,
        )
        print(f"  }} else {{", file=f)
        print(
            f"    dst.{m}[i] = ((s{w})a.{m}[i] >> (b.{m}[i] & 0x{w-1:x})) + (((s{w})a.{m}[i] >> ((b.{m}[i] & 0x{w-1:x}) - 1)) & 0x1);",
            file=f,
        )
        print(f"  }}", file=f)
        print(f"}}", file=f)
    with open(f"vsrari_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(f"  if (imm == 0) {{", file=f)
        print(
            f"    dst.{m}[i] = a.{m}[i];",
            file=f,
        )
        print(f"  }} else {{", file=f)
        print(
            f"    dst.{m}[i] = ((s{w})a.{m}[i] >> imm) + (((s{w})a.{m}[i] >> (imm - 1)) & 0x1);",
            file=f,
        )
        print(f"  }}", file=f)
        print(f"}}", file=f)
    with open(f"vpackev_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = (i % 2 == 1) ? a.{m}[i-1] : b.{m}[i];",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vpackod_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = (i % 2 == 1) ? a.{m}[i] : b.{m}[i+1];",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vpickev_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = (i < {64 // w}) ? b.{m}[i * 2] : a.{m}[(i - {64 // w}) * 2];",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vpickod_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = (i < {64 // w}) ? b.{m}[i * 2 + 1] : a.{m}[(i - {64 // w}) * 2 + 1];",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vreplve_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = a.{m}[idx % {128 // w}];",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vreplvei_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = a.{m}[idx];",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vreplgr2vr_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = val;",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vrepli_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = imm;",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vsigncov_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = (a.{m}[i] == 0) ? 0 : ((s{w})a.{m}[i] > 0 ? b.{m}[i] : -b.{m}[i]);",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vneg_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = -a.{m}[i];",
            file=f,
        )
        print(f"}}", file=f)
    if width != "d":
        with open(f"vshuf4i_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = a.{m}[(i & ~0x3) + ((imm >> (2 * (i & 0x3))) & 0x3)];",
                file=f,
            )
            print(f"}}", file=f)

for width in ["s", "d"]:
    m = members_fp[width]
    int_m = members[width]
    w = widths_fp[width]
    if width == "s":
        int_width = "w"
        int_w = 32
    else:
        int_width = "l"
        int_w = 64
    for name, op in [("div", "/"), ("mul", "*"), ("sub", "-"), ("add", "+")]:
        with open(f"vf{name}_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = a.{m}[i] {op} b.{m}[i];",
                file=f,
            )
            print(f"}}", file=f)
    if width == "s":
        for rounding in ["", "rm", "rp", "rz", "rne"]:
            with open(f"vftint{rounding}l_l_{width}.h", "w") as f:
                print(f"for (int i = 0;i < {64 // w};i++) {{", file=f)
                print(
                    f"  dst.dword[i] = (s{int_w})a.{m}[i]; // rounding mode is not expressed in C",
                    file=f,
                )
                print(f"}}", file=f)
            with open(f"vftint{rounding}h_l_{width}.h", "w") as f:
                print(f"for (int i = 0;i < {64 // w};i++) {{", file=f)
                print(
                    f"  dst.dword[i] = (s{int_w})a.{m}[i + {64 // w}]; // rounding mode is not expressed in C",
                    file=f,
                )
                print(f"}}", file=f)
    for rounding in ["", "rm", "rp", "rz", "rne"]:
        with open(f"vftint{rounding}_{int_width}_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
            print(
                f"  dst.{int_m}[i] = (s{int_w})a.{m}[i]; // rounding mode is not expressed in C",
                file=f,
            )
            print(f"}}", file=f)
    for rounding in ["", "rz"]:
        with open(f"vftint{rounding}_{int_width}u_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
            print(
                f"  dst.{int_m}[i] = (u{int_w})a.{m}[i]; // rounding mode is not expressed in C",
                file=f,
            )
            print(f"}}", file=f)

    for name in ["max", "min"]:
        with open(f"vf{name}_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = {op}(a.{m}[i], b.{m}[i]);",
                file=f,
            )
            print(f"}}", file=f)
    with open(f"vfmaxa_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = (abs(a.{m}[i]) > abs(b.{m}[i])) ? a.{m}[i] : b.{m}[i];",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vfmina_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = (abs(a.{m}[i]) < abs(b.{m}[i])) ? a.{m}[i] : b.{m}[i];",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vflogb_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = log2(a.{m}[i]);",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vfsqrt_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = sqrt(a.{m}[i]);",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vfrsqrt_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = 1.0 / sqrt(a.{m}[i]);",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vfrecip_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = 1 / a.{m}[i];",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vfrsqrte_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = 1.0 / sqrt(a.{m}[i]); // estimated",
            file=f,
        )
        print(f"}}", file=f)
    with open(f"vfrecipe_{width}.h", "w") as f:
        print(f"for (int i = 0;i < {128 // w};i++) {{", file=f)
        print(
            f"  dst.{m}[i] = 1 / a.{m}[i]; // estimated",
            file=f,
        )
        print(f"}}", file=f)

os.system("clang-format -i *.cpp *.h")
