# \_\_lasx_xvextl_q_d

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvextl_q_d (__m256i a)
#include <lasxintrin.h>
Instruction: xvextl.q.d xr, xr
CPU Flags: LASX
```

### Description

Sign-extend 64-bit elements in the lower half of every 128-bit vector of `a` to 128-bit.

### Operation

```
int i;
for (i = 0; i < 1; i++) {
  dst.qword[i] = (s128)(s64)a.dword[i];
}
for (; i < 2; i++) {
  dst.qword[i] = (s128)(s64)a.dword[i + 1];
}

// Expands to:

if (0) {
  dst.qword[0] = (s128)((s64)a.dword[0]);
  dst.qword[1] = (s128)((s64)a.dword[2]);
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
