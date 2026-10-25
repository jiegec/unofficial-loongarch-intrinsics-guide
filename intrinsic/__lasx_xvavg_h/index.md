# \_\_lasx_xvavg_h

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvavg_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvavg.h xr, xr, xr
CPU Flags: LASX
```

### Description

Compute the average (rounded towards negative infinity) of corresponding signed 16-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = ((s16)a.half[i] >> 1) + ((s16)b.half[i] >> 1) +
                ((a.half[i] & b.half[i]) & 1);
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
