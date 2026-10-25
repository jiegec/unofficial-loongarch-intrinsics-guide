# \_\_lasx_xvrotr_b

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvrotr_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvrotr.b xr, xr, xr
CPU Flags: LASX
```

### Description

Rotate right each unsigned 8-bit element in `a` by the corresponding element of `b` modulo 8, store the result in `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] =
      (a.byte[i] >> (b.byte[i] & 0x7)) | (a.byte[i] << (8 - (b.byte[i] & 0x7)));
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
