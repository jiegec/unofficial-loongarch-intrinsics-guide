# \_\_lasx_xvmsub_h

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvmsub_h (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmsub.h xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply 16-bit elements in `b` and `c`, subtract the low 16 bits of the product from elements in `a`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = -b.half[i] * c.half[i] + a.half[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
