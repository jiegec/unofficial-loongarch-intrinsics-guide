# \_\_lasx_xvpickod_d

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvpickod_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvpickod.d xr, xr, xr
CPU Flags: LASX
```

### Description

For each 128-bit lane, place odd-positioned 64-bit elements of `b` into its lower half and odd-positioned 64-bit elements of `a` into its upper half.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (i < 1) ? b.dword[i * 2 + 1] : a.dword[(i - 1) * 2 + 1];
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = (i < 3) ? b.dword[(i - 1) * 2 + 1] : a.dword[(i - 2) * 2 + 1];
}

// Expands to:

if (0) {
  dst.dword[0] = b.dword[1];
  dst.dword[1] = a.dword[1];
  dst.dword[2] = b.dword[3];
  dst.dword[3] = a.dword[3];
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
