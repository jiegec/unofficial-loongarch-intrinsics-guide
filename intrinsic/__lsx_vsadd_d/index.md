# \_\_lsx_vsadd_d

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vsadd_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsadd.d vr, vr, vr
CPU Flags: LSX
```

### Description

Saturating add corresponding signed 64-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)sadd((s64)a.dword[i], (s64)b.dword[i]);
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
