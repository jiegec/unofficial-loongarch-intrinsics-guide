# \_\_lasx_xvfrintrp_s

LASX intrinsic · group: [Floating Point Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_misc/index.md)

### Synopsis

```
__m256 __lasx_xvfrintrp_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvfrintrp.s xr, xr
CPU Flags: LASX
```

### Description

Round single-precision floating point elements in `a` to integers, rounding towards positive infinity, and store as floating point numbers.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = (f32)(s32)a.fp32[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
