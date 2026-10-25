# \_\_lasx_xvmaddwod_d_wu

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvmaddwod_d_wu (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmaddwod.d.wu xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply the odd-positioned unsigned 32-bit elements in `b` and the corresponding unsigned 32-bit elements in `c`, extending both to 64-bit, and add the product to the 64-bit elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (u64)(u32)b.word[2 * i + 1] * (u64)(u32)c.word[2 * i + 1] +
                 (u64)a.dword[i];
}

// Expands to:

if (0) {
  dst.dword[0] =
      (((u64)((u32)b.word[1])) * ((u64)((u32)c.word[1]))) + ((u64)a.dword[0]);
  dst.dword[1] =
      (((u64)((u32)b.word[3])) * ((u64)((u32)c.word[3]))) + ((u64)a.dword[1]);
  dst.dword[2] =
      (((u64)((u32)b.word[5])) * ((u64)((u32)c.word[5]))) + ((u64)a.dword[2]);
  dst.dword[3] =
      (((u64)((u32)b.word[7])) * ((u64)((u32)c.word[7]))) + ((u64)a.dword[3]);
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
