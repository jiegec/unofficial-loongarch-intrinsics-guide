# \_\_lasx_xvssrarni_bu_h

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvssrarni_bu_h (__m256i a, __m256i b, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvssrarni.bu.h xr, xr, imm
CPU Flags: LASX
```

### Description

Arithmetic right shift with rounding the signed 16-bit elements in `a` and `b` by `imm`, saturate the result to unsigned 8-bit, write the 8-bit result into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (i < 8) {
    s16 temp;
    if (imm == 0) {
      temp = (s16)b.half[i];
    } else {
      temp = ((s16)b.half[i] >> imm) + (((s16)b.half[i] >> (imm - 1)) & 1);
    }
    dst.byte[i] = clamp<s16>(temp, 0, 255);
  } else {
    s16 temp;
    if (imm == 0) {
      temp = (s16)a.half[i - 8];
    } else {
      temp =
          ((s16)a.half[i - 8] >> imm) + (((s16)a.half[i - 8] >> (imm - 1)) & 1);
    }
    dst.byte[i] = clamp<s16>(temp, 0, 255);
  }
}
for (int i = 16; i < 32; i++) {
  if (i < 24) {
    s16 temp;
    if (imm == 0) {
      temp = (s16)b.half[i - 8];
    } else {
      temp =
          ((s16)b.half[i - 8] >> imm) + (((s16)b.half[i - 8] >> (imm - 1)) & 1);
    }
    dst.byte[i] = clamp<s16>(temp, 0, 255);
  } else {
    s16 temp;
    if (imm == 0) {
      temp = (s16)a.half[i - 16];
    } else {
      temp = ((s16)a.half[i - 16] >> imm) +
             (((s16)a.half[i - 16] >> (imm - 1)) & 1);
    }
    dst.byte[i] = clamp<s16>(temp, 0, 255);
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
