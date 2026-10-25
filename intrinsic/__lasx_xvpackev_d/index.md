# \_\_lasx_xvpackev_d

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvpackev_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvpackev.d xr, xr, xr
CPU Flags: LASX
```

### Description

Interleave even-positioned 64-bit elements of `a` and `b`: the element from `b` is placed at the even output position and the element from `a` at the following odd position.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (i % 2 == 1) ? a.dword[i - 1] : b.dword[i];
}

// Expands to:

if (0) {
  dst.dword[0] = b.dword[0];
  dst.dword[1] = a.dword[0];
  dst.dword[2] = b.dword[2];
  dst.dword[3] = a.dword[2];
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
