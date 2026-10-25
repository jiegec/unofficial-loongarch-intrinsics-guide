# \_\_lasx_xvfrecip_d

LASX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_computation/index.md)

### Synopsis

```
__m256d __lasx_xvfrecip_d (__m256d a)
#include <lasxintrin.h>
Instruction: xvfrecip.d xr, xr
CPU Flags: LASX
```

### Description

Compute reciprocal of double precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = 1 / a.fp64[i];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 23      | 0.08(1/12)       |
| 3A6000 | LA664 | 23      | 0.25(1/4)        |
| 3C6000 | LA664 | 23      | 0.1(1/10.5)      |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
