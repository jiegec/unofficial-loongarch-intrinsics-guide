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
    def vadd_mul_sub_w_ev_od(op, desc, even_odd, wide, narrow, narrow2=None):
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
            intrinsic=f"__m128i __lsx_v{op}wev_{wide}_{narrow}{intrinsic_suffix} (__m128i a, __m128i b)",
            instr=f"v{op}wev.{wide}.{narrow}{inst_suffix} vr, vr, vr",
            desc=f"{desc} {even_odd}-positioned {signedness} {narrow_width}-bit elements in `a` and {signedness2} elements in `b`, save the {wide_width}-bit result in `dst`.",
        )

    @env.macro
    def vaddwev(wide, narrow, narrow2=None):
        return vadd_mul_sub_w_ev_od("add", "Add", "even", wide, narrow, narrow2)

    @env.macro
    def vmulwev(wide, narrow, narrow2=None):
        return vadd_mul_sub_w_ev_od("mul", "Multiply", "even", wide, narrow, narrow2)

    @env.macro
    def vsubwev(wide, narrow):
        return vadd_mul_sub_w_ev_od("sub", "Subtract", "even", wide, narrow)

    @env.macro
    def vaddwod(wide, narrow, narrow2=None):
        return vadd_mul_sub_w_ev_od("add", "Add", "odd", wide, narrow, narrow2)

    @env.macro
    def vmulwod(wide, narrow, narrow2=None):
        return vadd_mul_sub_w_ev_od("mul", "Multiply", "odd", wide, narrow, narrow2)

    @env.macro
    def vsubwod(wide, narrow):
        return vadd_mul_sub_w_ev_od("sub", "Subtract", "odd", wide, narrow)


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
    def vmod(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vmod_{name} (__m128i a, __m128i b)",
            instr=f"vmod.{name} vr, vr, vr",
            desc=f"Modulo residual {signedness} {width}-bit elements in `a` by elements in `b`.",
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
    def vmin(name):
        return vminmax("min", name)

    @env.macro
    def vmax(name):
        return vminmax("max", name)

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
    def vmini(name):
        return vminmaxi("min", name)

    @env.macro
    def vmaxi(name):
        return vminmaxi("max", name)

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

    @env.macro
    def vsub(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vsub_{name} (__m128i a, __m128i b)",
            instr=f"vsub.{name} vr, vr, vr",
            desc=f"Subtract {width}-bit elements in `a` and `b`, save the result in `dst`.",
        )

    @env.macro
    def vlogical(op):
        return instruction(
            intrinsic=f"__m128i __lsx_v{op}_v (__m128i a, __m128i b)",
            instr=f"v{op}.v vr, vr, vr",
            desc=f"Compute bitwise {op.upper()} between elements in `a` and `b`.",
        )

    @env.macro
    def vlogicali(op):
        return instruction (
            intrinsic=f"__m128i __lsx_v{op}i_b (__m128i a, imm0_255 imm)",
            instr=f"v{op}i.b vr, vr, imm",
            desc=f"Compute bitwise {op.upper()} between elements in `a` and `imm`.",
        )

    @env.macro
    def vmadd(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vmadd_{name} (__m128i a, __m128i b, __m128i c)",
            instr=f"vmadd.{name} vr, vr, vr",
            desc=f"Multiply {width}-bit elements in `b` and `c`, add to elements in `a`, save the result in `dst`.",
        )

    @env.macro
    def vmaddw_ev_od(name, even_odd, wide, narrow, narrow2=None):
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
            intrinsic=f"__m128i __lsx_vmaddw{name}_{wide}_{narrow}{intrinsic_suffix} (__m128i a, __m128i b, __m128i c)",
            instr=f"vmaddw{name}.{wide}.{narrow}{inst_suffix} vr, vr, vr",
            desc=f"Multiply {even_odd}-positioned {signedness} {narrow_width}-bit elements in `b` and {signedness2} elements in `c`, add to {wide_width}-bit elements in `a`.",
        )

    @env.macro
    def vmaddwev(wide, narrow, narrow2=None):
        return vmaddw_ev_od("ev", "even", wide, narrow, narrow2)

    @env.macro
    def vmaddwod(wide, narrow, narrow2=None):
        return vmaddw_ev_od("od", "odd", wide, narrow, narrow2)

    @env.macro
    def vmul(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vmul_{name} (__m128i a, __m128i b)",
            instr=f"vmul.{name} vr, vr, vr",
            desc=f"Multiply {width}-bit elements in `a` and `b`, save the result in `dst`.",
        )

    @env.macro
    def vmuh(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vmuh_{name} (__m128i a, __m128i b)",
            instr=f"vmuh.{name} vr, vr, vr",
            desc=f"Multiply {signedness} {width}-bit elements in `a` and `b`, save the high {width}-bit result in `dst`.",
        )

    @env.macro
    def vmsub(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vmsub_{name} (__m128i a, __m128i b, __m128i c)",
            instr=f"vmsub.{name} vr, vr, vr",
            desc=f"Multiply {width}-bit elements in `b` and `c`, negate and add elements in `a`, save the result in `dst`.",
        )

    @env.macro
    def vpcnt(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_pcnt_{name} (__m128i a)",
            instr=f"vpcnt.{name} vr, vr",
            desc=f"Count the number of ones in {width}-bit elements in `a`.",
        )

    @env.macro
    def vstelm(name):
        width = widths[name]
        member = {
            "b": "byte",
            "h": "half",
            "w": "word",
            "d": "dword"
        }[name]
        imm_upper = 128 // width - 1
        return instruction(
            intrinsic=f"void __lsx_vstelm_{name} (__m128i data, void * addr, imm_n128_127 offset, imm0_{imm_upper} lane)",
            instr=f"vstelm.{name} vr, r, imm, imm",
            desc=f"Store the element in `data` specified by `lane` to memory address `addr + offset`.",
            code=f"""
memory_store({width}, data.{member}[lane], addr + offset);
"""
        )

    @env.macro
    def vseq(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vseq_{name} (__m128i a, __m128i b)",
            instr=f"vseq.{name} vr, vr, vr",
            desc=f"Compare the {width}-bit elements in `a` and `b`, store all-ones to `dst` if equal, zero otherwise.",
        )

    @env.macro
    def vslt(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vslt_{name} (__m128i a, __m128i b)",
            instr=f"vslt.{name} vr, vr, vr",
            desc=f"Compare the {signedness} {width}-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than `b`, zero otherwise.",
        )

    @env.macro
    def vsle(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vslt_{name} (__m128i a, __m128i b)",
            instr=f"vslt.{name} vr, vr, vr",
            desc=f"Compare the {signedness} {width}-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than or equal `b`, zero otherwise.",
        )

    @env.macro
    def vsadd(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vsadd_{name} (__m128i a, __m128i b)",
            instr=f"vsadd.{name} vr, vr, vr",
            desc=f"Saturing add the {signedness} {width}-bit elements in `a` and `b`, store the result to `dst`.",
        )

    @env.macro
    def vssub(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vsadd_{name} (__m128i a, __m128i b)",
            instr=f"vsadd.{name} vr, vr, vr",
            desc=f"Saturing add the {signedness} {width}-bit elements in `a` and `b`, store the result to `dst`.",
        )

    @env.macro
    def vsll(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vsll_{name} (__m128i a, __m128i b)",
            instr=f"vsll.{name} vr, vr, vr",
            desc=f"Logical left shift the unsigned {width}-bit elements in `a` by elements in `b`, store the result to `dst`.",
        )

    @env.macro
    def vsrl(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vsrl_{name} (__m128i a, __m128i b)",
            instr=f"vsrl.{name} vr, vr, vr",
            desc=f"Logical right shift the unsigned {width}-bit elements in `a` by elements in `b`, store the result to `dst`.",
        )

    @env.macro
    def vsra(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vsra_{name} (__m128i a, __m128i b)",
            instr=f"vsra.{name} vr, vr, vr",
            desc=f"Arithmetic right shift the signed {width}-bit elements in `a` by elements in `b`, store the result to `dst`.",
        )

    @env.macro
    def vrotr(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vrotr_{name} (__m128i a, __m128i b)",
            instr=f"vrotr.{name} vr, vr, vr",
            desc=f"Rotate right the unsigned {width}-bit elements in `a` by elements in `b`, store the result to `dst`.",
        )

    @env.macro
    def vsrlr(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vsrlr_{name} (__m128i a, __m128i b)",
            instr=f"vsrlr.{name} vr, vr, vr",
            desc=f"Logical right shift (with rounding) the unsigned {width}-bit elements in `a` by elements in `b`, store the result to `dst`.",
        )

    @env.macro
    def vsrar(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vsrar_{name} (__m128i a, __m128i b)",
            instr=f"vsrar.{name} vr, vr, vr",
            desc=f"Arithmetic right shift (with rounding) the signed {width}-bit elements in `a` by elements in `b`, store the result to `dst`.",
        )

    @env.macro
    def vpackev(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vpackev_{name} (__m128i a, __m128i b)",
            instr=f"vpackev.{name} vr, vr, vr",
            desc=f"Collect and pack even-positioned {width}-bit elements in `a` and `b` and store `dst`.",
        )

    @env.macro
    def vpackod(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vpackod_{name} (__m128i a, __m128i b)",
            instr=f"vpackod.{name} vr, vr, vr",
            desc=f"Collect and pack odd-positioned {width}-bit elements in `a` and `b` and store `dst`.",
        )