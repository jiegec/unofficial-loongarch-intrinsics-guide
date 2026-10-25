# \_\_lasx_xvabsd_b

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvabsd_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvabsd.b xr, xr, xr
CPU Flags: LASX
```

### Description

Compute the absolute difference between corresponding signed 8-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = ((s8)a.byte[i] > (s8)b.byte[i]) ? (a.byte[i] - b.byte[i])
                                                : (b.byte[i] - a.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
