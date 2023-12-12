import math

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
"""

    @env.macro
    def vadd(name):
        width = widths[name]
        return instruction(
            intrinsic=f"__m128i __lsx_vadd_{name} (__m128i a, __m128i b)",
            instr=f"vadd.{name} vr, vr, vr",
            desc=f"Add {width}-bit elements in `a` and `b`, save the result in `dst`.",
        )

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
