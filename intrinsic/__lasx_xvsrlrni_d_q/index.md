# \_\_lasx_xvsrlrni_d_q

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvsrlrni_d_q (__m256i a, __m256i b, imm0_127 imm)
#include <lasxintrin.h>
Instruction: xvsrlrni.d.q xr, xr, imm
CPU Flags: LASX
```

### Description

Logical right shift with rounding the unsigned 128-bit elements in `a` and `b` by `imm`, write the 64-bit results into `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (i < 1) {
    if (imm == 0) {
      dst.dword[i] = (u64)(u128)b.qword[i];
    } else {
      dst.dword[i] = (u64)(((u128)b.qword[i] >> imm) +
                           (((u128)b.qword[i] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.dword[i] = (u64)(u128)a.qword[i - 1];
    } else {
      dst.dword[i] = (u64)(((u128)a.qword[i - 1] >> imm) +
                           (((u128)a.qword[i - 1] >> (imm - 1)) & 0x1));
    }
  }
}
for (int i = 2; i < 4; i++) {
  if (i < 3) {
    if (imm == 0) {
      dst.dword[i] = (u64)(u128)b.qword[i - 1];
    } else {
      dst.dword[i] = (u64)(((u128)b.qword[i - 1] >> imm) +
                           (((u128)b.qword[i - 1] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.dword[i] = (u64)(u128)a.qword[i - 2];
    } else {
      dst.dword[i] = (u64)(((u128)a.qword[i - 2] >> imm) +
                           (((u128)a.qword[i - 2] >> (imm - 1)) & 0x1));
    }
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
