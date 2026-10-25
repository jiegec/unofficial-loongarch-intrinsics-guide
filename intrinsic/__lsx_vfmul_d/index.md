# \_\_lsx_vfmul_d

LSX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_computation/index.md)

### Synopsis

```
__m128d __lsx_vfmul_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfmul.d vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply corresponding double precision floating point elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = a.fp64[i] * b.fp64[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 5       | 2                |
| 3A6000   | LA664  | 5       | 2                |
| 3C6000   | LA664  | 5       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
