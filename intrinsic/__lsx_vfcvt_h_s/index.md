# \_\_lsx_vfcvt_h_s

LSX intrinsic · group: [Floating Point Conversion](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_conversion/index.md)

### Synopsis

```
__m128i __lsx_vfcvt_h_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfcvt.h.s vr, vr, vr
CPU Flags: LSX
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
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 1                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
