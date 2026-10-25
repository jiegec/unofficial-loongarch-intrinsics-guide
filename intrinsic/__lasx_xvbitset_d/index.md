# \_\_lasx_xvbitset_d

LASX intrinsic · group: [Bitwise Operations](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/bitwise_operations/index.md)

### Synopsis

```
__m256i __lasx_xvbitset_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvbitset.d xr, xr, xr
CPU Flags: LASX
```

### Description

For each 64-bit element, set the bit indexed by the corresponding element of `b` modulo 64, save the result in `dst`.

### Examples

```
__m256i __lasx_xvbitset_d(__m256i{0x0000000000000000, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{0xabababababababab, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678})
= 0x0000080000000000 0x99aabbceddeeff00 0xabcdef1212341234 0xabbbaabbddeeddee
```

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = a.dword[i] | ((u64)1 << (b.dword[i] % 64));
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
