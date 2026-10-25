# \_\_lasx_xvhsubw_d_w

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvhsubw_d_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvhsubw.d.w xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract even-positioned signed 32-bit elements in `b` from odd-positioned signed 32-bit elements in `a`, sign-extending both to 64-bit, to get the 64-bit result.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (s64)(s32)a.word[2 * i + 1] - (s64)(s32)b.word[2 * i];
}

// Expands to:

if (0) {
  dst.dword[0] = ((s64)((s32)a.word[1])) - ((s64)((s32)b.word[0]));
  dst.dword[1] = ((s64)((s32)a.word[3])) - ((s64)((s32)b.word[2]));
  dst.dword[2] = ((s64)((s32)a.word[5])) - ((s64)((s32)b.word[4]));
  dst.dword[3] = ((s64)((s32)a.word[7])) - ((s64)((s32)b.word[6]));
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
