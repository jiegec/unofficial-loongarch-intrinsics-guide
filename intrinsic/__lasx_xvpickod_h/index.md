# \_\_lasx_xvpickod_h

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvpickod_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvpickod.h xr, xr, xr
CPU Flags: LASX
```

### Description

For each 128-bit lane, place odd-positioned 16-bit elements of `b` into its lower half and odd-positioned 16-bit elements of `a` into its upper half.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (i < 4) ? b.half[i * 2 + 1] : a.half[(i - 4) * 2 + 1];
}
for (int i = 8; i < 16; i++) {
  dst.half[i] = (i < 12) ? b.half[(i - 4) * 2 + 1] : a.half[(i - 8) * 2 + 1];
}

// Expands to:

if (0) {
  dst.half[0] = b.half[1];
  dst.half[1] = b.half[3];
  dst.half[2] = b.half[5];
  dst.half[3] = b.half[7];
  dst.half[4] = a.half[1];
  dst.half[5] = a.half[3];
  dst.half[6] = a.half[5];
  dst.half[7] = a.half[7];
  dst.half[8] = b.half[9];
  dst.half[9] = b.half[11];
  dst.half[10] = b.half[13];
  dst.half[11] = b.half[15];
  dst.half[12] = a.half[9];
  dst.half[13] = a.half[11];
  dst.half[14] = a.half[13];
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
