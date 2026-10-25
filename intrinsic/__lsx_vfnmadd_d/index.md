# \_\_lsx_vfnmadd_d

LSX intrinsic · group: [Fused Multiply-Add](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/fma/index.md)

### Synopsis

```
__m128d __lsx_vfnmadd_d (__m128d a, __m128d b, __m128d c)
#include <lsxintrin.h>
Instruction: vfnmadd.d vr, vr, vr, vr
CPU Flags: LSX
```

### Description

Compute packed double precision floating point FMA(Fused Multiply-Add): multiply elements in `a` and `b`, accumulate to elements in `c` and store the negated result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = -(a.fp64[i] * b.fp64[i] + c.fp64[i]);
}
```

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
