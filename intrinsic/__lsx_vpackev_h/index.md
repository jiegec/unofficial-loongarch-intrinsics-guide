# \_\_lsx_vpackev_h

LSX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/misc/index.md)

### Synopsis

```
__m128i __lsx_vpackev_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vpackev.h vr, vr, vr
CPU Flags: LSX
```

### Description

Interleave even-positioned 16-bit elements of `a` and `b`: the element from `b` is placed at the even output position and the element from `a` at the following odd position.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (i % 2 == 1) ? a.half[i - 1] : b.half[i];
}

// Expands to:

if (0) {
  dst.half[0] = b.half[0];
  dst.half[1] = a.half[0];
  dst.half[2] = b.half[2];
  dst.half[3] = a.half[2];
  dst.half[4] = b.half[4];
  dst.half[5] = a.half[4];
  dst.half[6] = b.half[6];
  dst.half[7] = a.half[6];
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
