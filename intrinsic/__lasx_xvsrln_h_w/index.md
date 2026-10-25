# \_\_lasx_xvsrln_h_w

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvsrln_h_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsrln.h.w xr, xr, xr
CPU Flags: LASX
```

### Description

Logical right shift each unsigned 32-bit element in `a` by the corresponding element of `b` modulo 32, write the lower 16 bits of the shifted result into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (i < 4) ? (u16)((u32)a.word[i] >> (b.word[i] & 31)) : 0;
}
for (int i = 8; i < 16; i++) {
  dst.half[i] =
      (i < 12) ? (u16)((u32)a.word[i - 4] >> (b.word[i - 4] & 31)) : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
