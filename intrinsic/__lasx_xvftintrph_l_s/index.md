# \_\_lasx_xvftintrph_l_s

LASX intrinsic · group: [Floating Point Conversion](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_conversion/index.md)

### Synopsis

```
__m256i __lasx_xvftintrph_l_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvftintrph.l.s xr, xr
CPU Flags: LASX
```

### Description

Convert single-precision floating point elements in higher part of every 128-bit vector of `a` to signed 64-bit integer, rounding towards positive infinity.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)a.fp32[i + 2]; // rounding mode is not expressed in C
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = (s64)a.fp32[i + 4]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.dword[0] = (s64)a.fp32[2];
  dst.dword[1] = (s64)a.fp32[3];
  dst.dword[2] = (s64)a.fp32[6];
  dst.dword[3] = (s64)a.fp32[7];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 1                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
