# \_\_lasx_xvexth_qu_du

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvexth_qu_du (__m256i a)
#include <lasxintrin.h>
Instruction: xvexth.qu.du xr, xr
CPU Flags: LASX
```

### Description

Zero-extend 64-bit elements in the higher half of every 128-bit vector of `a` to 128-bit.

### Operation

```
int i;
for (i = 0; i < 1; i++) {
  dst.qword[i] = (u128)(u64)a.dword[i + 1];
}
for (; i < 2; i++) {
  dst.qword[i] = (u128)(u64)a.dword[i + 2];
}

// Expands to:

if (0) {
  dst.qword[0] = (u128)((u64)a.dword[1]);
  dst.qword[1] = (u128)((u64)a.dword[3]);
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
