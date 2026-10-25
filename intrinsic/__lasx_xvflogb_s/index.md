# \_\_lasx_xvflogb_s

LASX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_computation/index.md)

### Synopsis

```
__m256 __lasx_xvflogb_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvflogb.s xr, xr
CPU Flags: LASX
```

### Description

Compute base-2 logarithm of single precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = log2(a.fp32[i]);
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 4                |
| 3C6000 | LA664 | 4       | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
