# \_\_lsx_vssrlrni_hu_w

LSX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shift/index.md)

### Synopsis

```
__m128i __lsx_vssrlrni_hu_w (__m128i a, __m128i b, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vssrlrni.hu.w vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift with rounding the unsigned 32-bit elements in `a` and `b` by `imm`, saturate the result to unsigned 16-bit, write the 16-bit result into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (i < 4) {
    u32 temp;
    if (imm == 0) {
      temp = (u32)b.word[i];
    } else {
      temp = ((u32)b.word[i] >> imm) + (((u32)b.word[i] >> (imm - 1)) & 1);
    }
    dst.half[i] = clamp<u32>(temp, 0, 65535);
  } else {
    u32 temp;
    if (imm == 0) {
      temp = (u32)a.word[i - 4];
    } else {
      temp =
          ((u32)a.word[i - 4] >> imm) + (((u32)a.word[i - 4] >> (imm - 1)) & 1);
    }
    dst.half[i] = clamp<u32>(temp, 0, 65535);
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
