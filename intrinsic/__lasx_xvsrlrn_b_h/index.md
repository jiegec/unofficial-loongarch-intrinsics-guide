# \_\_lasx_xvsrlrn_b_h

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvsrlrn_b_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsrlrn.b.h xr, xr, xr
CPU Flags: LASX
```

### Description

Logical right shift with rounding each unsigned 16-bit element in `a` by the corresponding element of `b` modulo 16, write the lower 8 bits of the shifted result into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (i < 8) {
    u8 shift = (b.half[i] & 15);
    if (shift == 0) {
      dst.byte[i] = (u8)(u16)a.half[i];
    } else {
      dst.byte[i] = (u8)(((u16)a.half[i] >> shift) +
                         (((u16)a.half[i] >> (shift - 1)) & 0x1));
    }
  } else {
    dst.byte[i] = 0;
  }
}
for (int i = 16; i < 32; i++) {
  if (i < 24) {
    u8 shift = (b.half[i - 8] & 15);
    if (shift == 0) {
      dst.byte[i] = (u8)(u16)a.half[i - 8];
    } else {
      dst.byte[i] = (u8)(((u16)a.half[i - 8] >> shift) +
                         (((u16)a.half[i - 8] >> (shift - 1)) & 0x1));
    }
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
