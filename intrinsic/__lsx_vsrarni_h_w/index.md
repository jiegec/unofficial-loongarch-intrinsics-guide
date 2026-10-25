# \_\_lsx_vsrarni_h_w

LSX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shift/index.md)

### Synopsis

```
__m128i __lsx_vsrarni_h_w (__m128i a, __m128i b, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vsrarni.h.w vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift with rounding the signed 32-bit elements in `a` and `b` by `imm`, write the lower 16 bits of the shifted result into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (i < 4) {
    if (imm == 0) {
      dst.half[i] = (s16)(s32)b.word[i];
    } else {
      dst.half[i] = (s16)(((s32)b.word[i] >> imm) +
                          (((s32)b.word[i] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.half[i] = (s16)(s32)a.word[i - 4];
    } else {
      dst.half[i] = (s16)(((s32)a.word[i - 4] >> imm) +
                          (((s32)a.word[i - 4] >> (imm - 1)) & 0x1));
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
