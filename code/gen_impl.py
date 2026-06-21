import glob
import os
import pycparser
import subprocess
from concurrent.futures import ThreadPoolExecutor
from pycparser import c_generator

os.chdir(os.path.dirname(os.path.abspath(__file__)))

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
                    f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})a.{m}[i + {vlen // double_w}];",
                    file=f,
                )
                print(f"}}", file=f)
            else:
                print(f"int i;", file=f)
                print(f"for (i = 0;i < {vlen // 2 // double_w};i++) {{", file=f)
                print(
                    f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})a.{m}[i + {vlen // 2 // double_w}];",
                    file=f,
                )
                print(f"}}", file=f)
                print(f"for (;i < {vlen // double_w};i++) {{", file=f)
                print(
                    f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})a.{m}[i + {vlen // double_w}];",
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
                if prefix == "v":
                    print(f"for (int i = 0;i < {vlen // double_w};i++) {{", file=f)
                    print(
                        f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})a.{m}[i] << imm;",
                        file=f,
                    )
                    print(f"}}", file=f)
                else:
                    print(f"for (int i = 0;i < {vlen // 2 // double_w};i++) {{", file=f)
                    print(
                        f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})a.{m}[i] << imm;",
                        file=f,
                    )
                    print(f"}}", file=f)
                    print(
                        f"for (int i = {vlen // 2 // double_w};i < {vlen // double_w};i++) {{",
                        file=f,
                    )
                    print(
                        f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})a.{m}[i + {vlen // 2 // double_w}] << imm;",
                        file=f,
                    )
                    print(f"}}", file=f)
            if sign == "s":
                for name, sign in [("srl", "u"), ("sra", "s")]:
                    with open(f"{prefix}{name}n_{width}_{double_width}.h", "w") as f:
                        if prefix == "v":
                            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                            print(
                                f"  dst.{m}[i] = (i < {vlen // 2 // w}) ? ({sign}{w})(({sign}{double_w})a.{double_m}[i] >> (b.{double_m}[i] & {double_w-1})) : 0;",
                                file=f,
                            )
                            print(f"}}", file=f)
                        else:
                            print(
                                f"for (int i = 0;i < {vlen // 2 // w};i++) {{", file=f
                            )
                            print(
                                f"  dst.{m}[i] = (i < {vlen // 4 // w}) ? ({sign}{w})(({sign}{double_w})a.{double_m}[i] >> (b.{double_m}[i] & {double_w-1})) : 0;",
                                file=f,
                            )
                            print(f"}}", file=f)
                            print(
                                f"for (int i = {vlen // 2 // w};i < {vlen // w};i++) {{",
                                file=f,
                            )
                            print(
                                f"  dst.{m}[i] = (i < {3 * vlen // 4 // w}) ? ({sign}{w})(({sign}{double_w})a.{double_m}[i - {vlen // 4 // w}] >> (b.{double_m}[i - {vlen // 4 // w}] & {double_w-1})) : 0;",
                                file=f,
                            )
                            print(f"}}", file=f)
                    with open(f"{prefix}{name}rn_{width}_{double_width}.h", "w") as f:
                        if prefix == "v":
                            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                            print(f"if (i < {vlen // 2 // w}) {{", file=f)
                            print(
                                f"u8 shift = (b.{double_m}[i] & {double_w-1});", file=f
                            )
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
                        else:
                            print(
                                f"for (int i = 0;i < {vlen // 2 // w};i++) {{", file=f
                            )
                            print(f"if (i < {vlen // 4 // w}) {{", file=f)
                            print(
                                f"u8 shift = (b.{double_m}[i] & {double_w-1});", file=f
                            )
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
                            print(
                                f"for (int i = {vlen // 2 // w};i < {vlen // w};i++) {{",
                                file=f,
                            )
                            print(f"if (i < {3 * vlen // 4 // w}) {{", file=f)
                            print(
                                f"u8 shift = (b.{double_m}[i - {vlen // 4 // w}] & {double_w-1});",
                                file=f,
                            )
                            print(f"if (shift == 0) {{", file=f)
                            print(
                                f"  dst.{m}[i] = ({sign}{w})({sign}{double_w})a.{double_m}[i - {vlen // 4 // w}];",
                                file=f,
                            )
                            print(f"}} else {{", file=f)
                            print(
                                f"  dst.{m}[i] = ({sign}{w})((({sign}{double_w})a.{double_m}[i - {vlen // 4 // w}] >> shift) + ((({sign}{double_w})a.{double_m}[i - {vlen // 4 // w}] >> (shift - 1)) & 0x1));",
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
                with open(
                    f"{prefix}s{name}n_{width}_{double_width_signed}.h", "w"
                ) as f:
                    if prefix == "v":
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
                    else:
                        print(f"for (int i = 0;i < {vlen // 2 // w};i++) {{", file=f)
                        print(f"if (i < {vlen // 4 // w}) {{", file=f)
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

                        print(
                            f"for (int i = {vlen // 2 // w};i < {vlen // w};i++) {{",
                            file=f,
                        )
                        print(f"if (i < {3 * vlen // 4 // w}) {{", file=f)
                        print(
                            f"  {shift_sign}{double_w} temp = ({shift_sign}{double_w})a.{double_m}[i - {vlen // 4 // w}] >> (b.{double_m}[i - {vlen // 4 // w}] & {double_w-1});",
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
                with open(
                    f"{prefix}s{name}rn_{width}_{double_width_signed}.h", "w"
                ) as f:
                    if prefix == "v":
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
                    else:
                        print(f"for (int i = 0;i < {vlen // 2 // w};i++) {{", file=f)
                        print(f"if (i < {vlen // 4 // w}) {{", file=f)
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

                        print(
                            f"for (int i = {vlen // 2 // w};i < {vlen // w};i++) {{",
                            file=f,
                        )
                        print(f"if (i < {3 * vlen // 4 // w}) {{", file=f)
                        print(f"{shift_sign}{double_w} temp;", file=f)
                        print(
                            f"if ((b.{double_m}[i - {vlen // 4 // w}] & {double_w-1}) == 0) {{",
                            file=f,
                        )
                        print(
                            f"  temp = ({shift_sign}{double_w})a.{double_m}[i - {vlen // 4 // w}];",
                            file=f,
                        )
                        print(f"}} else {{", file=f)
                        print(
                            f"  temp = (({shift_sign}{double_w})a.{double_m}[i - {vlen // 4 // w}] >> (b.{double_m}[i - {vlen // 4 // w}] & {double_w-1})) + ((({shift_sign}{double_w})a.{double_m}[i - {vlen // 4 // w}] >> ((b.{double_m}[i - {vlen // 4 // w}] & {double_w-1}) - 1)) & 1);",
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
                    if prefix == "v":
                        print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                        print(
                            f"  dst.{m}[i] = (i < {vlen // 2 // w}) ? ({sign}{w})(({sign}{double_w})b.{double_m}[i] >> imm) : ({sign}{w})(({sign}{double_w})a.{double_m}[i - {vlen // 2 // w}] >> imm);",
                            file=f,
                        )
                        print(f"}}", file=f)
                    else:
                        print(f"for (int i = 0;i < {vlen // 2 // w};i++) {{", file=f)
                        print(
                            f"  dst.{m}[i] = (i < {vlen // 4 // w}) ? ({sign}{w})(({sign}{double_w})b.{double_m}[i] >> imm) : ({sign}{w})(({sign}{double_w})a.{double_m}[i - {vlen // 4 // w}] >> imm);",
                            file=f,
                        )
                        print(f"}}", file=f)

                        print(
                            f"for (int i = {vlen // 2 // w};i < {vlen // w};i++) {{",
                            file=f,
                        )
                        print(
                            f"  dst.{m}[i] = (i < {3 * vlen // 4 // w}) ? ({sign}{w})(({sign}{double_w})b.{double_m}[i - {vlen // 4 // w}] >> imm) : ({sign}{w})(({sign}{double_w})a.{double_m}[i - {vlen // 2 // w}] >> imm);",
                            file=f,
                        )
                        print(f"}}", file=f)
                with open(f"{prefix}{name}rni_{width}_{double_width}.h", "w") as f:
                    if prefix == "v":
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
                    else:
                        print(f"for (int i = 0;i < {vlen // 2 // w};i++) {{", file=f)
                        print(f"if (i < {vlen // 4 // w}) {{", file=f)
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
                            f"  dst.{m}[i] = ({sign}{w})({sign}{double_w})a.{double_m}[i - {vlen // 4 // w}];",
                            file=f,
                        )
                        print(f"}} else {{", file=f)
                        print(
                            f"  dst.{m}[i] = ({sign}{w})((({sign}{double_w})a.{double_m}[i - {vlen // 4 // w}] >> imm) + ((({sign}{double_w})a.{double_m}[i - {vlen // 4 // w}] >> (imm - 1)) & 0x1));",
                            file=f,
                        )
                        print(f"}}", file=f)
                        print(f"}}", file=f)
                        print(f"}}", file=f)

                        print(
                            f"for (int i = {vlen // 2 // w};i < {vlen // w};i++) {{",
                            file=f,
                        )
                        print(f"if (i < {3 * vlen // 4 // w}) {{", file=f)
                        print(f"if (imm == 0) {{", file=f)
                        print(
                            f"  dst.{m}[i] = ({sign}{w})({sign}{double_w})b.{double_m}[i - {vlen // 4 // w}];",
                            file=f,
                        )
                        print(f"}} else {{", file=f)
                        print(
                            f"  dst.{m}[i] = ({sign}{w})((({sign}{double_w})b.{double_m}[i - {vlen // 4 // w}] >> imm) + ((({sign}{double_w})b.{double_m}[i - {vlen // 4 // w}] >> (imm - 1)) & 0x1));",
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
                if prefix == "v":
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
                else:
                    print(f"for (int i = 0;i < {vlen // 2 // w};i++) {{", file=f)
                    print(f"if (i < {vlen // 4 // w}) {{", file=f)
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
                        f"  {shift_sign}{double_w} temp = ({shift_sign}{double_w})a.{double_m}[i - {vlen // 4 // w}] >> imm;",
                        file=f,
                    )
                    print(
                        f"  dst.{m}[i] = clamp<{shift_sign}{double_w}>(temp, {min}, {max});",
                        file=f,
                    )
                    print(f"}}", file=f)
                    print(f"}}", file=f)

                    print(
                        f"for (int i = {vlen // 2 // w};i < {vlen // w};i++) {{", file=f
                    )
                    print(f"if (i < {3 * vlen // 4 // w}) {{", file=f)
                    print(
                        f"  {shift_sign}{double_w} temp = ({shift_sign}{double_w})b.{double_m}[i - {vlen // 4 // w}] >> imm;",
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
                if prefix == "v":
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
                else:
                    print(f"for (int i = 0;i < {vlen // 2 // w};i++) {{", file=f)
                    print(f"if (i < {vlen // 4 // w}) {{", file=f)
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
                        f"  temp = ({shift_sign}{double_w})a.{double_m}[i - {vlen // 4 // w}];",
                        file=f,
                    )
                    print(f"}} else {{", file=f)
                    print(
                        f"  temp = (({shift_sign}{double_w})a.{double_m}[i - {vlen // 4 // w}] >> imm) + ((({shift_sign}{double_w})a.{double_m}[i - {vlen // 4 // w}] >> (imm - 1)) & 1);",
                        file=f,
                    )
                    print(f"}}", file=f)
                    print(
                        f"  dst.{m}[i] = clamp<{shift_sign}{double_w}>(temp, {min}, {max});",
                        file=f,
                    )
                    print(f"}}", file=f)
                    print(f"}}", file=f)

                    print(
                        f"for (int i = {vlen // 2 // w};i < {vlen // w};i++) {{", file=f
                    )
                    print(f"if (i < {3 * vlen // 4 // w}) {{", file=f)
                    print(f"{shift_sign}{double_w} temp;", file=f)
                    print(f"if (imm == 0) {{", file=f)
                    print(
                        f"  temp = ({shift_sign}{double_w})b.{double_m}[i - {vlen // 4 // w}];",
                        file=f,
                    )
                    print(f"}} else {{", file=f)
                    print(
                        f"  temp = (({shift_sign}{double_w})b.{double_m}[i - {vlen // 4 // w}] >> imm) + ((({shift_sign}{double_w})b.{double_m}[i - {vlen // 4 // w}] >> (imm - 1)) & 1);",
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
        # skip xvpickve2gr_b/bu/h/hu
        if prefix != "xv" or (width[0] != "b" and width[0] != "h"):
            with open(f"{prefix}pickve2gr_{width}.h", "w") as f:
                print(f"dst = ({sign}{w})a.{m}[idx];", file=f)

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
                with open(
                    f"{prefix}{arith}wev_{double_width}_{width}{suffix}.h", "w"
                ) as f:
                    print(f"for (int i = 0;i < {vlen // double_w};i++) {{", file=f)
                    print(
                        f"  dst.{double_m}[i] = ({sign}{double_w})({sign}{w})a.{m}[2 * i] {op} ({sign2}{double_w})({sign2}{w})b.{m}[2 * i];",
                        file=f,
                    )
                    print(f"}}", file=f)
                with open(
                    f"{prefix}{arith}wod_{double_width}_{width}{suffix}.h", "w"
                ) as f:
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
            mask = 128 // w - 1  # not vlen
            if prefix == "v":
                print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = (i == ((imm >> 4) & {mask})) ? b.{m}[imm & {mask}] : a.{m}[i];",
                    file=f,
                )
                print(f"}}", file=f)
            else:
                print(f"int i;", file=f)
                print(f"for (i = 0;i < {vlen // 2 // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = (i == ((imm >> 4) & {mask})) ? b.{m}[imm & {mask}] : a.{m}[i];",
                    file=f,
                )
                print(f"}}", file=f)
                print(f"for (;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = (i - {vlen // 2 // w} == ((imm >> 4) & {mask})) ? b.{m}[(imm & {mask}) + {vlen // 2 // w}] : a.{m}[i];",
                    file=f,
                )
                print(f"}}", file=f)
        with open(f"{prefix}ilvh_{width}.h", "w") as f:
            if prefix == "v":
                print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = (i % 2 == 1) ? a.{m}[i / 2 + {vlen // 2 // w}] : b.{m}[i / 2 + {vlen // 2 // w}];",
                    file=f,
                )
                print(f"}}", file=f)
            else:
                print(f"int i;", file=f)
                print(f"for (i = 0;i < {vlen // 2 // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = (i % 2 == 1) ? a.{m}[i / 2 + {vlen // 4 // w}] : b.{m}[i / 2 + {vlen // 4 // w}];",
                    file=f,
                )
                print(f"}}", file=f)
                print(f"for (;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = (i % 2 == 1) ? a.{m}[i / 2 + {vlen // 2 // w}] : b.{m}[i / 2 + {vlen // 2 // w}];",
                    file=f,
                )
                print(f"}}", file=f)
        with open(f"{prefix}ilvl_{width}.h", "w") as f:
            if prefix == "v":
                print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = (i % 2 == 1) ? a.{m}[i / 2] : b.{m}[i / 2];",
                    file=f,
                )
                print(f"}}", file=f)
            else:
                print(f"int i;", file=f)
                print(f"for (i = 0;i < {vlen // 2 // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = (i % 2 == 1) ? a.{m}[i / 2] : b.{m}[i / 2];",
                    file=f,
                )
                print(f"}}", file=f)
                print(f"for (;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = (i % 2 == 1) ? a.{m}[i / 2 + {vlen // 4 // w}] : b.{m}[i / 2 + {vlen // 4 // w}];",
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
        if prefix == "xv" and (width == "d" or width == "w"):
            # xvinsve0_d/w
            with open(f"{prefix}insve0_{width}.h", "w") as f:
                print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = (i == imm) ? b.{m}[0] : a.{m}[i];",
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
            if prefix == "v":
                print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = (i < {vlen // 2 // w}) ? b.{m}[i * 2] : a.{m}[(i - {vlen // 2 // w}) * 2];",
                    file=f,
                )
                print(f"}}", file=f)
            else:
                print(f"for (int i = 0;i < {vlen // 2 // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = (i < {vlen // 4 // w}) ? b.{m}[i * 2] : a.{m}[(i - {vlen // 4 // w}) * 2];",
                    file=f,
                )
                print(f"}}", file=f)
                print(f"for (int i = {vlen // 2 // w};i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = (i < {3 * vlen // 4 // w}) ? b.{m}[(i - {vlen // 4 // w}) * 2] : a.{m}[(i - {vlen // 2 // w}) * 2];",
                    file=f,
                )
                print(f"}}", file=f)
        with open(f"{prefix}pickod_{width}.h", "w") as f:
            if prefix == "v":
                print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = (i < {vlen // 2 // w}) ? b.{m}[i * 2 + 1] : a.{m}[(i - {vlen // 2 // w}) * 2 + 1];",
                    file=f,
                )
                print(f"}}", file=f)
            else:
                print(f"for (int i = 0;i < {vlen // 2 // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = (i < {vlen // 4 // w}) ? b.{m}[i * 2 + 1] : a.{m}[(i - {vlen // 4 // w}) * 2 + 1];",
                    file=f,
                )
                print(f"}}", file=f)
                print(f"for (int i = {vlen // 2 // w};i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = (i < {3 * vlen // 4 // w}) ? b.{m}[(i - {vlen // 4 // w}) * 2 + 1] : a.{m}[(i - {vlen // 2 // w}) * 2 + 1];",
                    file=f,
                )
                print(f"}}", file=f)
        with open(f"{prefix}replve_{width}.h", "w") as f:
            mask = 128 // w  # not vlen
            if prefix == "v":
                print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = a.{m}[idx % {mask}];",
                    file=f,
                )
                print(f"}}", file=f)
            else:
                print(f"for (int i = 0;i < {vlen // 2 // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = a.{m}[idx % {mask}];",
                    file=f,
                )
                print(f"}}", file=f)
                print(f"for (int i = {vlen // 2 // w};i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = a.{m}[(idx % {mask}) + {vlen // 2 // w}];",
                    file=f,
                )
                print(f"}}", file=f)
        if prefix == "v":
            with open(f"{prefix}replvei_{width}.h", "w") as f:
                print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = a.{m}[idx];",
                    file=f,
                )
                print(f"}}", file=f)
        else:
            with open(f"{prefix}repl128vei_{width}.h", "w") as f:
                print(f"for (int i = 0;i < {vlen // 2 // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = a.{m}[idx];",
                    file=f,
                )
                print(f"}}", file=f)
                print(f"for (int i = {vlen // 2 // w};i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = a.{m}[idx + {vlen // 2 // w}];",
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
                if prefix == "v":
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
                else:
                    print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                    print(
                        f"  dst.{m}[i] = a.{m}[i];",
                        file=f,
                    )
                    print(f"}}", file=f)
                    print(f"int i;", file=f)
                    print(f"for (i = 0;i < {vlen // 2 // w};i++) {{", file=f)
                    print(
                        f"  if ((s{w})b.{m}[i] < 0) {{",
                        file=f,
                    )
                    print(f"break;", file=f)
                    print(f"}}", file=f)
                    print(f"}}", file=f)
                    mask = 128 // w  # not vlen
                    print(f"  dst.{m}[c.{m}[0] % {mask}] = i;", file=f)
                    print(f"for (i = {vlen // 2 // w};i < {vlen // w};i++) {{", file=f)
                    print(
                        f"  if ((s{w})b.{m}[i] < 0) {{",
                        file=f,
                    )
                    print(f"break;", file=f)
                    print(f"}}", file=f)
                    print(f"}}", file=f)
                    print(
                        f"  dst.{m}[(c.{m}[{vlen // 2 // w}] % {mask}) + {vlen // 2 // w}] = i - {vlen // 2 // w};",
                        file=f,
                    )
            with open(f"{prefix}frstpi_{width}.h", "w") as f:
                if prefix == "v":
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
                else:
                    print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                    print(
                        f"  dst.{m}[i] = a.{m}[i];",
                        file=f,
                    )
                    print(f"}}", file=f)
                    print(f"int i;", file=f)
                    print(f"for (i = 0;i < {vlen // 2 // w};i++) {{", file=f)
                    print(
                        f"  if ((s{w})b.{m}[i] < 0) {{",
                        file=f,
                    )
                    print(f"break;", file=f)
                    print(f"}}", file=f)
                    print(f"}}", file=f)
                    mask = 128 // w  # not vlen
                    print(f"  dst.{m}[imm % {mask}] = i;", file=f)
                    print(f"for (i = {vlen // 2 // w};i < {vlen // w};i++) {{", file=f)
                    print(
                        f"  if ((s{w})b.{m}[i] < 0) {{",
                        file=f,
                    )
                    print(f"break;", file=f)
                    print(f"}}", file=f)
                    print(f"}}", file=f)
                    print(
                        f"  dst.{m}[(imm % {mask}) + {vlen // 2 // w}] = i - {vlen // 2 // w};",
                        file=f,
                    )
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
                    if vlen == 128:
                        print(f"for (int i = 0;i < {vlen // 2 // w};i++) {{", file=f)
                        print(
                            f"  dst.dword[i] = (s64)a.{m}[i]; // rounding mode is not expressed in C",
                            file=f,
                        )
                        print(f"}}", file=f)
                    else:
                        print(
                            f"for (int i = 0;i < {vlen // 2 // w // 2};i++) {{", file=f
                        )
                        print(
                            f"  dst.dword[i] = (s64)a.{m}[i]; // rounding mode is not expressed in C",
                            file=f,
                        )
                        print(f"}}", file=f)
                        print(
                            f"for (int i = {vlen // 2 // w // 2};i < {vlen // 2 // w};i++) {{",
                            file=f,
                        )
                        print(
                            f"  dst.dword[i] = (s64)a.{m}[i + {vlen // 2 // w // 2}]; // rounding mode is not expressed in C",
                            file=f,
                        )
                        print(f"}}", file=f)
                with open(f"{prefix}ftint{rounding}h_l_{width}.h", "w") as f:
                    if vlen == 128:
                        print(f"for (int i = 0;i < {vlen // 2 // w};i++) {{", file=f)
                        print(
                            f"  dst.dword[i] = (s64)a.{m}[i + {vlen // 2 // w}]; // rounding mode is not expressed in C",
                            file=f,
                        )
                        print(f"}}", file=f)
                    else:
                        print(
                            f"for (int i = 0;i < {vlen // 2 // w // 2};i++) {{", file=f
                        )
                        print(
                            f"  dst.dword[i] = (s64)a.{m}[i + {vlen // 2 // w // 2}]; // rounding mode is not expressed in C",
                            file=f,
                        )
                        print(f"}}", file=f)
                        print(
                            f"for (int i = {vlen // 2 // w // 2};i < {vlen // 2 // w};i++) {{",
                            file=f,
                        )
                        print(
                            f"  dst.dword[i] = (s64)a.{m}[i + {vlen // 2 // w}]; // rounding mode is not expressed in C",
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
                    if prefix == "v":
                        print(f"for (int i = 0;i < {vlen // w * 2};i++) {{", file=f)
                        print(
                            f"  dst.word[i] = (i < {vlen // w}) ? (s32)b.{m}[i] : (s32)a.{m}[i - {vlen // w}]; // rounding mode is not expressed in C",
                            file=f,
                        )
                        print(f"}}", file=f)
                    else:
                        print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                        print(
                            f"  dst.word[i] = (i < {vlen // 2 // w}) ? (s32)b.{m}[i] : (s32)a.{m}[i - {vlen // 2 // w}]; // rounding mode is not expressed in C",
                            file=f,
                        )
                        print(f"}}", file=f)
                        print(
                            f"for (int i = {vlen // w};i < {vlen // w * 2};i++) {{",
                            file=f,
                        )
                        print(
                            f"  dst.word[i] = (i < {vlen // 2 // w + vlen // w}) ? (s32)b.{m}[i - {vlen // 2 // w}] : (s32)a.{m}[i - {vlen // w}]; // rounding mode is not expressed in C",
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
                    f"  dst.{m}[i] = f{name}(a.{m}[i], b.{m}[i]);",
                    file=f,
                )
                print(f"}}", file=f)
        with open(f"{prefix}fmax_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = fmax(a.{m}[i], b.{m}[i]);",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}fmin_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  dst.{m}[i] = fmin(a.{m}[i], b.{m}[i]);",
                file=f,
            )
            print(f"}}", file=f)
        with open(f"{prefix}fmaxa_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  if (abs(a.{m}[i]) > abs(b.{m}[i])) {{",
                file=f,
            )
            print(
                f"    dst.{m}[i] = a.{m}[i];",
                file=f,
            )
            print(
                f"  }} else if (abs(b.{m}[i]) > abs(a.{m}[i])) {{",
                file=f,
            )
            print(
                f"    dst.{m}[i] = b.{m}[i];",
                file=f,
            )
            print(
                f"  }} else {{",
                file=f,
            )
            print(
                f"    dst.{m}[i] = fmax(a.{m}[i], b.{m}[i]);",
                file=f,
            )
            print(f"  }}", file=f)
            print(f"}}", file=f)
        with open(f"{prefix}fmina_{width}.h", "w") as f:
            print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
            print(
                f"  if (abs(a.{m}[i]) < abs(b.{m}[i])) {{",
                file=f,
            )
            print(
                f"    dst.{m}[i] = a.{m}[i];",
                file=f,
            )
            print(
                f"  }} else if (abs(b.{m}[i]) < abs(a.{m}[i])) {{",
                file=f,
            )
            print(
                f"    dst.{m}[i] = b.{m}[i];",
                file=f,
            )
            print(
                f"  }} else {{",
                file=f,
            )
            print(
                f"    dst.{m}[i] = fmin(a.{m}[i], b.{m}[i]);",
                file=f,
            )
            print(f"  }}", file=f)
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
                    f"  dst.{m}[i] = (f{w})(s{int_w})a.{m}[i]; // rounding mode is not expressed in C",
                    file=f,
                )
                print(f"}}", file=f)
        for cond in [
            "af",
            "un",
            "eq",
            "ueq",
            "lt",
            "ult",
            "le",
            "ule",
            "ne",
            "or",
            "une",
        ]:
            for signal in ["c", "s"]:
                with open(f"{prefix}fcmp_{signal}{cond}_{width}.h", "w") as f:
                    print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                    print(
                        f"if (fp_compare_{signal}{cond}(a.{m}[i], b.{m}[i])) {{", file=f
                    )
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

        dest_width = {"s": "h", "d": "s"}[width]
        dest_m = {"s": "fp16", "d": "fp32"}[width]
        with open(f"{prefix}fcvt_{dest_width}_{width}.h", "w") as f:
            if prefix == "v":
                print("int i;", file=f)
                print(f"for (i = 0;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{dest_m}[i] = b.{m}[i];",
                    file=f,
                )
                print(f"}}", file=f)
                print(f"for (;i < {vlen // w * 2};i++) {{", file=f)
                print(
                    f"  dst.{dest_m}[i] = a.{m}[i - {vlen // w}];",
                    file=f,
                )
                print(f"}}", file=f)
            else:
                print("int i;", file=f)
                print(f"for (i = 0;i < {vlen // w // 2};i++) {{", file=f)
                print(
                    f"  dst.{dest_m}[i] = b.{m}[i];",
                    file=f,
                )
                print(f"}}", file=f)
                print(f"for (;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{dest_m}[i] = a.{m}[i - {vlen // w // 2}];",
                    file=f,
                )
                print(f"}}", file=f)
                print(f"for (;i < {vlen // w // 2 * 3};i++) {{", file=f)
                print(
                    f"  dst.{dest_m}[i] = b.{m}[i - {vlen // w // 2}];",
                    file=f,
                )
                print(f"}}", file=f)
                print(f"for (;i < {vlen // w * 2};i++) {{", file=f)
                print(
                    f"  dst.{dest_m}[i] = a.{m}[i - {vlen // w}];",
                    file=f,
                )
                print(f"}}", file=f)

        src_width = {"s": "h", "d": "s"}[width]
        src_m = {"s": "fp16", "d": "fp32"}[width]
        for half in ["h", "l"]:
            with open(f"{prefix}fcvt{half}_{width}_{src_width}.h", "w") as f:
                if prefix == "v":
                    print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                    if half == "h":
                        print(
                            f"  dst.{m}[i] = a.{src_m}[i + {vlen // w}];",
                            file=f,
                        )
                    else:
                        print(
                            f"  dst.{m}[i] = a.{src_m}[i];",
                            file=f,
                        )
                    print(f"}}", file=f)
                else:
                    print(f"int i;", file=f)
                    print(f"for (i = 0;i < {vlen // w // 2};i++) {{", file=f)
                    if half == "h":
                        print(
                            f"  dst.{m}[i] = a.{src_m}[i + {vlen // w // 2}];",
                            file=f,
                        )
                    else:
                        print(
                            f"  dst.{m}[i] = a.{src_m}[i];",
                            file=f,
                        )
                    print(f"}}", file=f)
                    print(f"for (;i < {vlen // w};i++) {{", file=f)
                    if half == "h":
                        print(
                            f"  dst.{m}[i] = a.{src_m}[i + {vlen // w}];",
                            file=f,
                        )
                    else:
                        print(
                            f"  dst.{m}[i] = a.{src_m}[i + {vlen // w // 2}];",
                            file=f,
                        )
                    print(f"}}", file=f)

    if prefix == "xv":
        # xvpickve
        for width in ["w", "w_f", "d", "d_f"]:
            w = widths[width[0]]
            m = members[width[0]]
            with open(f"{prefix}pickve_{width}.h", "w") as f:
                print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                print(f"  dst.{m}[i] = (i == 0) ? a.{m}[imm] : 0;", file=f)
                print(f"}}", file=f)
        # xvreplve0
        for width in ["b", "h", "w", "q", "d"]:
            w = widths[width]
            m = members[width]
            with open(f"{prefix}replve0_{width}.h", "w") as f:
                print(f"for (int i = 0;i < {vlen // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = a.{m}[0];",
                    file=f,
                )
                print(f"}}", file=f)
    if prefix == "v":
        # vext2xv
        for width, width2 in [
            ("h", "b"),
            ("w", "b"),
            ("d", "b"),
            ("w", "h"),
            ("d", "h"),
            ("d", "w"),
            ("hu", "bu"),
            ("wu", "bu"),
            ("du", "bu"),
            ("wu", "hu"),
            ("du", "hu"),
            ("du", "wu"),
        ]:
            w = widths[width]
            m = members[width]
            w2 = widths[width2]
            m2 = members[width2]
            if "u" in width:
                sign = "u"
            else:
                sign = "s"
            with open(f"{prefix}ext2xv_{width}_{width2}.h", "w") as f:
                print(f"for (int i = 0;i < {256 // w};i++) {{", file=f)
                print(
                    f"  dst.{m}[i] = ({sign}{w})({sign}{w2})a.{m2}[i];",
                    file=f,
                )
                print(f"}}", file=f)

# Scalar adc instructions (GPR, not SIMD)
adc_widths = {
    "b": (8, "uint8_t"),
    "h": (16, "uint16_t"),
    "w": (32, "uint32_t"),
    "d": (64, "uint64_t"),
}
for width_name, (bits, uint_type) in adc_widths.items():
    with open(f"adc_{width_name}.h", "w") as f:
        print(f"{uint_type} lhs = a;", file=f)
        print(f"{uint_type} rhs = b;", file=f)
        print(f"uint8_t cf = EFLAGS.CF;", file=f)
        print(f"{uint_type} r = lhs + rhs + cf;", file=f)
        if width_name == "d":
            print(f"dst = r;", file=f)
        else:
            print(f"dst = sext(r, {bits});", file=f)

# Scalar arm instructions (GPR, not SIMD)
arm_insts = {
    "armadd_w": """if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t lhs = (uint32_t)a;
  uint32_t rhs = (uint32_t)b;
  uint64_t wide = (uint64_t)lhs + rhs;
  uint32_t r = (uint32_t)wide;
  ARMFLAGS.C = (wide >> 32) & 1;
  ARMFLAGS.V = ((~(lhs ^ rhs)) & (lhs ^ r) & 0x80000000) != 0;
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}""",
    "armadc_w": """if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t lhs = (uint32_t)a;
  uint32_t rhs = (uint32_t)b;
  uint32_t carry_in = ARMFLAGS.C;
  uint64_t wide = (uint64_t)lhs + rhs + carry_in;
  uint32_t r = (uint32_t)wide;
  ARMFLAGS.C = (wide >> 32) & 1;
  ARMFLAGS.V = ((~(lhs ^ rhs)) & (lhs ^ r) & 0x80000000) != 0;
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}""",
    "armsub_w": """if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t lhs = (uint32_t)a;
  uint32_t rhs = (uint32_t)b;
  uint64_t subtrahend = (uint64_t)rhs;
  uint32_t r = (uint32_t)(lhs - subtrahend);
  ARMFLAGS.C = lhs < subtrahend;
  ARMFLAGS.V = ((lhs ^ rhs) & (lhs ^ r) & 0x80000000) != 0;
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}""",
    "armsbc_w": """if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t lhs = (uint32_t)a;
  uint32_t rhs = (uint32_t)b;
  uint32_t borrow_in = ARMFLAGS.C ? 0 : 1;
  uint64_t subtrahend = (uint64_t)rhs + borrow_in;
  uint32_t r = (uint32_t)(lhs - subtrahend);
  ARMFLAGS.C = lhs < subtrahend;
  ARMFLAGS.V = ((lhs ^ (uint32_t)subtrahend) & (lhs ^ r) & 0x80000000) != 0;
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}""",
    "armand_w": """if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t lhs = (uint32_t)a;
  uint32_t rhs = (uint32_t)b;
  uint32_t r = lhs & rhs;
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}""",
    "armor_w": """if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t lhs = (uint32_t)a;
  uint32_t rhs = (uint32_t)b;
  uint32_t r = lhs | rhs;
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}""",
    "armxor_w": """if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t lhs = (uint32_t)a;
  uint32_t rhs = (uint32_t)b;
  uint32_t r = lhs ^ rhs;
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}""",
    "armnot_w": """if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t r = ~(uint32_t)a;
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}""",
    "armmov_w": """if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t r = a;
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}""",
    "armmov_d": """if (arm_cond_holds(ARMFLAGS, cond)) {
  uint64_t r = a;
  ARMFLAGS.N = (r >> 63) & 1;
  ARMFLAGS.Z = r == 0;
}""",
    "armmove": """if (arm_cond_holds(ARMFLAGS, cond)) {
  dst = a;
}""",
    "armrrx_w": """if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t r = ((uint32_t)a >> 1) | (ARMFLAGS.C ? 0x80000000 : 0);
  ARMFLAGS.C = a & 1;
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}""",
    "armrotr_w": """if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t lhs = (uint32_t)a;
  uint32_t rhs = (uint32_t)b;
  uint32_t raw_count = rhs & 0xff;
  uint32_t c = rhs & 0x1f;
  unsigned n = c % 32;
  uint32_t r = n == 0 ? lhs : (lhs >> n) | (lhs << (32 - n));
  if (raw_count != 0) {
    ARMFLAGS.C = n == 0 ? ((lhs >> 31) & 1) : ((lhs >> (n - 1)) & 1);
  }
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}""",
    "armrotri_w": """if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t v = (uint32_t)a;
  uint32_t r = imm == 0 ? v : (v >> imm) | (v << (32 - imm));
  if (imm != 0) {
    ARMFLAGS.C = (v >> (imm - 1)) & 1;
  }
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}""",
    "armsll_w": """if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t lhs = (uint32_t)a;
  uint32_t c = (uint32_t)b & 0xff;
  uint32_t r;
  if (c == 0) {
    r = lhs;
  } else if (c < 32) {
    ARMFLAGS.C = (lhs >> (32 - c)) & 1;
    r = lhs << c;
  } else if (c == 32) {
    ARMFLAGS.C = lhs & 1;
    r = 0;
  } else {
    ARMFLAGS.C = 0;
    r = 0;
  }
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}""",
    "armslli_w": """if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t v = (uint32_t)a;
  uint32_t r = v << imm;
  ARMFLAGS.C = imm == 0 ? ARMFLAGS.C : (imm > 32 ? 0 : ((v >> (32 - imm)) & 1));
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}""",
    "armsra_w": """if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t lhs = (uint32_t)a;
  uint32_t rhs = (uint32_t)b;
  uint32_t r;
  uint32_t c = rhs & 0xff;
  if (c == 0) {
    r = lhs;
  } else if (c < 32) {
    r = (uint32_t)((int32_t)lhs >> c);
    ARMFLAGS.C = (lhs >> (c - 1)) & 1;
  } else {
    ARMFLAGS.C = (lhs >> 31) & 1;
    r = ARMFLAGS.C ? UINT32_MAX : 0;
  }
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}""",
    "armsrai_w": """if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t v = (uint32_t)a;
  uint32_t r = (uint32_t)((int32_t)v >> imm);
  ARMFLAGS.C = imm == 0 ? ARMFLAGS.C : (imm > 32 ? 0 : ((v >> (imm - 1)) & 1));
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}""",
    "armsrl_w": """if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t lhs = (uint32_t)a;
  uint32_t rhs = (uint32_t)b;
  uint32_t r;
  uint32_t c = rhs & 0xff;
  if (c == 0) {
    r = lhs;
  } else if (c < 32) {
    r = lhs >> c;
    ARMFLAGS.C = (lhs >> (c - 1)) & 1;
  } else if (c == 32) {
    r = 0;
    ARMFLAGS.C = (lhs >> 31) & 1;
  } else {
    r = 0;
    ARMFLAGS.C = 0;
  }
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}""",
    "armsrli_w": """if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t v = (uint32_t)a;
  uint32_t r = v >> imm;
  ARMFLAGS.C = imm == 0 ? ARMFLAGS.C : (imm > 32 ? 0 : ((v >> (imm - 1)) & 1));
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}""",
    "armmfflag": """dst = 0;
if (mask & (1 << 4))
  dst |= (uint64_t)ARMFLAGS.N << 31;
if (mask & (1 << 3))
  dst |= (uint64_t)ARMFLAGS.Z << 30;
if (mask & (1 << 0))
  dst |= (uint64_t)ARMFLAGS.C << 29;
if (mask & (1 << 5))
  dst |= (uint64_t)ARMFLAGS.V << 28;
dst = sext(dst, 32);""",
    "armmtflag": """uint64_t v = a;
if (mask & (1 << 4))
  ARMFLAGS.N = (v >> 31) & 1;
if (mask & (1 << 3))
  ARMFLAGS.Z = (v >> 30) & 1;
if (mask & (1 << 0))
  ARMFLAGS.C = (v >> 29) & 1;
if (mask & (1 << 5))
  ARMFLAGS.V = (v >> 28) & 1;""",
}
for inst_name, body in arm_insts.items():
    with open(f"{inst_name}.h", "w") as f:
        f.write(body)
        f.write("\n")

# Scalar x86 instructions (GPR, not SIMD)
x86_w = {
    "b": (8, "uint8_t", "int8_t", "UINT8_MAX", "INT8_MIN", "INT8_MAX", "0x80"),
    "h": (16, "uint16_t", "int16_t", "UINT16_MAX", "INT16_MIN", "INT16_MAX", "0x8000"),
    "w": (
        32,
        "uint32_t",
        "int32_t",
        "UINT32_MAX",
        "INT32_MIN",
        "INT32_MAX",
        "0x80000000",
    ),
    "d": (
        64,
        "uint64_t",
        "int64_t",
        "UINT64_MAX",
        "INT64_MIN",
        "INT64_MAX",
        "0x8000000000000000ULL",
    ),
}

for width, (bits, utype, stype, umax, smin, smax, msb) in x86_w.items():
    for base, cf_line_d, cf_line_rest in [
        (
            "add",
            "EFLAGS.CF = lhs > UINT64_MAX - rhs;",
            "EFLAGS.CF = (uint64_t)lhs + (uint64_t)rhs > %s;",
        ),
        (
            "adc",
            "EFLAGS.CF = lhs > UINT64_MAX - rhs - carry_in;",
            "EFLAGS.CF = (uint64_t)lhs + rhs + carry_in > %s;",
        ),
    ]:
        with open(f"x86{base}_{width}.h", "w") as f:
            print(f"{utype} lhs = ({utype})a;", file=f)
            print(f"{utype} rhs = ({utype})b;", file=f)
            if base == "adc":
                print(f"uint8_t carry_in = EFLAGS.CF;", file=f)
            print(
                f"{utype} result = lhs + rhs{ ' + carry_in' if base == 'adc' else ''};",
                file=f,
            )
            if width == "d":
                print(cf_line_d, file=f)
            else:
                print((cf_line_rest % umax), file=f)
            print(f"EFLAGS.AF = ((lhs ^ rhs ^ result) & 0x10) != 0;", file=f)
            print(
                f"EFLAGS.OF = ((~(lhs ^ rhs)) & (lhs ^ result) & {msb}) != 0;", file=f
            )
            print(f"EFLAGS.PF = parity_even((uint8_t)result);", file=f)
            print(f"EFLAGS.ZF = result == 0;", file=f)
            print(f"EFLAGS.SF = ({stype})result < 0;", file=f)

    # x86add_wu/du (unsigned add, OF=0)
    if width in ("w", "d"):
        with open(f"x86add_{width}u.h", "w") as f:
            print(f"{utype} lhs = ({utype})a;", file=f)
            print(f"{utype} rhs = ({utype})b;", file=f)
            print(f"{utype} result = lhs + rhs;", file=f)
            if width == "d":
                print("EFLAGS.CF = lhs > UINT64_MAX - rhs;", file=f)
            else:
                print(f"EFLAGS.CF = (uint64_t)lhs + (uint64_t)rhs > {umax};", file=f)
            print(f"EFLAGS.AF = ((lhs ^ rhs ^ result) & 0x10) != 0;", file=f)
            print(f"EFLAGS.OF = 0;", file=f)
            print(f"EFLAGS.PF = parity_even((uint8_t)result);", file=f)
            print(f"EFLAGS.ZF = result == 0;", file=f)
            print(f"EFLAGS.SF = ({stype})result < 0;", file=f)

    # x86and
    with open(f"x86and_{width}.h", "w") as f:
        print(f"{utype} lhs = ({utype})a;", file=f)
        print(f"{utype} rhs = ({utype})b;", file=f)
        print(f"{utype} result = lhs & rhs;", file=f)
        print(f"EFLAGS.CF = 0;", file=f)
        print(f"EFLAGS.OF = 0;", file=f)
        print(f"EFLAGS.AF = 0;", file=f)
        print(f"EFLAGS.PF = parity_even((uint8_t)result);", file=f)
        print(f"EFLAGS.ZF = result == 0;", file=f)
        print(f"EFLAGS.SF = ({stype})result < 0;", file=f)

    # x86dec
    with open(f"x86dec_{width}.h", "w") as f:
        print(f"{utype} v = ({utype})a;", file=f)
        print(f"{utype} r = v - 1;", file=f)
        print(f"// CF preserved from input EFLAGS.CF", file=f)
        print(f"EFLAGS.PF = parity_even((uint8_t)r);", file=f)
        print(f"EFLAGS.AF = ((v ^ 1 ^ r) & 0x10) != 0;", file=f)
        print(f"EFLAGS.ZF = r == 0;", file=f)
        print(f"EFLAGS.SF = ({stype})r < 0;", file=f)
        print(f"EFLAGS.OF = v == ({utype}){smin};", file=f)

    # x86inc
    with open(f"x86inc_{width}.h", "w") as f:
        print(f"{utype} v = ({utype})a;", file=f)
        print(f"{utype} r = v + 1;", file=f)
        print(f"// CF preserved from input EFLAGS.CF", file=f)
        print(f"EFLAGS.PF = parity_even((uint8_t)r);", file=f)
        print(f"EFLAGS.AF = ((v ^ 1 ^ r) & 0x10) != 0;", file=f)
        print(f"EFLAGS.ZF = r == 0;", file=f)
        print(f"EFLAGS.SF = ({stype})r < 0;", file=f)
        print(f"EFLAGS.OF = v == ({utype}){smax};", file=f)

    # x86mul (signed)
    with open(f"x86mul_{width}.h", "w") as f:
        print(f"{utype} lhs = ({utype})a;", file=f)
        print(f"{utype} rhs = ({utype})b;", file=f)
        print(
            f"__int128 product = (__int128)({stype})lhs * (__int128)({stype})rhs;",
            file=f,
        )
        print(
            f"bool overflow = product < (__int128){smin} || product > (__int128){smax};",
            file=f,
        )
        print(f"EFLAGS.CF = overflow;", file=f)
        print(f"EFLAGS.OF = overflow;", file=f)
        print(f"EFLAGS.SF = 0;", file=f)
        print(f"EFLAGS.ZF = 0;", file=f)
        print(f"EFLAGS.AF = 0;", file=f)
        print(f"EFLAGS.PF = 0;", file=f)

    # x86mul_{width}u (unsigned)
    with open(f"x86mul_{width}u.h", "w") as f:
        print(f"{utype} lhs = ({utype})a;", file=f)
        print(f"{utype} rhs = ({utype})b;", file=f)
        print(f"unsigned __int128 product =", file=f)
        print(
            f"    (unsigned __int128)({utype})lhs * (unsigned __int128)({utype})rhs;",
            file=f,
        )
        print(f"bool overflow = (product >> {bits}) != 0;", file=f)
        print(f"EFLAGS.CF = overflow;", file=f)
        print(f"EFLAGS.OF = overflow;", file=f)
        print(f"EFLAGS.SF = 0;", file=f)
        print(f"EFLAGS.ZF = 0;", file=f)
        print(f"EFLAGS.AF = 0;", file=f)
        print(f"EFLAGS.PF = 0;", file=f)

# Scalar x86 mfflag/mtflag (GPR, not SIMD)
x86_insts_special = {
    "x86mfflag": """dst = 0;
if (mask & (1 << 0))
  dst |= (uint64_t)EFLAGS.CF << 0;
if (mask & (1 << 1))
  dst |= (uint64_t)EFLAGS.PF << 2;
if (mask & (1 << 2))
  dst |= (uint64_t)EFLAGS.AF << 4;
if (mask & (1 << 3))
  dst |= (uint64_t)EFLAGS.ZF << 6;
if (mask & (1 << 4))
  dst |= (uint64_t)EFLAGS.SF << 7;
if (mask & (1 << 5))
  dst |= (uint64_t)EFLAGS.OF << 11;""",
    "x86mtflag": """uint64_t v = a;
if (mask & (1 << 0))
  EFLAGS.CF = (v >> 0) & 1;
if (mask & (1 << 1))
  EFLAGS.PF = (v >> 2) & 1;
if (mask & (1 << 2))
  EFLAGS.AF = (v >> 4) & 1;
if (mask & (1 << 3))
  EFLAGS.ZF = (v >> 6) & 1;
if (mask & (1 << 4))
  EFLAGS.SF = (v >> 7) & 1;
if (mask & (1 << 5))
  EFLAGS.OF = (v >> 11) & 1;""",
}
for inst_name, body in x86_insts_special.items():
    with open(f"{inst_name}.h", "w") as f:
        f.write(body)
        f.write("\n")

# Scalar addu12i instructions (GPR, not SIMD)
for width_name in ["d", "w"]:
    with open(f"addu12i_{width_name}.h", "w") as f:
        if width_name == "d":
            print(f"int64_t offset = (int64_t)sext(imm & 0x1f, 5) << 12;", file=f)
            print(f"dst = a + (uint64_t)offset;", file=f)
        else:
            print(f"int64_t offset = (int64_t)sext(imm & 0x1f, 5) << 12;", file=f)
            print(f"dst = sext((uint32_t)a + (uint32_t)offset, 32);", file=f)


# expand i to known value
def evaluate(ast, i):
    if isinstance(ast, pycparser.c_ast.Assignment):
        return pycparser.c_ast.Assignment(
            op=ast.op, lvalue=evaluate(ast.lvalue, i), rvalue=evaluate(ast.rvalue, i)
        )
    elif isinstance(ast, pycparser.c_ast.ArrayRef):
        return pycparser.c_ast.ArrayRef(
            name=ast.name, subscript=evaluate(ast.subscript, i)
        )
    elif isinstance(ast, pycparser.c_ast.ID):
        if ast.name == "i":
            return pycparser.c_ast.Constant(type="int", value=str(i))
    elif isinstance(ast, pycparser.c_ast.UnaryOp):
        op = ast.op
        expr = evaluate(ast.expr, i)
        # see if we can expand right now
        if isinstance(expr, pycparser.c_ast.Constant):
            if op == "~":
                return pycparser.c_ast.Constant(
                    type="int", value=str(~eval(expr.value))
                )
        return pycparser.c_ast.UnaryOp(op=ast.op, expr=expr)
    elif isinstance(ast, pycparser.c_ast.BinaryOp):
        op = ast.op
        left = evaluate(ast.left, i)
        right = evaluate(ast.right, i)
        # see if we can expand right now
        if isinstance(left, pycparser.c_ast.Constant) and isinstance(
            right, pycparser.c_ast.Constant
        ):
            if op == "<":
                return pycparser.c_ast.Constant(
                    type="bool", value=str(int(left.value) < int(right.value))
                )
            elif op == "==":
                return pycparser.c_ast.Constant(
                    type="bool", value=str(int(left.value) == int(right.value))
                )
            elif op == "+":
                return pycparser.c_ast.Constant(
                    type="int", value=str(int(left.value) + int(right.value))
                )
            elif op == "-":
                return pycparser.c_ast.Constant(
                    type="int", value=str(int(left.value) - int(right.value))
                )
            elif op == "*":
                return pycparser.c_ast.Constant(
                    type="int", value=str(int(left.value) * int(right.value))
                )
            elif op == "/":
                return pycparser.c_ast.Constant(
                    type="int", value=str(int(left.value) // int(right.value))
                )
            elif op == "%":
                return pycparser.c_ast.Constant(
                    type="int", value=str(int(left.value) % int(right.value))
                )
            elif op == "&":
                return pycparser.c_ast.Constant(
                    type="int", value=str(eval(left.value) & eval(right.value))
                )
        return pycparser.c_ast.BinaryOp(op=ast.op, left=left, right=right)
    elif isinstance(ast, pycparser.c_ast.TernaryOp):
        cond = evaluate(ast.cond, i)
        iftrue = evaluate(ast.iftrue, i)
        iffalse = evaluate(ast.iffalse, i)
        if isinstance(cond, pycparser.c_ast.Constant):
            if cond.type == "bool" and cond.value == "True":
                return iftrue
            elif cond.type == "bool" and cond.value == "False":
                return iffalse
        return pycparser.c_ast.TernaryOp(cond=cond, iftrue=iftrue, iffalse=iffalse)
    elif isinstance(ast, pycparser.c_ast.Cast):
        expr = evaluate(ast.expr, i)
        return pycparser.c_ast.Cast(to_type=ast.to_type, expr=expr)

    return ast


# attempt to expand loops
for file in glob.glob("*.h"):
    if (
        "pick" not in file
        and "pack" not in file
        and "vilv" not in file
        and "wev_" not in file
        and "wod_" not in file
        and "addw" not in file
        and "subw" not in file
        and "exth" not in file
        and "extl" not in file
        and "extrins" not in file
        and "vshuf4i" not in file
        and "fcvt" not in file
        and "xvsllwil" not in file
        and "xvftint" not in file
        and "xvffint" not in file
        and ("ftint" not in file or "_w_d" not in file)
    ):
        continue
    orig = open(file, "r", encoding="utf-8").read()
    content = (
        """
        typedef signed char s8;
        typedef unsigned char u8;
        typedef signed short s16;
        typedef unsigned short u16;
        typedef signed int s32;
        typedef unsigned int u32;
        typedef signed long long s64;
        typedef unsigned long long u64;
        typedef __int128 s128;
        typedef unsigned __int128 u128;
        typedef float f32;
        typedef double f64;
        """
        + "void test() {"
        + subprocess.check_output(
            ["cpp", "-"], stdin=open(file, "r", encoding="utf-8"), encoding="utf-8"
        )
        + "\n}"
    )
    try:
        parser = pycparser.CParser()
        parsed = parser.parse(content, "test.c")
    except pycparser.plyparser.ParseError as err:
        continue
    expanded = []
    output_content = orig
    # strip existing expanded code
    if "// Expands to:" in output_content:
        output_content = output_content[: output_content.find("// Expands to:")]
    for item in parsed.ext[-1].body:
        if isinstance(item, pycparser.c_ast.For):
            # print("Got For", item)

            # extract range_from
            if isinstance(item.init, pycparser.c_ast.DeclList):
                if item.init.decls[0].name != "i":
                    continue
                range_from = int(item.init.decls[0].init.value)
            elif isinstance(item.init, pycparser.c_ast.Assignment):
                if item.init.lvalue.name != "i":
                    continue
                range_from = int(item.init.rvalue.value)
            elif item.init is None:
                # inherit i from last loop
                range_from = range_to
            else:
                continue

            # extract range_to
            if item.cond.op != "<" or item.cond.left.name != "i":
                continue
            range_to = int(item.cond.right.value)

            for i in range(range_from, range_to):
                # expand i in body
                for stmt in item.stmt:
                    # replace [i] with actual value
                    new_stmt = evaluate(stmt, i)
                    expanded.append(c_generator.CGenerator().visit(new_stmt))

    # add comments
    if len(expanded) > 0:
        output_content += "\n// Expands to:\n"
        output_content += "\nif (0) {\n"
        for expand in expanded:
            output_content += expand + ";\n"
        output_content += "\n}\n"

    open(file, "w", encoding="utf-8").write(output_content)

files = glob.glob("*.cpp") + glob.glob("*.h")
with ThreadPoolExecutor() as pool:
    for f in files:
        pool.submit(subprocess.run, ["clang-format", "-i", f])
