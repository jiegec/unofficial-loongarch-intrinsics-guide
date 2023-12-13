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

{{ vslli('b') }}
{{ vslli('h') }}
{{ vslli('w') }}
{{ vslli('d') }}

{{ vsllwil('h', 'b') }}
{{ vsllwil('hu', 'bu') }}
{{ vsllwil('w', 'h') }}
{{ vsllwil('wu', 'hu') }}
{{ vsllwil('d', 'w') }}
{{ vsllwil('du', 'wu') }}

{{ vsra('b') }}
{{ vsra('h') }}
{{ vsra('w') }}
{{ vsra('d') }}

{{ vsrai('b') }}
{{ vsrai('h') }}
{{ vsrai('w') }}
{{ vsrai('d') }}

{{ vsran('b', 'h') }}
{{ vsran('h', 'w') }}
{{ vsran('w', 'd') }}

{{ vsrani('b', 'h') }}
{{ vsrani('h', 'w') }}
{{ vsrani('w', 'd') }}
{{ vsrani('d', 'q') }}

{{ vsrar('b') }}
{{ vsrar('h') }}
{{ vsrar('w') }}
{{ vsrar('d') }}

{{ vsrari('b') }}
{{ vsrari('h') }}
{{ vsrari('w') }}
{{ vsrari('d') }}


{{ vsrl('b') }}
{{ vsrl('h') }}
{{ vsrl('w') }}
{{ vsrl('d') }}

{{ vsrli('b') }}
{{ vsrli('h') }}
{{ vsrli('w') }}
{{ vsrli('d') }}

{{ vsrln('b', 'h') }}
{{ vsrln('h', 'w') }}
{{ vsrln('w', 'd') }}

{{ vsrlni('b', 'h') }}
{{ vsrlni('h', 'w') }}
{{ vsrlni('w', 'd') }}
{{ vsrlni('d', 'q') }}

{{ vsrlr('b') }}
{{ vsrlr('h') }}
{{ vsrlr('w') }}
{{ vsrlr('d') }}

{{ vsrlri('b') }}
{{ vsrlri('h') }}
{{ vsrlri('w') }}
{{ vsrlri('d') }}


{{ vrotr('b') }}
{{ vrotr('h') }}
{{ vrotr('w') }}
{{ vrotr('d') }}

{{ vrotri('b') }}
{{ vrotri('h') }}
{{ vrotri('w') }}
{{ vrotri('d') }}
