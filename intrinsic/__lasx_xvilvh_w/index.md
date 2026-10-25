# \_\_lasx_xvilvh_w

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvilvh_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvilvh.w xr, xr, xr
CPU Flags: LASX
```

### Description

For each 128-bit lane, interleave the 32-bit elements in the upper half of that lane from `a` and `b`: the element from `b` is placed at the even output position and the element from `a` at the following odd position.

### Operation

```
int i;
for (i = 0; i < 4; i++) {
  dst.word[i] = (i % 2 == 1) ? a.word[i / 2 + 2] : b.word[i / 2 + 2];
}
for (; i < 8; i++) {
  dst.word[i] = (i % 2 == 1) ? a.word[i / 2 + 4] : b.word[i / 2 + 4];
}

// Expands to:

if (0) {
  dst.word[0] = b.word[2];
  dst.word[1] = a.word[2];
  dst.word[2] = b.word[3];
  dst.word[3] = a.word[3];
  dst.word[4] = b.word[6];
  dst.word[5] = a.word[6];
  dst.word[6] = b.word[7];
  dst.word[7] = a.word[7];
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
