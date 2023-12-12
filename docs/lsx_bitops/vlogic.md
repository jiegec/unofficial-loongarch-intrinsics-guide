# Bitwise Logical

## __m128i __lsx_vandi_b (__m128i a, imm0_255 imm)

### Synopsis

```c++
__m128i __lsx_vandi_b (__m128i a, imm0_255 imm)
#include <lsxintrin.h>
Instruction: vandi.b vr, vr, imm
CPU Flags: LSX
```

### Description

Compute bitwise AND between 8-bit elements in `a` and `imm`.

### Operation

```c++
{% include 'vandi_b.h' %}
```

## __m128i __lsx_vandn_v (__m128i a, __m128i b)

### Synopsis

```c++
__m128i __lsx_vandn_v (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vandn.v vr, vr, vr
CPU Flags: LSX
```

### Description

Compute bitwise ANDN between elements in `a` and `b`.

### Operation

```c++
{% include 'vandn_v.h' %}
```

## __m128i __lsx_vand_v (__m128i a, __m128i b)

### Synopsis

```c++
__m128i __lsx_vand_v (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vand.v vr, vr, vr
CPU Flags: LSX
```

### Description

Compute bitwise AND between elements in `a` and `b`.

### Operation

```c++
{% include 'vand_v.h' %}
```
