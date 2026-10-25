# \_\_lasx_xvsle_wu

LASX intrinsic · group: [Integer Comparison](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_comparison/index.md)

### Synopsis

```
__m256i __lasx_xvsle_wu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsle.wu xr, xr, xr
CPU Flags: LASX
```

### Description

Compare the unsigned 32-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = ((u32)a.word[i] <= (u32)b.word[i]) ? 0xFFFFFFFF : 0;
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
