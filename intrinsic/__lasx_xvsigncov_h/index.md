# \_\_lasx_xvsigncov_h

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvsigncov_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsigncov.h xr, xr, xr
CPU Flags: LASX
```

### Description

If the 16-bit element in `a` equals to zero, set the result to zero. If the signed 16-bit element in `a` is positive, copy element in `b` to result. Otherwise, copy negated element in `b` to result. If `a` and `b` are the same vectors, it is equivalent to computing absolute value.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] =
      (a.half[i] == 0) ? 0 : ((s16)a.half[i] > 0 ? b.half[i] : -b.half[i]);
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
