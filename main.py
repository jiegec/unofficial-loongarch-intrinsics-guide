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
    }

    def include(file):
        return open(f"code/{file}").read().strip()

    def instruction(intrinsic, instr, desc):
        file_name = instr.split(" ")[0].replace(".", "_")
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
{include(f'{file_name}.h')}
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

