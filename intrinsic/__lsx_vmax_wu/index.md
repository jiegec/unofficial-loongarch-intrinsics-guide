# \_\_lsx_vmax_wu

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vmax_wu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmax.wu vr, vr, vr
CPU Flags: LSX
```

### Description

Compute elementwise maximum for corresponding unsigned 32-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = max((u32)a.word[i], (u32)b.word[i]);
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
