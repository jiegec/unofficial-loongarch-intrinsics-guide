# \_\_lasx_xvsllwil_hu_bu

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvsllwil_hu_bu (__m256i a, imm0_7 imm)
#include <lasxintrin.h>
Instruction: xvsllwil.hu.bu xr, xr, imm
CPU Flags: LASX
```

### Description

For each 128-bit lane, zero-extend and shift the 8-bit elements in the lower part of that lane by `imm` to 16-bit result.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (u16)(u8)a.byte[i] << imm;
}
for (int i = 8; i < 16; i++) {
  dst.half[i] = (u16)(u8)a.byte[i + 8] << imm;
}

// Expands to:

if (0) {
  dst.half[0] = ((u16)((u8)a.byte[0])) << imm;
  dst.half[1] = ((u16)((u8)a.byte[1])) << imm;
  dst.half[2] = ((u16)((u8)a.byte[2])) << imm;
  dst.half[3] = ((u16)((u8)a.byte[3])) << imm;
  dst.half[4] = ((u16)((u8)a.byte[4])) << imm;
  dst.half[5] = ((u16)((u8)a.byte[5])) << imm;
  dst.half[6] = ((u16)((u8)a.byte[6])) << imm;
  dst.half[7] = ((u16)((u8)a.byte[7])) << imm;
  dst.half[8] = ((u16)((u8)a.byte[16])) << imm;
  dst.half[9] = ((u16)((u8)a.byte[17])) << imm;
  dst.half[10] = ((u16)((u8)a.byte[18])) << imm;
  dst.half[11] = ((u16)((u8)a.byte[19])) << imm;
  dst.half[12] = ((u16)((u8)a.byte[20])) << imm;
  dst.half[13] = ((u16)((u8)a.byte[21])) << imm;
  dst.half[14] = ((u16)((u8)a.byte[22])) << imm;
  dst.half[15] = ((u16)((u8)a.byte[23])) << imm;
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
