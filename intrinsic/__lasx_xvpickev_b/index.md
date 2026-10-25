# \_\_lasx_xvpickev_b

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvpickev_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvpickev.b xr, xr, xr
CPU Flags: LASX
```

### Description

For each 128-bit lane, place even-positioned 8-bit elements of `b` into its lower half and even-positioned 8-bit elements of `a` into its upper half.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (i < 8) ? b.byte[i * 2] : a.byte[(i - 8) * 2];
}
for (int i = 16; i < 32; i++) {
  dst.byte[i] = (i < 24) ? b.byte[(i - 8) * 2] : a.byte[(i - 16) * 2];
}

// Expands to:

if (0) {
  dst.byte[0] = b.byte[0];
  dst.byte[1] = b.byte[2];
  dst.byte[2] = b.byte[4];
  dst.byte[3] = b.byte[6];
  dst.byte[4] = b.byte[8];
  dst.byte[5] = b.byte[10];
  dst.byte[6] = b.byte[12];
  dst.byte[7] = b.byte[14];
  dst.byte[8] = a.byte[0];
  dst.byte[9] = a.byte[2];
  dst.byte[10] = a.byte[4];
  dst.byte[11] = a.byte[6];
  dst.byte[12] = a.byte[8];
  dst.byte[13] = a.byte[10];
  dst.byte[14] = a.byte[12];
  dst.byte[15] = a.byte[14];
  dst.byte[16] = b.byte[16];
  dst.byte[17] = b.byte[18];
  dst.byte[18] = b.byte[20];
  dst.byte[19] = b.byte[22];
  dst.byte[20] = b.byte[24];
  dst.byte[21] = b.byte[26];
  dst.byte[22] = b.byte[28];
  dst.byte[23] = b.byte[30];
  dst.byte[24] = a.byte[16];
  dst.byte[25] = a.byte[18];
  dst.byte[26] = a.byte[20];
  dst.byte[27] = a.byte[22];
  dst.byte[28] = a.byte[24];
  dst.byte[29] = a.byte[26];
  dst.byte[30] = a.byte[28];
  dst.byte[31] = a.byte[30];
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
