# \_\_lasx_xvexth_hu_bu

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvexth_hu_bu (__m256i a)
#include <lasxintrin.h>
Instruction: xvexth.hu.bu xr, xr
CPU Flags: LASX
```

### Description

Zero-extend 8-bit elements in the higher half of every 128-bit vector of `a` to 16-bit.

### Operation

```
int i;
for (i = 0; i < 8; i++) {
  dst.half[i] = (u16)(u8)a.byte[i + 8];
}
for (; i < 16; i++) {
  dst.half[i] = (u16)(u8)a.byte[i + 16];
}

// Expands to:

if (0) {
  dst.half[0] = (u16)((u8)a.byte[8]);
  dst.half[1] = (u16)((u8)a.byte[9]);
  dst.half[2] = (u16)((u8)a.byte[10]);
  dst.half[3] = (u16)((u8)a.byte[11]);
  dst.half[4] = (u16)((u8)a.byte[12]);
  dst.half[5] = (u16)((u8)a.byte[13]);
  dst.half[6] = (u16)((u8)a.byte[14]);
  dst.half[7] = (u16)((u8)a.byte[15]);
  dst.half[8] = (u16)((u8)a.byte[24]);
  dst.half[9] = (u16)((u8)a.byte[25]);
  dst.half[10] = (u16)((u8)a.byte[26]);
  dst.half[11] = (u16)((u8)a.byte[27]);
  dst.half[12] = (u16)((u8)a.byte[28]);
  dst.half[13] = (u16)((u8)a.byte[29]);
  dst.half[14] = (u16)((u8)a.byte[30]);
  dst.half[15] = (u16)((u8)a.byte[31]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
