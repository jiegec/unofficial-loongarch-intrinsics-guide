# \_\_lasx_xvfrsqrt_d

LASX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_computation/index.md)

### Synopsis

```
__m256d __lasx_xvfrsqrt_d (__m256d a)
#include <lasxintrin.h>
Instruction: xvfrsqrt.d xr, xr
CPU Flags: LASX
```

### Description

Compute reciprocal of square root of double precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = 1.0 / sqrt(a.fp64[i]);
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 15      | 0.04(1/27.5)     |
| 3A6000 | LA664 | 15      | 0.04(1/26.5)     |
| 3C6000 | LA664 | 15      | 0.04(1/26)       |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
