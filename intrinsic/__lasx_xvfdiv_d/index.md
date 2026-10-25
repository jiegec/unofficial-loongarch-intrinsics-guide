# \_\_lasx_xvfdiv_d

LASX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_computation/index.md)

### Synopsis

```
__m256d __lasx_xvfdiv_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfdiv.d xr, xr, xr
CPU Flags: LASX
```

### Description

Divide corresponding double precision floating point elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = a.fp64[i] / b.fp64[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 8, 17   | 0.08(1/12.5)     |
| 3A6000 | LA664 | 8, 21.5 | 0.25(1/4)        |
| 3C6000 | LA664 | 8, 16.5 | 0.33(1/3)        |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
