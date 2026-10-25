# \_\_lasx_xvbitseti_h

LASX intrinsic · group: [Bitwise Operations](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/bitwise_operations/index.md)

### Synopsis

```
__m256i __lasx_xvbitseti_h (__m256i a, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvbitseti.h xr, xr, imm
CPU Flags: LASX
```

### Description

For each 16-bit element, set the bit indexed by `imm`, save the result in `dst`.

### Examples

```
__m256i __lasx_xvbitseti_h( __m256i{ 0x0000000000000000, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, 1)
= 0x0002000200020002 0x99aabbceddeeff02 0xabcfef1212361236 0xaabbaabbddeeddee
```

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = a.half[i] | ((u16)1 << imm);
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
