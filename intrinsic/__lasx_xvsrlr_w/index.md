# \_\_lasx_xvsrlr_w

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvsrlr_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsrlr.w xr, xr, xr
CPU Flags: LASX
```

### Description

Logical right shift with rounding each unsigned 32-bit element in `a` by the corresponding element of `b` modulo 32, store the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  if ((b.word[i] & 0x1f) == 0) {
    dst.word[i] = a.word[i];
  } else {
    dst.word[i] = (a.word[i] >> (b.word[i] & 0x1f)) +
                  ((a.word[i] >> ((b.word[i] & 0x1f) - 1)) & 0x1);
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
