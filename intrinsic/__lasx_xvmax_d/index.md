# \_\_lasx_xvmax_d

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvmax_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmax.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compute elementwise maximum for corresponding signed 64-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = max((s64)a.dword[i], (s64)b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
