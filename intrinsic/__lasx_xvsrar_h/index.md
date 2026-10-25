# \_\_lasx_xvsrar_h

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvsrar_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsrar.h xr, xr, xr
CPU Flags: LASX
```

### Description

Arithmetic right shift with rounding each signed 16-bit element in `a` by the corresponding element of `b` modulo 16, store the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  if ((b.half[i] & 0xf) == 0) {
    dst.half[i] = a.half[i];
  } else {
    dst.half[i] = ((s16)a.half[i] >> (b.half[i] & 0xf)) +
                  (((s16)a.half[i] >> ((b.half[i] & 0xf) - 1)) & 0x1);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
