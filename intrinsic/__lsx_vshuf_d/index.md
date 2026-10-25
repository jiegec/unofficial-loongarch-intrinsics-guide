# \_\_lsx_vshuf_d

LSX intrinsic · group: [Shuffling](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shuffling/index.md)

### Synopsis

```
__m128i __lsx_vshuf_d (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vshuf.d vr, vr, vr
CPU Flags: LSX
```

### Description

For each 128-bit lane, shuffle 64-bit elements from `b` and `c` using the low 2 bits of the indices from the corresponding lane of `a`, and save the result to `dst`.

### Examples

```
__m128i __lsx_vshuf_d(__m128i{0x0000000000000001, 0x0000000000000002}, __m128i{0x1122334455667788, 0x99aabbccddeeff00}, __m128i{0xabcdef1314156678, 0x1234123443214321})
= 0x1234123443214321 0x1122334455667788
```

### Operation

```
for (int i = 0; i < 2; i++) {
  if ((a.dword[i] % 256) >= 64 && (UARCH_LA264 || UARCH_LA464)) {
    // Caveat: observed in LA264 and LA464
    dst.dword[i] = 0;
  } else if ((a.dword[i] % 4) < 2) {
    dst.dword[i] = c.dword[a.dword[i] % 2];
  } else {
    dst.dword[i] = b.dword[a.dword[i] % 2];
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 2                |
| 3C6000   | LA664  | 1       | 2                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
