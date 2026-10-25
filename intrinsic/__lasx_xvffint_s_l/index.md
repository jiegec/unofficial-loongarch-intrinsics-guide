# \_\_lasx_xvffint_s_l

LASX intrinsic · group: [Floating Point Conversion](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_conversion/index.md)

### Synopsis

```
__m256 __lasx_xvffint_s_l (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvffint.s.l xr, xr, xr
CPU Flags: LASX
```

### Description

For each 128-bit lane, convert signed 64-bit integer elements in the corresponding lane of `a` into the upper half and elements in `b` into the lower half, as single-precision floating point numbers.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] =
      (i < 2) ? (f32)(s64)b.dword[i]
              : (f32)(s64)a.dword[i - 2]; // rounding mode is not expressed in C
}
for (int i = 4; i < 8; i++) {
  dst.fp32[i] =
      (i < 6) ? (f32)(s64)b.dword[i - 2]
              : (f32)(s64)a.dword[i - 4]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.fp32[0] = (f32)((s64)b.dword[0]);
  dst.fp32[1] = (f32)((s64)b.dword[1]);
  dst.fp32[2] = (f32)((s64)a.dword[0]);
  dst.fp32[3] = (f32)((s64)a.dword[1]);
  dst.fp32[4] = (f32)((s64)b.dword[2]);
  dst.fp32[5] = (f32)((s64)b.dword[3]);
  dst.fp32[6] = (f32)((s64)a.dword[2]);
  dst.fp32[7] = (f32)((s64)a.dword[3]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 1                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
