# \_\_lsx_vextrins_w

LSX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/misc/index.md)

### Synopsis

```
__m128i __lsx_vextrins_w (__m128i a, __m128i b, imm0_255 imm)
#include <lsxintrin.h>
Instruction: vextrins.w vr, vr, imm
CPU Flags: LSX
```

### Description

Copy the 32-bit element of `b` selected by `imm[1:0]` into the element of `a` selected by `imm[5:4]`; other elements remain unchanged.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (i == ((imm >> 4) & 3)) ? b.word[imm & 3] : a.word[i];
}

// Expands to:

if (0) {
  dst.word[0] = (0 == ((imm >> 4) & 3)) ? (b.word[imm & 3]) : (a.word[0]);
  dst.word[1] = (1 == ((imm >> 4) & 3)) ? (b.word[imm & 3]) : (a.word[1]);
  dst.word[2] = (2 == ((imm >> 4) & 3)) ? (b.word[imm & 3]) : (a.word[2]);
  dst.word[3] = (3 == ((imm >> 4) & 3)) ? (b.word[imm & 3]) : (a.word[3]);
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
