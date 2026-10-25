# \_\_lasx_xvmax_h

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvmax_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmax.h xr, xr, xr
CPU Flags: LASX
```

### Description

Compute elementwise maximum for corresponding signed 16-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = max((s16)a.half[i], (s16)b.half[i]);
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
