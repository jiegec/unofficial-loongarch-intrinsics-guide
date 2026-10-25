# \_\_lasx_xvsllwil_h_b

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvsllwil_h_b (__m256i a, imm0_7 imm)
#include <lasxintrin.h>
Instruction: xvsllwil.h.b xr, xr, imm
CPU Flags: LASX
```

### Description

For each 128-bit lane, sign-extend and shift the 8-bit elements in the lower part of that lane by `imm` to 16-bit result.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (s16)(s8)a.byte[i] << imm;
}
for (int i = 8; i < 16; i++) {
  dst.half[i] = (s16)(s8)a.byte[i + 8] << imm;
}

// Expands to:

if (0) {
  dst.half[0] = ((s16)((s8)a.byte[0])) << imm;
  dst.half[1] = ((s16)((s8)a.byte[1])) << imm;
  dst.half[2] = ((s16)((s8)a.byte[2])) << imm;
  dst.half[3] = ((s16)((s8)a.byte[3])) << imm;
  dst.half[4] = ((s16)((s8)a.byte[4])) << imm;
  dst.half[5] = ((s16)((s8)a.byte[5])) << imm;
  dst.half[6] = ((s16)((s8)a.byte[6])) << imm;
  dst.half[7] = ((s16)((s8)a.byte[7])) << imm;
  dst.half[8] = ((s16)((s8)a.byte[16])) << imm;
  dst.half[9] = ((s16)((s8)a.byte[17])) << imm;
  dst.half[10] = ((s16)((s8)a.byte[18])) << imm;
  dst.half[11] = ((s16)((s8)a.byte[19])) << imm;
  dst.half[12] = ((s16)((s8)a.byte[20])) << imm;
  dst.half[13] = ((s16)((s8)a.byte[21])) << imm;
  dst.half[14] = ((s16)((s8)a.byte[22])) << imm;
  dst.half[15] = ((s16)((s8)a.byte[23])) << imm;
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
