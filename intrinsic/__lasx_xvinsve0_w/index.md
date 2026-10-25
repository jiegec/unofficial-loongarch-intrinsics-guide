# \_\_lasx_xvinsve0_w

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvinsve0_w (__m256i a, __m256i b, imm0_7 imm)
#include <lasxintrin.h>
Instruction: xvinsve0.w xr, xr, imm
CPU Flags: LASX
```

### Description

Insert the first 32-bit element of `b` into lane `imm` of `a`; the other lanes are unchanged.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (i == imm) ? b.word[0] : a.word[i];
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
