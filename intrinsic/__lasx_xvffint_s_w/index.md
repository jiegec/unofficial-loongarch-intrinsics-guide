# \_\_lasx_xvffint_s_w

LASX intrinsic · group: [Floating Point Conversion](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_conversion/index.md)

### Synopsis

```
__m256 __lasx_xvffint_s_w (__m256i a)
#include <lasxintrin.h>
Instruction: xvffint.s.w xr, xr
CPU Flags: LASX
```

### Description

Convert signed 32-bit integer elements in `a` to single-precision floating point numbers.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = (f32)(s32)a.word[i]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.fp32[0] = (f32)((s32)a.word[0]);
  dst.fp32[1] = (f32)((s32)a.word[1]);
  dst.fp32[2] = (f32)((s32)a.word[2]);
  dst.fp32[3] = (f32)((s32)a.word[3]);
  dst.fp32[4] = (f32)((s32)a.word[4]);
  dst.fp32[5] = (f32)((s32)a.word[5]);
  dst.fp32[6] = (f32)((s32)a.word[6]);
  dst.fp32[7] = (f32)((s32)a.word[7]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 4                |
| 3C6000 | LA664 | 4       | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
