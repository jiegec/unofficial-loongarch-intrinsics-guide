# \_\_lasx_xvsadd_w

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvsadd_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsadd.w xr, xr, xr
CPU Flags: LASX
```

### Description

Saturating add corresponding signed 32-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (s32)sadd((s32)a.word[i], (s32)b.word[i]);
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
