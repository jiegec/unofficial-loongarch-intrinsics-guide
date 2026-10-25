# \_\_lsx_vssrlrn_b_h

LSX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shift/index.md)

### Synopsis

```
__m128i __lsx_vssrlrn_b_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssrlrn.b.h vr, vr, vr
CPU Flags: LSX
```

### Description

Logical right shift with rounding each unsigned 16-bit element in `a` by the corresponding element of `b` modulo 16, saturate the result to signed 8-bit, write it into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (i < 8) {
    u16 temp;
    if ((b.half[i] & 15) == 0) {
      temp = (u16)a.half[i];
    } else {
      temp = ((u16)a.half[i] >> (b.half[i] & 15)) +
             (((u16)a.half[i] >> ((b.half[i] & 15) - 1)) & 1);
    }
    dst.byte[i] = clamp<u16>(temp, 0, 127);
  } else {
    dst.byte[i] = 0;
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
