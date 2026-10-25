# \_\_lasx_xvbitsel_v

LASX intrinsic · group: [Bitwise Operations](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/bitwise_operations/index.md)

### Synopsis

```
__m256i __lasx_xvbitsel_v (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvbitsel.v xr, xr, xr, xr
CPU Flags: LASX
```

### Description

Compute bitwise selection: for each bit position, if the bit in `c` equals to one, copy the bit from `b` to `dst`, otherwise copy from `a`.

### Examples

```
__m256i __lasx_xvbitsel_v(__m256i{0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{0xabababababababab, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678}, __m256i{0xffff0000aaaabbbb, 0x1111222233334444, 0x00000000ffffffff, 0xffffffff00000000})
= 0xabab3344ffeeefab 0x98ba9beccfedfb00 0xabcdef1243214321 0x56785678ddeeddee
```

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (c.dword[i] & b.dword[i]) | (~c.dword[i] & a.dword[i]);
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
