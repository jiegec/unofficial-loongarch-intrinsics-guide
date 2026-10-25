# \_\_lsx_vavgr_b

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vavgr_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vavgr.b vr, vr, vr
CPU Flags: LSX
```

### Description

Compute the average (rounded towards positive infinity) of corresponding signed 8-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = ((s8)a.byte[i] >> 1) + ((s8)b.byte[i] >> 1) +
                ((a.byte[i] | b.byte[i]) & 1);
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
