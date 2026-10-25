# \_\_lsx_vfcvt_s_d

LSX intrinsic · group: [Floating Point Conversion](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_conversion/index.md)

### Synopsis

```
__m128 __lsx_vfcvt_s_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfcvt.s.d vr, vr, vr
CPU Flags: LSX
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

// Expands to:

if (0) {
  dst.fp32[0] = b.fp64[0];
  dst.fp32[1] = b.fp64[1];
  dst.fp32[2] = a.fp64[0];
  dst.fp32[3] = a.fp64[1];
}
```

Tested on real machine.

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
