# \_\_lasx_xvsrarn_h_w

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvsrarn_h_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsrarn.h.w xr, xr, xr
CPU Flags: LASX
```

### Description

Arithmetic right shift with rounding each signed 32-bit element in `a` by the corresponding element of `b` modulo 32, write the lower 16 bits of the shifted result into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (i < 4) {
    u8 shift = (b.word[i] & 31);
    if (shift == 0) {
      dst.half[i] = (s16)(s32)a.word[i];
    } else {
      dst.half[i] = (s16)(((s32)a.word[i] >> shift) +
                          (((s32)a.word[i] >> (shift - 1)) & 0x1));
    }
  } else {
    dst.half[i] = 0;
  }
}
for (int i = 8; i < 16; i++) {
  if (i < 12) {
    u8 shift = (b.word[i - 4] & 31);
    if (shift == 0) {
      dst.half[i] = (s16)(s32)a.word[i - 4];
    } else {
      dst.half[i] = (s16)(((s32)a.word[i - 4] >> shift) +
                          (((s32)a.word[i - 4] >> (shift - 1)) & 0x1));
    }
  } else {
    dst.half[i] = 0;
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
