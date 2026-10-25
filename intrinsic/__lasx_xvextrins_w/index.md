# \_\_lasx_xvextrins_w

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvextrins_w (__m256i a, __m256i b, imm0_255 imm)
#include <lasxintrin.h>
Instruction: xvextrins.w xr, xr, imm
CPU Flags: LASX
```

### Description

For each 128-bit lane, copy the 32-bit element of `b` selected by `imm[1:0]` into the element of `a` selected by `imm[5:4]`; other elements remain unchanged.

### Operation

```
int i;
for (i = 0; i < 4; i++) {
  dst.word[i] = (i == ((imm >> 4) & 3)) ? b.word[imm & 3] : a.word[i];
}
for (; i < 8; i++) {
  dst.word[i] = (i - 4 == ((imm >> 4) & 3)) ? b.word[(imm & 3) + 4] : a.word[i];
}

// Expands to:

if (0) {
  dst.word[0] = (0 == ((imm >> 4) & 3)) ? (b.word[imm & 3]) : (a.word[0]);
  dst.word[1] = (1 == ((imm >> 4) & 3)) ? (b.word[imm & 3]) : (a.word[1]);
  dst.word[2] = (2 == ((imm >> 4) & 3)) ? (b.word[imm & 3]) : (a.word[2]);
  dst.word[3] = (3 == ((imm >> 4) & 3)) ? (b.word[imm & 3]) : (a.word[3]);
  dst.word[4] = (0 == ((imm >> 4) & 3)) ? (b.word[(imm & 3) + 4]) : (a.word[4]);
  dst.word[5] = (1 == ((imm >> 4) & 3)) ? (b.word[(imm & 3) + 4]) : (a.word[5]);
  dst.word[6] = (2 == ((imm >> 4) & 3)) ? (b.word[(imm & 3) + 4]) : (a.word[6]);
  dst.word[7] = (3 == ((imm >> 4) & 3)) ? (b.word[(imm & 3) + 4]) : (a.word[7]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
