# \_\_lasx_xvsrlrni_h_w

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvsrlrni_h_w (__m256i a, __m256i b, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvsrlrni.h.w xr, xr, imm
CPU Flags: LASX
```

### Description

Logical right shift with rounding the unsigned 32-bit elements in `a` and `b` by `imm`, write the lower 16 bits of the shifted result into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (i < 4) {
    if (imm == 0) {
      dst.half[i] = (u16)(u32)b.word[i];
    } else {
      dst.half[i] = (u16)(((u32)b.word[i] >> imm) +
                          (((u32)b.word[i] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.half[i] = (u16)(u32)a.word[i - 4];
    } else {
      dst.half[i] = (u16)(((u32)a.word[i - 4] >> imm) +
                          (((u32)a.word[i - 4] >> (imm - 1)) & 0x1));
    }
  }
}
for (int i = 8; i < 16; i++) {
  if (i < 12) {
    if (imm == 0) {
      dst.half[i] = (u16)(u32)b.word[i - 4];
    } else {
      dst.half[i] = (u16)(((u32)b.word[i - 4] >> imm) +
                          (((u32)b.word[i - 4] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.half[i] = (u16)(u32)a.word[i - 8];
    } else {
      dst.half[i] = (u16)(((u32)a.word[i - 8] >> imm) +
                          (((u32)a.word[i - 8] >> (imm - 1)) & 0x1));
    }
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
