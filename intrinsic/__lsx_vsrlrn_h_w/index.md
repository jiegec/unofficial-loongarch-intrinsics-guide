# \_\_lsx_vsrlrn_h_w

LSX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shift/index.md)

### Synopsis

```
__m128i __lsx_vsrlrn_h_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsrlrn.h.w vr, vr, vr
CPU Flags: LSX
```

### Description

Logical right shift with rounding each unsigned 32-bit element in `a` by the corresponding element of `b` modulo 32, write the lower 16 bits of the shifted result into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (i < 4) {
    u8 shift = (b.word[i] & 31);
    if (shift == 0) {
      dst.half[i] = (u16)(u32)a.word[i];
    } else {
      dst.half[i] = (u16)(((u32)a.word[i] >> shift) +
                          (((u32)a.word[i] >> (shift - 1)) & 0x1));
    }
  } else {
    dst.half[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
