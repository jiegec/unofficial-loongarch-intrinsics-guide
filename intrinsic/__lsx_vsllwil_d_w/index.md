# \_\_lsx_vsllwil_d_w

LSX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shift/index.md)

### Synopsis

```
__m128i __lsx_vsllwil_d_w (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vsllwil.d.w vr, vr, imm
CPU Flags: LSX
```

### Description

For each 128-bit lane, sign-extend and shift the 32-bit elements in the lower part of that lane by `imm` to 64-bit result.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)(s32)a.word[i] << imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 1                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 0.5(1/2)         |
| 2K3000   | LA364E | 2       | 0.5(1/2)         |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
