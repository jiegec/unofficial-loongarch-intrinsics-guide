# \_\_lasx_xvandn_v

LASX intrinsic · group: [Logical](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/logical/index.md)

### Synopsis

```
__m256i __lasx_xvandn_v (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvandn.v xr, xr, xr
CPU Flags: LASX
```

### Description

Compute bitwise ANDN (~a & b) of elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = b.dword[i] & (~a.dword[i]);
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
