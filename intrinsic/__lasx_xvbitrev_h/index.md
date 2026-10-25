# \_\_lasx_xvbitrev_h

LASX intrinsic · group: [Bitwise Operations](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/bitwise_operations/index.md)

### Synopsis

```
__m256i __lasx_xvbitrev_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvbitrev.h xr, xr, xr
CPU Flags: LASX
```

### Description

For each 16-bit element, toggle the bit indexed by the corresponding element of `b` modulo 16, save the result in `dst`.

### Examples

```
__m256i __lasx_xvbitrev_h(__m256i{0x0f0f0f0f0f0f0f0f, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{0xabababababababab, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678})
= 0x070f070f070f070f 0x99babbdcddecff02 0xabddef0212361236 0xabbbabbbdceedcee
```

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = a.half[i] ^ ((u16)1 << (b.half[i] % 16));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
