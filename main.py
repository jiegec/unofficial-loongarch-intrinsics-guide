import math
import os

def define_env(env):
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

    signednesses = {
        "b": "signed",
        "bu": "unsigned",
        "h": "signed",
        "hu": "unsigned",
        "w": "signed",
        "wu": "unsigned",
        "d": "signed",
        "du": "unsigned",
        "q": "signed",
        "qu": "unsigned",
    }

    precisions = {
        "s": "single",
        "d": "double"
    }

    fp_types = {
        "s": "__m128",
        "d": "__m128d"
    }

    def include(file):
        return open(f"code/{file}").read().strip()

    def instruction(intrinsic, instr, desc, code=None):
        file_name = instr.split(" ")[0].replace(".", "_")
        if code is None:
            code = include(f'{file_name}.h')
        code = code.strip()
        if os.path.exists(f"code/{file_name}.cpp"):
            tested = "Tested on real machine."
        else:
            tested = ""
        return f"""
## {intrinsic}

### Synopsis

```c++
{intrinsic}
#include <lsxintrin.h>
Instruction: {instr}
CPU Flags: LSX
```

### Description

{desc}

### Operation

```c++
{code}
```

{tested}

"""

    @env.macro
    def vabsd(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vabsd_{name} (__m128i a, __m128i b)",
            instr=f"vabsd.{name} vr, vr, vr",
            desc=f"Compute absolute difference of {signedness} {width}-bit elements in `a` and `b`, save the result in `dst`.",
        )

    @env.macro
    def vadd(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vadd_{name} (__m128i a, __m128i b)",
            instr=f"vadd.{name} vr, vr, vr",
            desc=f"Add {width}-bit elements in `a` and `b`, save the result in `dst`.",
        )

    @env.macro
    def vadda(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vadda_{name} (__m128i a, __m128i b)",
            instr=f"vadda.{name} vr, vr, vr",
            desc=f"Add absolute of {width}-bit elements in `a` and `b`, save the result in `dst`.",
        )

    @env.macro
    def vaddi(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vaddi_{name} (__m128i a, imm0_31 imm)",
            instr=f"vaddi.{name} vr, vr, imm",
            desc=f"Add {width}-bit elements in `a` and `imm`, save the result in `dst`.",
        )

    @env.macro
    def vaddwev(wide, narrow, narrow2=None):
        wide_width = widths[wide]
        if narrow2 is None:
            narrow2 = narrow
            inst_suffix = ""
            intrinsic_suffix = ""
        else:
            inst_suffix = f".{narrow2}"
            intrinsic_suffix = f"_{narrow2}"
        narrow_width = widths[narrow]
        signedness = signednesses[narrow]
        signedness2 = signednesses[narrow2]
        return instruction(
            intrinsic=f"__m128i __lsx_vaddwev_{wide}_{narrow}{intrinsic_suffix} (__m128i a, __m128i b)",
            instr=f"vaddwev.{wide}.{narrow}{inst_suffix} vr, vr, vr",
            desc=f"Add even-positioned {signedness} {narrow_width}-bit elements in `a` and {signedness2} elements in `b`, save the {wide_width}-bit result in `dst`.",
        )

    @env.macro
    def vaddwod(wide, narrow, narrow2=None):
        wide_width = widths[wide]
        if narrow2 is None:
            narrow2 = narrow
            inst_suffix = ""
            intrinsic_suffix = ""
        else:
            inst_suffix = f".{narrow2}"
            intrinsic_suffix = f"_{narrow2}"
        narrow_width = widths[narrow]
        signedness = signednesses[narrow]
        signedness2 = signednesses[narrow2]
        return instruction(
            intrinsic=f"__m128i __lsx_vaddwod_{wide}_{narrow}{intrinsic_suffix} (__m128i a, __m128i b)",
            instr=f"vaddwod.{wide}.{narrow}{inst_suffix} vr, vr, vr",
            desc=f"Add odd-positioned {signedness} {narrow_width}-bit elements in `a` and {signedness2} elements in `b`, save the {wide_width}-bit result in `dst`.",
        )

    @env.macro
    def vavg(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vavg_{name} (__m128i a, __m128i b)",
            instr=f"vavg.{name} vr, vr, vr",
            desc=f"Compute the average (rounded towards negative infinity) of {signedness} {width}-bit elements in `a` and `b`, save the result in `dst`.",
        )

    @env.macro
    def vavgr(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vavgr_{name} (__m128i a, __m128i b)",
            instr=f"vavgr.{name} vr, vr, vr",
            desc=f"Compute the average (rounded towards positive infinity) of {signedness} {width}-bit elements in `a` and `b`, save the result in `dst`.",
        )

    @env.macro
    def vbitclr(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vbitclr_{name} (__m128i a, __m128i b)",
            instr=f"vbitclr.{name} vr, vr, vr",
            desc=f"Clear the bit specified by elements in `b` from {width}-bit elements in `a`, save the result in `dst`.",
        )

    @env.macro
    def vbitclri(name):
        width = widths[name]
        imm_upper = width - 1
        return instruction(
            intrinsic=f"__m128i __lsx_vbitclri_{name} (__m128i a, imm0_{imm_upper} imm)",
            instr=f"vbitclri.{name} vr, vr, imm",
            desc=f"Clear the bit specified by `imm` from {width}-bit elements in `a`, save the result in `dst`.",
        )

    @env.macro
    def vbitset(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vbitset_{name} (__m128i a, __m128i b)",
            instr=f"vbitset.{name} vr, vr, vr",
            desc=f"Set the bit specified by elements in `b` from {width}-bit elements in `a`, save the result in `dst`.",
        )

    @env.macro
    def vbitseti(name):
        width = widths[name]
        imm_upper = width - 1
        return instruction(
            intrinsic=f"__m128i __lsx_vbitseti_{name} (__m128i a, imm0_{imm_upper} imm)",
            instr=f"vbitseti.{name} vr, vr, imm",
            desc=f"Set the bit specified by `imm` from {width}-bit elements in `a`, save the result in `dst`.",
        )

    @env.macro
    def vbitrev(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vbitrev_{name} (__m128i a, __m128i b)",
            instr=f"vbitrev.{name} vr, vr, vr",
            desc=f"Toggle the bit specified by elements in `b` from {width}-bit elements in `a`, save the result in `dst`.",
        )

    @env.macro
    def vbitrevi(name):
        width = widths[name]
        imm_upper = width - 1
        return instruction(
            intrinsic=f"__m128i __lsx_vbitrevi_{name} (__m128i a, imm0_{imm_upper} imm)",
            instr=f"vbitrevi.{name} vr, vr, imm",
            desc=f"Toggle the bit specified by `imm` from {width}-bit elements in `a`, save the result in `dst`.",
        )

    @env.macro
    def vclo(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_clo_{name} (__m128i a)",
            instr=f"vclo.{name} vr, vr",
            desc=f"Count leading ones of {width}-bit elements in `a`.",
        )

    @env.macro
    def vclz(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_clz_{name} (__m128i a)",
            instr=f"vclz.{name} vr, vr",
            desc=f"Count leading zeros of {width}-bit elements in `a`.",
        )

    @env.macro
    def vdiv(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_div_{name} (__m128i a, __m128i b)",
            instr=f"vdiv.{name} vr, vr, vr",
            desc=f"Divide {signedness} {width}-bit elements in `a` by elements in `b`.",
        )

    @env.macro
    def vexth(name, name2):
        width = widths[name[0]]
        width2 = widths[name2[0]]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vexth_{name}_{name2} (__m128i a)",
            instr=f"vexth.{name}.{name2} vr, vr",
            desc=f"Extend {signedness} {width2}-bit elements in the higher half of `a` to {width}-bit.",
        )

    @env.macro
    def vextl(name, name2):
        width = widths[name[0]]
        width2 = widths[name2[0]]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vextl_{name}_{name2} (__m128i a)",
            instr=f"vextl.{name}.{name2} vr, vr",
            desc=f"Extend {signedness} {width2}-bit elements in the lower half of `a` to {width}-bit.",
        )

    @env.macro
    def vextrins(name):
        width = widths[name[0]]
        return instruction(
            intrinsic=f"__m128i __lsx_vextrins_{name} (__m128i a, __m128i b, imm0_255 imm)",
            instr=f"vextrins.{name} vr, vr, imm",
            desc=f"Extract one {width}-bit element in `b` and insert it to `a` according to `imm`.",
        )

    @env.macro
    def vfcmp(cond):
        if cond[0] == "c":
            trap = "Do not trap for QNaN."
        else:
            trap = "Trap for QNaN."
        desc = {
            "af": "AF(Always False)",
            "un": "UN(Unordered)",
            "eq": "EQ(Equal)",
            "ueq": "UEQ(Unordered or Equal)",
            "lt": "LT(Less than)",
            "ult": "ULT(Unordered or Less than)",
            "le": "LE(Less than or Equal)",
            "ule": "ULE(Unordered, Less than or Equal)",
            "ne": "NE(Not Equal)",
            "or": "OR(Ordered)",
            "une": "UNE(Unordered or Not Equal)",
        }[cond[1:]]
        return instruction(
            intrinsic=f"__m128i __lsx_vfcmp_{cond}_s (__m128 a, __m128 b)",
            instr=f"vfcmp.{cond}.s vr, vr, vr",
            desc=f"Compare single precision elements in `a` and `b`, save the comparison result (all ones if {desc}, all zeros otherwise) into `dst`. {trap}",
            code=f"""
for (int i = 0;i < 4;i++) {{
    if (fp_compare_{cond}(a.fp32[i], b.fp32[i])) {{
        dst.word[i] = 0xFFFFFFFF;
    }} else {{
        dst.word[i] = 0;
    }}
}}
            """
        ) + "\n" + instruction(
            intrinsic=f"__m128i __lsx_vfcmp_{cond}_d (__m128d a, __m128d b)",
            instr=f"vfcmp.{cond}.d vr, vr, vr",
            desc=f"Compare double precision elements in `a` and `b`, save the comparison result (all ones if {desc}, all zeros otherwise) into `dst`. {trap}",
            code=f"""
for (int i = 0;i < 2;i++) {{
    if (fp_compare_{cond}(a.fp64[i], b.fp64[i])) {{
        dst.word[i] = 0xFFFFFFFFFFFFFFFF;
    }} else {{
        dst.word[i] = 0;
    }}
}}
            """
        )

    @env.macro
    def vfdiv(name):
        precision = precisions[name]
        fp_type = fp_types[name]
        return instruction(
            intrinsic=f"{fp_type} __lsx_vfdiv_{name} ({fp_type} a, {fp_type} b)",
            instr=f"vfdiv.{name} vr, vr, vr",
            desc=f"Divide {fp_type} precision floating point elements in `a` by elements in `b`.",
        )

    @env.macro
    def vhaddw(name, name2):
        width = widths[name[0]]
        width2 = widths[name2[0]]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vhaddw_{name}_{name2} (__m128i a, __m128i b)",
            instr=f"vhaddw.{name}.{name2} vr, vr, vr",
            desc=f"Add odd-positioned {signedness} {width2}-bit elements in `a` to even-positioned {signedness} {width2}-bit elements in 'b' to get {width}-bit result.",
        )

    @env.macro
    def vhsubw(name, name2):
        width = widths[name[0]]
        width2 = widths[name2[0]]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vhsubw_{name}_{name2} (__m128i a, __m128i b)",
            instr=f"vhsubw.{name}.{name2} vr, vr, vr",
            desc=f"Subtract odd-positioned {signedness} {width2}-bit elements in `a` by even-positioned {signedness} {width2}-bit elements in 'b' to get {width}-bit result.",
        )

    @env.macro
    def vilvh(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vilvh_{name} (__m128i a, __m128i b)",
            instr=f"vilvh.{name} vr, vr, vr",
            desc=f"Interleave {width}-bit elements in higher half of `a` and `b`.",
        )

    @env.macro
    def vilvl(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vilvl_{name} (__m128i a, __m128i b)",
            instr=f"vilvl.{name} vr, vr, vr",
            desc=f"Interleave {width}-bit elements in lower half of `a` and `b`.",
        )

    @env.macro
    def vinsgr2vr(name):
        width = widths[name]
        imm_upper = 128 // width - 1
        if name == "d":
            long = "long "
        else:
            long = ""
        return instruction(
            intrinsic=f"__m128i __lsx_insgr2vr_{name} (__m128i a, {long}int b, imm0_{imm_upper} imm)",
            instr=f"vinsgr2vr.{name} vr, r, imm",
            desc=f"Insert {width}-bit element into lane indexed `imm`.",
        )

    @env.macro
    def vminmax(min_max, name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_v{min_max}_{name} (__m128i a, __m128i b)",
            instr=f"v{min_max}.{name} vr, vr, vr",
            desc=f"Compute elementwise {min_max}imum for {signedness} {width}-bit elements in `a` and `b`.",
        )

    @env.macro
    def vminmaxi(min_max, name):
        width = widths[name]
        signedness = signednesses[name]
        if signedness == "unsigned":
            imm_range = "0_31"
        else:
            imm_range = "n16_15"
        return instruction(
            intrinsic=f"__m128i __lsx_v{min_max}i_{name} (__m128i a, imm_{imm_range} imm)",
            instr=f"v{min_max}i.{name} vr, vr, imm",
            desc=f"Compute elementwise {min_max}imum for {signedness} {width}-bit elements in `a` and `imm`.",
        )

    @env.macro
    def vshuf_hwd(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vshuf_{name} (__m128i a, __m128i b, __m128i c)",
            instr=f"vshuf.{name} vr, vr, vr",
            desc=f"Shuffle {width}-bit elements in `b` and `c` with indices from `a`, save the result to `dst`.",
        )

    @env.macro
    def vldrepl(name):
        width = widths[name]
        imm_range = 2048 // (width // 8)
        shift = int(math.log2(width // 8))
        return instruction(
            intrinsic=f"__m128i __lsx_vldrepl_{name} (void * addr, imm_n{imm_range}_{imm_range-1} offset)",
            instr=f"vldrepl.{name} vr, r, imm",
            desc=f"Read {width}-bit data from memory address `addr + (offset << {shift})`, replicate the data to all vector lanes and save into `dst`.",
        )

