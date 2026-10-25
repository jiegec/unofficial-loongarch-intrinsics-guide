# \_\_lsx_vfrintrp_s

LSX intrinsic · group: [Floating Point Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_misc/index.md)

### Synopsis

```
__m128 __lsx_vfrintrp_s (__m128 a)
#include <lsxintrin.h>
Instruction: vfrintrp.s vr, vr
CPU Flags: LSX
```

### Description

Round single-precision floating point elements in `a` to integers, rounding towards positive infinity, and store as floating point numbers.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = (f32)(s32)a.fp32[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
