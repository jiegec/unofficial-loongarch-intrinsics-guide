# \_\_lasx_xvftintrz_wu_s

LASX intrinsic · group: [Floating Point Conversion](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_conversion/index.md)

### Synopsis

```
__m256i __lasx_xvftintrz_wu_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvftintrz.wu.s xr, xr
CPU Flags: LASX
```

### Description

Convert single-precision floating point elements in `a` to unsigned 32-bit integer, rounding towards zero.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (u32)a.fp32[i]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.word[0] = (u32)a.fp32[0];
  dst.word[1] = (u32)a.fp32[1];
  dst.word[2] = (u32)a.fp32[2];
  dst.word[3] = (u32)a.fp32[3];
  dst.word[4] = (u32)a.fp32[4];
  dst.word[5] = (u32)a.fp32[5];
  dst.word[6] = (u32)a.fp32[6];
  dst.word[7] = (u32)a.fp32[7];
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
