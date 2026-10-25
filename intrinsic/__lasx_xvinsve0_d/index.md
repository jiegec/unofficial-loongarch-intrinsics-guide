# \_\_lasx_xvinsve0_d

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvinsve0_d (__m256i a, __m256i b, imm0_3 imm)
#include <lasxintrin.h>
Instruction: xvinsve0.d xr, xr, imm
CPU Flags: LASX
```

### Description

Insert the first 64-bit element of `b` into lane `imm` of `a`; the other lanes are unchanged.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (i == imm) ? b.dword[0] : a.dword[i];
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
