# \_\_lasx_xvssrlni_du_q

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvssrlni_du_q (__m256i a, __m256i b, imm0_127 imm)
#include <lasxintrin.h>
Instruction: xvssrlni.du.q xr, xr, imm
CPU Flags: LASX
```

### Description

Logical right shift the unsigned 128-bit elements in `a` and `b` by `imm`, saturate the result to unsigned 64-bit, write the 64-bit results into `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (i < 1) {
    u128 temp = (u128)b.qword[i] >> imm;
    dst.dword[i] = clamp<u128>(temp, 0, 18446744073709551615);
  } else {
    u128 temp = (u128)a.qword[i - 1] >> imm;
    dst.dword[i] = clamp<u128>(temp, 0, 18446744073709551615);
  }
}
for (int i = 2; i < 4; i++) {
  if (i < 3) {
    u128 temp = (u128)b.qword[i - 1] >> imm;
    dst.dword[i] = clamp<u128>(temp, 0, 18446744073709551615);
  } else {
    u128 temp = (u128)a.qword[i - 2] >> imm;
    dst.dword[i] = clamp<u128>(temp, 0, 18446744073709551615);
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
