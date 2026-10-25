# \_\_lasx_xvfcvt_h_s

LASX intrinsic · group: [Floating Point Conversion](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_conversion/index.md)

### Synopsis

```
__m256i __lasx_xvfcvt_h_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfcvt.h.s xr, xr, xr
CPU Flags: LASX
```

### Description

For each 128-bit lane, convert single-precision floating point elements in the corresponding lane of `a` into the upper half and elements in `b` into the lower half, as half-precision floating point numbers.

### Operation

```
int i;
for (i = 0; i < 4; i++) {
  dst.fp16[i] = b.fp32[i];
}
for (; i < 8; i++) {
  dst.fp16[i] = a.fp32[i - 4];
}
for (; i < 12; i++) {
  dst.fp16[i] = b.fp32[i - 4];
}
for (; i < 16; i++) {
  dst.fp16[i] = a.fp32[i - 8];
}

// Expands to:

if (0) {
  dst.fp16[0] = b.fp32[0];
  dst.fp16[1] = b.fp32[1];
  dst.fp16[2] = b.fp32[2];
  dst.fp16[3] = b.fp32[3];
  dst.fp16[4] = a.fp32[0];
  dst.fp16[5] = a.fp32[1];
  dst.fp16[6] = a.fp32[2];
  dst.fp16[7] = a.fp32[3];
  dst.fp16[8] = b.fp32[4];
  dst.fp16[9] = b.fp32[5];
  dst.fp16[10] = b.fp32[6];
  dst.fp16[11] = b.fp32[7];
  dst.fp16[12] = a.fp32[4];
  dst.fp16[13] = a.fp32[5];
  dst.fp16[14] = a.fp32[6];
  dst.fp16[15] = a.fp32[7];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 1                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
