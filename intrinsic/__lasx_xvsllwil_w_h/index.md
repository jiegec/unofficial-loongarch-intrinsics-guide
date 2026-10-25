# \_\_lasx_xvsllwil_w_h

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvsllwil_w_h (__m256i a, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvsllwil.w.h xr, xr, imm
CPU Flags: LASX
```

### Description

For each 128-bit lane, sign-extend and shift the 16-bit elements in the lower part of that lane by `imm` to 32-bit result.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (s32)(s16)a.half[i] << imm;
}
for (int i = 4; i < 8; i++) {
  dst.word[i] = (s32)(s16)a.half[i + 4] << imm;
}

// Expands to:

if (0) {
  dst.word[0] = ((s32)((s16)a.half[0])) << imm;
  dst.word[1] = ((s32)((s16)a.half[1])) << imm;
  dst.word[2] = ((s32)((s16)a.half[2])) << imm;
  dst.word[3] = ((s32)((s16)a.half[3])) << imm;
  dst.word[4] = ((s32)((s16)a.half[8])) << imm;
  dst.word[5] = ((s32)((s16)a.half[9])) << imm;
  dst.word[6] = ((s32)((s16)a.half[10])) << imm;
  dst.word[7] = ((s32)((s16)a.half[11])) << imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
