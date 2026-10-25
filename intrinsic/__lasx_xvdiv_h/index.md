# \_\_lasx_xvdiv_h

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvdiv_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvdiv.h xr, xr, xr
CPU Flags: LASX
```

### Description

Divide each signed 16-bit element in `a` by the corresponding element in `b`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = (b.half[i] == 0) ? 0 : ((s16)a.half[i] / (s16)b.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency  | Throughput (IPC) |
| ------ | ----- | -------- | ---------------- |
| 3C5000 | LA464 | 21.5, 22 | 0.08(1/13)       |
| 3A6000 | LA664 | 17       | 0.12(1/8.5)      |
| 3C6000 | LA664 | 34, 40   | 0.05(1/19)       |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
