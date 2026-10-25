# \_\_lasx_xvexth_h_b

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvexth_h_b (__m256i a)
#include <lasxintrin.h>
Instruction: xvexth.h.b xr, xr
CPU Flags: LASX
```

### Description

Sign-extend 8-bit elements in the higher half of every 128-bit vector of `a` to 16-bit.

### Operation

```
int i;
for (i = 0; i < 8; i++) {
  dst.half[i] = (s16)(s8)a.byte[i + 8];
}
for (; i < 16; i++) {
  dst.half[i] = (s16)(s8)a.byte[i + 16];
}

// Expands to:

if (0) {
  dst.half[0] = (s16)((s8)a.byte[8]);
  dst.half[1] = (s16)((s8)a.byte[9]);
  dst.half[2] = (s16)((s8)a.byte[10]);
  dst.half[3] = (s16)((s8)a.byte[11]);
  dst.half[4] = (s16)((s8)a.byte[12]);
  dst.half[5] = (s16)((s8)a.byte[13]);
  dst.half[6] = (s16)((s8)a.byte[14]);
  dst.half[7] = (s16)((s8)a.byte[15]);
  dst.half[8] = (s16)((s8)a.byte[24]);
  dst.half[9] = (s16)((s8)a.byte[25]);
  dst.half[10] = (s16)((s8)a.byte[26]);
  dst.half[11] = (s16)((s8)a.byte[27]);
  dst.half[12] = (s16)((s8)a.byte[28]);
  dst.half[13] = (s16)((s8)a.byte[29]);
  dst.half[14] = (s16)((s8)a.byte[30]);
  dst.half[15] = (s16)((s8)a.byte[31]);
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
