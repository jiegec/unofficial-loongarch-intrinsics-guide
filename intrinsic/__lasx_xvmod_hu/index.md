# \_\_lasx_xvmod_hu

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvmod_hu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmod.hu xr, xr, xr
CPU Flags: LASX
```

### Description

Compute the remainder of each unsigned 16-bit element in `a` divided by the corresponding element in `b`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = (b.half[i] == 0) ? 0 : ((u16)a.half[i] % (u16)b.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 17, 23  | 0.06(1/16)       |
| 3A6000 | LA664 | 17, 25  | 0.11(1/9.5)      |
| 3C6000 | LA664 | 17      | 0.13(1/7.5)      |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
