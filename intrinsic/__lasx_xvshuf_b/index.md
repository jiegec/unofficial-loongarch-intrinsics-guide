# \_\_lasx_xvshuf_b

LASX intrinsic · group: [Shuffling](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shuffling/index.md)

### Synopsis

```
__m256i __lasx_xvshuf_b (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvshuf.b xr, xr, xr, xr
CPU Flags: LASX
```

### Description

For each 128-bit lane, shuffle bytes from `a` and `b` using the low 5 bits of the indices from the corresponding lane of `c`.

Caveat: the indices are placed in `c`, while in other `vshuf` intrinsics, they are placed in `a`.

Caution: the result is undefined when the highest 3 bits of the index byte are non-zero.

### Examples

```
__m256i __lasx_xvshuf_b(__m256i{0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{0xabcdef1314156678, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678}, __m256i{0x1f1f00001a0a1b0b, 0x1111120213031404, 0x0102030405060708, 0x1112131405060708})
= 0x99997878ee21dd43 0x7777661555144413 0x4321433412341278 0x1234121212341278
```

### Operation

```
for (int i = 0; i < 32; i++) {
  if ((c.byte[i] % 256) >= 64 && (UARCH_LA264 || UARCH_LA464)) {
    // Caveat: observed in LA264 and LA464
    dst.byte[i] = 0;
  } else if ((c.byte[i] % 32) < 16) {
    dst.byte[i] = b.byte[(c.byte[i] % 32) + ((i >= 16) ? 16 : 0)];
  } else {
    dst.byte[i] = a.byte[(c.byte[i] % 32) + ((i >= 16) ? 0 : -16)];
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
