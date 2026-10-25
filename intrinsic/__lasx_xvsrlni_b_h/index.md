# \_\_lasx_xvsrlni_b_h

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvsrlni_b_h (__m256i a, __m256i b, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvsrlni.b.h xr, xr, imm
CPU Flags: LASX
```

### Description

Logical right shift the unsigned 16-bit elements in `a` and `b` by `imm`, write the lower 8 bits of the shifted result into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] =
      (i < 8) ? (u8)((u16)b.half[i] >> imm) : (u8)((u16)a.half[i - 8] >> imm);
}
for (int i = 16; i < 32; i++) {
  dst.byte[i] = (i < 24) ? (u8)((u16)b.half[i - 8] >> imm)
                         : (u8)((u16)a.half[i - 16] >> imm);
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
