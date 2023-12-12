# Bitwise Selection

## __m128d __lsx_vfnmsub_d (__m128d a, __m128d b, __m128d c)

### Synopsis

```c++
__m128i __lsx_vbitsel_v (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vbitsel.v vr, vr, vr
CPU Flags: LSX
```

### Description

Compute bitwise selection: for each bit position, if the bit in `c` equals to one, copy the bit from `b` to `dst`, otherwise copy from `a`.

### Operation

```c++
{% include 'vbitsel_v.h' %}
```
