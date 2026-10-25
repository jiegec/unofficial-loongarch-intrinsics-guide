# \_\_lsx_vshuf4i_h

LSX intrinsic · group: [Shuffling](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shuffling/index.md)

### Synopsis

```
__m128i __lsx_vshuf4i_h (__m128i a, imm0_255 imm)
#include <lsxintrin.h>
Instruction: vshuf4i.h vr, vr, imm
CPU Flags: LSX
```

### Description

For each group of four consecutive 16-bit elements in `a`, permute the group according to the four 2-bit fields of `imm`, save the result to `dst`.

### Examples

```
__m128i __lsx_vshuf4i_h(__m128i{0xabcdef1314156678, 0x1234123443214321}, 0x12)
= 0x667814156678ef13 0x4321432143211234
```

### Operation

```
for (int i = 0; i < 8; i++) {
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
