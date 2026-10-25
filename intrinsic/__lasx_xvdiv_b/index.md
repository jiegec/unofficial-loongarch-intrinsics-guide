# \_\_lasx_xvdiv_b

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvdiv_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvdiv.b xr, xr, xr
CPU Flags: LASX
```

### Description

Divide each signed 8-bit element in `a` by the corresponding element in `b`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = (b.byte[i] == 0) ? 0 : ((s8)a.byte[i] / (s8)b.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 32, 36  | 0.05(1/20.5)     |
| 3A6000 | LA664 | 29, 32  | 0.06(1/15.5)     |
| 3C6000 | LA664 | 55, 57  | 0.04(1/27.5)     |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
