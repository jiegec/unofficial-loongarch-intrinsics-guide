# \_\_lasx_xvdiv_w

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvdiv_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvdiv.w xr, xr, xr
CPU Flags: LASX
```

### Description

Divide each signed 32-bit element in `a` by the corresponding element in `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (b.word[i] == 0) ? 0 : ((s32)a.word[i] / (s32)b.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency  | Throughput (IPC) |
| ------ | ----- | -------- | ---------------- |
| 3C5000 | LA464 | 11, 17.5 | 0.09(1/11.5)     |
| 3A6000 | LA664 | 11       | 0.18(1/5.5)      |
| 3C6000 | LA664 | 23.5, 30 | 0.13(1/7.5)      |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
