# \_\_lsx_vsrarni_w_d

LSX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shift/index.md)

### Synopsis

```
__m128i __lsx_vsrarni_w_d (__m128i a, __m128i b, imm0_63 imm)
#include <lsxintrin.h>
Instruction: vsrarni.w.d vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift with rounding the signed 64-bit elements in `a` and `b` by `imm`, write the lower 32 bits of the shifted result into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (i < 2) {
    if (imm == 0) {
      dst.word[i] = (s32)(s64)b.dword[i];
    } else {
      dst.word[i] = (s32)(((s64)b.dword[i] >> imm) +
                          (((s64)b.dword[i] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.word[i] = (s32)(s64)a.dword[i - 2];
    } else {
      dst.word[i] = (s32)(((s64)a.dword[i - 2] >> imm) +
                          (((s64)a.dword[i - 2] >> (imm - 1)) & 0x1));
    }
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
