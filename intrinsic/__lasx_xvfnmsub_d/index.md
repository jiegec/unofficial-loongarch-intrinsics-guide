# \_\_lasx_xvfnmsub_d

LASX intrinsic · group: [Fused Multiply-Add](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/fma/index.md)

### Synopsis

```
__m256d __lasx_xvfnmsub_d (__m256d a, __m256d b, __m256d c)
#include <lasxintrin.h>
Instruction: xvfnmsub.d xr, xr, xr, xr
CPU Flags: LASX
```

### Description

Compute packed double precision floating point FMA(Fused Multiply-Add): multiply elements in `a` and `b`, subtract elements in `c` and store the negated result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = -(a.fp64[i] * b.fp64[i] - c.fp64[i]);
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
