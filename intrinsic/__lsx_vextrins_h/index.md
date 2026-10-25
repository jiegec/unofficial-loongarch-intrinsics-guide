# \_\_lsx_vextrins_h

LSX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/misc/index.md)

### Synopsis

```
__m128i __lsx_vextrins_h (__m128i a, __m128i b, imm0_255 imm)
#include <lsxintrin.h>
Instruction: vextrins.h vr, vr, imm
CPU Flags: LSX
```

### Description

Copy the 16-bit element of `b` selected by `imm[2:0]` into the element of `a` selected by `imm[6:4]`; other elements remain unchanged.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (i == ((imm >> 4) & 7)) ? b.half[imm & 7] : a.half[i];
}

// Expands to:

if (0) {
  dst.half[0] = (0 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[0]);
  dst.half[1] = (1 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[1]);
  dst.half[2] = (2 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[2]);
  dst.half[3] = (3 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[3]);
  dst.half[4] = (4 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[4]);
  dst.half[5] = (5 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[5]);
  dst.half[6] = (6 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[6]);
  dst.half[7] = (7 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[7]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
