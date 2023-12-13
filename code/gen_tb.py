import os

widths_signed = ["b", "h", "w", "d"]
widths_unsigned = ["bu", "hu", "wu", "du"]
widths_fp = ["s", "d"]
widths_all = ["b", "bu", "h", "hu", "w", "wu", "d", "du"]
widths_vexth = ["h_b", "hu_bu", "w_h", "wu_hu", "d_w", "du_wu", "q_d", "qu_du"]
widths_vsllwil = ["h_b", "hu_bu", "w_h", "wu_hu", "d_w", "du_wu"]
widths_vsrln = ["b_h", "h_w", "w_d"]
widths_vssrln = ["b_h", "bu_h", "h_w", "hu_w", "w_d", "wu_d"]
widths_vsrlni = ["b_h", "h_w", "w_d", "d_q"]
widths_vssrlni = ["b_h", "bu_h", "h_w", "hu_w", "w_d", "wu_d", "d_q", "du_q"]
widths_vfrstp = ["b", "h"]
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
    "vaddi": (widths_unsigned, "v128 a, int imm", [0, 31]),
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
    "vfrstp": (widths_vfrstp, "v128 a, v128 b, v128 c"),
    "vfrstpi": (widths_vfrstp, "v128 a, v128 b, int imm", [0, 4, 31]),
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
    "vmskltz": (widths_signed, "v128 a"),
    "vmskgez": (["b"], "v128 a"),
    "vmsknz": (["b"], "v128 a"),
    "vmuh": (widths_all, "v128 a, v128 b"),
    "vmul": (widths_signed, "v128 a, v128 b"),
    "vmulwev": (widths_vaddw, "v128 a, v128 b"),
    "vmulwod": (widths_vaddw, "v128 a, v128 b"),
    "vneg": (widths_signed, "v128 a"),
    "vpackev": (widths_signed, "v128 a, v128 b"),
    "vpackod": (widths_signed, "v128 a, v128 b"),
    "vpcnt": (widths_signed, "v128 a"),
    "vpickev": (widths_signed, "v128 a, v128 b"),
    "vpickod": (widths_signed, "v128 a, v128 b"),
    "vrotr": (widths_signed, "v128 a, v128 b"),
    "vrotri": (widths_signed, "v128 a, int imm", [0, 7]),
    "vrepli": (widths_signed, "int imm", [0, 1, 511]),
    "vreplve": (widths_signed, "v128 a, int idx", [0, 1]),
    "vreplvei": (widths_signed, "v128 a, int idx", [0, 1]),
    "vreplgr2vr": (widths_signed, "int val", [0, 1, 256]),
    "vsadd": (widths_all, "v128 a, v128 b"),
    "vsat": (widths_all, "v128 a, int imm", [0, 7]),
    "vseq": (widths_signed, "v128 a, v128 b"),
    "vseqi": (widths_signed, "v128 a, int imm", [-16, 0, 15]),
    "vshuf4i": (["b", "h", "w"], "v128 a, int imm", [0, 13, 100, 128, 255]),
    "vsigncov": (widths_signed, "v128 a, v128 b"),
    "vsllwil": (widths_vsllwil, "v128 a, int imm", [0, 7]),
    "vssub": (widths_all, "v128 a, v128 b"),
    "vsub": (widths_signed, "v128 a, v128 b"),
    "vsubi": (widths_unsigned, "v128 a, int imm", [0, 31]),
    "vsll": (widths_signed, "v128 a, v128 b"),
    "vslli": (widths_signed, "v128 a, int imm", [0, 7]),
    "vslt": (widths_all, "v128 a, v128 b"),
    "vslti": (widths_all, "v128 a, int imm", [0, 15]),
    "vsle": (widths_all, "v128 a, v128 b"),
    "vslei": (widths_all, "v128 a, int imm", [0, 15]),
    "vsra": (widths_signed, "v128 a, v128 b"),
    "vsrai": (widths_signed, "v128 a, int imm", [0, 7]),
    "vsran": (widths_vsrln, "v128 a, v128 b"),
    "vsrani": (widths_vsrlni, "v128 a, v128 b, int imm", [0, 7, 15]),
    "vsrar": (widths_signed, "v128 a, v128 b"),
    "vsrari": (widths_signed, "v128 a, int imm", [0, 7]),
    "vsrarn": (widths_vsrln, "v128 a, v128 b"),
    "vsrarni": (widths_vsrlni, "v128 a, v128 b, int imm", [0, 7, 15]),
    "vsrl": (widths_signed, "v128 a, v128 b"),
    "vsrli": (widths_signed, "v128 a, int imm", [0, 7]),
    "vsrln": (widths_vsrln, "v128 a, v128 b"),
    "vsrlni": (widths_vsrlni, "v128 a, v128 b, int imm", [0, 7, 15]),
    "vsrlr": (widths_signed, "v128 a, v128 b"),
    "vsrlri": (widths_signed, "v128 a, int imm", [0, 7]),
    "vsrlrn": (widths_vsrln, "v128 a, v128 b"),
    "vsrlrni": (widths_vsrlni, "v128 a, v128 b, int imm", [0, 7, 15]),
    "vssran": (widths_vssrln, "v128 a, v128 b"),
    "vssrani": (widths_vssrlni, "v128 a, v128 b, int imm", [0, 7, 15]),
    "vssrarn": (widths_vssrln, "v128 a, v128 b"),
    "vssrarni": (widths_vssrlni, "v128 a, v128 b, int imm", [0, 7, 15]),
    "vssrln": (widths_vssrln, "v128 a, v128 b"),
    "vssrlni": (widths_vssrlni, "v128 a, v128 b, int imm", [0, 7, 15]),
    "vssrlrn": (widths_vssrln, "v128 a, v128 b"),
    "vssrlrni": (widths_vssrlni, "v128 a, v128 b, int imm", [0, 7, 15]),
    "vsub": (widths_signed, "v128 a, v128 b"),
    "vsubwev": (widths_vsubw, "v128 a, v128 b"),
    "vsubwod": (widths_vsubw, "v128 a, v128 b"),
}

for name in tb:
    t = tb[name]
    widths = t[0]
    args = t[1]

    for width in widths:
        # LSX & LASX
        for prefix in ["", "x"]:
            inst_name = prefix + name + "_" + width

            # skip xvinsgr2vr_b/h
            if inst_name in ["xvinsgr2vr_b", "xvinsgr2vr_h"]:
                continue
            # skip xvreplvei
            if inst_name.startswith("xvreplvei"):
                continue

            fuzz_args = 0
            for arg in args.split(", "):
                if "v128" in arg:
                    fuzz_args += 1

            print(f"Saving {inst_name}.cpp")
            with open(f"{inst_name}.cpp", "w") as f:
                if prefix == "":
                    vtype = "v128"
                    fuzz = "FUZZ"
                else:
                    vtype = "v256"
                    fuzz = "XFUZZ"
                print('#include "common.h"', file=f)
                print("", file=f)
                print(f"{vtype} {inst_name}({args.replace('v128', vtype)}) {{", file=f)
                print(f"  {vtype} dst;", file=f)
                print(f'#include "{inst_name}.h"', file=f)
                print("  return dst;", file=f)
                print("}", file=f)
                print("", file=f)
                print("void test() {", file=f)
                if len(t) >= 3:
                    for imm in t[2]:
                        print(f"  {fuzz}{fuzz_args}({inst_name}, {imm});", file=f)
                else:
                    print(f"  {fuzz}{fuzz_args}({inst_name});", file=f)
                print("}", file=f)

os.system("clang-format -i *.cpp *.h")
