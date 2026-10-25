# \_\_lsx_vbitseli_b

LSX intrinsic · group: [Bitwise Operations](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/bitwise_operations/index.md)

### Synopsis

```
__m128i __lsx_vbitseli_b (__m128i a, __m128i b, imm0_255 imm)
#include <lsxintrin.h>
Instruction: vbitseli.b vr, vr, imm
CPU Flags: LSX
```

### Description

Use each bit of `a` as a mask: where the bit is 1, select the corresponding bit of `imm`; where it is 0, select the corresponding bit of `b`. The result is written to `dst`.

### Examples

```
__m128i __lsx_vbitseli_b(__m128i{0x1122334455667788, 0x99aabbccddeeff00}, __m128i{0xabababababababab, 0x1234123443214321}, 0x12)
= 0xba8b9aabba8b9a23 0x1216123012031221
```

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (~a.byte[i] & b.byte[i]) | (a.byte[i] & (u8)imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 2                |
| 3C6000   | LA664  | 1       | 2                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
