# \_\_lsx_vshuf4i_w

LSX intrinsic · group: [Shuffling](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shuffling/index.md)

### Synopsis

```
__m128i __lsx_vshuf4i_w (__m128i a, imm0_255 imm)
#include <lsxintrin.h>
Instruction: vshuf4i.w vr, vr, imm
CPU Flags: LSX
```

### Description

For each group of four consecutive 32-bit elements in `a`, permute the group according to the four 2-bit fields of `imm`, save the result to `dst`.

### Examples

```
__m128i __lsx_vshuf4i_w(__m128i{0xabcdef1314156678, 0x1234123443214321}, 0x12)
= 0x1415667843214321 0x14156678abcdef13
```

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = a.word[(i & ~0x3) + ((imm >> (2 * (i & 0x3))) & 0x3)];
}

// Expands to:

if (0) {
  dst.word[0] = a.word[0 + ((imm >> 0) & 0x3)];
  dst.word[1] = a.word[0 + ((imm >> 2) & 0x3)];
  dst.word[2] = a.word[0 + ((imm >> 4) & 0x3)];
  dst.word[3] = a.word[0 + ((imm >> 6) & 0x3)];
}
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
