# \_\_lasx_xvneg_w

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvneg_w (__m256i a)
#include <lasxintrin.h>
Instruction: xvneg.w xr, xr
CPU Flags: LASX
```

### Description

Negate 32-bit elements in `a` and save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = -a.word[i];
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
