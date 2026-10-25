# \_\_lsx_vsllwil_hu_bu

LSX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shift/index.md)

### Synopsis

```
__m128i __lsx_vsllwil_hu_bu (__m128i a, imm0_7 imm)
#include <lsxintrin.h>
Instruction: vsllwil.hu.bu vr, vr, imm
CPU Flags: LSX
```

### Description

For each 128-bit lane, zero-extend and shift the 8-bit elements in the lower part of that lane by `imm` to 16-bit result.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (u16)(u8)a.byte[i] << imm;
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
