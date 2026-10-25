# \_\_lasx_xvsrari_h

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvsrari_h (__m256i a, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvsrari.h xr, xr, imm
CPU Flags: LASX
```

### Description

Arithmetic right shift (with rounding) the signed 16-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (imm == 0) {
    dst.half[i] = a.half[i];
  } else {
    dst.half[i] =
        ((s16)a.half[i] >> imm) + (((s16)a.half[i] >> (imm - 1)) & 0x1);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
