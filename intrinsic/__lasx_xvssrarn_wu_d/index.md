# \_\_lasx_xvssrarn_wu_d

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvssrarn_wu_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssrarn.wu.d xr, xr, xr
CPU Flags: LASX
```

### Description

Arithmetic right shift with rounding each signed 64-bit element in `a` by the corresponding element of `b` modulo 64, saturate the result to unsigned 32-bit, write it into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (i < 2) {
    s64 temp;
    if ((b.dword[i] & 63) == 0) {
      temp = (s64)a.dword[i];
    } else {
      temp = ((s64)a.dword[i] >> (b.dword[i] & 63)) +
             (((s64)a.dword[i] >> ((b.dword[i] & 63) - 1)) & 1);
    }
    dst.word[i] = clamp<s64>(temp, 0, 4294967295);
  } else {
    dst.word[i] = 0;
  }
}
for (int i = 4; i < 8; i++) {
  if (i < 6) {
    s64 temp;
    if ((b.dword[i - 2] & 63) == 0) {
      temp = (s64)a.dword[i - 2];
    } else {
      temp = ((s64)a.dword[i - 2] >> (b.dword[i - 2] & 63)) +
             (((s64)a.dword[i - 2] >> ((b.dword[i - 2] & 63) - 1)) & 1);
    }
    dst.word[i] = clamp<s64>(temp, 0, 4294967295);
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
