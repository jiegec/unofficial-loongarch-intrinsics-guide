# \_\_lasx_xvfrintrm_d

LASX intrinsic · group: [Floating Point Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_misc/index.md)

### Synopsis

```
__m256d __lasx_xvfrintrm_d (__m256d a)
#include <lasxintrin.h>
Instruction: xvfrintrm.d xr, xr
CPU Flags: LASX
```

### Description

Round double-precision floating point elements in `a` to integers, rounding towards negative infinity, and store as floating point numbers.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = (f64)(s64)a.fp64[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
