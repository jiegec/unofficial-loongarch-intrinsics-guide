import math
import glob
import os
import json
import markdown
import re
import csv

# dirty way to reduce code
cur_simd = "lsx"
cur_vlen = 128

# read latency & throughput
cpus = ["3C5000", "3A6000", "3C6000", "2K1000LA", "2K3000"]
measure = {x: {} for x in cpus}
for cpu in cpus:
    with open(f"code/measure-{cpu}.csv", newline="") as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            latency = []
            for part in row["latency"].split("/"):
                if part == "":
                    lat = "N/A"
                else:
                    lat = float(part)
                    if abs((lat - round(lat)) / lat) < 0.02:
                        lat = round(lat)
                latency.append(lat)
            latency = sorted(list(set(latency)))

            throughput_ipc = float(row["throughput(ipc)"])
            if abs(throughput_ipc - round(throughput_ipc)) < 0.03:
                throughput_ipc = round(throughput_ipc)

            # handle small ipc better by 1/cpi
            if throughput_ipc < 1.0:
                throughput_cpi = float(row["throughput(cpi)"])
                if abs(throughput_cpi - round(throughput_cpi)) < 0.03:
                    throughput_cpi = round(throughput_cpi)

                throughput_ipc = f"{throughput_ipc}(1/{throughput_cpi})"

            measure[cpu][row["name"]] = {
                "latency": ", ".join(map(str, latency)),
                "throughput(ipc)": throughput_ipc,
            }

# read examples
examples = {}
for line in open('code/examples.md', 'r'):
    if '(' in line:
        name = line.split('(')[0].strip().split(' ')[-1]
        if name not in examples:
            examples[name] = []
        line = line.strip()
        expr, res = line.split(":")
        expr = expr.replace(" COMMA ", ", ")
        examples[name].append(f"{expr}\n={res}")

# depends on implementation of env.macro()
def my_macro(env):
    def wrap(fn):
        def vfn(*args):
            global cur_simd, cur_vlen
            cur_simd = "lsx"
            cur_vlen = 128
            return fn(*args)

        env.macros[f"{fn.__name__}"] = vfn

        def xvfn(*args):
            global cur_simd, cur_vlen
            cur_simd = "lasx"
            cur_vlen = 256
            return fn(*args)

        env.macros[f"x{fn.__name__}"] = xvfn
        return fn

    return wrap


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

    precisions = {"s": "single", "d": "double"}

    fp_types = {"s": "__m128", "d": "__m128d"}

    def instruction(intrinsic, instr, desc):
        global cur_simd
        # try to be smart
        file_name = None
        intrinsic_name = ""
        for part in intrinsic.split(" "):
            if part.startswith("__lsx_"):
                file_name = part[6:]
                intrinsic_name = part
            elif part.startswith("__lasx_"):
                file_name = part[7:]
                intrinsic_name = part
        if cur_simd == "lasx" and file_name[0] != "x":
            file_name = "x" + file_name
            instr = "x" + instr
            intrinsic = intrinsic.replace("m128", "m256").replace("_lsx_", "_lasx_x")
            # replace vr to xr in instr
            instr = re.sub("\\bvr\\b", "xr", instr)
            intrinsic_name = intrinsic_name.replace("__lsx_", "__lasx_x")
        if not os.path.exists(f"code/{file_name}.h"):
            file_name = instr.split(" ")[0].replace(".", "_")

        code = open(f"code/{file_name}.h").read().strip()
        code = code.strip()
        if os.path.exists(f"code/{file_name}.cpp"):
            tested = "Tested on real machine."
        else:
            tested = ""

        if os.path.exists(f"docs/diagram/{file_name}.svg"):
            diagram = f'![](../diagram/{file_name}.svg)'
        else:
            diagram = ""

        global measure
        global cpus
        instr_name = instr.split(" ")[0].replace(".", "_")
        show_cpus = []
        latencies = []
        throughputs = []
        for cpu in cpus:
            if instr_name in measure[cpu]:
                show_cpus.append(cpu)
                latencies.append(measure[cpu][instr_name]["latency"])
                throughputs.append(measure[cpu][instr_name]["throughput(ipc)"])

        if len(show_cpus) > 0:
            latency_throughput = f"""
### Latency and Throughput

| CPU | Latency | Throughput (IPC) |
|-----|---------|------------------|
"""
            for i in range(len(show_cpus)):
                latency_throughput += (
                    f"| {show_cpus[i]} | {latencies[i]} | {throughputs[i]} |\n"
                )
        else:
            latency_throughput = ""

        if intrinsic_name in examples:
            inner = "\n".join(examples[intrinsic_name])
            examples_text = f"""
### Examples

```c++
{inner}
```
"""
        else:
            examples_text = ""

        return f"""
## {intrinsic}

### Synopsis

```c++
{intrinsic}
#include <{cur_simd}intrin.h>
Instruction: {instr}
CPU Flags: {cur_simd.upper()}
```

### Description

{desc}

{diagram}

{examples_text}

### Operation

```c++
{code}
```

{tested}

{latency_throughput}

"""

    @my_macro(env)
    def vabsd(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vabsd_{name} (__m128i a, __m128i b)",
            instr=f"vabsd.{name} vr, vr, vr",
            desc=f"Compute absolute difference of {signedness} {width}-bit elements in `a` and `b`, save the result in `dst`.",
        )

    @my_macro(env)
    def vadd(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vadd_{name} (__m128i a, __m128i b)",
            instr=f"vadd.{name} vr, vr, vr",
            desc=f"Add {width}-bit elements in `a` and `b`, save the result in `dst`.",
        )

    @my_macro(env)
    def vadda(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vadda_{name} (__m128i a, __m128i b)",
            instr=f"vadda.{name} vr, vr, vr",
            desc=f"Add absolute of {width}-bit elements in `a` and `b`, save the result in `dst`.",
        )

    @my_macro(env)
    def vaddi(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vaddi_{name} (__m128i a, imm0_31 imm)",
            instr=f"vaddi.{name} vr, vr, imm",
            desc=f"Add {width}-bit elements in `a` and `imm`, save the result in `dst`.",
        )

    @my_macro(env)
    def vsubi(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vsubi_{name} (__m128i a, imm0_31 imm)",
            instr=f"vsubi.{name} vr, vr, imm",
            desc=f"Subtract {width}-bit elements in `a` by `imm`, save the result in `dst`.",
        )

    @my_macro(env)
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
        if even_odd == "even":
            suffix = "ev"
        else:
            suffix = "od"
        return instruction(
            intrinsic=f"__m128i __lsx_v{op}w{suffix}_{wide}_{narrow}{intrinsic_suffix} (__m128i a, __m128i b)",
            instr=f"v{op}w{suffix}.{wide}.{narrow}{inst_suffix} vr, vr, vr",
            desc=f"{desc} {even_odd}-positioned {signedness} {narrow_width}-bit elements in `a` and {signedness2} elements in `b`, save the {wide_width}-bit result in `dst`.",
        )

    @my_macro(env)
    def vaddwev(wide, narrow, narrow2=None):
        return vadd_mul_sub_w_ev_od("add", "Add", "even", wide, narrow, narrow2)

    @my_macro(env)
    def vmulwev(wide, narrow, narrow2=None):
        return vadd_mul_sub_w_ev_od("mul", "Multiply", "even", wide, narrow, narrow2)

    @my_macro(env)
    def vsubwev(wide, narrow):
        return vadd_mul_sub_w_ev_od("sub", "Subtract", "even", wide, narrow)

    @my_macro(env)
    def vaddwod(wide, narrow, narrow2=None):
        return vadd_mul_sub_w_ev_od("add", "Add", "odd", wide, narrow, narrow2)

    @my_macro(env)
    def vmulwod(wide, narrow, narrow2=None):
        return vadd_mul_sub_w_ev_od("mul", "Multiply", "odd", wide, narrow, narrow2)

    @my_macro(env)
    def vsubwod(wide, narrow):
        return vadd_mul_sub_w_ev_od("sub", "Subtract", "odd", wide, narrow)

    @my_macro(env)
    def vavg(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vavg_{name} (__m128i a, __m128i b)",
            instr=f"vavg.{name} vr, vr, vr",
            desc=f"Compute the average (rounded towards negative infinity) of {signedness} {width}-bit elements in `a` and `b`, save the result in `dst`.",
        )

    @my_macro(env)
    def vavgr(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vavgr_{name} (__m128i a, __m128i b)",
            instr=f"vavgr.{name} vr, vr, vr",
            desc=f"Compute the average (rounded towards positive infinity) of {signedness} {width}-bit elements in `a` and `b`, save the result in `dst`.",
        )

    @my_macro(env)
    def vbitclr(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vbitclr_{name} (__m128i a, __m128i b)",
            instr=f"vbitclr.{name} vr, vr, vr",
            desc=f"Clear the bit specified by elements in `b` from {width}-bit elements in `a`, save the result in `dst`.",
        )

    @my_macro(env)
    def vbitclri(name):
        width = widths[name]
        imm_upper = width - 1
        return instruction(
            intrinsic=f"__m128i __lsx_vbitclri_{name} (__m128i a, imm0_{imm_upper} imm)",
            instr=f"vbitclri.{name} vr, vr, imm",
            desc=f"Clear the bit specified by `imm` from {width}-bit elements in `a`, save the result in `dst`.",
        )

    @my_macro(env)
    def vbitset(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vbitset_{name} (__m128i a, __m128i b)",
            instr=f"vbitset.{name} vr, vr, vr",
            desc=f"Set the bit specified by elements in `b` from {width}-bit elements in `a`, save the result in `dst`.",
        )

    @my_macro(env)
    def vbitseti(name):
        width = widths[name]
        imm_upper = width - 1
        return instruction(
            intrinsic=f"__m128i __lsx_vbitseti_{name} (__m128i a, imm0_{imm_upper} imm)",
            instr=f"vbitseti.{name} vr, vr, imm",
            desc=f"Set the bit specified by `imm` from {width}-bit elements in `a`, save the result in `dst`.",
        )

    @my_macro(env)
    def vbitrev(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vbitrev_{name} (__m128i a, __m128i b)",
            instr=f"vbitrev.{name} vr, vr, vr",
            desc=f"Toggle the bit specified by elements in `b` from {width}-bit elements in `a`, save the result in `dst`.",
        )

    @my_macro(env)
    def vbitrevi(name):
        width = widths[name]
        imm_upper = width - 1
        return instruction(
            intrinsic=f"__m128i __lsx_vbitrevi_{name} (__m128i a, imm0_{imm_upper} imm)",
            instr=f"vbitrevi.{name} vr, vr, imm",
            desc=f"Toggle the bit specified by `imm` from {width}-bit elements in `a`, save the result in `dst`.",
        )

    @my_macro(env)
    def vclo(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vclo_{name} (__m128i a)",
            instr=f"vclo.{name} vr, vr",
            desc=f"Count leading ones of {width}-bit elements in `a`.",
        )

    @my_macro(env)
    def vclz(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vclz_{name} (__m128i a)",
            instr=f"vclz.{name} vr, vr",
            desc=f"Count leading zeros of {width}-bit elements in `a`.",
        )

    @my_macro(env)
    def vdiv(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vdiv_{name} (__m128i a, __m128i b)",
            instr=f"vdiv.{name} vr, vr, vr",
            desc=f"Divide {signedness} {width}-bit elements in `a` by elements in `b`.",
        )

    @my_macro(env)
    def vmod(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vmod_{name} (__m128i a, __m128i b)",
            instr=f"vmod.{name} vr, vr, vr",
            desc=f"Modulo residual {signedness} {width}-bit elements in `a` by elements in `b`.",
        )

    @my_macro(env)
    def vexth(name, name2):
        width = widths[name[0]]
        width2 = widths[name2[0]]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vexth_{name}_{name2} (__m128i a)",
            instr=f"vexth.{name}.{name2} vr, vr",
            desc=f"Extend {signedness} {width2}-bit elements in the higher half of `a` to {width}-bit.",
        )

    @my_macro(env)
    def vextl(name, name2):
        width = widths[name[0]]
        width2 = widths[name2[0]]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vextl_{name}_{name2} (__m128i a)",
            instr=f"vextl.{name}.{name2} vr, vr",
            desc=f"Extend {signedness} {width2}-bit elements in the lower half of `a` to {width}-bit.",
        )

    @my_macro(env)
    def vextrins(name):
        width = widths[name[0]]
        return instruction(
            intrinsic=f"__m128i __lsx_vextrins_{name} (__m128i a, __m128i b, imm0_255 imm)",
            instr=f"vextrins.{name} vr, vr, imm",
            desc=f"Extract one {width}-bit element in `b` and insert it to `a` according to `imm`.",
        )

    @my_macro(env)
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
        return (
            instruction(
                intrinsic=f"__m128i __lsx_vfcmp_{cond}_s (__m128 a, __m128 b)",
                instr=f"vfcmp.{cond}.s vr, vr, vr",
                desc=f"Compare single precision elements in `a` and `b`, save the comparison result (all ones if {desc}, all zeros otherwise) into `dst`. {trap}",
            )
            + "\n"
            + instruction(
                intrinsic=f"__m128i __lsx_vfcmp_{cond}_d (__m128d a, __m128d b)",
                instr=f"vfcmp.{cond}.d vr, vr, vr",
                desc=f"Compare double precision elements in `a` and `b`, save the comparison result (all ones if {desc}, all zeros otherwise) into `dst`. {trap}",
            )
        )

    @my_macro(env)
    def vfmul(name):
        precision = precisions[name]
        fp_type = fp_types[name]
        return instruction(
            intrinsic=f"{fp_type} __lsx_vfmul_{name} ({fp_type} a, {fp_type} b)",
            instr=f"vfmul.{name} vr, vr, vr",
            desc=f"Multiply {precision} precision floating point elements in `a` and elements in `b`.",
        )

    @my_macro(env)
    def vfdiv(name):
        precision = precisions[name]
        fp_type = fp_types[name]
        return instruction(
            intrinsic=f"{fp_type} __lsx_vfdiv_{name} ({fp_type} a, {fp_type} b)",
            instr=f"vfdiv.{name} vr, vr, vr",
            desc=f"Divide {precision} precision floating point elements in `a` by elements in `b`.",
        )

    @my_macro(env)
    def vfadd(name):
        precision = precisions[name]
        fp_type = fp_types[name]
        return instruction(
            intrinsic=f"{fp_type} __lsx_vfadd_{name} ({fp_type} a, {fp_type} b)",
            instr=f"vfadd.{name} vr, vr, vr",
            desc=f"Add {precision} precision floating point elements in `a` to elements in `b`.",
        )

    @my_macro(env)
    def vfsub(name):
        precision = precisions[name]
        fp_type = fp_types[name]
        return instruction(
            intrinsic=f"{fp_type} __lsx_vfsub_{name} ({fp_type} a, {fp_type} b)",
            instr=f"vfsub.{name} vr, vr, vr",
            desc=f"Subtract {precision} precision floating point elements in `a` by elements in `b`.",
        )

    @my_macro(env)
    def vfmax(name):
        precision = precisions[name]
        fp_type = fp_types[name]
        return instruction(
            intrinsic=f"{fp_type} __lsx_vfmax_{name} ({fp_type} a, {fp_type} b)",
            instr=f"vfmax.{name} vr, vr, vr",
            desc=f"Compute maximum of {precision} precision floating point elements in `a` and `b`.",
        )

    @my_macro(env)
    def vfmaxa(name):
        precision = precisions[name]
        fp_type = fp_types[name]
        return instruction(
            intrinsic=f"{fp_type} __lsx_vfmaxa_{name} ({fp_type} a, {fp_type} b)",
            instr=f"vfmaxa.{name} vr, vr, vr",
            desc=f"Compute maximum of {precision} precision floating point elements in `a` and `b` by magnitude.",
        )

    @my_macro(env)
    def vfmin(name):
        precision = precisions[name]
        fp_type = fp_types[name]
        return instruction(
            intrinsic=f"{fp_type} __lsx_vfmin_{name} ({fp_type} a, {fp_type} b)",
            instr=f"vfmax.{name} vr, vr, vr",
            desc=f"Compute minimum of {precision} precision floating point elements in `a` and `b`.",
        )

    @my_macro(env)
    def vfmina(name):
        precision = precisions[name]
        fp_type = fp_types[name]
        return instruction(
            intrinsic=f"{fp_type} __lsx_vfmina_{name} ({fp_type} a, {fp_type} b)",
            instr=f"vfmina.{name} vr, vr, vr",
            desc=f"Compute minimum of {precision} precision floating point elements in `a` and `b` by magnitude.",
        )

    @my_macro(env)
    def vhaddw(name, name2):
        width = widths[name[0]]
        width2 = widths[name2[0]]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vhaddw_{name}_{name2} (__m128i a, __m128i b)",
            instr=f"vhaddw.{name}.{name2} vr, vr, vr",
            desc=f"Add odd-positioned {signedness} {width2}-bit elements in `a` to even-positioned {signedness} {width2}-bit elements in `b` to get {width}-bit result.",
        )

    @my_macro(env)
    def vhsubw(name, name2):
        width = widths[name[0]]
        width2 = widths[name2[0]]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vhsubw_{name}_{name2} (__m128i a, __m128i b)",
            instr=f"vhsubw.{name}.{name2} vr, vr, vr",
            desc=f"Subtract odd-positioned {signedness} {width2}-bit elements in `a` by even-positioned {signedness} {width2}-bit elements in `b` to get {width}-bit result.",
        )

    @my_macro(env)
    def vilvh(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vilvh_{name} (__m128i a, __m128i b)",
            instr=f"vilvh.{name} vr, vr, vr",
            desc=f"Interleave {width}-bit elements in higher half of `a` and `b`.",
        )

    @my_macro(env)
    def vilvl(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vilvl_{name} (__m128i a, __m128i b)",
            instr=f"vilvl.{name} vr, vr, vr",
            desc=f"Interleave {width}-bit elements in lower half of `a` and `b`.",
        )

    @my_macro(env)
    def vinsgr2vr(name):
        global cur_vlen
        width = widths[name]
        imm_upper = cur_vlen // width - 1
        if name == "d":
            long = "long "
        else:
            long = ""
        return instruction(
            intrinsic=f"__m128i __lsx_vinsgr2vr_{name} (__m128i a, {long}int b, imm0_{imm_upper} imm)",
            instr=f"vinsgr2vr.{name} vr, r, imm",
            desc=f"Insert {width}-bit element into lane indexed `imm`.",
        )

    @env.macro
    def xvinsve0(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m256i __lasx_xvinsve0_{name} (__m256i a, __m256i b, imm0_{256 // width - 1} imm)",
            instr=f"xvinsve0.{name} xr, xr, imm",
            desc=f"Insert the first {width}-bit lane of `b` into lane indexed `imm` of `a`.",
        )

    @env.macro
    def vext2xv(name, name2):
        global cur_simd
        cur_simd = "lsx"  # avoid replacing vext2xv to xvext2xv
        width = widths[name]
        width2 = widths[name2]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m256i __lasx_vext2xv_{name}_{name2} (__m256i a)",
            instr=f"vext2xv.{name}.{name2} xr, xr",
            desc=f"Extend {signedness} {width2}-bit lane of `a` to {signedness} {width}-bit elements.",
        )

    @my_macro(env)
    def vminmax(min_max, name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_v{min_max}_{name} (__m128i a, __m128i b)",
            instr=f"v{min_max}.{name} vr, vr, vr",
            desc=f"Compute elementwise {min_max}imum for {signedness} {width}-bit elements in `a` and `b`.",
        )

    @my_macro(env)
    def vmin(name):
        return vminmax("min", name)

    @my_macro(env)
    def vmax(name):
        return vminmax("max", name)

    @my_macro(env)
    def vminmaxi(min_max, name):
        width = widths[name]
        signedness = signednesses[name]
        if signedness == "unsigned":
            imm_range = "0_31"
        else:
            imm_range = "_n16_15"
        return instruction(
            intrinsic=f"__m128i __lsx_v{min_max}i_{name} (__m128i a, imm{imm_range} imm)",
            instr=f"v{min_max}i.{name} vr, vr, imm",
            desc=f"Compute elementwise {min_max}imum for {signedness} {width}-bit elements in `a` and `imm`.",
        )

    @my_macro(env)
    def vmini(name):
        return vminmaxi("min", name)

    @my_macro(env)
    def vmaxi(name):
        return vminmaxi("max", name)

    @my_macro(env)
    def vshuf_b():
        name = "b"
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vshuf_b (__m128i a, __m128i b, __m128i c)",
            instr=f"vshuf.{name} vr, vr, vr, vr",
            desc=f"""
Shuffle bytes from `a` and `b` with indices from `c`.

Caveat: the indices are placed in `c`, while in other `vshuf` intrinsics, they are placed in `a`.
""",
        )

    @my_macro(env)
    def vshuf_hwd(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vshuf_{name} (__m128i a, __m128i b, __m128i c)",
            instr=f"vshuf.{name} vr, vr, vr",
            desc=f"Shuffle {width}-bit elements in `b` and `c` with indices from `a`, save the result to `dst`.",
        )

    @my_macro(env)
    def vldrepl(name):
        width = widths[name]
        imm_range = 2048 // (width // 8)
        shift = int(math.log2(width // 8))
        return instruction(
            intrinsic=f"__m128i __lsx_vldrepl_{name} (void * addr, imm_n{imm_range}_{imm_range-1} offset)",
            instr=f"vldrepl.{name} vr, r, imm",
            desc=f"Read {width}-bit data from memory address `addr + (offset << {shift})`, replicate the data to all vector lanes and save into `dst`.",
        )

    @my_macro(env)
    def vsub(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vsub_{name} (__m128i a, __m128i b)",
            instr=f"vsub.{name} vr, vr, vr",
            desc=f"Subtract {width}-bit elements in `a` and `b`, save the result in `dst`.",
        )

    @my_macro(env)
    def vlogical(op):
        return instruction(
            intrinsic=f"__m128i __lsx_v{op}_v (__m128i a, __m128i b)",
            instr=f"v{op}.v vr, vr, vr",
            desc=f"Compute bitwise {op.upper()} between elements in `a` and `b`.",
        )

    @my_macro(env)
    def vlogicali(op):
        return instruction(
            intrinsic=f"__m128i __lsx_v{op}i_b (__m128i a, imm0_255 imm)",
            instr=f"v{op}i.b vr, vr, imm",
            desc=f"Compute bitwise {op.upper()} between elements in `a` and `imm`.",
        )

    @my_macro(env)
    def vmadd(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vmadd_{name} (__m128i a, __m128i b, __m128i c)",
            instr=f"vmadd.{name} vr, vr, vr",
            desc=f"Multiply {width}-bit elements in `b` and `c`, add to elements in `a`, save the result in `dst`.",
        )

    @my_macro(env)
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

    @my_macro(env)
    def vmaddwev(wide, narrow, narrow2=None):
        return vmaddw_ev_od("ev", "even", wide, narrow, narrow2)

    @my_macro(env)
    def vmaddwod(wide, narrow, narrow2=None):
        return vmaddw_ev_od("od", "odd", wide, narrow, narrow2)

    @my_macro(env)
    def vmul(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vmul_{name} (__m128i a, __m128i b)",
            instr=f"vmul.{name} vr, vr, vr",
            desc=f"Multiply {width}-bit elements in `a` and `b`, save the result in `dst`.",
        )

    @my_macro(env)
    def vmuh(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vmuh_{name} (__m128i a, __m128i b)",
            instr=f"vmuh.{name} vr, vr, vr",
            desc=f"Multiply {signedness} {width}-bit elements in `a` and `b`, save the high {width}-bit result in `dst`.",
        )

    @my_macro(env)
    def vmsub(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vmsub_{name} (__m128i a, __m128i b, __m128i c)",
            instr=f"vmsub.{name} vr, vr, vr",
            desc=f"Multiply {width}-bit elements in `b` and `c`, negate and add elements in `a`, save the result in `dst`.",
        )

    @my_macro(env)
    def vpcnt(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vpcnt_{name} (__m128i a)",
            instr=f"vpcnt.{name} vr, vr",
            desc=f"Count the number of ones (population, popcount) in {width}-bit elements in `a`.",
        )

    @my_macro(env)
    def vstelm(name):
        global cur_vlen
        width = widths[name]
        imm_upper = cur_vlen // width - 1
        return instruction(
            intrinsic=f"void __lsx_vstelm_{name} (__m128i data, void * addr, imm_n128_127 offset, imm0_{imm_upper} lane)",
            instr=f"vstelm.{name} vr, r, imm, imm",
            desc=f"Store the {width}-bit element in `data` specified by `lane` to memory address `addr + offset`.",
        )

    @my_macro(env)
    def vseq(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vseq_{name} (__m128i a, __m128i b)",
            instr=f"vseq.{name} vr, vr, vr",
            desc=f"Compare the {width}-bit elements in `a` and `b`, store all-ones to `dst` if equal, zero otherwise.",
        )

    @my_macro(env)
    def vseqi(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vseqi_{name} (__m128i a, imm_n16_15 imm)",
            instr=f"vseqi.{name} vr, vr, imm",
            desc=f"Compare the {width}-bit elements in `a` and `imm`, store all-ones to `dst` if equal, zero otherwise.",
        )

    @my_macro(env)
    def vslt(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vslt_{name} (__m128i a, __m128i b)",
            instr=f"vslt.{name} vr, vr, vr",
            desc=f"Compare the {signedness} {width}-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than `b`, zero otherwise.",
        )

    @my_macro(env)
    def vslti(name):
        width = widths[name]
        signedness = signednesses[name]
        if signedness == "signed":
            imm_range = "imm_n16_15"
        else:
            imm_range = "imm0_31"
        return instruction(
            intrinsic=f"__m128i __lsx_vslti_{name} (__m128i a, {imm_range} imm)",
            instr=f"vslti.{name} vr, vr, imm",
            desc=f"Compare the {signedness} {width}-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than `b`, zero otherwise.",
        )

    @my_macro(env)
    def vsle(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vsle_{name} (__m128i a, __m128i b)",
            instr=f"vsle.{name} vr, vr, vr",
            desc=f"Compare the {signedness} {width}-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than or equal `b`, zero otherwise.",
        )

    @my_macro(env)
    def vslei(name):
        width = widths[name]
        signedness = signednesses[name]
        if signedness == "signed":
            imm_range = "imm_n16_15"
        else:
            imm_range = "imm0_31"
        return instruction(
            intrinsic=f"__m128i __lsx_vslei_{name} (__m128i a, {imm_range} imm)",
            instr=f"vslei.{name} vr, vr, imm",
            desc=f"Compare the {signedness} {width}-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than or equal `b`, zero otherwise.",
        )

    @my_macro(env)
    def vsadd(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vsadd_{name} (__m128i a, __m128i b)",
            instr=f"vsadd.{name} vr, vr, vr",
            desc=f"Saturing add the {signedness} {width}-bit elements in `a` and `b`, store the result to `dst`.",
        )

    @my_macro(env)
    def vssub(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vssub_{name} (__m128i a, __m128i b)",
            instr=f"vssub.{name} vr, vr, vr",
            desc=f"Saturing subtract the {signedness} {width}-bit elements in `a` and `b`, store the result to `dst`.",
        )

    @my_macro(env)
    def vsll(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vsll_{name} (__m128i a, __m128i b)",
            instr=f"vsll.{name} vr, vr, vr",
            desc=f"Logical left shift the unsigned {width}-bit elements in `a` by elements in `b`, store the result to `dst`.",
        )

    @my_macro(env)
    def vslli(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vslli_{name} (__m128i a, imm0_{width-1} imm)",
            instr=f"vslli.{name} vr, vr, imm",
            desc=f"Logical left shift the unsigned {width}-bit elements in `a` by `imm`, store the result to `dst`.",
        )

    @my_macro(env)
    def vsrl(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vsrl_{name} (__m128i a, __m128i b)",
            instr=f"vsrl.{name} vr, vr, vr",
            desc=f"Logical right shift the unsigned {width}-bit elements in `a` by elements in `b`, store the result to `dst`.",
        )

    @my_macro(env)
    def vsrli(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vsrli_{name} (__m128i a, imm0_{width-1} imm)",
            instr=f"vsrli.{name} vr, vr, imm",
            desc=f"Logical right shift the unsigned {width}-bit elements in `a` by `imm`, store the result to `dst`.",
        )

    @my_macro(env)
    def vsra(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vsra_{name} (__m128i a, __m128i b)",
            instr=f"vsra.{name} vr, vr, vr",
            desc=f"Arithmetic right shift the signed {width}-bit elements in `a` by elements in `b`, store the result to `dst`.",
        )

    @my_macro(env)
    def vsrai(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vsrai_{name} (__m128i a, imm0_{width-1} imm)",
            instr=f"vsrai.{name} vr, vr, imm",
            desc=f"Arithmetic right shift the signed {width}-bit elements in `a` by `imm`, store the result to `dst`.",
        )

    @my_macro(env)
    def vrotr(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vrotr_{name} (__m128i a, __m128i b)",
            instr=f"vrotr.{name} vr, vr, vr",
            desc=f"Rotate right the unsigned {width}-bit elements in `a` by elements in `b`, store the result to `dst`.",
        )

    @my_macro(env)
    def vrotri(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vrotri_{name} (__m128i a, imm0_{width-1} imm)",
            instr=f"vrotri.{name} vr, vr, imm",
            desc=f"Rotate right the unsigned {width}-bit elements in `a` by `imm`, store the result to `dst`.",
        )

    @my_macro(env)
    def vsrlr(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vsrlr_{name} (__m128i a, __m128i b)",
            instr=f"vsrlr.{name} vr, vr, vr",
            desc=f"Logical right shift (with rounding) the unsigned {width}-bit elements in `a` by elements in `b`, store the result to `dst`.",
        )

    @my_macro(env)
    def vsrlri(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vsrlri_{name} (__m128i a, imm0_{width-1} imm)",
            instr=f"vsrlri.{name} vr, vr, imm",
            desc=f"Logical right shift (with rounding) the unsigned {width}-bit elements in `a` by `imm`, store the result to `dst`.",
        )

    @my_macro(env)
    def vsrar(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vsrar_{name} (__m128i a, __m128i b)",
            instr=f"vsrar.{name} vr, vr, vr",
            desc=f"Arithmetic right shift (with rounding) the signed {width}-bit elements in `a` by elements in `b`, store the result to `dst`.",
        )

    @my_macro(env)
    def vsrari(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vsrari_{name} (__m128i a, imm0_{width-1} imm)",
            instr=f"vsrari.{name} vr, vr, imm",
            desc=f"Arithmetic right shift (with rounding) the signed {width}-bit elements in `a` by `imm`, store the result to `dst`.",
        )

    @my_macro(env)
    def vpackev(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vpackev_{name} (__m128i a, __m128i b)",
            instr=f"vpackev.{name} vr, vr, vr",
            desc=f"Collect and pack even-positioned {width}-bit elements in `a` and `b` and store `dst`.",
        )

    @my_macro(env)
    def vpackod(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vpackod_{name} (__m128i a, __m128i b)",
            instr=f"vpackod.{name} vr, vr, vr",
            desc=f"Collect and pack odd-positioned {width}-bit elements in `a` and `b` and store `dst`.",
        )

    @my_macro(env)
    def vpickev(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vpickev_{name} (__m128i a, __m128i b)",
            instr=f"vpickev.{name} vr, vr, vr",
            desc=f"Pick even-positioned {width}-bit elements in `b` first, then pick even-positioned {width}-bit elements in `a`.",
        )

    @my_macro(env)
    def vpickod(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vpickod_{name} (__m128i a, __m128i b)",
            instr=f"vpickod.{name} vr, vr, vr",
            desc=f"Pick odd-positioned {width}-bit elements in `b` first, then pick odd-positioned {width}-bit elements in `a`.",
        )

    @my_macro(env)
    def vreplve(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vreplve_{name} (__m128i a, int idx)",
            instr=f"vreplve.{name} vr, vr, r",
            desc=f"Repeat the element in lane `idx` of `a` to fill whole vector.",
        )

    @my_macro(env)
    def vreplvei(name):
        width = widths[name]
        imm_upper = 128 // width - 1
        return instruction(
            intrinsic=f"__m128i __lsx_vreplvei_{name} (__m128i a, imm0_{imm_upper} idx)",
            instr=f"vreplvei.{name} vr, vr, imm",
            desc=f"Repeat the element in lane `idx` of `a` to fill whole vector.",
        )

    @env.macro
    def xvrepl128vei(name):
        width = widths[name]
        imm_upper = 128 // width - 1
        return instruction(
            intrinsic=f"__m256i __lasx_xvrepl128vei_{name} (__m256i a, imm0_{imm_upper} idx)",
            instr=f"xvrepl128vei.{name} xr, xr, imm",
            desc=f"Repeat the element in lane `idx` of `a` to fill whole vector.",
        )

    @my_macro(env)
    def vrepli(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vrepli_{name} (imm_n512_511 imm)",
            instr=f"vldi vr, imm",
            desc=f"Repeat `imm` to fill whole vector.",
        )

    @my_macro(env)
    def vreplgr2vr(name):
        width = widths[name]
        if name == "d":
            long = "long "
        else:
            long = ""
        return instruction(
            intrinsic=f"__m128i __lsx_vreplgr2vr_{name} ({long}int val)",
            instr=f"vreplgr2vr.{name} vr, r",
            desc=f"Repeat `val` to whole vector.",
        )

    @my_macro(env)
    def vsigncov(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vsigncov_{name} (__m128i a, __m128i b)",
            instr=f"vsigncov.{name} vr, vr, vr",
            desc=f"If the {width}-bit element in `a` equals to zero, set the result to zero. If the signed {width}-bit element in `a` is positive, copy element in `b` to result. Otherwise, copy negated element in `b` to result. If `a` and `b` are the same vectors, it is equivalent to computing absolute value.",
        )

    @my_macro(env)
    def vneg(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vneg_{name} (__m128i a)",
            instr=f"vneg.{name} vr, vr",
            desc=f"Negate {width}-bit elements in `a` and save the result in `dst`.",
        )

    @my_macro(env)
    def vmskltz(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vmskltz_{name} (__m128i a)",
            instr=f"vmskltz.{name} vr, vr",
            desc=f"For each {width}-bit element in `a`, if the element is less than zero, set one bit in `dst`, otherwise clear it.",
        )

    @my_macro(env)
    def vmskgez(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vmskgez_{name} (__m128i a)",
            instr=f"vmskgez.{name} vr, vr",
            desc=f"For each {width}-bit element in `a`, if the element is greater than or equal to zero, set one bit in `dst`, otherwise clear it.",
        )

    @my_macro(env)
    def vmsknz(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vmsknz_{name} (__m128i a)",
            instr=f"vmsknz.{name} vr, vr",
            desc=f"For each {width}-bit element in `a`, if the element is non-zero, set one bit in `dst`, otherwise clear it.",
        )

    @my_macro(env)
    def vpickve2gr(name):
        width = widths[name]
        return_type = {
            "b": "int",
            "bu": "unsigned int",
            "h": "int",
            "hu": "unsigned int",
            "w": "int",
            "wu": "unsigned int",
            "d": "long int",
            "du": "unsigned long int",
        }[name]
        global cur_vlen
        return instruction(
            intrinsic=f"{return_type} __lsx_vpickve2gr_{name} (__m128i a, imm0_{cur_vlen // width - 1} idx)",
            instr=f"vpickve2gr.{name} r, vr, imm",
            desc=f"Pick the `lane` specified by `idx` from `a` and store into `dst`.",
        )

    @my_macro(env)
    def vshuf4i(name):
        width = widths[name]
        if name == "d":
            b = " __m128i b,"
            b_desc = " and `b`"
        else:
            b = ""
            b_desc = ""
        return instruction(
            intrinsic=f"__m128i __lsx_vshuf4i_{name} (__m128i a,{b} imm0_255 imm)",
            instr=f"vshuf4i.{name} vr, vr, imm",
            desc=f"Shuffle every four {width}-bit elements in `a`{b_desc} with indices packed in `imm`, save the result to `dst`.",
        )

    @my_macro(env)
    def bz_v():
        return instruction(
            intrinsic=f"int __lsx_bz_v (__m128i a)",
            instr=f"vseteqz.v fcc, vr; bcnez",
            desc=f"Expected to be used in branches: branch if the whole vector `a` equals to zero.",
        )

    @my_macro(env)
    def bnz_v():
        return instruction(
            intrinsic=f"int __lsx_bnz_v (__m128i a)",
            instr=f"vsetnez.v fcc, vr; bcnez",
            desc=f"Expected to be used in branches: branch if the whole vector `a` is non-zero.",
        )

    @my_macro(env)
    def bz(name):
        width = widths[name]
        return instruction(
            intrinsic=f"int __lsx_bz_{name} (__m128i a)",
            instr=f"vsetanyeqz.{name} fcc, vr; bcnez",
            desc=f"Expected to be used in branches: branch if any {width}-bit element in `a` equals to zero.",
        )

    @my_macro(env)
    def bnz(name):
        width = widths[name]
        return instruction(
            intrinsic=f"int __lsx_bnz_{name} (__m128i a)",
            instr=f"vsetallnez.{name} fcc, vr; bcnez",
            desc=f"Expected to be used in branches: branch if all {width}-bit elements in `a` are non-zero.",
        )

    @my_macro(env)
    def vsllwil(name, name2):
        width = widths[name[0]]
        width2 = widths[name2[0]]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vsllwil_{name}_{name2} (__m128i a, imm0_{width2-1} imm)",
            instr=f"vsllwil.{name}.{name2} vr, vr, imm",
            desc=f"Extend and shift {signedness} {width2}-bit elements in `a` by `imm` to {signedness} {width}-bit result.",
        )

    @my_macro(env)
    def vsrln(name, name2):
        width = widths[name[0]]
        width2 = widths[name2[0]]
        return instruction(
            intrinsic=f"__m128i __lsx_vsrln_{name}_{name2} (__m128i a, __m128i b)",
            instr=f"vsrln.{name}.{name2} vr, vr, vr",
            desc=f"Logical right shift the unsigned {width2}-bit elements in `a` by elements in `b`, truncate to {width}-bit and store the result to `dst`.",
        )

    @my_macro(env)
    def vsran(name, name2):
        width = widths[name[0]]
        width2 = widths[name2[0]]
        return instruction(
            intrinsic=f"__m128i __lsx_vsran_{name}_{name2} (__m128i a, __m128i b)",
            instr=f"vsran.{name}.{name2} vr, vr, vr",
            desc=f"Arithmetic right shift the signed {width2}-bit elements in `a` by elements in `b`, truncate to {width}-bit and store the result to `dst`.",
        )

    @my_macro(env)
    def vsrlni(name, name2):
        width = widths[name[0]]
        width2 = widths[name2[0]]
        return instruction(
            intrinsic=f"__m128i __lsx_vsrlni_{name}_{name2} (__m128i a, __m128i b, imm0_{width2-1} imm)",
            instr=f"vsrlni.{name}.{name2} vr, vr, imm",
            desc=f"Logical right shift the unsigned {width2}-bit elements in `a` and `b` by `imm`, truncate to {width}-bit and store the result to `dst`.",
        )

    @my_macro(env)
    def vsrani(name, name2):
        width = widths[name[0]]
        width2 = widths[name2[0]]
        return instruction(
            intrinsic=f"__m128i __lsx_vsrani_{name}_{name2} (__m128i a, __m128i b, imm0_{width2-1} imm)",
            instr=f"vsrani.{name}.{name2} vr, vr, imm",
            desc=f"Arithmetic right shift the signed {width2}-bit elements in `a` and `b` by `imm`, truncate to {width}-bit and store the result to `dst`.",
        )

    @my_macro(env)
    def vsrlrn(name, name2):
        width = widths[name[0]]
        width2 = widths[name2[0]]
        return instruction(
            intrinsic=f"__m128i __lsx_vsrlrn_{name}_{name2} (__m128i a, __m128i b)",
            instr=f"vsrlrn.{name}.{name2} vr, vr, vr",
            desc=f"Logical right shift (with rounding) the unsigned {width2}-bit elements in `a` by elements in `b`, truncate to {width}-bit and store the result to `dst`.",
        )

    @my_macro(env)
    def vsrarn(name, name2):
        width = widths[name[0]]
        width2 = widths[name2[0]]
        return instruction(
            intrinsic=f"__m128i __lsx_vsrarn_{name}_{name2} (__m128i a, __m128i b)",
            instr=f"vsrarn.{name}.{name2} vr, vr, vr",
            desc=f"Arithmetic right shift (with rounding) the signed {width2}-bit elements in `a` by elements in `b`, truncate to {width}-bit and store the result to `dst`.",
        )

    @my_macro(env)
    def vsrlrni(name, name2):
        width = widths[name[0]]
        width2 = widths[name2[0]]
        return instruction(
            intrinsic=f"__m128i __lsx_vsrlrni_{name}_{name2} (__m128i a, __m128i b, imm0_{width2-1} imm)",
            instr=f"vsrlrni.{name}.{name2} vr, vr, imm",
            desc=f"Logical right shift (with rounding) the unsigned {width2}-bit elements in `a` and `b` by `imm`, truncate to {width}-bit and store the result to `dst`.",
        )

    @my_macro(env)
    def vsrarni(name, name2):
        width = widths[name[0]]
        width2 = widths[name2[0]]
        return instruction(
            intrinsic=f"__m128i __lsx_vsrarni_{name}_{name2} (__m128i a, __m128i b, imm0_{width2-1} imm)",
            instr=f"vsrarni.{name}.{name2} vr, vr, imm",
            desc=f"Arithmetic right shift (with rounding) the signed {width2}-bit elements in `a` and `b` by `imm`, truncate to {width}-bit and store the result to `dst`.",
        )

    @my_macro(env)
    def vssrln(name, name2):
        width = widths[name[0]]
        signedness = signednesses[name]
        width2 = widths[name2[0]]
        return instruction(
            intrinsic=f"__m128i __lsx_vssrln_{name}_{name2} (__m128i a, __m128i b)",
            instr=f"vssrln.{name}.{name2} vr, vr, vr",
            desc=f"Logical right shift the unsigned {width2}-bit elements in `a` by elements in `b`, clamp to fit in {signedness} {width}-bit integer and store the result to `dst`.",
        )

    @my_macro(env)
    def vssran(name, name2):
        width = widths[name[0]]
        signedness = signednesses[name]
        width2 = widths[name2[0]]
        return instruction(
            intrinsic=f"__m128i __lsx_vssran_{name}_{name2} (__m128i a, __m128i b)",
            instr=f"vssran.{name}.{name2} vr, vr, vr",
            desc=f"Arithmetic right shift the signed {width2}-bit elements in `a` by elements in `b`, clamp to fit in {signedness} {width}-bit integer and store the result to `dst`.",
        )

    @my_macro(env)
    def vssrlni(name, name2):
        width = widths[name[0]]
        signedness = signednesses[name]
        width2 = widths[name2[0]]
        return instruction(
            intrinsic=f"__m128i __lsx_vssrlni_{name}_{name2} (__m128i a, __m128i b, imm0_{width2-1} imm)",
            instr=f"vssrlni.{name}.{name2} vr, vr, imm",
            desc=f"Logical right shift the unsigned {width2}-bit elements in `a` and `b` by `imm`, clamp to fit in {signedness} {width}-bit integer and store the result to `dst`.",
        )

    @my_macro(env)
    def vssrani(name, name2):
        width = widths[name[0]]
        signedness = signednesses[name]
        width2 = widths[name2[0]]
        return instruction(
            intrinsic=f"__m128i __lsx_vssrani_{name}_{name2} (__m128i a, __m128i b, imm0_{width2-1} imm)",
            instr=f"vssrani.{name}.{name2} vr, vr, imm",
            desc=f"Arithmetic right shift the signed {width2}-bit elements in `a` and `b` by `imm`, clamp to fit in {signedness} {width}-bit integer and store the result to `dst`.",
        )

    @my_macro(env)
    def vssrlrn(name, name2):
        width = widths[name[0]]
        signedness = signednesses[name]
        width2 = widths[name2[0]]
        return instruction(
            intrinsic=f"__m128i __lsx_vssrlrn_{name}_{name2} (__m128i a, __m128i b)",
            instr=f"vssrlrn.{name}.{name2} vr, vr, vr",
            desc=f"Logical right shift (with rounding) the unsigned {width2}-bit elements in `a` by elements in `b`, clamp to fit in {signedness} {width}-bit integer and store the result to `dst`.",
        )

    @my_macro(env)
    def vssrarn(name, name2):
        width = widths[name[0]]
        signedness = signednesses[name]
        width2 = widths[name2[0]]
        return instruction(
            intrinsic=f"__m128i __lsx_vssrarn_{name}_{name2} (__m128i a, __m128i b)",
            instr=f"vssrarn.{name}.{name2} vr, vr, vr",
            desc=f"Arithmetic right shift (with rounding) the signed {width2}-bit elements in `a` by elements in `b`, clamp to fit in {signedness} {width}-bit integer and store the result to `dst`.",
        )

    @my_macro(env)
    def vssrlrni(name, name2):
        width = widths[name[0]]
        signedness = signednesses[name]
        width2 = widths[name2[0]]
        return instruction(
            intrinsic=f"__m128i __lsx_vssrlrni_{name}_{name2} (__m128i a, __m128i b, imm0_{width2-1} imm)",
            instr=f"vssrlrni.{name}.{name2} vr, vr, imm",
            desc=f"Logical right shift (with rounding) the unsigned {width2}-bit elements in `a` and `b` by `imm`, clamp to fit in {signedness} {width}-bit integer and store the result to `dst`.",
        )

    @my_macro(env)
    def vssrarni(name, name2):
        width = widths[name[0]]
        signedness = signednesses[name]
        width2 = widths[name2[0]]
        return instruction(
            intrinsic=f"__m128i __lsx_vssrarni_{name}_{name2} (__m128i a, __m128i b, imm0_{width2-1} imm)",
            instr=f"vssrarni.{name}.{name2} vr, vr, imm",
            desc=f"Arithmetic right shift (with rounding) the signed {width2}-bit elements in `a` and `b` by `imm`, clamp to fit in {signedness} {width}-bit integer and store the result to `dst`.",
        )

    @my_macro(env)
    def vsat(name):
        width = widths[name]
        signedness = signednesses[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vsat_{name} (__m128i a, imm0_{width - 1} imm)",
            instr=f"vsat.{name} vr, vr, imm",
            desc=f"Clamp {signedness} {width}-bit elements in `a` to range specified by `imm`.",
        )

    def get_rounding_mode(rounding):
        if rounding == "":
            rounding_mode = "using current rounding mode specified in `fscr`"
        elif rounding == "rm":
            rounding_mode = "rounding towards negative infinity"
        elif rounding == "rp":
            rounding_mode = "rounding towards positive infinity"
        elif rounding == "rz":
            rounding_mode = "rounding towards zero"
        elif rounding == "rne":
            rounding_mode = "rounding towards nearest even"
        else:
            assert False
        return rounding_mode

    @my_macro(env)
    def vftint_l_s(rounding, low_high):
        if low_high == "l":
            half = "lower"
        else:
            half = "higher"
        rounding_mode = get_rounding_mode(rounding)
        return instruction(
            intrinsic=f"__m128i __lsx_vftint{rounding}{low_high}_l_s (__m128 a)",
            instr=f"vftint{rounding}{low_high}.l.s vr, vr",
            desc=f"Convert single-precision floating point elements in {half} part of `a` to 64-bit integer, {rounding_mode}.",
        )

    @my_macro(env)
    def vftint_w_d(rounding):
        rounding_mode = get_rounding_mode(rounding)
        return instruction(
            intrinsic=f"__m128i __lsx_vftint{rounding}_w_d (__m128d a, __m128d b)",
            instr=f"vftint{rounding}.w.d vr, vr, vr",
            desc=f"Convert double-precision floating point elements in `a` and `b` to 32-bit integer, {rounding_mode}.",
        )

    @my_macro(env)
    def vftint(rounding, name, name2):
        if name2 == "d":
            arg_type = "__m128d"
            precision = "double"
            int_width = 64
        else:
            arg_type = "__m128"
            precision = "single"
            int_width = 32
        rounding_mode = get_rounding_mode(rounding)
        if len(name) == 1:
            signedness = "signed"
        else:
            signedness = "unsigned"
        return instruction(
            intrinsic=f"__m128i __lsx_vftint{rounding}_{name}_{name2} ({arg_type} a)",
            instr=f"vftint{rounding}.{name}.{name2} vr, vr",
            desc=f"Convert {precision}-precision floating point elements in `a` to {signedness} {int_width}-bit integer, {rounding_mode}.",
        )

    @my_macro(env)
    def vfop(desc, name, ty):
        if ty == "s":
            precision = "single"
            arg_type = "__m128"
        else:
            precision = "double"
            arg_type = "__m128d"
        return instruction(
            intrinsic=f"{arg_type} __lsx_vf{name}_{ty} ({arg_type} a)",
            instr=f"vf{name}.{ty} vr, vr",
            desc=f"Compute {desc} of {precision} precision floating point elements in `a`.",
        )

    @my_macro(env)
    def vmepatmsk():
        return instruction(
            intrinsic=f"__m128i __lsx_vmepatmsk_v (int mode, int uimm5)",
            instr=f"vmepatmsk.v vr, mode, uimm5",
            desc=f"Compute pattern according to `mode`, then add `uimm5` to each element.",
        )

    @my_macro(env)
    def vfrint(rounding, name):
        if name == "s":
            precision = "single"
            arg_type = "__m128"
        else:
            precision = "double"
            arg_type = "__m128d"
        rounding_mode = get_rounding_mode(rounding)
        return instruction(
            intrinsic=f"{arg_type} __lsx_vfrint{rounding}_{name} ({arg_type} a)",
            instr=f"vfrint{rounding}.{name} vr, vr",
            desc=f"Round single-precision floating point elements in `a` to integers, {rounding_mode}, and store as floating point numbers.",
        )

    @my_macro(env)
    def vffint_d_w(low_high):
        if low_high == "l":
            half = "lower"
        else:
            half = "higher"
        return instruction(
            intrinsic=f"__m128d __lsx_vffint{low_high}_d_w (__m128i a)",
            instr=f"vffint{low_high}.d.w vr, vr",
            desc=f"Convert 32-bit integer elements in {half} part of `a` to double precision floating point numbers.",
        )

    @my_macro(env)
    def vffint(name, name2):
        if name == "d":
            arg_type = "__m128d"
            precision = "double"
            int_width = 64
        else:
            arg_type = "__m128"
            precision = "single"
            int_width = 32
        if len(name2) == 1:
            signedness = "signed"
        else:
            signedness = "unsigned"
        return instruction(
            intrinsic=f"{arg_type} __lsx_vffint_{name}_{name2} (__m128i a)",
            instr=f"vffint.{name}.{name2} vr, vr",
            desc=f"Convert {signedness} {int_width}-bit integer elements in `a` to {precision}-precision floating point numbers.",
        )

    @my_macro(env)
    def vffint_s_l():
        return instruction(
            intrinsic=f"__m128 __lsx_vffint_s_l (__m128i a, __m128i b)",
            instr=f"vffint.s.l vr, vr, vr",
            desc=f"Convert 64-bit integer elements in `a` and `b` to single-precision floating point numbers.",
        )

    @my_macro(env)
    def vfrstp(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vfrstp_{name} (__m128i a, __m128i b, __m128i c)",
            instr=f"vfrstp.{name} vr, vr, vr",
            desc=f"Find the first negative {width}-bit element in `b`, set the index of the element to the lane of `a` specified by `c`.",
        )

    @my_macro(env)
    def vfrstpi(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vfrstpi_{name} (__m128i a, __m128i b, imm0_31 imm)",
            instr=f"vfrstpi.{name} vr, vr, imm",
            desc=f"Find the first negative {width}-bit element in `b`, set the index of the element to the lane of `a` specified by `imm`.",
        )

    @my_macro(env)
    def vbsll_srl(name, dir):
        return instruction(
            intrinsic=f"__m128i __lsx_vb{name}_v (__m128i a, imm0_31 imm)",
            instr=f"vb{name}.v vr, vr, imm",
            desc=f"Compute whole vector `a` shifted {dir} by `imm * 8` bits.",
        )

    @my_macro(env)
    def vpermi_w():
        return instruction(
            intrinsic=f"__m128i __lsx_vpermi_w (__m128i a, __m128i b, imm0_255 imm)",
            instr=f"vpermi.w vr, vr, imm",
            desc=f"Permute words from `a` and `b` with indices recorded in `imm` and store into `dst`.",
        )

    @env.macro
    def xvpermi_d():
        return instruction(
            intrinsic=f"__m256i __lasx_xvpermi_d (__m256i a, imm0_255 imm)",
            instr=f"xvpermi.d xr, xr, imm",
            desc=f"Permute double words from `a` with indices recorded in `imm` and store into `dst`.",
        )

    @env.macro
    def xvperm_w():
        return instruction(
            intrinsic=f"__m256i __lasx_xvperm_w (__m256i a, __m256i b)",
            instr=f"xvperm.w xr, xr, xr",
            desc=f"Permute words from `a` with indices recorded in `b` and store into `dst`.",
        )

    @env.macro
    def xvpermi_q():
        return instruction(
            intrinsic=f"__m256i __lasx_xvpermi_q (__m256i a, __m256i b, imm0_255 imm)",
            instr=f"xvpermi.q xr, xr, imm",
            desc=f"Permute quad words from `a` and `b` with indices recorded in `imm` and store into `dst`.",
        )

    @env.macro
    def xvpickve_int(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m256i __lasx_xvpickve_{name} (__m256i a, imm0_{256 // width - 1} imm)",
            instr=f"xvpickve.{name} xr, xr, imm",
            desc=f"Copy one {width}-bit lane from `a` specified by `imm` to the first lane of `dst`, and set the other lanes to zero.",
        )

    @env.macro
    def xvpickve_fp(name):
        width = widths[name]
        if name == "d":
            fp_type = "__m256d"
        else:
            fp_type = "__m256"
        return instruction(
            intrinsic=f"{fp_type} __lasx_xvpickve_{name}_f ({fp_type} a, imm0_{256 // width - 1} imm)",
            instr=f"xvpickve.{name} xr, xr, imm",
            desc=f"Copy one {width}-bit lane from `a` specified by `imm` to the first lane of `dst`, and set the other lanes to zero.",
        )

    @env.macro
    def xvreplve0(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m256i __lasx_xvreplve0_{name} (__m256i a)",
            instr=f"xvreplve0.{name} xr, xr",
            desc=f"Repeat the first {width}-bit lane from `a` to all lanes of `dst`.",
        )

    @my_macro(env)
    def vld():
        return instruction(
            intrinsic=f"__m128i __lsx_vld (void * addr, imm_n2048_2047 offset)",
            instr=f"vld vr, r, imm",
            desc=f"Read whole vector from memory address `addr + offset`, save the data into `dst`. Note that you can use this intrinsic to load floating point vectors, even though the return type represents integer vectors.",
        )

    @my_macro(env)
    def vldx():
        return instruction(
            intrinsic=f"__m128i __lsx_vldx (void * addr, long int offset)",
            instr=f"vldx vr, r, r",
            desc=f"Read whole vector from memory address `addr + offset`, save the data into `dst`.  Note that you can use this intrinsic to load floating point vectors, even though the return type represents integer vectors.",
        )

    @my_macro(env)
    def vst():
        return instruction(
            intrinsic=f"void __lsx_vst (__m128i data, void * addr, imm_n2048_2047 offset)",
            instr=f"vst vr, r, imm",
            desc=f"Write whole vector data in `data` to memory address `addr + offset`.",
        )

    @my_macro(env)
    def vstx():
        return instruction(
            intrinsic=f"void __lsx_vstx (__m128i data, void * addr, long int offset)",
            instr=f"vstx vr, r, r",
            desc=f"Write whole-vector data in `data` to memory address `addr + offset`.",
        )

    @my_macro(env)
    def vbitsel_v():
        return instruction(
            intrinsic=f"__m128i __lsx_vbitsel_v (__m128i a, __m128i b, __m128i c)",
            instr=f"vbitsel.v vr, vr, vr, vr",
            desc=f"Compute bitwise selection: for each bit position, if the bit in `c` equals to one, copy the bit from `b` to `dst`, otherwise copy from `a`.",
        )

    @my_macro(env)
    def vbitseli_b():
        return instruction(
            intrinsic=f"__m128i __lsx_vbitseli_b (__m128i a, __m128i b, imm0_255 imm)",
            instr=f"vbitseli.b vr, vr, imm",
            desc=f"Compute bitwise selection: for each bit position, if the bit in `a` equals to one, copy the bit from `imm` to `dst`, otherwise copy from `b`.",
        )

    @my_macro(env)
    def vfcvth_d_s():
        return instruction(
            intrinsic=f"__m128d __lsx_vfcvth_d_s (__m128 a)",
            instr=f"vfcvth.d.s vr, vr",
            desc=f"Convert single precision floating point elements in higher half of `a` to double precision.",
        )

    @my_macro(env)
    def vfcvtl_d_s():
        return instruction(
            intrinsic=f"__m128d __lsx_vfcvtl_d_s (__m128 a)",
            instr=f"vfcvtl.d.s vr, vr",
            desc=f"Convert single precision floating point elements in lower half of `a` to double precision.",
        )

    @my_macro(env)
    def vfcvt_s_d():
        return instruction(
            intrinsic=f"__m128 __lsx_vfcvt_s_d (__m128d a, __m128d b)",
            instr=f"vfcvt.s.d vr, vr, vr",
            desc=f"Convert double precision floating point elements in `a` and `b` to single precision.",
        )

    @my_macro(env)
    def vfcvth_s_h():
        return instruction(
            intrinsic=f"__m128 __lsx_vfcvth_s_h (__m128i a)",
            instr=f"vfcvth.s.h vr, vr",
            desc=f"Convert half precision floating point elements in higher half of `a` to single precision.",
        )

    @my_macro(env)
    def vfcvtl_s_h():
        return instruction(
            intrinsic=f"__m128 __lsx_vfcvtl_s_h (__m128i a)",
            instr=f"vfcvtl.s.h vr, vr",
            desc=f"Convert half precision floating point elements in lower half of `a` to single precision.",
        )

    @my_macro(env)
    def vfcvt_h_s():
        return instruction(
            intrinsic=f"__m128i __lsx_vfcvt_h_s (__m128 a, __m128 b)",
            instr=f"vfcvt.h.s vr, vr, vr",
            desc=f"Convert single precision floating point elements in `a` and `b` to half precision.",
        )

    @my_macro(env)
    def vfclass_d():
        return instruction(
            intrinsic=f"__m128i __lsx_vfclass_d (__m128d a)",
            instr=f"vfclass.d vr, vr",
            desc=f"Classifiy each double precision floating point elements in `a`.",
        )

    @my_macro(env)
    def vfclass_s():
        return instruction(
            intrinsic=f"__m128i __lsx_vfclass_s (__m128 a)",
            instr=f"vfclass.s vr, vr",
            desc=f"Classifiy each single precision floating point elements in `a`.",
        )

    @my_macro(env)
    def vfmadd_s():
        return instruction(
            intrinsic=f"__m128 __lsx_vfmadd_s (__m128 a, __m128 b, __m128 c)",
            instr=f"vfmadd.s vr, vr, vr, vr",
            desc=f"Compute packed single precision floating point FMA(Fused Multiply-Add): multiply elements in `a` and `b`, accumulate to elements in `c` and store the result in `dst`.",
        )

    @my_macro(env)
    def vfmadd_d():
        return instruction(
            intrinsic=f"__m128d __lsx_vfmadd_d (__m128d a, __m128d b, __m128d c)",
            instr=f"vfmadd.d vr, vr, vr, vr",
            desc=f"Compute packed double precision floating point FMA(Fused Multiply-Add): multiply elements in `a` and `b`, accumulate to elements in `c` and store the result in `dst`.",
        )

    @my_macro(env)
    def vfmsub_s():
        return instruction(
            intrinsic=f"__m128 __lsx_vfmsub_s (__m128 a, __m128 b, __m128 c)",
            instr=f"vfmsub.s vr, vr, vr, vr",
            desc=f"Compute packed single precision floating point FMA(Fused Multiply-Add): multiply elements in `a` and `b`, subtract elements in `c` and store the result in `dst`.",
        )

    @my_macro(env)
    def vfmsub_d():
        return instruction(
            intrinsic=f"__m128d __lsx_vfmsub_d (__m128d a, __m128d b, __m128d c)",
            instr=f"vfmsub.d vr, vr, vr, vr",
            desc=f"Compute packed double precision floating point FMA(Fused Multiply-Add): multiply elements in `a` and `b`, subtract elements in `c` and store the result in `dst`.",
        )

    @my_macro(env)
    def vfnmadd_s():
        return instruction(
            intrinsic=f"__m128 __lsx_vfnmadd_s (__m128 a, __m128 b, __m128 c)",
            instr=f"vfnmadd.s vr, vr, vr, vr",
            desc=f"Compute packed single precision floating point FMA(Fused Multiply-Add): multiply elements in `a` and `b`, accumulate to elements in `c` and store the negated result in `dst`.",
        )

    @my_macro(env)
    def vfnmadd_d():
        return instruction(
            intrinsic=f"__m128d __lsx_vfnmadd_d (__m128d a, __m128d b, __m128d c)",
            instr=f"vfnmadd.d vr, vr, vr, vr",
            desc=f"Compute packed double precision floating point FMA(Fused Multiply-Add): multiply elements in `a` and `b`, accumulate to elements in `c` and store the negated result in `dst`.",
        )

    @my_macro(env)
    def vfnmsub_s():
        return instruction(
            intrinsic=f"__m128 __lsx_vfnmsub_s (__m128 a, __m128 b, __m128 c)",
            instr=f"vfnmsub.s vr, vr, vr, vr",
            desc=f"Compute packed single precision floating point FMA(Fused Multiply-Add): multiply elements in `a` and `b`, subtract elements in `c` and store the negated result in `dst`.",
        )

    @my_macro(env)
    def vfnmsub_d():
        return instruction(
            intrinsic=f"__m128d __lsx_vfnmsub_d (__m128d a, __m128d b, __m128d c)",
            instr=f"vfnmsub.d vr, vr, vr, vr",
            desc=f"Compute packed double precision floating point FMA(Fused Multiply-Add): multiply elements in `a` and `b`, subtract elements in `c` and store the negated result in `dst`.",
        )

    @my_macro(env)
    def vldi():
        return instruction(
            intrinsic=f"__m128i __lsx_vldi (imm_n1024_1023 imm)",
            instr=f"vldi vr, imm",
            desc=f"""
Initialize `dst` using predefined patterns:

- `imm[12:10]=0b000`: broadcast `imm[7:0]` as 8-bit elements to all lanes
- `imm[12:10]=0b001`: broadcast sign-extended `imm[9:0]` as 16-bit elements to all lanes
- `imm[12:10]=0b010`: broadcast sign-extended `imm[9:0]` as 32-bit elements to all lanes
- `imm[12:10]=0b011`: broadcast sign-extended `imm[9:0]` as 64-bit elements to all lanes
- `imm[12:8]=0b10000`: broadcast `imm[7:0]` as 32-bit elements to all lanes
- `imm[12:8]=0b10001`: broadcast `imm[7:0] << 8` as 32-bit elements to all lanes
- `imm[12:8]=0b10010`: broadcast `imm[7:0] << 16` as 32-bit elements to all lanes
- `imm[12:8]=0b10011`: broadcast `imm[7:0] << 24` as 32-bit elements to all lanes
- `imm[12:8]=0b10100`: broadcast `imm[7:0]` as 16-bit elements to all lanes
- `imm[12:8]=0b10101`: broadcast `imm[7:0] << 8` as 16-bit elements to all lanes
- `imm[12:8]=0b10110`: broadcast `(imm[7:0] << 8) | 0xFF` as 32-bit elements to all lanes
- `imm[12:8]=0b10111`: broadcast `(imm[7:0] << 16) | 0xFFFF` as 32-bit elements to all lanes
- `imm[12:8]=0b11000`: broadcast `imm[7:0]` as 8-bit elements to all lanes
- `imm[12:8]=0b11001`: repeat each bit of `imm[7:0]` eight times, and broadcast the result as 64-bit elements to all lanes
- `imm[12:8]=0b11010`: broadcast `(imm[7] << 31) | ((1-imm[6]) << 30) | ((imm[6] * 0x1F) << 25) | (imm[5:0] << 19)` as 32-bit elements to all lanes
- `imm[12:8]=0b11011`: broadcast `(imm[7] << 31) | ((1-imm[6]) << 30) | ((imm[6] * 0x1F) << 25) | (imm[5:0] << 19)` as 64-bit elements to all lanes
- `imm[12:8]=0b11100`: broadcast `(imm[7] << 63) | ((1-imm[6]) << 62) | ((imm[6] * 0xFF) << 54) | (imm[5:0] << 48)` as 64-bit elements to all lanes
""",
        )

    @my_macro(env)
    def vfscale(desc, ty):
        name = "scaleb"
        if ty == "s":
            precision = "single"
            arg_type = "__m128"
        else:
            precision = "double"
            arg_type = "__m128d"
        return instruction(
            intrinsic=f"{arg_type} __lsx_vf{name}_{ty} ({arg_type} a, __m128i b)",
            instr=f"vf{name}.{ty} vr, vr, vr",
            desc=f"Compute {desc} of {precision} precision floating point elements in `a` by integer elements in `b`. Currently undocumented.",
        )

    @env.macro
    def xvhseli_d():
        return instruction(
            intrinsic=f"__m256i __lasx_xvhseli_d (__m256i a, imm0_31 imm)",
            instr=f"xvhseli.d xr, xr, imm",
            desc=f"Select double words from `a` with indices recorded in `imm` and store into `dst`.",
        )

    @env.macro
    def all_intrinsics(render=True):
        result = []
        for file in glob.glob("docs/*/*.md"):
            for line in open(file, "r"):
                if line.startswith("#"):
                    title = line[1:].strip()
                if "{{" in line and "}}" in line:
                    code = line.split("{{")[1].split("}}")[0]
                    # Dangerous! But we trust ourselves.
                    content = eval(code, env.macros)
                    # handle cases for multiple instructions in one call
                    for part in re.split(r"^## ", content, flags=re.MULTILINE):
                        if len(part.strip()) == 0:
                            continue
                        lines = ("## " + part).split("\n")
                        for i in range(len(lines)):
                            line = lines[i]
                            if line.startswith("##"):
                                body = "\n".join(lines[i + 1 :])
                                intrinsic = line[2:].strip()
                                if "_lsx_" in intrinsic:
                                    extension = "LSX"
                                else:
                                    extension = "LASX"

                                if render:
                                    result.append(
                                        {
                                            "name": intrinsic,
                                            "markdown": body.strip(),
                                            "content": markdown.markdown(
                                                body.strip(),
                                                extensions=[
                                                    "fenced_code",
                                                    "codehilite",
                                                    "tables",
                                                ],
                                            ),
                                            "group": title,
                                            "extension": extension,
                                            "display": True
                                        }
                                    )
                                else:
                                    insts = []
                                    for line in body.split("\n"):
                                        if line.startswith("Instruction:"):
                                            # check instruction format
                                            inst = line[12:].strip()
                                            for name in inst.split(";"):
                                                insts.append(name.strip().split(" ")[0])

                                    # do not render
                                    result.append(
                                        {
                                            "name": intrinsic.split("(")[0]
                                            .strip()
                                            .split(" ")[-1],
                                            "c_intrinsic": intrinsic,
                                            "instructions": insts,
                                            "content": body.strip(),
                                            "group": title,
                                            "extension": extension,
                                        }
                                    )
                                break
        return json.dumps(sorted(result, key=lambda e: e["name"]))

    @env.macro
    def all_groups():
        result = []
        for file in glob.glob("docs/*/*.md"):
            for line in open(file, "r"):
                if line.startswith("#"):
                    title = line[1:].strip()
                    result.append(title)
                    break
        return json.dumps(sorted(list(set(result))))

    @env.macro
    def latency_throughput_table():
        result = "<table><thead><tr><th rowspan=2>Instruction</th>"
        for cpu in cpus:
            result += f"<th colspan=2>{cpu}</th>"
        result += f"</tr><tr>"
        for cpu in cpus:
            result += f"<th>Latency</th><th>Throughput (IPC)</th>"
        result += f"</tr></thead>"

        result += "<tbody>"

        insts = []
        for cpu in cpus:
            insts.extend(measure[cpu].keys())
        insts = sorted(list(set(insts)))

        for inst in insts:
            result += "<tr>"
            result += f"<td>{inst.replace('_', '.')}</td>"
            for cpu in cpus:
                if inst in measure[cpu]:
                    result += f"<td>{measure[cpu][inst]['latency']}</td>"
                    result += f"<td>{measure[cpu][inst]['throughput(ipc)']}</td>"
                else:
                    result += f"<td></td>"
                    result += f"<td></td>"
            result += "</tr>"

        result += "</tbody>"
        return result


if __name__ == "__main__":
    # Fake an env
    class Env:
        def __init__(self):
            self.macros = {}

        def macro(self, fn):
            self.macros[fn.__name__] = fn

    env = Env()
    define_env(env)
    result = env.macros["all_intrinsics"](render=False)
    json.dump(json.loads(result), open("intrinsics.json", "w"), indent=True)
