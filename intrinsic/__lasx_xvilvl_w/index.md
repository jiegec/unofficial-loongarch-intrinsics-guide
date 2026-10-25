# \_\_lasx_xvilvl_w

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvilvl_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvilvl.w xr, xr, xr
CPU Flags: LASX
```

### Description

For each 128-bit lane, interleave the 32-bit elements in the lower half of that lane from `a` and `b`: the element from `b` is placed at the even output position and the element from `a` at the following odd position.

### Operation

```
int i;
for (i = 0; i < 4; i++) {
  dst.word[i] = (i % 2 == 1) ? a.word[i / 2] : b.word[i / 2];
}
for (; i < 8; i++) {
  dst.word[i] = (i % 2 == 1) ? a.word[i / 2 + 2] : b.word[i / 2 + 2];
}

// Expands to:

if (0) {
  dst.word[0] = b.word[0];
  dst.word[1] = a.word[0];
  dst.word[2] = b.word[1];
  dst.word[3] = a.word[1];
  dst.word[4] = b.word[4];
  dst.word[5] = a.word[4];
  dst.word[6] = b.word[5];
  dst.word[7] = a.word[5];
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
