# Permutation

## __m128i __lsx_vpermi_w (__m128i a, __m128i b, imm0_255 imm)

### Synopsis

```c++
__m128i __lsx_vpermi_w (__m128i a, __m128i b, imm0_255 imm)
#include <lsxintrin.h>
Instruction: vpermi.w vr, vr, imm
CPU Flags: LSX
```

### Description

Permute words from `a` and `b` with indices recorded in `imm` and store into `dst`.

### Operation

```c++
{% include 'vpermi_w.h' %}
```
