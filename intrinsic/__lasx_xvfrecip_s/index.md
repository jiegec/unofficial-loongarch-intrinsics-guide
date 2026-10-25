# \_\_lasx_xvfrecip_s

LASX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_computation/index.md)

### Synopsis

```
__m256 __lasx_xvfrecip_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvfrecip.s xr, xr
CPU Flags: LASX
```

### Description

Compute reciprocal of single precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = 1 / a.fp32[i];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 27      | 0.14(1/7)        |
| 3A6000 | LA664 | 27      | 0.18(1/5.5)      |
| 3C6000 | LA664 | 27      | 0.12(1/8.5)      |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
