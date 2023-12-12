import os

widths_signed = ["b", "h", "w", "d"]
widths_all = ["b", "bu", "h", "hu", "w", "wu", "d", "du"]
widths_vaddw = [
    "h_b",
    "h_bu",
    "h_bu_b",
    "w_h",
    "w_hu",
    "w_hu_h",
    "d_w",
    "d_wu",
    "d_wu_w",
    "q_d",
    "q_du",
    "q_du_d",
]

tb = {
    # widths, args, extra args for imm
    "vavg": (widths_all, "v128 a, v128 b"),
    "vavgr": (widths_all, "v128 a, v128 b"),
    "vaddwev": (widths_vaddw, "v128 a, v128 b"),
    "vaddwod": (widths_vaddw, "v128 a, v128 b"),
    "vbitclr": (widths_signed, "v128 a, v128 b"),
    "vbitclri": (widths_signed, "v128 a, int imm", [0, 3, 7]),
    "vbitset": (widths_signed, "v128 a, v128 b"),
    "vbitseti": (widths_signed, "v128 a, int imm", [0, 3, 7]),
    "vbitrev": (widths_signed, "v128 a, v128 b"),
    "vbitrevi": (widths_signed, "v128 a, int imm", [0, 3, 7]),
    "vclo": (widths_signed, "v128 a"),
    "vclz": (widths_signed, "v128 a"),
}

for name in tb:
    t = tb[name]
    widths = t[0]
    args = t[1]

    for width in widths:
        inst_name = name + "_" + width

        fuzz_args = 0
        for arg in args.split(", "):
            if "v128" in arg:
                fuzz_args += 1

        print(f"Saving {inst_name}.cpp")
        with open(f"{inst_name}.cpp", "w") as f:
            print('#include "common.h"', file=f)
            print("", file=f)
            print(f"v128 {inst_name}({args}) {{", file=f)
            print("  v128 dst;", file=f)
            print(f'#include "{inst_name}.h"', file=f)
            print("  return dst;", file=f)
            print("}", file=f)
            print("", file=f)
            print("void test() {", file=f)
            if len(t) >= 3:
                for imm in t[2]:
                    print(f"  FUZZ{fuzz_args}({inst_name}, {imm});", file=f)
            else:
                print(f"  FUZZ{fuzz_args}({inst_name});", file=f)
            print("}", file=f)

os.system("clang-format -i *.cpp *.h")