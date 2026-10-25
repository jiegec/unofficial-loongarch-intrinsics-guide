# \_\_lasx_xvssran_w_d

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvssran_w_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssran.w.d xr, xr, xr
CPU Flags: LASX
```

### Description

Arithmetic right shift each signed 64-bit element in `a` by the corresponding element of `b` modulo 64, saturate the result to signed 32-bit, write it into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (i < 2) {
    s64 temp = (s64)a.dword[i] >> (b.dword[i] & 63);
    dst.word[i] = clamp<s64>(temp, -2147483648, 2147483647);
  } else {
    dst.word[i] = 0;
  }
}
for (int i = 4; i < 8; i++) {
  if (i < 6) {
    s64 temp = (s64)a.dword[i - 2] >> (b.dword[i - 2] & 63);
    dst.word[i] = clamp<s64>(temp, -2147483648, 2147483647);
  } else {
    dst.word[i] = 0;
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
