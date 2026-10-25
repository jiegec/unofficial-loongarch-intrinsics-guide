# \_\_lsx_vfmadd_s

LSX intrinsic · group: [Fused Multiply-Add](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/fma/index.md)

### Synopsis

```
__m128 __lsx_vfmadd_s (__m128 a, __m128 b, __m128 c)
#include <lsxintrin.h>
Instruction: vfmadd.s vr, vr, vr, vr
CPU Flags: LSX
```

### Description

Compute packed single precision floating point FMA(Fused Multiply-Add): multiply elements in `a` and `b`, accumulate to elements in `c` and store the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = a.fp32[i] * b.fp32[i] + c.fp32[i];
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
