# \_\_lsx_vextrins_d

LSX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/misc/index.md)

### Synopsis

```
__m128i __lsx_vextrins_d (__m128i a, __m128i b, imm0_255 imm)
#include <lsxintrin.h>
Instruction: vextrins.d vr, vr, imm
CPU Flags: LSX
```

### Description

Copy the 64-bit element of `b` selected by `imm[0]` into the element of `a` selected by `imm[4]`; other elements remain unchanged.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (i == ((imm >> 4) & 1)) ? b.dword[imm & 1] : a.dword[i];
}

// Expands to:

if (0) {
  dst.dword[0] = (0 == ((imm >> 4) & 1)) ? (b.dword[imm & 1]) : (a.dword[0]);
  dst.dword[1] = (1 == ((imm >> 4) & 1)) ? (b.dword[imm & 1]) : (a.dword[1]);
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
