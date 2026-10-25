# \_\_lasx_xvsrani_w_d

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvsrani_w_d (__m256i a, __m256i b, imm0_63 imm)
#include <lasxintrin.h>
Instruction: xvsrani.w.d xr, xr, imm
CPU Flags: LASX
```

### Description

Arithmetic right shift the signed 64-bit elements in `a` and `b` by `imm`, write the lower 32 bits of the shifted result into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2) ? (s32)((s64)b.dword[i] >> imm)
                        : (s32)((s64)a.dword[i - 2] >> imm);
}
for (int i = 4; i < 8; i++) {
  dst.word[i] = (i < 6) ? (s32)((s64)b.dword[i - 2] >> imm)
                        : (s32)((s64)a.dword[i - 4] >> imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
