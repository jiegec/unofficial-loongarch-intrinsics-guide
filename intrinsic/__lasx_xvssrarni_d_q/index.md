# \_\_lasx_xvssrarni_d_q

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvssrarni_d_q (__m256i a, __m256i b, imm0_127 imm)
#include <lasxintrin.h>
Instruction: xvssrarni.d.q xr, xr, imm
CPU Flags: LASX
```

### Description

Arithmetic right shift with rounding the signed 128-bit elements in `a` and `b` by `imm`, saturate the result to signed 64-bit, write the 64-bit results into `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (i < 1) {
    s128 temp;
    if (imm == 0) {
      temp = (s128)b.qword[i];
    } else {
      temp = ((s128)b.qword[i] >> imm) + (((s128)b.qword[i] >> (imm - 1)) & 1);
    }
    dst.dword[i] = clamp<s128>(temp, -9223372036854775808, 9223372036854775807);
  } else {
    s128 temp;
    if (imm == 0) {
      temp = (s128)a.qword[i - 1];
    } else {
      temp = ((s128)a.qword[i - 1] >> imm) +
             (((s128)a.qword[i - 1] >> (imm - 1)) & 1);
    }
    dst.dword[i] = clamp<s128>(temp, -9223372036854775808, 9223372036854775807);
  }
}
for (int i = 2; i < 4; i++) {
  if (i < 3) {
    s128 temp;
    if (imm == 0) {
      temp = (s128)b.qword[i - 1];
    } else {
      temp = ((s128)b.qword[i - 1] >> imm) +
             (((s128)b.qword[i - 1] >> (imm - 1)) & 1);
    }
    dst.dword[i] = clamp<s128>(temp, -9223372036854775808, 9223372036854775807);
  } else {
    s128 temp;
    if (imm == 0) {
      temp = (s128)a.qword[i - 2];
    } else {
      temp = ((s128)a.qword[i - 2] >> imm) +
             (((s128)a.qword[i - 2] >> (imm - 1)) & 1);
    }
    dst.dword[i] = clamp<s128>(temp, -9223372036854775808, 9223372036854775807);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
