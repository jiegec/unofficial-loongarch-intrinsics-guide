# \_\_lasx_xvfadd_s

LASX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_computation/index.md)

### Synopsis

```
__m256 __lasx_xvfadd_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfadd.s xr, xr, xr
CPU Flags: LASX
```

### Description

Add corresponding single precision floating point elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = a.fp32[i] + b.fp32[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
