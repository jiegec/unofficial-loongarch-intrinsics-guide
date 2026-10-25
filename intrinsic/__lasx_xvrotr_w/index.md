# \_\_lasx_xvrotr_w

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvrotr_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvrotr.w xr, xr, xr
CPU Flags: LASX
```

### Description

Rotate right each unsigned 32-bit element in `a` by the corresponding element of `b` modulo 32, store the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (a.word[i] >> (b.word[i] & 0x1f)) |
                (a.word[i] << (32 - (b.word[i] & 0x1f)));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
