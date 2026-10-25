# \_\_lasx_xvavg_du

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvavg_du (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvavg.du xr, xr, xr
CPU Flags: LASX
```

### Description

Compute the average (rounded towards negative infinity) of corresponding unsigned 64-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = ((u64)a.dword[i] >> 1) + ((u64)b.dword[i] >> 1) +
                 ((a.dword[i] & b.dword[i]) & 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
