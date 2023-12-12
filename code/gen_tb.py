import os

widths_signed = ["b", "h", "w", "d"]
widths_fp = ["s", "d"]
widths_all = ["b", "bu", "h", "hu", "w", "wu", "d", "du"]
widths_vexth = ["h_b", "hu_bu", "w_h", "wu_hu", "d_w", "du_wu", "q_d", "qu_du"]
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
widths_vsubw = [
    "h_b",
    "h_bu",
    "w_h",
    "w_hu",
    "d_w",
    "d_wu",
    "q_d",
    "q_du",
]

tb = {
    # widths, args, extra args for imm
    "vadd": (widths_signed, "v128 a, v128 b"),
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
    "vdiv": (widths_all, "v128 a, v128 b"),
    "vexth": (widths_vexth, "v128 a"),
    "vextl": (["q_d", "qu_du"], "v128 a"),
    "vextrins": (
        widths_signed,
        "v128 a, v128 b, int imm",
        [0, 3, 7, 15, 16, 32, 64, 128, 255],
    ),
    "vhaddw": (widths_vexth, "v128 a, v128 b"),
    "vhsubw": (widths_vexth, "v128 a, v128 b"),
    "vilvh": (widths_signed, "v128 a, v128 b"),
    "vilvl": (widths_signed, "v128 a, v128 b"),
    "vinsgr2vr": (
        widths_signed,
        "v128 a, long int b, int imm",
        ["0, 0", "1234, 1", "5678, 1"],
    ),
    "vmadd": (widths_signed, "v128 a, v128 b, v128 c"),
    "vmaddwev": (widths_vaddw, "v128 a, v128 b, v128 c"),
    "vmaddwod": (widths_vaddw, "v128 a, v128 b, v128 c"),
    "vmax": (widths_all, "v128 a, v128 b"),
    "vmaxi": (widths_all, "v128 a, int imm", [0, 3, 15]),
    "vmin": (widths_all, "v128 a, v128 b"),
    "vmini": (widths_all, "v128 a, int imm", [0, 3, 15]),
    "vmod": (widths_all, "v128 a, v128 b"),
    "vmsub": (widths_signed, "v128 a, v128 b, v128 c"),
    "vmuh": (widths_all, "v128 a, v128 b"),
    "vmul": (widths_signed, "v128 a, v128 b"),
    "vmulwev": (widths_vaddw, "v128 a, v128 b"),
    "vmulwod": (widths_vaddw, "v128 a, v128 b"),
    "vpackev": (widths_signed, "v128 a, v128 b"),
    "vpackod": (widths_signed, "v128 a, v128 b"),
    "vpcnt": (widths_signed, "v128 a"),
    "vpickev": (widths_signed, "v128 a, v128 b"),
    "vpickod": (widths_signed, "v128 a, v128 b"),
    "vrotr": (widths_signed, "v128 a, v128 b"),
    "vreplve": (widths_signed, "v128 a, int idx", [0, 1]),
    "vreplvei": (widths_signed, "v128 a, int idx", [0, 1]),
    "vsadd": (widths_all, "v128 a, v128 b"),
    "vssub": (widths_all, "v128 a, v128 b"),
    "vseq": (widths_signed, "v128 a, v128 b"),
    "vsll": (widths_signed, "v128 a, v128 b"),
    "vslt": (widths_all, "v128 a, v128 b"),
    "vsle": (widths_all, "v128 a, v128 b"),
    "vsrl": (widths_signed, "v128 a, v128 b"),
    "vsrlr": (widths_signed, "v128 a, v128 b"),
    "vsra": (widths_signed, "v128 a, v128 b"),
    "vsrar": (widths_signed, "v128 a, v128 b"),
    "vsub": (widths_signed, "v128 a, v128 b"),
    "vsubwev": (widths_vsubw, "v128 a, v128 b"),
    "vsubwod": (widths_vsubw, "v128 a, v128 b"),
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
