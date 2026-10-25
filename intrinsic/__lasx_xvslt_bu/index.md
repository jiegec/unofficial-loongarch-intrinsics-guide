# \_\_lasx_xvslt_bu

LASX intrinsic · group: [Integer Comparison](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_comparison/index.md)

### Synopsis

```
__m256i __lasx_xvslt_bu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvslt.bu xr, xr, xr
CPU Flags: LASX
```

### Description

Compare the unsigned 8-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = ((u8)a.byte[i] < (u8)b.byte[i]) ? 0xFF : 0;
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
