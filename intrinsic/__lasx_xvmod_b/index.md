# \_\_lasx_xvmod_b

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvmod_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmod.b xr, xr, xr
CPU Flags: LASX
```

### Description

Compute the remainder of each signed 8-bit element in `a` divided by the corresponding element in `b`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = (b.byte[i] == 0) ? 0 : ((s8)a.byte[i] % (s8)b.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 29, 33  | 0.05(1/21.5)     |
| 3A6000 | LA664 | 29, 41  | 0.06(1/15.5)     |
| 3C6000 | LA664 | 29      | 0.07(1/13.5)     |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
