# \_\_lsx_vsrarn_w_d

LSX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shift/index.md)

### Synopsis

```
__m128i __lsx_vsrarn_w_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsrarn.w.d vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift with rounding each signed 64-bit element in `a` by the corresponding element of `b` modulo 64, write the lower 32 bits of the shifted result into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (i < 2) {
    u8 shift = (b.dword[i] & 63);
    if (shift == 0) {
      dst.word[i] = (s32)(s64)a.dword[i];
    } else {
      dst.word[i] = (s32)(((s64)a.dword[i] >> shift) +
                          (((s64)a.dword[i] >> (shift - 1)) & 0x1));
    }
  } else {
    dst.word[i] = 0;
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
