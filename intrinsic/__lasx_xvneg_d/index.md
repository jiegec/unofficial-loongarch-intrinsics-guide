# \_\_lasx_xvneg_d

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvneg_d (__m256i a)
#include <lasxintrin.h>
Instruction: xvneg.d xr, xr
CPU Flags: LASX
```

### Description

Negate 64-bit elements in `a` and save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = -a.dword[i];
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
