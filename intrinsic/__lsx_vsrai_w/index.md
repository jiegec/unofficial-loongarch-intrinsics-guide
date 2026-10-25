# \_\_lsx_vsrai_w

LSX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shift/index.md)

### Synopsis

```
__m128i __lsx_vsrai_w (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vsrai.w vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 32-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = ((s32)a.word[i]) >> imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
