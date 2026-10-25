# \_\_lsx_vssrlni_bu_h

LSX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shift/index.md)

### Synopsis

```
__m128i __lsx_vssrlni_bu_h (__m128i a, __m128i b, imm0_15 imm)
#include <lsxintrin.h>
Instruction: vssrlni.bu.h vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 16-bit elements in `a` and `b` by `imm`, saturate the result to unsigned 8-bit, write the 8-bit result into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (i < 8) {
    u16 temp = (u16)b.half[i] >> imm;
    dst.byte[i] = clamp<u16>(temp, 0, 255);
  } else {
    u16 temp = (u16)a.half[i - 8] >> imm;
    dst.byte[i] = clamp<u16>(temp, 0, 255);
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
