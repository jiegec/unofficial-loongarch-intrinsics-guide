# \_\_lsx_vsrai_d

LSX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shift/index.md)

### Synopsis

```
__m128i __lsx_vsrai_d (__m128i a, imm0_63 imm)
#include <lsxintrin.h>
Instruction: vsrai.d vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 64-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = ((s64)a.dword[i]) >> imm;
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
