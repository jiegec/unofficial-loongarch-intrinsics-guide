# \_\_lasx_xvmin_w

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvmin_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmin.w xr, xr, xr
CPU Flags: LASX
```

### Description

Compute elementwise minimum for corresponding signed 32-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = min((s32)a.word[i], (s32)b.word[i]);
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
