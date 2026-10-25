# \_\_lsx_vpackev_b

LSX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/misc/index.md)

### Synopsis

```
__m128i __lsx_vpackev_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vpackev.b vr, vr, vr
CPU Flags: LSX
```

### Description

Interleave even-positioned 8-bit elements of `a` and `b`: the element from `b` is placed at the even output position and the element from `a` at the following odd position.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (i % 2 == 1) ? a.byte[i - 1] : b.byte[i];
}

// Expands to:

if (0) {
  dst.byte[0] = b.byte[0];
  dst.byte[1] = a.byte[0];
  dst.byte[2] = b.byte[2];
  dst.byte[3] = a.byte[2];
  dst.byte[4] = b.byte[4];
  dst.byte[5] = a.byte[4];
  dst.byte[6] = b.byte[6];
  dst.byte[7] = a.byte[6];
  dst.byte[8] = b.byte[8];
  dst.byte[9] = a.byte[8];
  dst.byte[10] = b.byte[10];
  dst.byte[11] = a.byte[10];
  dst.byte[12] = b.byte[12];
  dst.byte[13] = a.byte[12];
  dst.byte[14] = b.byte[14];
  dst.byte[15] = a.byte[14];
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
