# Shift

## __m128i __lsx_vbsll_v (__m128i a, imm0_31 imm)

### Synopsis

```c++
__m128i __lsx_vbsll_v (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vbsll.v vr, vr, imm
CPU Flags: LSX
```

### Description

Compute 128-bit `a` shifted left by `imm * 8` bits.

### Operation

```c++
{% include 'vbsll_v.h' %}
```

## __m128i __lsx_vbsrl_v (__m128i a, imm0_31 imm)

### Synopsis

```c++
__m128i __lsx_vbsrl_v (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vbsrl.v vr, vr, imm
CPU Flags: LSX
```

### Description

Compute 128-bit `a` shifted right by `imm * 8` bits.

### Operation

```c++
{% include 'vbsrl_v.h' %}
```

{{ vsll('b') }}
{{ vsll('h') }}
{{ vsll('w') }}
{{ vsll('d') }}

{{ vsrl('b') }}
{{ vsrl('h') }}
{{ vsrl('w') }}
{{ vsrl('d') }}

{{ vsrlr('b') }}
{{ vsrlr('h') }}
{{ vsrlr('w') }}
{{ vsrlr('d') }}

{{ vsra('b') }}
{{ vsra('h') }}
{{ vsra('w') }}
{{ vsra('d') }}

{{ vsrar('b') }}
{{ vsrar('h') }}
{{ vsrar('w') }}
{{ vsrar('d') }}

{{ vrotr('b') }}
{{ vrotr('h') }}
{{ vrotr('w') }}
{{ vrotr('d') }}
