# \_\_lasx_xvexth_d_w

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvexth_d_w (__m256i a)
#include <lasxintrin.h>
Instruction: xvexth.d.w xr, xr
CPU Flags: LASX
```

### Description

Sign-extend 32-bit elements in the higher half of every 128-bit vector of `a` to 64-bit.

### Operation

```
int i;
for (i = 0; i < 2; i++) {
  dst.dword[i] = (s64)(s32)a.word[i + 2];
}
for (; i < 4; i++) {
  dst.dword[i] = (s64)(s32)a.word[i + 4];
}

// Expands to:

if (0) {
  dst.dword[0] = (s64)((s32)a.word[2]);
  dst.dword[1] = (s64)((s32)a.word[3]);
  dst.dword[2] = (s64)((s32)a.word[6]);
  dst.dword[3] = (s64)((s32)a.word[7]);
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
