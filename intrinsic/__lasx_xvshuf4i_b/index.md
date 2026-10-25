# \_\_lasx_xvshuf4i_b

LASX intrinsic · group: [Shuffling](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shuffling/index.md)

### Synopsis

```
__m256i __lasx_xvshuf4i_b (__m256i a, imm0_255 imm)
#include <lasxintrin.h>
Instruction: xvshuf4i.b xr, xr, imm
CPU Flags: LASX
```

### Description

For each group of four consecutive 8-bit elements in `a`, permute the group according to the four 2-bit fields of `imm`, save the result to `dst`.

### Examples

```
__m256i __lasx_xvshuf4i_b( __m256i{ 0xabcdef1314156678, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678}, 0x12)
= 0x13ef13cd78667815 0x3412343421432121 0x3412343421432121 0x7856787878567878
```

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[(i & ~0x3) + ((imm >> (2 * (i & 0x3))) & 0x3)];
}

// Expands to:

if (0) {
  dst.byte[0] = a.byte[0 + ((imm >> 0) & 0x3)];
  dst.byte[1] = a.byte[0 + ((imm >> 2) & 0x3)];
  dst.byte[2] = a.byte[0 + ((imm >> 4) & 0x3)];
  dst.byte[3] = a.byte[0 + ((imm >> 6) & 0x3)];
  dst.byte[4] = a.byte[4 + ((imm >> 0) & 0x3)];
  dst.byte[5] = a.byte[4 + ((imm >> 2) & 0x3)];
  dst.byte[6] = a.byte[4 + ((imm >> 4) & 0x3)];
  dst.byte[7] = a.byte[4 + ((imm >> 6) & 0x3)];
  dst.byte[8] = a.byte[8 + ((imm >> 0) & 0x3)];
  dst.byte[9] = a.byte[8 + ((imm >> 2) & 0x3)];
  dst.byte[10] = a.byte[8 + ((imm >> 4) & 0x3)];
  dst.byte[11] = a.byte[8 + ((imm >> 6) & 0x3)];
  dst.byte[12] = a.byte[12 + ((imm >> 0) & 0x3)];
  dst.byte[13] = a.byte[12 + ((imm >> 2) & 0x3)];
  dst.byte[14] = a.byte[12 + ((imm >> 4) & 0x3)];
  dst.byte[15] = a.byte[12 + ((imm >> 6) & 0x3)];
  dst.byte[16] = a.byte[16 + ((imm >> 0) & 0x3)];
  dst.byte[17] = a.byte[16 + ((imm >> 2) & 0x3)];
  dst.byte[18] = a.byte[16 + ((imm >> 4) & 0x3)];
  dst.byte[19] = a.byte[16 + ((imm >> 6) & 0x3)];
  dst.byte[20] = a.byte[20 + ((imm >> 0) & 0x3)];
  dst.byte[21] = a.byte[20 + ((imm >> 2) & 0x3)];
  dst.byte[22] = a.byte[20 + ((imm >> 4) & 0x3)];
  dst.byte[23] = a.byte[20 + ((imm >> 6) & 0x3)];
  dst.byte[24] = a.byte[24 + ((imm >> 0) & 0x3)];
  dst.byte[25] = a.byte[24 + ((imm >> 2) & 0x3)];
  dst.byte[26] = a.byte[24 + ((imm >> 4) & 0x3)];
  dst.byte[27] = a.byte[24 + ((imm >> 6) & 0x3)];
  dst.byte[28] = a.byte[28 + ((imm >> 0) & 0x3)];
  dst.byte[29] = a.byte[28 + ((imm >> 2) & 0x3)];
  dst.byte[30] = a.byte[28 + ((imm >> 4) & 0x3)];
  dst.byte[31] = a.byte[28 + ((imm >> 6) & 0x3)];
}
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
