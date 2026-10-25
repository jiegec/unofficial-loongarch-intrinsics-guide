# \_\_lasx_xvshuf_h

LASX intrinsic · group: [Shuffling](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shuffling/index.md)

### Synopsis

```
__m256i __lasx_xvshuf_h (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvshuf.h xr, xr, xr
CPU Flags: LASX
```

### Description

For each 128-bit lane, shuffle 16-bit elements from `b` and `c` using the low 4 bits of the indices from the corresponding lane of `a`, and save the result to `dst`.

### Examples

```
__m256i __lasx_xvshuf_h(__m256i{0x0001000200030004, 0x0005000a000b000c, 0x000f000e00010002, 0x0008000900020001}, __m256i{0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{0xabcdef1314156678, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678})
= 0x1415ef13abcd4321 0x432133441122ff00 0xaabbaabb43211234 0x1234123412344321
```

### Operation

```
for (int i = 0; i < 16; i++) {
  if ((a.half[i] % 256) >= 64 && (UARCH_LA264 || UARCH_LA464)) {
    // Caveat: observed in LA264 and LA464
    dst.half[i] = 0;
  } else if ((a.half[i] % 16) < 8) {
    dst.half[i] = c.half[(a.half[i] % 16) + ((i >= 8) ? 8 : 0)];
  } else {
    dst.half[i] = b.half[(a.half[i] % 16) + ((i >= 8) ? 0 : -8)];
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
