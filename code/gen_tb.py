import glob
import subprocess
from concurrent.futures import ThreadPoolExecutor

widths_signed = ["b", "h", "w", "d"]
widths_unsigned = ["bu", "hu", "wu", "du"]
widths_fp = ["s", "d"]
widths_all = ["b", "bu", "h", "hu", "w", "wu", "d", "du"]
widths_vexth = ["h_b", "hu_bu", "w_h", "wu_hu", "d_w", "du_wu", "q_d", "qu_du"]
widths_vext2xv = [
    "h_b",
    "hu_bu",
    "w_b",
    "wu_bu",
    "w_h",
    "wu_hu",
    "d_b",
    "du_bu",
    "d_h",
    "du_hu",
    "d_w",
    "du_wu",
]
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
    "vadd": (widths_signed + ["q"], "v128 a, v128 b"),
    "vadda": (widths_signed, "v128 a, v128 b"),
    "vaddi": (widths_unsigned, "v128 a, int imm", [0, 31]),
    "vand": (["v"], "v128 a, v128 b"),
    "vandn": (["v"], "v128 a, v128 b"),
    "vandi": (["b"], "v128 a, int imm", [0, 31]),
    "vabsd": (widths_all, "v128 a, v128 b"),
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
    "vbsll": (["v"], "v128 a, int imm", [0, 3]),
    "vbsrl": (["v"], "v128 a, int imm", [0, 3]),
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
    "vext2xv": (widths_vext2xv, "v128 a"),
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
    "vnor": (["v"], "v128 a, v128 b"),
    "vnori": (["b"], "v128 a, int imm", [0, 31]),
    "vor": (["v"], "v128 a, v128 b"),
    "vorn": (["v"], "v128 a, v128 b"),
    "vori": (["b"], "v128 a, int imm", [0, 31]),
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
    "vshuf": (widths_signed, "v128 a, v128 b, v128 c"),
    "vshuf4i": (["b", "h", "w", "d"], "v128 a, int imm", [0, 13, 100, 128, 255]),
    "vsigncov": (widths_signed, "v128 a, v128 b"),
    "vsllwil": (widths_vsllwil, "v128 a, int imm", [0, 7]),
    "vssub": (widths_all, "v128 a, v128 b"),
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
    "vsub": (widths_signed + ["q"], "v128 a, v128 b"),
    "vsubi": (widths_unsigned, "v128 a, int imm", [0, 31]),
    "vsubwev": (widths_vsubw, "v128 a, v128 b"),
    "vsubwod": (widths_vsubw, "v128 a, v128 b"),
    "vxor": (["v"], "v128 a, v128 b"),
    "vxori": (["b"], "v128 a, int imm", [0, 31]),
    # Float binary
    "vfadd": (widths_fp, "v128 a, v128 b"),
    "vfsub": (widths_fp, "v128 a, v128 b"),
    "vfdiv": (widths_fp, "v128 a, v128 b"),
    "vfmul": (widths_fp, "v128 a, v128 b"),
    "vfmax": (widths_fp, "v128 a, v128 b"),
    "vfmin": (widths_fp, "v128 a, v128 b"),
    "vfmaxa": (widths_fp, "v128 a, v128 b"),
    "vfmina": (widths_fp, "v128 a, v128 b"),
    # Float conversions (unary)
    "vffint": (["d_l", "d_lu", "s_w", "s_wu"], "v128 a"),
    "vffinth": (["d_w"], "v128 a"),
    "vffintl": (["d_w"], "v128 a"),
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
            # skip xvext2xv
            if inst_name.startswith("xvext2xv"):
                continue
            # skip xvreplvei
            if inst_name.startswith("xvreplvei"):
                continue
            # skip xv/vshuf4i.d due to extra argument
            if inst_name == "vshuf4i_d" or inst_name == "xvshuf4i_d":
                continue

            fuzz_args = 0
            for arg in args.split(", "):
                if "v128" in arg:
                    fuzz_args += 1

            print(f"Saving {inst_name}.cpp")
            with open(f"{inst_name}.cpp", "w") as f:
                # vext2xv is in lasx
                if prefix == "" and not inst_name.startswith("vext2xv"):
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

# Scalar adc instructions (GPR, not SIMD)
for width in ["b", "h", "w", "d"]:
    inst_name = f"adc_{width}"
    print(f"Saving {inst_name}.cpp")
    with open(f"{inst_name}.cpp", "w") as f:
        print('#include "common.h"', file=f)
        print("", file=f)
        print(
            f"uint64_t {inst_name}(eflags &EFLAGS, uint64_t a, uint64_t b) {{", file=f
        )
        print(f"  uint64_t dst;", file=f)
        print(f'#include "{inst_name}.h"', file=f)
        print(f"  return dst;", file=f)
        print("}", file=f)
        print("", file=f)
        print(
            f"uint64_t ref_{inst_name}(eflags &EFLAGS, uint64_t a, uint64_t b) {{",
            file=f,
        )
        print(f"  uint16_t eflags = EFLAGS.raw;", file=f)
        print(f"  uint64_t dst;", file=f)
        print(
            f'  asm volatile("x86mtflag %1, 0x3f\\nadc.{width} %0, %2, %3\\nx86mfflag %1, 0x3f"',
            file=f,
        )
        print(f'               : "=r"(dst), "+r"(eflags)', file=f)
        print(f'               : "r"(a), "r"(b)', file=f)
        print(f'               : "memory");', file=f)
        print(f"  EFLAGS.raw = eflags;", file=f)
        print(f"  return dst;", file=f)
        print("}", file=f)
        print("", file=f)
        print(f"void test() {{ IFUZZ2({inst_name}); }}", file=f)
        print("", file=f)

# Scalar arm instructions (GPR, not SIMD)
# Use same inst_name list as gen_impl.py
for inst_name in ["armadc_w", "armadd_w", "armand_w", "armor_w"]:
    stem = inst_name[:-2]
    print(f"Saving {inst_name}.cpp")
    with open(f"{inst_name}.cpp", "w") as f:
        print('#include "common.h"', file=f)
        print("", file=f)
        print(
            f"uint64_t {inst_name}(eflags &ARMFLAGS, uint64_t a, uint64_t b, int cond) {{",
            file=f,
        )
        print(f"  uint64_t dst = 0;", file=f)
        print(f'#include "{inst_name}.h"', file=f)
        print(f"  return dst;", file=f)
        print("}", file=f)
        print("", file=f)
        print(f"#define ref_{inst_name}(eflags, a, b, cond) \\", file=f)
        print(f"  ({{uint16_t flags = eflags.raw; \\", file=f)
        print(
            f'     asm volatile("x86mtflag %0, 0x3f\\n{stem}.w %1, %2, %3\\nx86mfflag %0, 0x3f" \\',
            file=f,
        )
        print(f'                  : "+r"(flags) \\', file=f)
        print(f'                  : "r"(a), "r"(b), "n"(cond) \\', file=f)
        print(f'                  : "memory"); \\', file=f)
        print(f"     eflags.raw = flags; \\", file=f)
        print(f"     0; }})", file=f)
        print("", file=f)
        print(f"void test() {{", file=f)
        print(f"  IFUZZ2({inst_name}, 0);", file=f)
        print(f"  IFUZZ2({inst_name}, 7);", file=f)
        print(f"  IFUZZ2({inst_name}, 14);", file=f)
        print(f"  IFUZZ2({inst_name}, 15);", file=f)
        print("}", file=f)
        print("", file=f)

# Scalar addu12i instructions (GPR, not SIMD)
for width_name in ["d", "w"]:
    inst_name = f"addu12i_{width_name}"
    print(f"Saving {inst_name}.cpp")
    with open(f"{inst_name}.cpp", "w") as f:
        print('#include "common.h"', file=f)
        print("", file=f)
        print(f"uint64_t {inst_name}(eflags &EFLAGS, uint64_t a, int imm) {{", file=f)
        print(f"  uint64_t dst;", file=f)
        print(f'#include "{inst_name}.h"', file=f)
        print(f"  return dst;", file=f)
        print("}", file=f)
        print("", file=f)
        print(f"#define ref_{inst_name}(eflags, a, imm) \\", file=f)
        print(f"  ({{uint64_t _result; \\", file=f)
        print(f'     asm volatile("addu12i.{width_name} %0, %1, %2" \\', file=f)
        print(
            f'                  : "=r"(_result) : "r"(a), "n"((((imm) & 0x1f) ^ 16) - 16) \\',
            file=f,
        )
        print(f'                  : "memory"); \\', file=f)
        print(f"     _result; }})", file=f)
        print("", file=f)
        print(f"void test() {{", file=f)
        print(f"  IFUZZ1({inst_name}, 0);", file=f)
        print(f"  IFUZZ1({inst_name}, 1);", file=f)
        print(f"  IFUZZ1({inst_name}, 31);", file=f)
        print("}", file=f)
        print("", file=f)

files = glob.glob("*.cpp") + glob.glob("*.h")
with ThreadPoolExecutor() as pool:
    for f in files:
        pool.submit(subprocess.run, ["clang-format", "-i", f])
