# \_\_lasx_xvilvh_h

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvilvh_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvilvh.h xr, xr, xr
CPU Flags: LASX
```

### Description

For each 128-bit lane, interleave the 16-bit elements in the upper half of that lane from `a` and `b`: the element from `b` is placed at the even output position and the element from `a` at the following odd position.

### Operation

```
int i;
for (i = 0; i < 8; i++) {
  dst.half[i] = (i % 2 == 1) ? a.half[i / 2 + 4] : b.half[i / 2 + 4];
}
for (; i < 16; i++) {
  dst.half[i] = (i % 2 == 1) ? a.half[i / 2 + 8] : b.half[i / 2 + 8];
}

// Expands to:

if (0) {
  dst.half[0] = b.half[4];
  dst.half[1] = a.half[4];
  dst.half[2] = b.half[5];
  dst.half[3] = a.half[5];
  dst.half[4] = b.half[6];
  dst.half[5] = a.half[6];
  dst.half[6] = b.half[7];
  dst.half[7] = a.half[7];
  dst.half[8] = b.half[12];
  dst.half[9] = a.half[12];
  dst.half[10] = b.half[13];
  dst.half[11] = a.half[13];
  dst.half[12] = b.half[14];
  dst.half[13] = a.half[14];
  dst.half[14] = b.half[15];
  dst.half[15] = a.half[15];
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
