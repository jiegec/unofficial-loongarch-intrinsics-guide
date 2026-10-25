# \_\_lasx_xvssrarni_w_d

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvssrarni_w_d (__m256i a, __m256i b, imm0_63 imm)
#include <lasxintrin.h>
Instruction: xvssrarni.w.d xr, xr, imm
CPU Flags: LASX
```

### Description

Arithmetic right shift with rounding the signed 64-bit elements in `a` and `b` by `imm`, saturate the result to signed 32-bit, write the 32-bit result into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (i < 2) {
    s64 temp;
    if (imm == 0) {
      temp = (s64)b.dword[i];
    } else {
      temp = ((s64)b.dword[i] >> imm) + (((s64)b.dword[i] >> (imm - 1)) & 1);
    }
    dst.word[i] = clamp<s64>(temp, -2147483648, 2147483647);
  } else {
    s64 temp;
    if (imm == 0) {
      temp = (s64)a.dword[i - 2];
    } else {
      temp = ((s64)a.dword[i - 2] >> imm) +
             (((s64)a.dword[i - 2] >> (imm - 1)) & 1);
    }
    dst.word[i] = clamp<s64>(temp, -2147483648, 2147483647);
  }
}
for (int i = 4; i < 8; i++) {
  if (i < 6) {
    s64 temp;
    if (imm == 0) {
      temp = (s64)b.dword[i - 2];
    } else {
      temp = ((s64)b.dword[i - 2] >> imm) +
             (((s64)b.dword[i - 2] >> (imm - 1)) & 1);
    }
    dst.word[i] = clamp<s64>(temp, -2147483648, 2147483647);
  } else {
    s64 temp;
    if (imm == 0) {
      temp = (s64)a.dword[i - 4];
    } else {
      temp = ((s64)a.dword[i - 4] >> imm) +
             (((s64)a.dword[i - 4] >> (imm - 1)) & 1);
    }
    dst.word[i] = clamp<s64>(temp, -2147483648, 2147483647);
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
