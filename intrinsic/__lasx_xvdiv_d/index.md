# \_\_lasx_xvdiv_d

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvdiv_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvdiv.d xr, xr, xr
CPU Flags: LASX
```

### Description

Divide each signed 64-bit element in `a` by the corresponding element in `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (b.dword[i] == 0) ? 0 : ((s64)a.dword[i] / (s64)b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency  | Throughput (IPC) |
| ------ | ----- | -------- | ---------------- |
| 3C5000 | LA464 | 8, 18.5  | 0.11(1/9)        |
| 3A6000 | LA664 | 8        | 0.25(1/4)        |
| 3C6000 | LA664 | 18.5, 19 | 0.12(1/8.5)      |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
