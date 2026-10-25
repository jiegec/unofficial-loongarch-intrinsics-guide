# \_\_lasx_xvbitseli_b

LASX intrinsic · group: [Bitwise Operations](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/bitwise_operations/index.md)

### Synopsis

```
__m256i __lasx_xvbitseli_b (__m256i a, __m256i b, imm0_255 imm)
#include <lasxintrin.h>
Instruction: xvbitseli.b xr, xr, imm
CPU Flags: LASX
```

### Description

Use each bit of `a` as a mask: where the bit is 1, select the corresponding bit of `imm`; where it is 0, select the corresponding bit of `b`. The result is written to `dst`.

### Examples

```
__m256i __lasx_xvbitseli_b( __m256i{ 0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{ 0xabababababababab, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678}, 0x12)
= 0xba8b9aabba8b9a23 0x1216123012031221 0x1230123653115311 0x5652565212121212
```

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = (~a.byte[i] & b.byte[i]) | (a.byte[i] & (u8)imm);
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
