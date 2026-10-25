# \_\_lasx_xvsll_d

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvsll_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsll.d xr, xr, xr
CPU Flags: LASX
```

### Description

Logical left shift each unsigned 64-bit element in `a` by the corresponding element of `b` modulo 64, store the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = a.dword[i] << (b.dword[i] & 0x3f);
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
