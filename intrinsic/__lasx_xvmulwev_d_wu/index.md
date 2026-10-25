# \_\_lasx_xvmulwev_d_wu

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvmulwev_d_wu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmulwev.d.wu xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply the even-positioned unsigned 32-bit elements in `a` and the corresponding unsigned 32-bit elements in `b`, extending both to 64-bit, and save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 * i] * (u64)(u32)b.word[2 * i];
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[0])) * ((u64)((u32)b.word[0]));
  dst.dword[1] = ((u64)((u32)a.word[2])) * ((u64)((u32)b.word[2]));
  dst.dword[2] = ((u64)((u32)a.word[4])) * ((u64)((u32)b.word[4]));
  dst.dword[3] = ((u64)((u32)a.word[6])) * ((u64)((u32)b.word[6]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
