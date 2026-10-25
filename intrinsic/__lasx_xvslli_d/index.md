# \_\_lasx_xvslli_d

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvslli_d (__m256i a, imm0_63 imm)
#include <lasxintrin.h>
Instruction: xvslli.d xr, xr, imm
CPU Flags: LASX
```

### Description

Logical left shift the unsigned 64-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = a.dword[i] << imm;
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
