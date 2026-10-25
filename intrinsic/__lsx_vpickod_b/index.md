# \_\_lsx_vpickod_b

LSX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/misc/index.md)

### Synopsis

```
__m128i __lsx_vpickod_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vpickod.b vr, vr, vr
CPU Flags: LSX
```

### Description

For each 128-bit lane, place odd-positioned 8-bit elements of `b` into its lower half and odd-positioned 8-bit elements of `a` into its upper half.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (i < 8) ? b.byte[i * 2 + 1] : a.byte[(i - 8) * 2 + 1];
}

// Expands to:

if (0) {
  dst.byte[0] = b.byte[1];
  dst.byte[1] = b.byte[3];
  dst.byte[2] = b.byte[5];
  dst.byte[3] = b.byte[7];
  dst.byte[4] = b.byte[9];
  dst.byte[5] = b.byte[11];
  dst.byte[6] = b.byte[13];
  dst.byte[7] = b.byte[15];
  dst.byte[8] = a.byte[1];
  dst.byte[9] = a.byte[3];
  dst.byte[10] = a.byte[5];
  dst.byte[11] = a.byte[7];
  dst.byte[12] = a.byte[9];
  dst.byte[13] = a.byte[11];
  dst.byte[14] = a.byte[13];
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
