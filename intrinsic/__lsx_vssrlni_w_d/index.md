# \_\_lsx_vssrlni_w_d

LSX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shift/index.md)

### Synopsis

```
__m128i __lsx_vssrlni_w_d (__m128i a, __m128i b, imm0_63 imm)
#include <lsxintrin.h>
Instruction: vssrlni.w.d vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 64-bit elements in `a` and `b` by `imm`, saturate the result to signed 32-bit, write the 32-bit result into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (i < 2) {
    u64 temp = (u64)b.dword[i] >> imm;
    dst.word[i] = clamp<u64>(temp, 0, 2147483647);
  } else {
    u64 temp = (u64)a.dword[i - 2] >> imm;
    dst.word[i] = clamp<u64>(temp, 0, 2147483647);
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
