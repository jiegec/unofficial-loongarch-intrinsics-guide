# \_\_lsx_vpickev_h

LSX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/misc/index.md)

### Synopsis

```
__m128i __lsx_vpickev_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vpickev.h vr, vr, vr
CPU Flags: LSX
```

### Description

For each 128-bit lane, place even-positioned 16-bit elements of `b` into its lower half and even-positioned 16-bit elements of `a` into its upper half.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (i < 4) ? b.half[i * 2] : a.half[(i - 4) * 2];
}

// Expands to:

if (0) {
  dst.half[0] = b.half[0];
  dst.half[1] = b.half[2];
  dst.half[2] = b.half[4];
  dst.half[3] = b.half[6];
  dst.half[4] = a.half[0];
  dst.half[5] = a.half[2];
  dst.half[6] = a.half[4];
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
