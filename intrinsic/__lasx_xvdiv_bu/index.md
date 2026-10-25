# \_\_lasx_xvdiv_bu

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvdiv_bu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvdiv.bu xr, xr, xr
CPU Flags: LASX
```

### Description

Divide each unsigned 8-bit element in `a` by the corresponding element in `b`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = (b.byte[i] == 0) ? 0 : ((u8)a.byte[i] / (u8)b.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 29, 36  | 0.05(1/20.5)     |
| 3A6000 | LA664 | 29, 33  | 0.06(1/16.5)     |
| 3C6000 | LA664 | 29, 36  | 0.07(1/13.5)     |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
