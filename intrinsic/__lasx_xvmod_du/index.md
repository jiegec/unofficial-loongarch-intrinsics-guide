# \_\_lasx_xvmod_du

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvmod_du (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmod.du xr, xr, xr
CPU Flags: LASX
```

### Description

Compute the remainder of each unsigned 64-bit element in `a` divided by the corresponding element in `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (b.dword[i] == 0) ? 0 : ((u64)a.dword[i] % (u64)b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 8, 10   | 0.11(1/9.5)      |
| 3A6000 | LA664 | 8, 10   | 0.25(1/4)        |
| 3C6000 | LA664 | 8       | 0.33(1/3)        |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
