# \_\_lasx_xvfsqrt_s

LASX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_computation/index.md)

### Synopsis

```
__m256 __lasx_xvfsqrt_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvfsqrt.s xr, xr
CPU Flags: LASX
```

### Description

Compute square root of single precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = sqrt(a.fp32[i]);
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 15      | 0.07(1/13.5)     |
| 3A6000 | LA664 | 15      | 0.08(1/12)       |
| 3C6000 | LA664 | 25      | 0.09(1/11.5)     |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
