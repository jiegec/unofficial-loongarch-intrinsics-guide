# Floatint Point Misc

## __m128i __lsx_vfclass_d (__m128d a)

### Synopsis

```c++
__m128i __lsx_vfclass_d (__m128d a)
#include <lsxintrin.h>
Instruction: vfclass.d vr, vr
CPU Flags: LSX
```

### Description

Classifiy each double precision floating point elements in `a`.

### Operation

```c++
for (int i = 0;i < 2;i++) {
    dst.dword[i] = fp_classify(a.fp64[i]);
}
```

## __m128i __lsx_vfclass_s (__m128 a)

### Synopsis

```c++
__m128i __lsx_vfclass_s (__m128d a)
#include <lsxintrin.h>
Instruction: vfclass.s vr, vr
CPU Flags: LSX
```

### Description

Classifiy each single precision floating point elements in `a`.

### Operation

```c++
for (int i = 0;i < 4;i++) {
    dst.word[i] = fp_classify(a.fp32[i]);
}
```
