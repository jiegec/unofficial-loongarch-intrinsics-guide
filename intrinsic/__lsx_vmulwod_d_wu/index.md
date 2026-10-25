# \_\_lsx_vmulwod_d_wu

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vmulwod_d_wu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmulwod.d.wu vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply the odd-positioned unsigned 32-bit elements in `a` and the corresponding unsigned 32-bit elements in `b`, extending both to 64-bit, and save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 * i + 1] * (u64)(u32)b.word[2 * i + 1];
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[1])) * ((u64)((u32)b.word[1]));
  dst.dword[1] = ((u64)((u32)a.word[3])) * ((u64)((u32)b.word[3]));
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
