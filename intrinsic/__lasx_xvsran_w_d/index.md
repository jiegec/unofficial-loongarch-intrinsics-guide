# \_\_lasx_xvsran_w_d

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvsran_w_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsran.w.d xr, xr, xr
CPU Flags: LASX
```

### Description

Arithmetic right shift each signed 64-bit element in `a` by the corresponding element of `b` modulo 64, write the lower 32 bits of the shifted result into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2) ? (s32)((s64)a.dword[i] >> (b.dword[i] & 63)) : 0;
}
for (int i = 4; i < 8; i++) {
  dst.word[i] =
      (i < 6) ? (s32)((s64)a.dword[i - 2] >> (b.dword[i - 2] & 63)) : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
