# \_\_lsx_vmaddwod_w_hu_h

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vmaddwod_w_hu_h (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmaddwod.w.hu.h vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply the odd-positioned unsigned 16-bit elements in `b` and the corresponding signed 16-bit elements in `c`, extending both to 32-bit, and add the product to the 32-bit elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
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
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
