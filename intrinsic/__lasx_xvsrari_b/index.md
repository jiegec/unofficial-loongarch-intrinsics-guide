# \_\_lasx_xvsrari_b

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvsrari_b (__m256i a, imm0_7 imm)
#include <lasxintrin.h>
Instruction: xvsrari.b xr, xr, imm
CPU Flags: LASX
```

### Description

Arithmetic right shift (with rounding) the signed 8-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  if (imm == 0) {
    dst.byte[i] = a.byte[i];
  } else {
    dst.byte[i] = ((s8)a.byte[i] >> imm) + (((s8)a.byte[i] >> (imm - 1)) & 0x1);
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
