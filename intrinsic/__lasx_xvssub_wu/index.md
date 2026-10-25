# \_\_lasx_xvssub_wu

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvssub_wu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssub.wu xr, xr, xr
CPU Flags: LASX
```

### Description

Saturating subtract corresponding unsigned 32-bit elements in `b` from elements in `a`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (u32)ssub((u32)a.word[i], (u32)b.word[i]);
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
