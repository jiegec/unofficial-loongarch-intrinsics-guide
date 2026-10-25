# \_\_lasx_xvadda_h

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvadda_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvadda.h xr, xr, xr
CPU Flags: LASX
```

### Description

Add the absolute values of the 16-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = abs((s16)a.half[i]) + abs((s16)b.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
