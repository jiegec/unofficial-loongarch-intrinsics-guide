# \_\_lasx_xvssrln_b_h

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvssrln_b_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssrln.b.h xr, xr, xr
CPU Flags: LASX
```

### Description

Logical right shift each unsigned 16-bit element in `a` by the corresponding element of `b` modulo 16, saturate the result to signed 8-bit, write it into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (i < 8) {
    u16 temp = (u16)a.half[i] >> (b.half[i] & 15);
    dst.byte[i] = clamp<u16>(temp, 0, 127);
  } else {
    dst.byte[i] = 0;
  }
}
for (int i = 16; i < 32; i++) {
  if (i < 24) {
    u16 temp = (u16)a.half[i - 8] >> (b.half[i - 8] & 15);
    dst.byte[i] = clamp<u16>(temp, 0, 127);
  } else {
    dst.byte[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
