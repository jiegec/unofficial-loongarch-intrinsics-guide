# \_\_lsx_vilvh_b

LSX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/misc/index.md)

### Synopsis

```
__m128i __lsx_vilvh_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vilvh.b vr, vr, vr
CPU Flags: LSX
```

### Description

For each 128-bit lane, interleave the 8-bit elements in the upper half of that lane from `a` and `b`: the element from `b` is placed at the even output position and the element from `a` at the following odd position.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (i % 2 == 1) ? a.byte[i / 2 + 8] : b.byte[i / 2 + 8];
}

// Expands to:

if (0) {
  dst.byte[0] = b.byte[8];
  dst.byte[1] = a.byte[8];
  dst.byte[2] = b.byte[9];
  dst.byte[3] = a.byte[9];
  dst.byte[4] = b.byte[10];
  dst.byte[5] = a.byte[10];
  dst.byte[6] = b.byte[11];
  dst.byte[7] = a.byte[11];
  dst.byte[8] = b.byte[12];
  dst.byte[9] = a.byte[12];
  dst.byte[10] = b.byte[13];
  dst.byte[11] = a.byte[13];
  dst.byte[12] = b.byte[14];
  dst.byte[13] = a.byte[14];
  dst.byte[14] = b.byte[15];
  dst.byte[15] = a.byte[15];
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
