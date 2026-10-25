# \_\_lsx_vbitsel_v

LSX intrinsic · group: [Bitwise Operations](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/bitwise_operations/index.md)

### Synopsis

```
__m128i __lsx_vbitsel_v (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vbitsel.v vr, vr, vr, vr
CPU Flags: LSX
```

### Description

Compute bitwise selection: for each bit position, if the bit in `c` equals to one, copy the bit from `b` to `dst`, otherwise copy from `a`.

### Examples

```
__m128i __lsx_vbitsel_v(__m128i{0x1122334455667788, 0x99aabbccddeeff00}, __m128i{0xabababababababab, 0x1234123443214321}, __m128i{0xffff0000aaaabbbb, 0x1111222233334444})
= 0xabab3344ffeeefab 0x98ba9beccfedfb00
```

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (c.dword[i] & b.dword[i]) | (~c.dword[i] & a.dword[i]);
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
