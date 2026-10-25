# \_\_lasx_vext2xv_wu_bu

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_vext2xv_wu_bu (__m256i a)
#include <lsxintrin.h>
Instruction: vext2xv.wu.bu xr, xr
CPU Flags: LSX
```

### Description

Zero-extend the lowest 8 8-bit elements of `a` to 32-bit elements.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (u32)(u8)a.byte[i];
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
