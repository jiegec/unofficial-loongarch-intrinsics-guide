# \_\_lasx_xvshuf4i_w

LASX intrinsic · group: [Shuffling](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shuffling/index.md)

### Synopsis

```
__m256i __lasx_xvshuf4i_w (__m256i a, imm0_255 imm)
#include <lasxintrin.h>
Instruction: xvshuf4i.w xr, xr, imm
CPU Flags: LASX
```

### Description

For each group of four consecutive 32-bit elements in `a`, permute the group according to the four 2-bit fields of `imm`, save the result to `dst`.

### Examples

```
__m256i __lasx_xvshuf4i_w( __m256i{ 0xabcdef1314156678, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678}, 0x12)
= 0x1415667843214321 0x14156678abcdef13 0x4321432156785678 0x4321432112341234
```

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = a.word[(i & ~0x3) + ((imm >> (2 * (i & 0x3))) & 0x3)];
}

// Expands to:

if (0) {
  dst.word[0] = a.word[0 + ((imm >> 0) & 0x3)];
  dst.word[1] = a.word[0 + ((imm >> 2) & 0x3)];
  dst.word[2] = a.word[0 + ((imm >> 4) & 0x3)];
  dst.word[3] = a.word[0 + ((imm >> 6) & 0x3)];
  dst.word[4] = a.word[4 + ((imm >> 0) & 0x3)];
  dst.word[5] = a.word[4 + ((imm >> 2) & 0x3)];
  dst.word[6] = a.word[4 + ((imm >> 4) & 0x3)];
  dst.word[7] = a.word[4 + ((imm >> 6) & 0x3)];
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
