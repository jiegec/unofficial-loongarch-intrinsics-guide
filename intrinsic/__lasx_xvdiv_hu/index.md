# \_\_lasx_xvdiv_hu

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvdiv_hu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvdiv.hu xr, xr, xr
CPU Flags: LASX
```

### Description

Divide each unsigned 16-bit element in `a` by the corresponding element in `b`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = (b.half[i] == 0) ? 0 : ((u16)a.half[i] / (u16)b.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency  | Throughput (IPC) |
| ------ | ----- | -------- | ---------------- |
| 3C5000 | LA464 | 17, 21.5 | 0.07(1/15)       |
| 3A6000 | LA664 | 17, 22   | 0.11(1/9)        |
| 3C6000 | LA664 | 17, 21.5 | 0.13(1/7.5)      |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
