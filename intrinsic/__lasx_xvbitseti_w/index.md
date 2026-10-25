# \_\_lasx_xvbitseti_w

LASX intrinsic · group: [Bitwise Operations](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/bitwise_operations/index.md)

### Synopsis

```
__m256i __lasx_xvbitseti_w (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvbitseti.w xr, xr, imm
CPU Flags: LASX
```

### Description

For each 32-bit element, set the bit indexed by `imm`, save the result in `dst`.

### Examples

```
__m256i __lasx_xvbitseti_w( __m256i{ 0x0000000000000000, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, 1)
= 0x0000000200000002 0x99aabbceddeeff02 0xabcdef1212341236 0xaabbaabbddeeddee
```

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = a.word[i] | ((u32)1 << imm);
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
