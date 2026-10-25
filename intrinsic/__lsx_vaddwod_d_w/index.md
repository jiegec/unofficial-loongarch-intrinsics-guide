# \_\_lsx_vaddwod_d_w

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vaddwod_d_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vaddwod.d.w vr, vr, vr
CPU Flags: LSX
```

### Description

Add the odd-positioned signed 32-bit elements in `a` and the corresponding signed 32-bit elements in `b`, extending both to 64-bit, and save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)(s32)a.word[2 * i + 1] + (s64)(s32)b.word[2 * i + 1];
}

// Expands to:

if (0) {
  dst.dword[0] = ((s64)((s32)a.word[1])) + ((s64)((s32)b.word[1]));
  dst.dword[1] = ((s64)((s32)a.word[3])) + ((s64)((s32)b.word[3]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
