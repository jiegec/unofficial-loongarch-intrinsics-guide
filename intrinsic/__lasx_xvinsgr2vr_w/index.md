# \_\_lasx_xvinsgr2vr_w

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvinsgr2vr_w (__m256i a, int b, imm0_7 imm)
#include <lasxintrin.h>
Instruction: xvinsgr2vr.w xr, r, imm
CPU Flags: LASX
```

### Description

Insert the low 32 bits of `b` into lane `imm` of `a`; the other lanes are unchanged.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (i == imm) ? b : a.word[i];
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
