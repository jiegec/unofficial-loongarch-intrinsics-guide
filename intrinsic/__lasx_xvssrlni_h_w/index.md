# \_\_lasx_xvssrlni_h_w

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvssrlni_h_w (__m256i a, __m256i b, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvssrlni.h.w xr, xr, imm
CPU Flags: LASX
```

### Description

Logical right shift the unsigned 32-bit elements in `a` and `b` by `imm`, saturate the result to signed 16-bit, write the 16-bit result into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (i < 4) {
    u32 temp = (u32)b.word[i] >> imm;
    dst.half[i] = clamp<u32>(temp, 0, 32767);
  } else {
    u32 temp = (u32)a.word[i - 4] >> imm;
    dst.half[i] = clamp<u32>(temp, 0, 32767);
  }
}
for (int i = 8; i < 16; i++) {
  if (i < 12) {
    u32 temp = (u32)b.word[i - 4] >> imm;
    dst.half[i] = clamp<u32>(temp, 0, 32767);
  } else {
    u32 temp = (u32)a.word[i - 8] >> imm;
    dst.half[i] = clamp<u32>(temp, 0, 32767);
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
