# \_\_lasx_xvssrlni_wu_d

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvssrlni_wu_d (__m256i a, __m256i b, imm0_63 imm)
#include <lasxintrin.h>
Instruction: xvssrlni.wu.d xr, xr, imm
CPU Flags: LASX
```

### Description

Logical right shift the unsigned 64-bit elements in `a` and `b` by `imm`, saturate the result to unsigned 32-bit, write the 32-bit result into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (i < 2) {
    u64 temp = (u64)b.dword[i] >> imm;
    dst.word[i] = clamp<u64>(temp, 0, 4294967295);
  } else {
    u64 temp = (u64)a.dword[i - 2] >> imm;
    dst.word[i] = clamp<u64>(temp, 0, 4294967295);
  }
}
for (int i = 4; i < 8; i++) {
  if (i < 6) {
    u64 temp = (u64)b.dword[i - 2] >> imm;
    dst.word[i] = clamp<u64>(temp, 0, 4294967295);
  } else {
    u64 temp = (u64)a.dword[i - 4] >> imm;
    dst.word[i] = clamp<u64>(temp, 0, 4294967295);
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
