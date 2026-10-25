# \_\_lsx_vmaddwod_d_w

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vmaddwod_d_w (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmaddwod.d.w vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply the odd-positioned signed 32-bit elements in `b` and the corresponding signed 32-bit elements in `c`, extending both to 64-bit, and add the product to the 64-bit elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)(s32)b.word[2 * i + 1] * (s64)(s32)c.word[2 * i + 1] +
                 (s64)a.dword[i];
}

// Expands to:

if (0) {
  dst.dword[0] =
      (((s64)((s32)b.word[1])) * ((s64)((s32)c.word[1]))) + ((s64)a.dword[0]);
  dst.dword[1] =
      (((s64)((s32)b.word[3])) * ((s64)((s32)c.word[3]))) + ((s64)a.dword[1]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
