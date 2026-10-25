# \_\_lasx_xvftintrzl_l_s

LASX intrinsic · group: [Floating Point Conversion](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_conversion/index.md)

### Synopsis

```
__m256i __lasx_xvftintrzl_l_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvftintrzl.l.s xr, xr
CPU Flags: LASX
```

### Description

Convert single-precision floating point elements in lower part of every 128-bit vector of `a` to signed 64-bit integer, rounding towards zero.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)a.fp32[i]; // rounding mode is not expressed in C
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = (s64)a.fp32[i + 2]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.dword[0] = (s64)a.fp32[0];
  dst.dword[1] = (s64)a.fp32[1];
  dst.dword[2] = (s64)a.fp32[4];
  dst.dword[3] = (s64)a.fp32[5];
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
