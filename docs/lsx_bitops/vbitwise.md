# Bitwise Operations

## __m128i __lsx_vbitsel_v (__m128i a, __m128i b, __m128i c)

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

{{ vbitclr('b') }}
{{ vbitclr('h') }}
{{ vbitclr('w') }}
{{ vbitclr('d') }}

{{ vbitclri('b') }}
{{ vbitclri('h') }}
{{ vbitclri('w') }}
{{ vbitclri('d') }}