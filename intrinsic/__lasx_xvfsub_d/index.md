# \_\_lasx_xvfsub_d

LASX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_computation/index.md)

### Synopsis

```
__m256d __lasx_xvfsub_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfsub.d xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract corresponding double precision floating point elements in `b` from elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = a.fp64[i] - b.fp64[i];
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
