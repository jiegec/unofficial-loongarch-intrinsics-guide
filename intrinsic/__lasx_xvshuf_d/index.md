# \_\_lasx_xvshuf_d

LASX intrinsic · group: [Shuffling](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shuffling/index.md)

### Synopsis

```
__m256i __lasx_xvshuf_d (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvshuf.d xr, xr, xr
CPU Flags: LASX
```

### Description

For each 128-bit lane, shuffle 64-bit elements from `b` and `c` using the low 2 bits of the indices from the corresponding lane of `a`, and save the result to `dst`.

### Examples

```
__m256i __lasx_xvshuf_d(__m256i{0x0000000000000000, 0x0000000000000003, 0x0000000000000002, 0x0000000000000001}, __m256i{0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{0xabcdef1314156678, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678})
= 0xabcdef1314156678 0x99aabbccddeeff00 0xabcdef1212341234 0x5678567856785678
```

### Operation

```
for (int i = 0; i < 4; i++) {
  if ((a.dword[i] % 256) >= 64 && (UARCH_LA264 || UARCH_LA464)) {
    // Caveat: observed in LA264 and LA464
    dst.dword[i] = 0;
  } else if ((a.dword[i] % 4) < 2) {
    dst.dword[i] = c.dword[(a.dword[i] % 4) + ((i >= 2) ? 2 : 0)];
  } else {
    dst.dword[i] = b.dword[(a.dword[i] % 4) + ((i >= 2) ? 0 : -2)];
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 2                |
| 3C6000 | LA664 | 1       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
