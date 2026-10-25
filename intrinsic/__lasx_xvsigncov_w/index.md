# \_\_lasx_xvsigncov_w

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvsigncov_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsigncov.w xr, xr, xr
CPU Flags: LASX
```

### Description

If the 32-bit element in `a` equals to zero, set the result to zero. If the signed 32-bit element in `a` is positive, copy element in `b` to result. Otherwise, copy negated element in `b` to result. If `a` and `b` are the same vectors, it is equivalent to computing absolute value.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] =
      (a.word[i] == 0) ? 0 : ((s32)a.word[i] > 0 ? b.word[i] : -b.word[i]);
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
