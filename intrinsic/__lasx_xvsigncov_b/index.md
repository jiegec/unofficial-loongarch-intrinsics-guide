# \_\_lasx_xvsigncov_b

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvsigncov_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsigncov.b xr, xr, xr
CPU Flags: LASX
```

### Description

If the 8-bit element in `a` equals to zero, set the result to zero. If the signed 8-bit element in `a` is positive, copy element in `b` to result. Otherwise, copy negated element in `b` to result. If `a` and `b` are the same vectors, it is equivalent to computing absolute value.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] =
      (a.byte[i] == 0) ? 0 : ((s8)a.byte[i] > 0 ? b.byte[i] : -b.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 2                |
| 3C6000 | LA664 | 1       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
