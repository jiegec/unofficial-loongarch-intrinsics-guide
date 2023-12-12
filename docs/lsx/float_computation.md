# Floating Point Computation

## __m128d __lsx_vfadd_d (__m128d a, __m128d b)

### Synopsis

```c++
__m128d __lsx_vfadd_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfadd.d vr, vr, vr
CPU Flags: LSX
```

### Description

Add double precision floating point elements in `a` to `b` and store the result in `dst`.

### Operation

```c++
for (int i = 0;i < 2;i++) {
    dst.fp64[i] = a.fp64[i] + b.fp64[i];
}
```

## __m128 __lsx_vfadd_s (__m128 a, __m128 b)

### Synopsis

```c++
__m128d __lsx_vfadd_s (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfadd.s vr, vr, vr
CPU Flags: LSX
```

### Description

Add single precision floating point elements in `a` to `b` and store the result in `dst`.

### Operation

```c++
for (int i = 0;i < 4;i++) {
    dst.fp32[i] = a.fp32[i] + b.fp32[i];
}
```
