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

## __m128d __lsx_vfmadd_d (__m128d a, __m128d b, __m128d c)

### Synopsis

```c++
__m128d __lsx_vfmadd_d (__m128d a, __m128d b, __m128d c)
#include <lsxintrin.h>
Instruction: vfmadd.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compute packed double precision floating point FMA(Fused Multiply-Add): multiply elements in `a` and `b`, accumulate to elements in `c` and store the result in `dst`.

### Operation

```c++
for (int i = 0;i < 2;i++) {
    dst.fp64[i] = a.fp64[i] * b.fp64[i] + c.fp64[i];
}
```

## __m128 __lsx_vfmsub_s (__m128 a, __m128 b, __m128 c)

### Synopsis

```c++
__m128 __lsx_vfmsub_s (__m128 a, __m128 b, __m128 c)
#include <lsxintrin.h>
Instruction: vfmsub.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compute packed single precision floating point FMA(Fused Multiply-Add): multiply elements in `a` and `b`, subtract elements in `c` and store the result in `dst`.

### Operation

```c++
for (int i = 0;i < 4;i++) {
    dst.fp32[i] = a.fp32[i] * b.fp32[i] - c.fp32[i];
}
```

## __m128d __lsx_vfmsub_d (__m128d a, __m128d b, __m128d c)

### Synopsis

```c++
__m128d __lsx_vfmsub_d (__m128d a, __m128d b, __m128d c)
#include <lsxintrin.h>
Instruction: vfmsub.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compute packed double precision floating point FMA(Fused Multiply-Add): multiply elements in `a` and `b`, subtract elements in `c` and store the result in `dst`.

### Operation

```c++
for (int i = 0;i < 2;i++) {
    dst.fp64[i] = a.fp64[i] * b.fp64[i] - c.fp64[i];
}
```

## __m128 __lsx_vfnmadd_s (__m128 a, __m128 b, __m128 c)

### Synopsis

```c++
__m128 __lsx_vfnmadd_s (__m128 a, __m128 b, __m128 c)
#include <lsxintrin.h>
Instruction: vfnmadd.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compute packed single precision floating point FMA(Fused Multiply-Add): multiply elements in `a` and `b`, accumulate to elements in `c` and store the negated result in `dst`.

### Operation

```c++
for (int i = 0;i < 4;i++) {
    dst.fp32[i] = -(a.fp32[i] * b.fp32[i] + c.fp32[i]);
}
```

## __m128d __lsx_vfnmadd_d (__m128d a, __m128d b, __m128d c)

### Synopsis

```c++
__m128d __lsx_vfnmadd_d (__m128d a, __m128d b, __m128d c)
#include <lsxintrin.h>
Instruction: vfnmadd.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compute packed double precision floating point FMA(Fused Multiply-Add): multiply elements in `a` and `b`, accumulate to elements in `c` and store the negated result in `dst`.

### Operation

```c++
for (int i = 0;i < 2;i++) {
    dst.fp64[i] = (a.fp64[i] * b.fp64[i] + c.fp64[i]);
}
```

## __m128 __lsx_vfnmsub_s (__m128 a, __m128 b, __m128 c)

### Synopsis

```c++
__m128 __lsx_vfnmsub_s (__m128 a, __m128 b, __m128 c)
#include <lsxintrin.h>
Instruction: vfnmsub.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compute packed single precision floating point FMA(Fused Multiply-Add): multiply elements in `a` and `b`, subtract elements in `c` and store the negated result in `dst`.

### Operation

```c++
for (int i = 0;i < 4;i++) {
    dst.fp32[i] = -(a.fp32[i] * b.fp32[i] - c.fp32[i]);
}
```

## __m128d __lsx_vfnmsub_d (__m128d a, __m128d b, __m128d c)

### Synopsis

```c++
__m128d __lsx_vfnmsub_d (__m128d a, __m128d b, __m128d c)
#include <lsxintrin.h>
Instruction: vfnmsub.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compute packed double precision floating point FMA(Fused Multiply-Add): multiply elements in `a` and `b`, subtract elements in `c` and store the negated result in `dst`.

### Operation

```c++
for (int i = 0;i < 2;i++) {
    dst.fp64[i] = -(a.fp64[i] * b.fp64[i] - c.fp64[i]);
}
```
