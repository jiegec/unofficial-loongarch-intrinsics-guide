# \_\_lasx_xvhseli_d

LASX intrinsic · group: [Undocumented Intrinsics](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/undocumented/index.md)

### Synopsis

```
__m256i __lasx_xvhseli_d (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvhseli.d xr, xr, imm
CPU Flags: LASX
```

### Description

Select each double word of `dst` from an adjacent pair in `a` according to `imm`: bits 0 and 1 choose the sources for D[0] and D[1] from {a.D[0], a.D[1]}, and bits 2 and 3 choose the sources for D[2] and D[3] from {a.D[2], a.D[3]}.

### Operation

```
dst.dword[0] = (imm & 1) ? a.dword[1] : a.dword[0];
dst.dword[1] = (imm & 2) ? a.dword[1] : a.dword[0];
dst.dword[2] = (imm & 4) ? a.dword[3] : a.dword[2];
dst.dword[3] = (imm & 8) ? a.dword[3] : a.dword[2];
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 1                |
| 3A6000 | LA664 | 1       | 1                |
| 3C6000 | LA664 | 1       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
