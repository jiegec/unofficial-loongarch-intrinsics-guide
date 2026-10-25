# \_\_lasx_xvexth_wu_hu

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvexth_wu_hu (__m256i a)
#include <lasxintrin.h>
Instruction: xvexth.wu.hu xr, xr
CPU Flags: LASX
```

### Description

Zero-extend 16-bit elements in the higher half of every 128-bit vector of `a` to 32-bit.

### Operation

```
int i;
for (i = 0; i < 4; i++) {
  dst.word[i] = (u32)(u16)a.half[i + 4];
}
for (; i < 8; i++) {
  dst.word[i] = (u32)(u16)a.half[i + 8];
}

// Expands to:

if (0) {
  dst.word[0] = (u32)((u16)a.half[4]);
  dst.word[1] = (u32)((u16)a.half[5]);
  dst.word[2] = (u32)((u16)a.half[6]);
  dst.word[3] = (u32)((u16)a.half[7]);
  dst.word[4] = (u32)((u16)a.half[12]);
  dst.word[5] = (u32)((u16)a.half[13]);
  dst.word[6] = (u32)((u16)a.half[14]);
  dst.word[7] = (u32)((u16)a.half[15]);
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
