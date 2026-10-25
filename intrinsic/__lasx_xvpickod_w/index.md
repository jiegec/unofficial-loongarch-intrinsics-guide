# \_\_lasx_xvpickod_w

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvpickod_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvpickod.w xr, xr, xr
CPU Flags: LASX
```

### Description

For each 128-bit lane, place odd-positioned 32-bit elements of `b` into its lower half and odd-positioned 32-bit elements of `a` into its upper half.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2) ? b.word[i * 2 + 1] : a.word[(i - 2) * 2 + 1];
}
for (int i = 4; i < 8; i++) {
  dst.word[i] = (i < 6) ? b.word[(i - 2) * 2 + 1] : a.word[(i - 4) * 2 + 1];
}

// Expands to:

if (0) {
  dst.word[0] = b.word[1];
  dst.word[1] = b.word[3];
  dst.word[2] = a.word[1];
  dst.word[3] = a.word[3];
  dst.word[4] = b.word[5];
  dst.word[5] = b.word[7];
  dst.word[6] = a.word[5];
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
