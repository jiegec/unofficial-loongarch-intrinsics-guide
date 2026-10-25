# \_\_lasx_xvfcvth_s_h

LASX intrinsic · group: [Floating Point Conversion](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_conversion/index.md)

### Synopsis

```
__m256 __lasx_xvfcvth_s_h (__m256i a)
#include <lasxintrin.h>
Instruction: xvfcvth.s.h xr, xr
CPU Flags: LASX
```

### Description

Convert half precision floating point elements in higher half of every 128-bit vector of `a` to single precision.

### Operation

```
int i;
for (i = 0; i < 4; i++) {
  dst.fp32[i] = a.fp16[i + 4];
}
for (; i < 8; i++) {
  dst.fp32[i] = a.fp16[i + 8];
}

// Expands to:

if (0) {
  dst.fp32[0] = a.fp16[4];
  dst.fp32[1] = a.fp16[5];
  dst.fp32[2] = a.fp16[6];
  dst.fp32[3] = a.fp16[7];
  dst.fp32[4] = a.fp16[12];
  dst.fp32[5] = a.fp16[13];
  dst.fp32[6] = a.fp16[14];
  dst.fp32[7] = a.fp16[15];
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
