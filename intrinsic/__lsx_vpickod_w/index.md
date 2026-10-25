# \_\_lsx_vpickod_w

LSX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/misc/index.md)

### Synopsis

```
__m128i __lsx_vpickod_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vpickod.w vr, vr, vr
CPU Flags: LSX
```

### Description

For each 128-bit lane, place odd-positioned 32-bit elements of `b` into its lower half and odd-positioned 32-bit elements of `a` into its upper half.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2) ? b.word[i * 2 + 1] : a.word[(i - 2) * 2 + 1];
}

// Expands to:

if (0) {
  dst.word[0] = b.word[1];
  dst.word[1] = b.word[3];
  dst.word[2] = a.word[1];
  dst.word[3] = a.word[3];
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
