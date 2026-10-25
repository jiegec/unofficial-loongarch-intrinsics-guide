# \_\_lasx_xvpermi_d

LASX intrinsic · group: [Permutation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/permutation/index.md)

### Synopsis

```
__m256i __lasx_xvpermi_d (__m256i a, imm0_255 imm)
#include <lasxintrin.h>
Instruction: xvpermi.d xr, xr, imm
CPU Flags: LASX
```

### Description

Select four double words from `a` using `imm[1:0]`, `imm[3:2]`, `imm[5:4]`, and `imm[7:6]`, and store them in `dst`.

### Examples

```
__m256i __lasx_xvpermi_d( __m256i{ 0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, 0x12)
= 0xabcdef1212341234 0x1122334455667788 0x99aabbccddeeff00 0x1122334455667788
```

### Operation

```
dst.dword[0] = a.dword[imm & 0x3];
dst.dword[1] = a.dword[(imm >> 2) & 0x3];
dst.dword[2] = a.dword[(imm >> 4) & 0x3];
dst.dword[3] = a.dword[(imm >> 6) & 0x3];
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
