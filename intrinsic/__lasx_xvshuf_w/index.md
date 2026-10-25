# \_\_lasx_xvshuf_w

LASX intrinsic · group: [Shuffling](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shuffling/index.md)

### Synopsis

```
__m256i __lasx_xvshuf_w (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvshuf.w xr, xr, xr
CPU Flags: LASX
```

### Description

For each 128-bit lane, shuffle 32-bit elements from `b` and `c` using the low 3 bits of the indices from the corresponding lane of `a`, and save the result to `dst`.

### Examples

```
__m256i __lasx_xvshuf_w(__m256i{0x0000000200000004, 0x0000000700000005, 0x0000000100000003, 0x0000000400000000}, __m256i{0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{0xabcdef1314156678, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678})
= 0x4321432155667788 0x99aabbcc11223344 0x1234123456785678 0x1234123443214321
```

### Operation

```
for (int i = 0; i < 8; i++) {
  if ((a.word[i] % 256) >= 64 && (UARCH_LA264 || UARCH_LA464)) {
    // Caveat: observed in LA264 and LA464
    dst.word[i] = 0;
  } else if ((a.word[i] % 8) < 4) {
    dst.word[i] = c.word[(a.word[i] % 8) + ((i >= 4) ? 4 : 0)];
  } else {
    dst.word[i] = b.word[(a.word[i] % 8) + ((i >= 4) ? 0 : -4)];
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
