# \_\_lasx_xvmaddwev_w_h

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvmaddwev_w_h (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmaddwev.w.h xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply the even-positioned signed 16-bit elements in `b` and the corresponding signed 16-bit elements in `c`, extending both to 32-bit, and add the product to the 32-bit elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] =
      (s32)(s16)b.half[2 * i] * (s32)(s16)c.half[2 * i] + (s32)a.word[i];
}

// Expands to:

if (0) {
  dst.word[0] =
      (((s32)((s16)b.half[0])) * ((s32)((s16)c.half[0]))) + ((s32)a.word[0]);
  dst.word[1] =
      (((s32)((s16)b.half[2])) * ((s32)((s16)c.half[2]))) + ((s32)a.word[1]);
  dst.word[2] =
      (((s32)((s16)b.half[4])) * ((s32)((s16)c.half[4]))) + ((s32)a.word[2]);
  dst.word[3] =
      (((s32)((s16)b.half[6])) * ((s32)((s16)c.half[6]))) + ((s32)a.word[3]);
  dst.word[4] =
      (((s32)((s16)b.half[8])) * ((s32)((s16)c.half[8]))) + ((s32)a.word[4]);
  dst.word[5] =
      (((s32)((s16)b.half[10])) * ((s32)((s16)c.half[10]))) + ((s32)a.word[5]);
  dst.word[6] =
      (((s32)((s16)b.half[12])) * ((s32)((s16)c.half[12]))) + ((s32)a.word[6]);
  dst.word[7] =
      (((s32)((s16)b.half[14])) * ((s32)((s16)c.half[14]))) + ((s32)a.word[7]);
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
