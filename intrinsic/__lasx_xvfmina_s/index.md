# \_\_lasx_xvfmina_s

LASX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_computation/index.md)

### Synopsis

```
__m256 __lasx_xvfmina_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfmina.s xr, xr, xr
CPU Flags: LASX
```

### Description

Compute the minimum (minNumMag) of single precision floating point elements in `a` and `b`, selecting the element with the smaller absolute value.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (abs(a.fp32[i]) < abs(b.fp32[i])) {
    dst.fp32[i] = a.fp32[i];
  } else if (abs(b.fp32[i]) < abs(a.fp32[i])) {
    dst.fp32[i] = b.fp32[i];
  } else {
    dst.fp32[i] = fmin(a.fp32[i], b.fp32[i]);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
