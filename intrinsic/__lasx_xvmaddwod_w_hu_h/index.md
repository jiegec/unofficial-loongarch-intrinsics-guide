# \_\_lasx_xvmaddwod_w_hu_h

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvmaddwod_w_hu_h (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmaddwod.w.hu.h xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply the odd-positioned unsigned 16-bit elements in `b` and the corresponding signed 16-bit elements in `c`, extending both to 32-bit, and add the product to the 32-bit elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (u32)(u16)b.half[2 * i + 1] * (s32)(s16)c.half[2 * i + 1] +
                (s32)a.word[i];
}

// Expands to:

if (0) {
  dst.word[0] =
      (((u32)((u16)b.half[1])) * ((s32)((s16)c.half[1]))) + ((s32)a.word[0]);
  dst.word[1] =
      (((u32)((u16)b.half[3])) * ((s32)((s16)c.half[3]))) + ((s32)a.word[1]);
  dst.word[2] =
      (((u32)((u16)b.half[5])) * ((s32)((s16)c.half[5]))) + ((s32)a.word[2]);
  dst.word[3] =
      (((u32)((u16)b.half[7])) * ((s32)((s16)c.half[7]))) + ((s32)a.word[3]);
  dst.word[4] =
      (((u32)((u16)b.half[9])) * ((s32)((s16)c.half[9]))) + ((s32)a.word[4]);
  dst.word[5] =
      (((u32)((u16)b.half[11])) * ((s32)((s16)c.half[11]))) + ((s32)a.word[5]);
  dst.word[6] =
      (((u32)((u16)b.half[13])) * ((s32)((s16)c.half[13]))) + ((s32)a.word[6]);
  dst.word[7] =
      (((u32)((u16)b.half[15])) * ((s32)((s16)c.half[15]))) + ((s32)a.word[7]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
