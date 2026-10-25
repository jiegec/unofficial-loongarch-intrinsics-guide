# \_\_lsx_vpickev_d

LSX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/misc/index.md)

### Synopsis

```
__m128i __lsx_vpickev_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vpickev.d vr, vr, vr
CPU Flags: LSX
```

### Description

For each 128-bit lane, place even-positioned 64-bit elements of `b` into its lower half and even-positioned 64-bit elements of `a` into its upper half.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (i < 1) ? b.dword[i * 2] : a.dword[(i - 1) * 2];
}

// Expands to:

if (0) {
  dst.dword[0] = b.dword[0];
  dst.dword[1] = a.dword[0];
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
