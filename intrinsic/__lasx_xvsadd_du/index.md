# \_\_lasx_xvsadd_du

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvsadd_du (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsadd.du xr, xr, xr
CPU Flags: LASX
```

### Description

Saturating add corresponding unsigned 64-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (u64)sadd((u64)a.dword[i], (u64)b.dword[i]);
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
