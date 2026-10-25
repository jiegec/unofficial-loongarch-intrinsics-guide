# \_\_lasx_xvaddwev_w_hu_h

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvaddwev_w_hu_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvaddwev.w.hu.h xr, xr, xr
CPU Flags: LASX
```

### Description

Add the even-positioned unsigned 16-bit elements in `a` and the corresponding signed 16-bit elements in `b`, extending both to 32-bit, and save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i] + (s32)(s16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[0])) + ((s32)((s16)b.half[0]));
  dst.word[1] = ((u32)((u16)a.half[2])) + ((s32)((s16)b.half[2]));
  dst.word[2] = ((u32)((u16)a.half[4])) + ((s32)((s16)b.half[4]));
  dst.word[3] = ((u32)((u16)a.half[6])) + ((s32)((s16)b.half[6]));
  dst.word[4] = ((u32)((u16)a.half[8])) + ((s32)((s16)b.half[8]));
  dst.word[5] = ((u32)((u16)a.half[10])) + ((s32)((s16)b.half[10]));
  dst.word[6] = ((u32)((u16)a.half[12])) + ((s32)((s16)b.half[12]));
  dst.word[7] = ((u32)((u16)a.half[14])) + ((s32)((s16)b.half[14]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
