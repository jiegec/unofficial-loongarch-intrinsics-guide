# \_\_lsx_vshuf4i_b

LSX intrinsic · group: [Shuffling](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shuffling/index.md)

### Synopsis

```
__m128i __lsx_vshuf4i_b (__m128i a, imm0_255 imm)
#include <lsxintrin.h>
Instruction: vshuf4i.b vr, vr, imm
CPU Flags: LSX
```

### Description

For each group of four consecutive 8-bit elements in `a`, permute the group according to the four 2-bit fields of `imm`, save the result to `dst`.

### Examples

```
__m128i __lsx_vshuf4i_b(__m128i{0xabcdef1314156678, 0x1234123443214321}, 0x12)
= 0x13ef13cd78667815 0x3412343421432121
```

### Operation

```
for (int i = 0; i < 16; i++) {
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
