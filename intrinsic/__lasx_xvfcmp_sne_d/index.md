# \_\_lasx_xvfcmp_sne_d

LASX intrinsic · group: [Floating Point Comparison](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_comparison/index.md)

### Synopsis

```
__m256i __lasx_xvfcmp_sne_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfcmp.sne.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compare corresponding double precision elements in `a` and `b`, save the comparison result (all ones if NE(Not Equal), all zeros otherwise) into `dst`. An exception is raised for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_sne(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
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
