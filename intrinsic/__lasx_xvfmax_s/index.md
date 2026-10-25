# \_\_lasx_xvfmax_s

LASX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_computation/index.md)

### Synopsis

```
__m256 __lasx_xvfmax_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfmax.s xr, xr, xr
CPU Flags: LASX
```

### Description

Compute the maximum (maxNum) of single precision floating point elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = fmax(a.fp32[i], b.fp32[i]);
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
