# \_\_lasx_xvfrsqrt_s

LASX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_computation/index.md)

### Synopsis

```
__m256 __lasx_xvfrsqrt_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvfrsqrt.s xr, xr
CPU Flags: LASX
```

### Description

Compute reciprocal of square root of single precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = 1.0 / sqrt(a.fp32[i]);
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 25      | 0.03(1/32)       |
| 3A6000 | LA664 | 25      | 0.05(1/19)       |
| 3C6000 | LA664 | 21      | 0.11(1/9.5)      |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
