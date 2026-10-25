# \_\_lasx_xvpermi_w

LASX intrinsic · group: [Permutation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/permutation/index.md)

### Synopsis

```
__m256i __lasx_xvpermi_w (__m256i a, __m256i b, imm0_255 imm)
#include <lasxintrin.h>
Instruction: xvpermi.w xr, xr, imm
CPU Flags: LASX
```

### Description

For each 128-bit lane, use `imm[1:0]` and `imm[3:2]` to select words 0 and 1 from `b`, and `imm[5:4]` and `imm[7:6]` to select words 2 and 3 from `a`; store the result in `dst`.

### Examples

```
__m256i __lasx_xvpermi_w( __m256i{ 0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{ 0xababababbbbbbbbb, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678}, 0x12)
= 0xbbbbbbbb43214321 0x5566778811223344 0x4321432156785678 0x12341234abcdef12
```

### Operation

```
dst.word[0] = b.word[imm & 0x3];
dst.word[1] = b.word[(imm >> 2) & 0x3];
dst.word[2] = a.word[(imm >> 4) & 0x3];
dst.word[3] = a.word[(imm >> 6) & 0x3];
dst.word[4] = b.word[4 + (imm & 0x3)];
dst.word[5] = b.word[4 + ((imm >> 2) & 0x3)];
dst.word[6] = a.word[4 + ((imm >> 4) & 0x3)];
dst.word[7] = a.word[4 + ((imm >> 6) & 0x3)];
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
