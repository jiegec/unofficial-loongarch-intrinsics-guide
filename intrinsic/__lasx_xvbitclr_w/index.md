# \_\_lasx_xvbitclr_w

LASX intrinsic · group: [Bitwise Operations](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/bitwise_operations/index.md)

### Synopsis

```
__m256i __lasx_xvbitclr_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvbitclr.w xr, xr, xr
CPU Flags: LASX
```

### Description

For each 32-bit element, clear the bit indexed by the corresponding element of `b` modulo 32, save the result in `dst`.

### Examples

```
__m256i __lasx_xvbitclr_w(__m256i{0xffffffffffffffff, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{0xabababababababab, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678})
= 0xfffff7fffffff7ff 0x99aabbccddeeff00 0xabcdef1212341234 0xaabbaabbdceeddee
```

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = a.word[i] & (~((u32)1 << (b.word[i] % 32)));
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
