# \_\_lasx_xvmaddwev_d_w

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvmaddwev_d_w (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmaddwev.d.w xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply the even-positioned signed 32-bit elements in `b` and the corresponding signed 32-bit elements in `c`, extending both to 64-bit, and add the product to the 64-bit elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] =
      (s64)(s32)b.word[2 * i] * (s64)(s32)c.word[2 * i] + (s64)a.dword[i];
}

// Expands to:

if (0) {
  dst.dword[0] =
      (((s64)((s32)b.word[0])) * ((s64)((s32)c.word[0]))) + ((s64)a.dword[0]);
  dst.dword[1] =
      (((s64)((s32)b.word[2])) * ((s64)((s32)c.word[2]))) + ((s64)a.dword[1]);
  dst.dword[2] =
      (((s64)((s32)b.word[4])) * ((s64)((s32)c.word[4]))) + ((s64)a.dword[2]);
  dst.dword[3] =
      (((s64)((s32)b.word[6])) * ((s64)((s32)c.word[6]))) + ((s64)a.dword[3]);
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
