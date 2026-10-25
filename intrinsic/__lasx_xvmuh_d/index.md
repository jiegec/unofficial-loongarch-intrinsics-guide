# \_\_lasx_xvmuh_d

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvmuh_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmuh.d xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply corresponding signed 64-bit elements in `a` and `b`, save the upper half of the double-width product in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (((s128)(s64)a.dword[i] * (s128)(s64)b.dword[i])) >> 64;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
