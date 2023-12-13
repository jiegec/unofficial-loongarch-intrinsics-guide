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

for vlen, prefix in [(128, "v"), (256, "xv")]:
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
            with open(f"{prefix}{name}_{width}.h", "w") as f:
                print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = (({sign}{w})a.{m}[i] >> 1) + (({sign}{w})b.{m}[i] >> 1) + ((a.{m}[i] {op} b.{m}[i]) & 1);",
                    file=f,
                )
                print(f"}}", file=f)
        for name, op in [("div", "/"), ("mod", "%")]:
            with open(f"{prefix}{name}_{width}.h", "w") as f:
                print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = (b.{m}[i] == 0) ? 0 : (({sign}{w})a.{m}[i] {op} ({sign}{w})b.{m}[i]);",
                    file=f,
                )
                print(f"}}", file=f)
        with open(f"{prefix}exth_{double_width}_{width}.h", "w") as f:
            if prefix == "v":
                print(f"for (int i = 0;i < {vlen // double_w};i++) {{", file=f)
                print(
                    f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})a.{m}[{vlen // double_w} + i];",
                    file=f,
                )
                print(f"}}", file=f)
            else:
                print(f"int i;", file=f)
                print(f"for (i = 0;i < {vlen // 2 // double_w};i++) {{", file=f)
                print(
                    f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})a.{m}[{vlen // 2 // double_w} + i];",
                    file=f,
                )
                print(f"}}", file=f)
                print(f"for (;i < {vlen // double_w};i++) {{", file=f)
                print(
                    f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})a.{m}[{vlen // double_w} + i];",
                    file=f,
                )
                print(f"}}", file=f)
        for name, op in [("add", "+"), ("sub", "-")]:
            with open(f"{prefix}h{name}w_{double_width}_{width}.h", "w") as f:
                print(f"for (int i = 0;i < {vlen // double_w};i++) {{", file=f)
                print(
                    f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})a.{m}[2 * i + 1] {op} ({sign}{double_w})({sign}{w})b.{m}[2 * i];",
                    file=f,
                )
                print(f"}}", file=f)

        if width != "d" and width != "du":
            with open(f"{prefix}sllwil_{double_width}_{width}.h", "w") as f:
                print(f"for (int i = 0;i < {vlen // double_w};i++) {{", file=f)
                print(
                    f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})a.{m}[i] << imm;",
                    file=f,
                )
                print(f"}}", file=f)
            if sign == "s":
                for name, sign in [("srl", "u"), ("sra", "s")]:
                    with open(f"{prefix}{name}n_{width}_{double_width}.h", "w") as f:
                        print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                        print(
                            f"  dst.{m}[i] = (i < {vlen // 2 // w}) ? ({sign}{w})(({sign}{double_w})a.{double_m}[i] >> (b.{double_m}[i] & {double_w-1})) : 0;",
                            file=f,
                        )
                        print(f"}}", file=f)
                    with open(f"{prefix}{name}rn_{width}_{double_width}.h", "w") as f:
                        print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                        print(f"if (i < {vlen // 2 // w}) {{", file=f)
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
                with open(f"{prefix}s{name}n_{width}_{double_width_signed}.h", "w") as f:
                    print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                    print(f"if (i < {vlen // 2 // w}) {{", file=f)
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
                with open(f"{prefix}s{name}rn_{width}_{double_width_signed}.h", "w") as f:
                    print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                    print(f"if (i < {vlen // 2 // w}) {{", file=f)
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
                with open(f"{prefix}{name}ni_{width}_{double_width}.h", "w") as f:
                    print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                    print(
                        f"  dst.{m}[i] = (i < {vlen // 2 // w}) ? ({sign}{w})(({sign}{double_w})b.{double_m}[i] >> imm) : ({sign}{w})(({sign}{double_w})a.{double_m}[i - {vlen // 2 // w}] >> imm);",
                        file=f,
                    )
                    print(f"}}", file=f)
                with open(f"{prefix}{name}rni_{width}_{double_width}.h", "w") as f:
                    print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                    print(f"if (i < {vlen // 2 // w}) {{", file=f)
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
                        f"  dst.{m}[i] = ({sign}{w})({sign}{double_w})a.{double_m}[i - {vlen // 2 // w}];",
                        file=f,
                    )
                    print(f"}} else {{", file=f)
                    print(
                        f"  dst.{m}[i] = ({sign}{w})((({sign}{double_w})a.{double_m}[i - {vlen // 2 // w}] >> imm) + ((({sign}{double_w})a.{double_m}[i - {vlen // 2 // w}] >> (imm - 1)) & 0x1));",
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
            with open(f"{prefix}s{name}ni_{width}_{double_width_signed}.h", "w") as f:
                print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                print(f"if (i < {vlen // 2 // w}) {{", file=f)
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
                    f"  {shift_sign}{double_w} temp = ({shift_sign}{double_w})a.{double_m}[i - {vlen // 2 // w}] >> imm;",
                    file=f,
                )
                print(
                    f"  dst.{m}[i] = clamp<{shift_sign}{double_w}>(temp, {min}, {max});",
                    file=f,
                )
                print(f"}}", file=f)
                print(f"}}", file=f)
            with open(f"{prefix}s{name}rni_{width}_{double_width_signed}.h", "w") as f:
                print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                print(f"if (i < {vlen // 2 // w}) {{", file=f)
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
                    f"  temp = ({shift_sign}{double_w})a.{double_m}[i - {vlen // 2 // w}];",
                    file=f,
                )
                print(f"}} else {{", file=f)
                print(
                    f"  temp = (({shift_sign}{double_w})a.{double_m}[i - {vlen // 2 // w}] >> imm) + ((({shift_sign}{double_w})a.{double_m}[i - {vlen // 2 // w}] >> (imm - 1)) & 1);",
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
            with open(f"{prefix}extl_{double_width}_{width}.h", "w") as f:
                if prefix == "v":
                    print(f"for (int i = 0;i < {vlen // double_w};i++) {{", file=f)
                    print(
                        f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})a.{m}[i];",
                        file=f,
                    )
                    print(f"}}", file=f)
                else:
                    print(f"int i;", file=f)
                    print(f"for (i = 0;i < {vlen // 2 // double_w};i++) {{", file=f)
                    print(
                        f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})a.{m}[i];",
                        file=f,
                    )
                    print(f"}}", file=f)
                    print(f"for (;i < {vlen // double_w};i++) {{", file=f)
                    print(
                        f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})a.{m}[i + {vlen // 2 // double_w}];",
                        file=f,
                    )
                    print(f"}}", file=f)

        for op in ["max", "min"]:
            with open(f"{prefix}{op}_{width}.h", "w") as f:
                print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = {op}(({sign}{w})a.{m}[i], ({sign}{w})b.{m}[i]);",
                    file=f,
                )
                print(f"}}", file=f)
            with open(f"{prefix}{op}i_{width}.h", "w") as f:
                print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = {op}(({sign}{w})a.{m}[i], ({sign}{w})imm);",
                    file=f,
                )
                print(f"}}", file=f)

        for name, op in [("lt", "<"), ("le", "<=")]:
            with open(f"{prefix}s{name}_{width}.h", "w") as f:
                print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = (({sign}{w})a.{m}[i] {op} ({sign}{w})b.{m}[i]) ? 0x{((1 << w) - 1):X} : 0;",
                    file=f,
                )
                print(f"}}", file=f)
            with open(f"{prefix}s{name}i_{width}.h", "w") as f:
                print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = (({sign}{w})a.{m}[i] {op} imm) ? 0x{((1 << w) - 1):X} : 0;",
                    file=f,
                )
                print(f"}}", file=f)
        for op in ["sadd", "ssub"]:
            with open(f"{prefix}{op}_{width}.h", "w") as f:
                print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = ({sign}{w}){op}(({sign}{w})a.{m}[i], ({sign}{w})b.{m}[i]);",
                    file=f,
                )
                print(f"}}", file=f)
        with open(f"{prefix}muh_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = ((({sign}{w * 2})({sign}{w})a.{m}[i] * ({sign}{w * 2})({sign}{w})b.{m}[i])) >> {w};",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}pickve2gr_{width}.h", "w") as f:
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
                with open(f"{prefix}{arith}wev_{double_width}_{width}{suffix}.h", "w") as f:
                    print(f"for (int i = 0;i < {vlen // double_w};i++) {{", file=f)
                    print(
                        f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})a.{m}[2 * i] {op} ({sign2}{double_w})({sign2}{w})b.{m}[2 * i];",
                        file=f,
                    )
                    print(f"}}", file=f)
                with open(f"{prefix}{arith}wod_{double_width}_{width}{suffix}.h", "w") as f:
                    print(f"for (int i = 0;i < {vlen // double_w};i++) {{", file=f)
                    print(
                        f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})a.{m}[2 * i + 1] {op} ({sign2}{double_w})({sign2}{w})b.{m}[2 * i + 1];",
                        file=f,
                    )
                    print(f"}}", file=f)
            with open(f"{prefix}maddwev_{double_width}_{width}{suffix}.h", "w") as f:
                print(f"for (int i = 0;i < {vlen // double_w};i++) {{", file=f)
                print(
                    f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})b.{m}[2 * i] * ({sign2}{double_w})({sign2}{w})c.{m}[2 * i] + ({sign2}{double_w})a.{double_m}[i];",
                    file=f,
                )
                print(f"}}", file=f)
            with open(f"{prefix}maddwod_{double_width}_{width}{suffix}.h", "w") as f:
                print(f"for (int i = 0;i < {vlen // double_w};i++) {{", file=f)
                print(
                    f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})b.{m}[2 * i + 1] * ({sign2}{double_w})({sign2}{w})c.{m}[2 * i + 1] + ({sign2}{double_w})a.{double_m}[i];",
                    file=f,
                )
                print(f"}}", file=f)
        with open(f"{prefix}sat_{width}.h", "w") as f:
            if sign == "s":
                min = "-(1 << imm)"
                max = "(1 << imm) - 1"
            else:
                min = "0"
                max = "(1 << (imm+1)) - 1"
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = clamp<{sign}{w}>(a.{m}[i], {min}, {max});",
                file=f,
            )
            print(f"}}", file=f)

    for width in ["b", "h", "w", "d"]:
        w = widths[width]
        m = members[width]
        for name, op in [("add", "+"), ("sub", "-"), ("mul", "*")]:
            with open(f"{prefix}{name}_{width}.h", "w") as f:
                print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = a.{m}[i] {op} b.{m}[i];",
                    file=f,
                )
                print(f"}}", file=f)
        for name, op in [("add", "+"), ("sub", "-")]:
            with open(f"{prefix}{name}i_{width}u.h", "w") as f:
                print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = a.{m}[i] {op} imm;",
                    file=f,
                )
                print(f"}}", file=f)
        with open(f"{prefix}madd_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = b.{m}[i] * c.{m}[i] + a.{m}[i];",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}msub_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = - b.{m}[i] * c.{m}[i] + a.{m}[i];",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}bitclr_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = a.{m}[i] & (~((u{w})1 << (b.{m}[i] % {w})));",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}bitclri_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = a.{m}[i] & (~((u{w})1 << imm));",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}bitset_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = a.{m}[i] | ((u{w})1 << (b.{m}[i] % {w}));",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}bitseti_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = a.{m}[i] | ((u{w})1 << imm);",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}bitrev_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = a.{m}[i] ^ ((u{w})1 << (b.{m}[i] % {w}));",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}bitrevi_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = a.{m}[i] ^ ((u{w})1 << imm);",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}clo_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = clo(a.{m}[i]);",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}clz_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = clz(a.{m}[i]);",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}pcnt_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = popcount(a.{m}[i]);",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}extrins_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            mask = vlen // w - 1
            print(
                f"  dst.{m}[i] = (i == ((imm >> 4) & {mask})) ? b.{m}[imm & {mask}] : a.{m}[i];",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}ilvh_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = (i % 2 == 1) ? a.{m}[i / 2 + {vlen // 2 // w}] : b.{m}[i / 2 + {64 // w}];",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}ilvl_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = (i % 2 == 1) ? a.{m}[i / 2] : b.{m}[i / 2];",
                file=f,
            )
            print(f"}}", file=f)
        if prefix != "xv" or (width != "h" and width != "b"):
            # no __lasx_xvinsgr2vr_h or __lasx_xvinsgr2vr_b
            with open(f"{prefix}insgr2vr_{width}.h", "w") as f:
                print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = (i == imm) ? b : a.{m}[i];",
                    file=f,
                )
                print(f"}}", file=f)
        with open(f"{prefix}seq_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = (a.{m}[i] == b.{m}[i]) ? 0x{((1 << w) - 1):X} : 0;",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}seqi_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = ((s{w})a.{m}[i] == imm) ? 0x{((1 << w) - 1):X} : 0;",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}sll_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = a.{m}[i] << (b.{m}[i] & 0x{w-1:x});",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}slli_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = a.{m}[i] << imm;",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}srl_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = a.{m}[i] >> (b.{m}[i] & 0x{w-1:x});",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}srli_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = a.{m}[i] >> imm;",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}sra_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = ((s{w})a.{m}[i]) >> (b.{m}[i] & 0x{w-1:x});",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}srai_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = ((s{w})a.{m}[i]) >> imm;",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}rotr_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = (a.{m}[i] >> (b.{m}[i] & 0x{w-1:x})) | (a.{m}[i] << ({w} - (b.{m}[i] & 0x{w-1:x})));",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}rotri_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = (a.{m}[i] >> imm) | (a.{m}[i] << ({w} - imm));",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}srlr_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
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
        with open(f"{prefix}srlri_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
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
        with open(f"{prefix}srar_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
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
        with open(f"{prefix}srari_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
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
        with open(f"{prefix}packev_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = (i % 2 == 1) ? a.{m}[i-1] : b.{m}[i];",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}packod_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = (i % 2 == 1) ? a.{m}[i] : b.{m}[i+1];",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}pickev_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = (i < {vlen // 2 // w}) ? b.{m}[i * 2] : a.{m}[(i - {64 // w}) * 2];",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}pickod_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = (i < {vlen // 2 // w}) ? b.{m}[i * 2 + 1] : a.{m}[(i - {64 // w}) * 2 + 1];",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}replve_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = a.{m}[idx % {vlen // w}];",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}replvei_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = a.{m}[idx];",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}replgr2vr_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = val;",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}repli_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = imm;",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}signcov_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = (a.{m}[i] == 0) ? 0 : ((s{w})a.{m}[i] > 0 ? b.{m}[i] : -b.{m}[i]);",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}neg_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = -a.{m}[i];",
                file=f,
            )
            print(f"}}", file=f)
        if width != "d":
            with open(f"{prefix}shuf4i_{width}.h", "w") as f:
                print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = a.{m}[(i & ~0x3) + ((imm >> (2 * (i & 0x3))) & 0x3)];",
                    file=f,
                )
                print(f"}}", file=f)
        if width == "b" or width == "h":
            with open(f"{prefix}frstp_{width}.h", "w") as f:
                print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = a.{m}[i];",
                    file=f,
                )
                print(f"}}", file=f)
                print(f"int i;", file=f)
                print(f"for (i = 0;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  if ((s{w})b.{m}[i] < 0) {{",
                    file=f,
                )
                print(f"break;", file=f)
                print(f"}}", file=f)
                print(f"}}", file=f)
                print(f"  dst.{m}[c.{m}[0] % {vlen // w}] = i;", file=f)
            with open(f"{prefix}frstpi_{width}.h", "w") as f:
                print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = a.{m}[i];",
                    file=f,
                )
                print(f"}}", file=f)
                print(f"int i;", file=f)
                print(f"for (i = 0;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  if ((s{w})b.{m}[i] < 0) {{",
                    file=f,
                )
                print(f"break;", file=f)
                print(f"}}", file=f)
                print(f"}}", file=f)
                print(f"  dst.{m}[imm % {vlen // w}] = i;", file=f)
        with open(f"{prefix}stelm_{width}.h", "w") as f:
            print(f"memory_store({w}, data.{m}[lane], addr + offset);", file=f)

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
            with open(f"{prefix}f{name}_{width}.h", "w") as f:
                print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = a.{m}[i] {op} b.{m}[i];",
                    file=f,
                )
                print(f"}}", file=f)
        if width == "s":
            for rounding in ["", "rm", "rp", "rz", "rne"]:
                with open(f"{prefix}ftint{rounding}l_l_{width}.h", "w") as f:
                    print(f"for (int i = 0;i < {vlen // 2 // w};i++) {{", file=f)
                    print(
                        f"  dst.dword[i] = (s{int_w})a.{m}[i]; // rounding mode is not expressed in C",
                        file=f,
                    )
                    print(f"}}", file=f)
                with open(f"{prefix}ftint{rounding}h_l_{width}.h", "w") as f:
                    print(f"for (int i = 0;i < {vlen // 2 // w};i++) {{", file=f)
                    print(
                        f"  dst.dword[i] = (s{int_w})a.{m}[i + {vlen // 2 // w}]; // rounding mode is not expressed in C",
                        file=f,
                    )
                    print(f"}}", file=f)
        for rounding in ["", "rm", "rp", "rz", "rne"]:
            with open(f"{prefix}ftint{rounding}_{int_width}_{width}.h", "w") as f:
                print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{int_m}[i] = (s{int_w})a.{m}[i]; // rounding mode is not expressed in C",
                    file=f,
                )
                print(f"}}", file=f)
        for rounding in ["", "rz"]:
            with open(f"{prefix}ftint{rounding}_{int_width}u_{width}.h", "w") as f:
                print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{int_m}[i] = (u{int_w})a.{m}[i]; // rounding mode is not expressed in C",
                    file=f,
                )
                print(f"}}", file=f)
        if width == "d":
            for rounding in ["", "rm", "rp", "rz", "rne"]:
                with open(f"{prefix}ftint{rounding}_w_{width}.h", "w") as f:
                    print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                    print(
                        f"  dst.{int_m}[i] = (i < {vlen // 2 // w}) ? (s{int_w})a.{m}[i] : (s{int_w})b.{m}[i]; // rounding mode is not expressed in C",
                        file=f,
                    )
                    print(f"}}", file=f)
        with open(f"{prefix}ffint_{width}_{int_width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = (f{int_w})(s{int_w})a.{int_m}[i]; // rounding mode is not expressed in C",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}ffint_{width}_{int_width}u.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = (f{int_w})(u{int_w})a.{int_m}[i]; // rounding mode is not expressed in C",
                file=f,
            )
            print(f"}}", file=f)

        for name in ["max", "min"]:
            with open(f"{prefix}f{name}_{width}.h", "w") as f:
                print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = {op}(a.{m}[i], b.{m}[i]);",
                    file=f,
                )
                print(f"}}", file=f)
        with open(f"{prefix}fmaxa_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = (abs(a.{m}[i]) > abs(b.{m}[i])) ? a.{m}[i] : b.{m}[i];",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}fmina_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = (abs(a.{m}[i]) < abs(b.{m}[i])) ? a.{m}[i] : b.{m}[i];",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}flogb_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = log2(a.{m}[i]);",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}fsqrt_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = sqrt(a.{m}[i]);",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}frsqrt_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = 1.0 / sqrt(a.{m}[i]);",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}frecip_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = 1 / a.{m}[i];",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}frsqrte_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = 1.0 / sqrt(a.{m}[i]); // estimated",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}frecipe_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = 1 / a.{m}[i]; // estimated",
                file=f,
            )
            print(f"}}", file=f)
        for rounding in ["", "rm", "rp", "rz", "rne"]:
            with open(f"{prefix}frint{rounding}_{width}.h", "w") as f:
                print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = ({m})(s{int_w})a.{m}[i]; // rounding mode is not expressed in C",
                    file=f,
                )
                print(f"}}", file=f)
        for cond in ["af", "un", "eq", "ueq", "lt", "ult", "le", "ule", "ne", "or", "une"]:
            for signal in ["c", "s"]:
                with open(f"{prefix}fcmp_{signal}{cond}_{width}.h", "w") as f:
                    print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                    print(f"if (fp_compare_{signal}{cond}(a.{m}[i], b.{m}[i])) {{", file=f)
                    print(
                        f"  dst.{int_m}[i] = 0x{2 ** w - 1:X};",
                        file=f,
                    )
                    print(f"}} else {{", file=f)
                    print(
                        f"  dst.{int_m}[i] = 0;",
                        file=f,
                    )
                    print(f"}}", file=f)

os.system("clang-format -i *.cpp *.h")
