# \_\_lsx_vssrlni_h_w

LSX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shift/index.md)

### Synopsis

```
__m128i __lsx_vssrlni_h_w (__m128i a, __m128i b, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vssrlni.h.w vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 32-bit elements in `a` and `b` by `imm`, saturate the result to signed 16-bit, write the 16-bit result into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (i < 4) {
    u32 temp = (u32)b.word[i] >> imm;
    dst.half[i] = clamp<u32>(temp, 0, 32767);
  } else {
    u32 temp = (u32)a.word[i - 4] >> imm;
    dst.half[i] = clamp<u32>(temp, 0, 32767);
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
