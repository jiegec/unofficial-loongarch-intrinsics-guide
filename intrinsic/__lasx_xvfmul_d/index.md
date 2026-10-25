# \_\_lasx_xvfmul_d

LASX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_computation/index.md)

### Synopsis

```
__m256d __lasx_xvfmul_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfmul.d xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply corresponding double precision floating point elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = a.fp64[i] * b.fp64[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 2                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
