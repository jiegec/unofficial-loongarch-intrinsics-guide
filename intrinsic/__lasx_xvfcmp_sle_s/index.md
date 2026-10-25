# \_\_lasx_xvfcmp_sle_s

LASX intrinsic · group: [Floating Point Comparison](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_comparison/index.md)

### Synopsis

```
__m256i __lasx_xvfcmp_sle_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfcmp.sle.s xr, xr, xr
CPU Flags: LASX
```

### Description

Compare corresponding single precision elements in `a` and `b`, save the comparison result (all ones if LE(Less than or Equal), all zeros otherwise) into `dst`. An exception is raised for QNaN.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (fp_compare_sle(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
