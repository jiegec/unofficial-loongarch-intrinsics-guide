# \_\_lasx_xvssrln_hu_w

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvssrln_hu_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssrln.hu.w xr, xr, xr
CPU Flags: LASX
```

### Description

Logical right shift each unsigned 32-bit element in `a` by the corresponding element of `b` modulo 32, saturate the result to unsigned 16-bit, write it into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (i < 4) {
    u32 temp = (u32)a.word[i] >> (b.word[i] & 31);
    dst.half[i] = clamp<u32>(temp, 0, 65535);
  } else {
    dst.half[i] = 0;
  }
}
for (int i = 8; i < 16; i++) {
  if (i < 12) {
    u32 temp = (u32)a.word[i - 4] >> (b.word[i - 4] & 31);
    dst.half[i] = clamp<u32>(temp, 0, 65535);
  } else {
    dst.half[i] = 0;
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
