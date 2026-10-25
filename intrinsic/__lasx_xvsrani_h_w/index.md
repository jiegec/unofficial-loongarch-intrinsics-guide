# \_\_lasx_xvsrani_h_w

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvsrani_h_w (__m256i a, __m256i b, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvsrani.h.w xr, xr, imm
CPU Flags: LASX
```

### Description

Arithmetic right shift the signed 32-bit elements in `a` and `b` by `imm`, write the lower 16 bits of the shifted result into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] =
      (i < 4) ? (s16)((s32)b.word[i] >> imm) : (s16)((s32)a.word[i - 4] >> imm);
}
for (int i = 8; i < 16; i++) {
  dst.half[i] = (i < 12) ? (s16)((s32)b.word[i - 4] >> imm)
                         : (s16)((s32)a.word[i - 8] >> imm);
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
