# \_\_lasx_xvfnmadd_s

LASX intrinsic · group: [Fused Multiply-Add](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/fma/index.md)

### Synopsis

```
__m256 __lasx_xvfnmadd_s (__m256 a, __m256 b, __m256 c)
#include <lasxintrin.h>
Instruction: xvfnmadd.s xr, xr, xr, xr
CPU Flags: LASX
```

### Description

Compute packed single precision floating point FMA(Fused Multiply-Add): multiply elements in `a` and `b`, accumulate to elements in `c` and store the negated result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = -(a.fp32[i] * b.fp32[i] + c.fp32[i]);
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 2                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
