# \_\_lsx_vsrlrni_b_h

LSX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shift/index.md)

### Synopsis

```
__m128i __lsx_vsrlrni_b_h (__m128i a, __m128i b, imm0_15 imm)
#include <lsxintrin.h>
Instruction: vsrlrni.b.h vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift with rounding the unsigned 16-bit elements in `a` and `b` by `imm`, write the lower 8 bits of the shifted result into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (i < 8) {
    if (imm == 0) {
      dst.byte[i] = (u8)(u16)b.half[i];
    } else {
      dst.byte[i] =
          (u8)(((u16)b.half[i] >> imm) + (((u16)b.half[i] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.byte[i] = (u8)(u16)a.half[i - 8];
    } else {
      dst.byte[i] = (u8)(((u16)a.half[i - 8] >> imm) +
                         (((u16)a.half[i - 8] >> (imm - 1)) & 0x1));
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
