# \_\_lsx_vextrins_b

LSX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/misc/index.md)

### Synopsis

```
__m128i __lsx_vextrins_b (__m128i a, __m128i b, imm0_255 imm)
#include <lsxintrin.h>
Instruction: vextrins.b vr, vr, imm
CPU Flags: LSX
```

### Description

Copy the 8-bit element of `b` selected by `imm[3:0]` into the element of `a` selected by `imm[7:4]`; other elements remain unchanged.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (i == ((imm >> 4) & 15)) ? b.byte[imm & 15] : a.byte[i];
}

// Expands to:

if (0) {
  dst.byte[0] = (0 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[0]);
  dst.byte[1] = (1 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[1]);
  dst.byte[2] = (2 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[2]);
  dst.byte[3] = (3 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[3]);
  dst.byte[4] = (4 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[4]);
  dst.byte[5] = (5 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[5]);
  dst.byte[6] = (6 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[6]);
  dst.byte[7] = (7 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[7]);
  dst.byte[8] = (8 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[8]);
  dst.byte[9] = (9 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[9]);
  dst.byte[10] = (10 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[10]);
  dst.byte[11] = (11 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[11]);
  dst.byte[12] = (12 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[12]);
  dst.byte[13] = (13 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[13]);
  dst.byte[14] = (14 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[14]);
  dst.byte[15] = (15 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[15]);
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
