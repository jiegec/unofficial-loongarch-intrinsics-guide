# \_\_lasx_vext2xv_d_w

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_vext2xv_d_w (__m256i a)
#include <lsxintrin.h>
Instruction: vext2xv.d.w xr, xr
CPU Flags: LSX
```

### Description

Sign-extend the lowest 4 32-bit elements of `a` to 64-bit elements.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (s64)(s32)a.word[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
