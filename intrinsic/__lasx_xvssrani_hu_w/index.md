# \_\_lasx_xvssrani_hu_w

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvssrani_hu_w (__m256i a, __m256i b, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvssrani.hu.w xr, xr, imm
CPU Flags: LASX
```

### Description

Arithmetic right shift the signed 32-bit elements in `a` and `b` by `imm`, saturate the result to unsigned 16-bit, write the 16-bit result into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (i < 4) {
    s32 temp = (s32)b.word[i] >> imm;
    dst.half[i] = clamp<s32>(temp, 0, 65535);
  } else {
    s32 temp = (s32)a.word[i - 4] >> imm;
    dst.half[i] = clamp<s32>(temp, 0, 65535);
  }
}
for (int i = 8; i < 16; i++) {
  if (i < 12) {
    s32 temp = (s32)b.word[i - 4] >> imm;
    dst.half[i] = clamp<s32>(temp, 0, 65535);
  } else {
    s32 temp = (s32)a.word[i - 8] >> imm;
    dst.half[i] = clamp<s32>(temp, 0, 65535);
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
