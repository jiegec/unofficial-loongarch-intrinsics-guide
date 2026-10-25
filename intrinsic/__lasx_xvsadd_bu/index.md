# \_\_lasx_xvsadd_bu

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvsadd_bu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsadd.bu xr, xr, xr
CPU Flags: LASX
```

### Description

Saturating add corresponding unsigned 8-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = (u8)sadd((u8)a.byte[i], (u8)b.byte[i]);
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
