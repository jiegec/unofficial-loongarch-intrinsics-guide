# \_\_lsx_vssrlrn_w_d

LSX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shift/index.md)

### Synopsis

```
__m128i __lsx_vssrlrn_w_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssrlrn.w.d vr, vr, vr
CPU Flags: LSX
```

### Description

Logical right shift with rounding each unsigned 64-bit element in `a` by the corresponding element of `b` modulo 64, saturate the result to signed 32-bit, write it into the lower half of each 128-bit lane of `dst`, and zero the upper half.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (i < 2) {
    u64 temp;
    if ((b.dword[i] & 63) == 0) {
      temp = (u64)a.dword[i];
    } else {
      temp = ((u64)a.dword[i] >> (b.dword[i] & 63)) +
             (((u64)a.dword[i] >> ((b.dword[i] & 63) - 1)) & 1);
    }
    dst.word[i] = clamp<u64>(temp, 0, 2147483647);
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
