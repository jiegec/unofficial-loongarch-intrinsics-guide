# \_\_lsx_vsrarni_d_q

LSX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shift/index.md)

### Synopsis

```
__m128i __lsx_vsrarni_d_q (__m128i a, __m128i b, imm0_127 imm)
#include <lsxintrin.h>
Instruction: vsrarni.d.q vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift with rounding the signed 128-bit elements in `a` and `b` by `imm`, write the 64-bit results into `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (i < 1) {
    if (imm == 0) {
      dst.dword[i] = (s64)(s128)b.qword[i];
    } else {
      dst.dword[i] = (s64)(((s128)b.qword[i] >> imm) +
                           (((s128)b.qword[i] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.dword[i] = (s64)(s128)a.qword[i - 1];
    } else {
      dst.dword[i] = (s64)(((s128)a.qword[i - 1] >> imm) +
                           (((s128)a.qword[i - 1] >> (imm - 1)) & 0x1));
    }
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
