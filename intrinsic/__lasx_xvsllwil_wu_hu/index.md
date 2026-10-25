# \_\_lasx_xvsllwil_wu_hu

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvsllwil_wu_hu (__m256i a, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvsllwil.wu.hu xr, xr, imm
CPU Flags: LASX
```

### Description

For each 128-bit lane, zero-extend and shift the 16-bit elements in the lower part of that lane by `imm` to 32-bit result.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (u32)(u16)a.half[i] << imm;
}
for (int i = 4; i < 8; i++) {
  dst.word[i] = (u32)(u16)a.half[i + 4] << imm;
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[0])) << imm;
  dst.word[1] = ((u32)((u16)a.half[1])) << imm;
  dst.word[2] = ((u32)((u16)a.half[2])) << imm;
  dst.word[3] = ((u32)((u16)a.half[3])) << imm;
  dst.word[4] = ((u32)((u16)a.half[8])) << imm;
  dst.word[5] = ((u32)((u16)a.half[9])) << imm;
  dst.word[6] = ((u32)((u16)a.half[10])) << imm;
  dst.word[7] = ((u32)((u16)a.half[11])) << imm;
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
