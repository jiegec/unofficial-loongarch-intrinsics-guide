# \_\_lasx_xvfsqrt_d

LASX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_computation/index.md)

### Synopsis

```
__m256d __lasx_xvfsqrt_d (__m256d a)
#include <lasxintrin.h>
Instruction: xvfsqrt.d xr, xr
CPU Flags: LASX
```

### Description

Compute square root of double precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = sqrt(a.fp64[i]);
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 36      | 0.05(1/18.5)     |
| 3A6000 | LA664 | 36      | 0.06(1/17.5)     |
| 3C6000 | LA664 | 36      | 0.06(1/17)       |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
