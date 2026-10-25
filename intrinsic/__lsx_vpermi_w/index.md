# \_\_lsx_vpermi_w

LSX intrinsic · group: [Permutation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/permutation/index.md)

### Synopsis

```
__m128i __lsx_vpermi_w (__m128i a, __m128i b, imm0_255 imm)
#include <lsxintrin.h>
Instruction: vpermi.w vr, vr, imm
CPU Flags: LSX
```

### Description

For each 128-bit lane, use `imm[1:0]` and `imm[3:2]` to select words 0 and 1 from `b`, and `imm[5:4]` and `imm[7:6]` to select words 2 and 3 from `a`; store the result in `dst`.

### Examples

```
__m128i __lsx_vpermi_w(__m128i{0x1122334455667788, 0x99aabbccddeeff00}, __m128i{0xababababbbbbbbbb, 0x1234123443214321}, 0x12)
= 0xbbbbbbbb43214321 0x5566778811223344
```

### Operation

```
dst.word[0] = b.word[imm & 0x3];
dst.word[1] = b.word[(imm >> 2) & 0x3];
dst.word[2] = a.word[(imm >> 4) & 0x3];
dst.word[3] = a.word[(imm >> 6) & 0x3];
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
