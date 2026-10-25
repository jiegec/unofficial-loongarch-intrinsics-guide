# \_\_lsx_vssrarn_bu_h

LSX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shift/index.md)

### Synopsis

```
__m128i __lsx_vssrarn_bu_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssrarn.bu.h vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift with rounding each signed 16-bit element in `a` by the corresponding element of `b` modulo 16, saturate the result to unsigned 8-bit, write it into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (i < 8) {
    s16 temp;
    if ((b.half[i] & 15) == 0) {
      temp = (s16)a.half[i];
    } else {
      temp = ((s16)a.half[i] >> (b.half[i] & 15)) +
             (((s16)a.half[i] >> ((b.half[i] & 15) - 1)) & 1);
    }
    dst.byte[i] = clamp<s16>(temp, 0, 255);
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
