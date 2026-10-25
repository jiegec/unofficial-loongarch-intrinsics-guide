# \_\_lasx_xvshuf4i_d

LASX intrinsic · group: [Shuffling](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shuffling/index.md)

### Synopsis

```
__m256i __lasx_xvshuf4i_d (__m256i a, __m256i b, imm0_255 imm)
#include <lasxintrin.h>
Instruction: xvshuf4i.d xr, xr, imm
CPU Flags: LASX
```

### Description

For each 128-bit lane, select two 64-bit elements from the four elements formed by the corresponding lanes of `a` and `b` using `imm[1:0]` and `imm[3:2]`, save the result to `dst`.

### Examples

```
__m256i __lasx_xvshuf4i_d( __m256i{ 0x1122334455667788, 0x99aabbccddeeff00, 0xabcdef1212341234, 0xaabbaabbddeeddee}, __m256i{ 0xabcdef1314156678, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678}, 0x12)
= 0xabcdef1314156678 0x1122334455667788 0x1234123443214321 0xabcdef1212341234
```

### Operation

```
dst.dword[0] = (imm & 2) ? b.dword[(imm & 1)] : a.dword[(imm & 1)];
dst.dword[1] =
    (imm & 8) ? b.dword[((imm >> 2) & 1)] : a.dword[((imm >> 2) & 1)];
dst.dword[2] = (imm & 2) ? b.dword[(imm & 1) + 2] : a.dword[(imm & 1) + 2];
dst.dword[3] =
    (imm & 8) ? b.dword[((imm >> 2) & 1) + 2] : a.dword[((imm >> 2) & 1) + 2];
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
