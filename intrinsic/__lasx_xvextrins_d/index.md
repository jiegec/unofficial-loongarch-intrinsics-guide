# \_\_lasx_xvextrins_d

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvextrins_d (__m256i a, __m256i b, imm0_255 imm)
#include <lasxintrin.h>
Instruction: xvextrins.d xr, xr, imm
CPU Flags: LASX
```

### Description

For each 128-bit lane, copy the 64-bit element of `b` selected by `imm[0]` into the element of `a` selected by `imm[4]`; other elements remain unchanged.

### Operation

```
int i;
for (i = 0; i < 2; i++) {
  dst.dword[i] = (i == ((imm >> 4) & 1)) ? b.dword[imm & 1] : a.dword[i];
}
for (; i < 4; i++) {
  dst.dword[i] =
      (i - 2 == ((imm >> 4) & 1)) ? b.dword[(imm & 1) + 2] : a.dword[i];
}

// Expands to:

if (0) {
  dst.dword[0] = (0 == ((imm >> 4) & 1)) ? (b.dword[imm & 1]) : (a.dword[0]);
  dst.dword[1] = (1 == ((imm >> 4) & 1)) ? (b.dword[imm & 1]) : (a.dword[1]);
  dst.dword[2] =
      (0 == ((imm >> 4) & 1)) ? (b.dword[(imm & 1) + 2]) : (a.dword[2]);
  dst.dword[3] =
      (1 == ((imm >> 4) & 1)) ? (b.dword[(imm & 1) + 2]) : (a.dword[3]);
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
