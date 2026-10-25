# \_\_lasx_xvfmaxa_d

LASX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_computation/index.md)

### Synopsis

```
__m256d __lasx_xvfmaxa_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfmaxa.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compute the maximum (maxNumMag) of double precision floating point elements in `a` and `b`, selecting the element with the larger absolute value.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (abs(a.fp64[i]) > abs(b.fp64[i])) {
    dst.fp64[i] = a.fp64[i];
  } else if (abs(b.fp64[i]) > abs(a.fp64[i])) {
    dst.fp64[i] = b.fp64[i];
  } else {
    dst.fp64[i] = fmax(a.fp64[i], b.fp64[i]);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
