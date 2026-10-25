# \_\_lasx_xvshuf4i_h

LASX intrinsic · group: [Shuffling](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shuffling/index.md)

### Synopsis

```
__m256i __lasx_xvshuf4i_h (__m256i a, imm0_255 imm)
#include <lasxintrin.h>
Instruction: xvshuf4i.h xr, xr, imm
CPU Flags: LASX
```

### Description

For each group of four consecutive 16-bit elements in `a`, permute the group according to the four 2-bit fields of `imm`, save the result to `dst`.

### Examples

```
__m256i __lasx_xvshuf4i_h( __m256i{ 0xabcdef1314156678, 0x1234123443214321, 0x1234123443214321, 0x5678567856785678}, 0x12)
= 0x667814156678ef13 0x4321432143211234 0x4321432143211234 0x5678567856785678
```

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = a.half[(i & ~0x3) + ((imm >> (2 * (i & 0x3))) & 0x3)];
}

// Expands to:

if (0) {
  dst.half[0] = a.half[0 + ((imm >> 0) & 0x3)];
  dst.half[1] = a.half[0 + ((imm >> 2) & 0x3)];
  dst.half[2] = a.half[0 + ((imm >> 4) & 0x3)];
  dst.half[3] = a.half[0 + ((imm >> 6) & 0x3)];
  dst.half[4] = a.half[4 + ((imm >> 0) & 0x3)];
  dst.half[5] = a.half[4 + ((imm >> 2) & 0x3)];
  dst.half[6] = a.half[4 + ((imm >> 4) & 0x3)];
  dst.half[7] = a.half[4 + ((imm >> 6) & 0x3)];
  dst.half[8] = a.half[8 + ((imm >> 0) & 0x3)];
  dst.half[9] = a.half[8 + ((imm >> 2) & 0x3)];
  dst.half[10] = a.half[8 + ((imm >> 4) & 0x3)];
  dst.half[11] = a.half[8 + ((imm >> 6) & 0x3)];
  dst.half[12] = a.half[12 + ((imm >> 0) & 0x3)];
  dst.half[13] = a.half[12 + ((imm >> 2) & 0x3)];
  dst.half[14] = a.half[12 + ((imm >> 4) & 0x3)];
  dst.half[15] = a.half[12 + ((imm >> 6) & 0x3)];
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
