# Fused Multiply-Add

## __m128 __lsx_vfmadd_s (__m128 a, __m128 b, __m128 c)

### Synopsis

```c++
__m128 __lsx_vfmadd_s (__m128 a, __m128 b, __m128 c)
#include <lsxintrin.h>
Instruction: vfmadd.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compute packed single precision floating point FMA(Fused Multiply-Add): multiply elements in `a` and `b`, accumulate to elements in `c` and store the result in `dst`.

### Operation

```c++
for (int i = 0;i < 4;i++) {
    dst.fp32[i] = a.fp32[i] * b.fp32[i] + c.fp32[i];
}
```