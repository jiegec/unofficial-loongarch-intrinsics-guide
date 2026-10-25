# \_\_lasx_xvfcvt_s_d

LASX intrinsic · group: [Floating Point Conversion](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_conversion/index.md)

### Synopsis

```
__m256 __lasx_xvfcvt_s_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfcvt.s.d xr, xr, xr
CPU Flags: LASX
```

### Description

For each 128-bit lane, convert double-precision floating point elements in the corresponding lane of `a` into the upper half and elements in `b` into the lower half, as single-precision floating point numbers.

### Operation

```
int i;
for (i = 0; i < 2; i++) {
  dst.fp32[i] = b.fp64[i];
}
for (; i < 4; i++) {
  dst.fp32[i] = a.fp64[i - 2];
}
for (; i < 6; i++) {
  dst.fp32[i] = b.fp64[i - 2];
}
for (; i < 8; i++) {
  dst.fp32[i] = a.fp64[i - 4];
}

// Expands to:

if (0) {
  dst.fp32[0] = b.fp64[0];
  dst.fp32[1] = b.fp64[1];
  dst.fp32[2] = a.fp64[0];
  dst.fp32[3] = a.fp64[1];
  dst.fp32[4] = b.fp64[2];
  dst.fp32[5] = b.fp64[3];
  dst.fp32[6] = a.fp64[2];
  dst.fp32[7] = a.fp64[3];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 1                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
