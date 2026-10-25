# \_\_lasx_xvinsgr2vr_d

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvinsgr2vr_d (__m256i a, long int b, imm0_3 imm)
#include <lasxintrin.h>
Instruction: xvinsgr2vr.d xr, r, imm
CPU Flags: LASX
```

### Description

Insert the low 64 bits of `b` into lane `imm` of `a`; the other lanes are unchanged.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (i == imm) ? b : a.dword[i];
}
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
