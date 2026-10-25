# \_\_lsx_vssrani_b_h

LSX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shift/index.md)

### Synopsis

```
__m128i __lsx_vssrani_b_h (__m128i a, __m128i b, imm0_15 imm)
#include <lsxintrin.h>
Instruction: vssrani.b.h vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 16-bit elements in `a` and `b` by `imm`, saturate the result to signed 8-bit, write the 8-bit result into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (i < 8) {
    s16 temp = (s16)b.half[i] >> imm;
    dst.byte[i] = clamp<s16>(temp, -128, 127);
  } else {
    s16 temp = (s16)a.half[i - 8] >> imm;
    dst.byte[i] = clamp<s16>(temp, -128, 127);
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
