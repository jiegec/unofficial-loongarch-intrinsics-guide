# \_\_lasx_xvftintrp_l_d

LASX intrinsic · group: [Floating Point Conversion](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_conversion/index.md)

### Synopsis

```
__m256i __lasx_xvftintrp_l_d (__m256d a)
#include <lasxintrin.h>
Instruction: xvftintrp.l.d xr, xr
CPU Flags: LASX
```

### Description

Convert double-precision floating point elements in `a` to signed 64-bit integer, rounding towards positive infinity.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (s64)a.fp64[i]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.dword[0] = (s64)a.fp64[0];
  dst.dword[1] = (s64)a.fp64[1];
  dst.dword[2] = (s64)a.fp64[2];
  dst.dword[3] = (s64)a.fp64[3];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 4                |
| 3C6000 | LA664 | 4       | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
