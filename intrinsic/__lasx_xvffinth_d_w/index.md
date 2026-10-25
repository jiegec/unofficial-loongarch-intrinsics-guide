# \_\_lasx_xvffinth_d_w

LASX intrinsic · group: [Floating Point Conversion](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_conversion/index.md)

### Synopsis

```
__m256d __lasx_xvffinth_d_w (__m256i a)
#include <lasxintrin.h>
Instruction: xvffinth.d.w xr, xr
CPU Flags: LASX
```

### Description

Convert signed 32-bit integer elements in higher part of every 128-bit vector of `a` to double precision floating point numbers.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = (f64)(s32)a.word[i + 2]; // rounding mode is not expressed in C
}
for (int i = 2; i < 4; i++) {
  dst.fp64[i] = (f64)(s32)a.word[i + 4]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.fp64[0] = (f64)((s32)a.word[2]);
  dst.fp64[1] = (f64)((s32)a.word[3]);
  dst.fp64[2] = (f64)((s32)a.word[6]);
  dst.fp64[3] = (f64)((s32)a.word[7]);
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
