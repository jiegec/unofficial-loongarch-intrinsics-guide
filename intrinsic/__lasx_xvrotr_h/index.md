# \_\_lasx_xvrotr_h

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvrotr_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvrotr.h xr, xr, xr
CPU Flags: LASX
```

### Description

Rotate right each unsigned 16-bit element in `a` by the corresponding element of `b` modulo 16, store the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = (a.half[i] >> (b.half[i] & 0xf)) |
                (a.half[i] << (16 - (b.half[i] & 0xf)));
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
