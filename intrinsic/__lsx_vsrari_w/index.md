# \_\_lsx_vsrari_w

LSX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shift/index.md)

### Synopsis

```
__m128i __lsx_vsrari_w (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vsrari.w vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift (with rounding) the signed 32-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (imm == 0) {
    dst.word[i] = a.word[i];
  } else {
    dst.word[i] =
        ((s32)a.word[i] >> imm) + (((s32)a.word[i] >> (imm - 1)) & 0x1);
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
