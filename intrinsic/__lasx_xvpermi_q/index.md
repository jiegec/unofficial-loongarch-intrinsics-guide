# \_\_lasx_xvpermi_q

LASX intrinsic · group: [Permutation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/permutation/index.md)

### Synopsis

```
__m256i __lasx_xvpermi_q (__m256i a, __m256i b, imm0_255 imm)
#include <lasxintrin.h>
Instruction: xvpermi.q xr, xr, imm
CPU Flags: LASX
```

### Description

Select the two quad words of `dst` from the four quad words of `a` and `b` using `imm[1:0]` and `imm[5:4]`.

### Examples

```
__m256i __lasx_xvpermi_q( __m256i{ 0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{ 0xababababbbbbbbbb, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678}, 0x12)
= 0x1122334455667788 0x99aabbccddeeff00 0x1234123443214321 0x5678567856785678
```

### Operation

```
if ((imm & 0x4) && (UARCH_LA264 || UARCH_LA464)) {
  // Caveat: observed in LA264 and LA464
  dst.qword[0] = 0;
} else {
  dst.qword[0] = (imm & 2) ? a.qword[imm & 0x1] : b.qword[imm & 0x1];
}
if ((imm & 0x80) && (UARCH_LA264 || UARCH_LA464)) {
  // Caveat: observed in LA264 and LA464
  dst.qword[1] = 0;
} else {
  dst.qword[1] =
      (imm & 0x20) ? a.qword[(imm >> 4) & 0x1] : b.qword[(imm >> 4) & 0x1];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2.67             |
| 3C6000 | LA664 | 3       | 2.67             |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
