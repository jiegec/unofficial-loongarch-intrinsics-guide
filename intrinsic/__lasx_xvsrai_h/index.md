# \_\_lasx_xvsrai_h

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvsrai_h (__m256i a, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvsrai.h xr, xr, imm
CPU Flags: LASX
```

### Description

Arithmetic right shift the signed 16-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = ((s16)a.half[i]) >> imm;
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
