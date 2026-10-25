# \_\_lasx_xvmod_wu

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvmod_wu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmod.wu xr, xr, xr
CPU Flags: LASX
```

### Description

Compute the remainder of each unsigned 32-bit element in `a` divided by the corresponding element in `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (b.word[i] == 0) ? 0 : ((u32)a.word[i] % (u32)b.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 11, 15  | 0.06(1/16)       |
| 3A6000 | LA664 | 11, 13  | 0.18(1/5.5)      |
| 3C6000 | LA664 | 11      | 0.22(1/4.5)      |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
