# \_\_lsx_vssran_wu_d

LSX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shift/index.md)

### Synopsis

```
__m128i __lsx_vssran_wu_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssran.wu.d vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift each signed 64-bit element in `a` by the corresponding element of `b` modulo 64, saturate the result to unsigned 32-bit, write it into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (i < 2) {
    s64 temp = (s64)a.dword[i] >> (b.dword[i] & 63);
    dst.word[i] = clamp<s64>(temp, 0, 4294967295);
  } else {
    dst.word[i] = 0;
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
