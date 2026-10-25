# \_\_lasx_xvaddwod_d_wu_w

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvaddwod_d_wu_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvaddwod.d.wu.w xr, xr, xr
CPU Flags: LASX
```

### Description

Add the odd-positioned unsigned 32-bit elements in `a` and the corresponding signed 32-bit elements in `b`, extending both to 64-bit, and save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 * i + 1] + (s64)(s32)b.word[2 * i + 1];
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[1])) + ((s64)((s32)b.word[1]));
  dst.dword[1] = ((u64)((u32)a.word[3])) + ((s64)((s32)b.word[3]));
  dst.dword[2] = ((u64)((u32)a.word[5])) + ((s64)((s32)b.word[5]));
  dst.dword[3] = ((u64)((u32)a.word[7])) + ((s64)((s32)b.word[7]));
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
