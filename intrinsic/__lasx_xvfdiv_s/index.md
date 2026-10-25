# \_\_lasx_xvfdiv_s

LASX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_computation/index.md)

### Synopsis

```
__m256 __lasx_xvfdiv_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfdiv.s xr, xr, xr
CPU Flags: LASX
```

### Description

Divide corresponding single precision floating point elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = a.fp32[i] / b.fp32[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency  | Throughput (IPC) |
| ------ | ----- | -------- | ---------------- |
| 3C5000 | LA464 | 11, 19.5 | 0.1(1/10.5)      |
| 3A6000 | LA664 | 11       | 0.18(1/5.5)      |
| 3C6000 | LA664 | 11       | 0.22(1/4.5)      |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
