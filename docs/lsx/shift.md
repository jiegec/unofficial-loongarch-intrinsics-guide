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

{{ vsrarn('b', 'h') }}
{{ vsrarn('h', 'w') }}
{{ vsrarn('w', 'd') }}

{{ vsrarni('b', 'h') }}
{{ vsrarni('h', 'w') }}
{{ vsrarni('w', 'd') }}
{{ vsrarni('d', 'q') }}

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

{{ vsrlrn('b', 'h') }}
{{ vsrlrn('h', 'w') }}
{{ vsrlrn('w', 'd') }}

{{ vsrlrni('b', 'h') }}
{{ vsrlrni('h', 'w') }}
{{ vsrlrni('w', 'd') }}
{{ vsrlrni('d', 'q') }}

{{ vssran('b', 'h') }}
{{ vssran('bu', 'h') }}
{{ vssran('h', 'w') }}
{{ vssran('hu', 'w') }}
{{ vssran('w', 'd') }}
{{ vssran('wu', 'd') }}

{{ vssrani('b', 'h') }}
{{ vssrani('bu', 'h') }}
{{ vssrani('h', 'w') }}
{{ vssrani('hu', 'w') }}
{{ vssrani('w', 'd') }}
{{ vssrani('wu', 'd') }}
{{ vssrani('d', 'q') }}
{{ vssrani('du', 'q') }}

{{ vssrarn('b', 'h') }}
{{ vssrarn('bu', 'h') }}
{{ vssrarn('h', 'w') }}
{{ vssrarn('hu', 'w') }}
{{ vssrarn('w', 'd') }}
{{ vssrarn('wu', 'd') }}

{{ vssrarni('b', 'h') }}
{{ vssrarni('bu', 'h') }}
{{ vssrarni('h', 'w') }}
{{ vssrarni('hu', 'w') }}
{{ vssrarni('w', 'd') }}
{{ vssrarni('wu', 'd') }}
{{ vssrarni('d', 'q') }}
{{ vssrarni('du', 'q') }}

{{ vssrln('b', 'h') }}
{{ vssrln('bu', 'h') }}
{{ vssrln('h', 'w') }}
{{ vssrln('hu', 'w') }}
{{ vssrln('w', 'd') }}
{{ vssrln('wu', 'd') }}

{{ vssrlni('b', 'h') }}
{{ vssrlni('bu', 'h') }}
{{ vssrlni('h', 'w') }}
{{ vssrlni('hu', 'w') }}
{{ vssrlni('w', 'd') }}
{{ vssrlni('wu', 'd') }}
{{ vssrlni('d', 'q') }}
{{ vssrlni('du', 'q') }}

{{ vssrlrn('b', 'h') }}
{{ vssrlrn('bu', 'h') }}
{{ vssrlrn('h', 'w') }}
{{ vssrlrn('hu', 'w') }}
{{ vssrlrn('w', 'd') }}
{{ vssrlrn('wu', 'd') }}

{{ vssrlrni('b', 'h') }}
{{ vssrlrni('bu', 'h') }}
{{ vssrlrni('h', 'w') }}
{{ vssrlrni('hu', 'w') }}
{{ vssrlrni('w', 'd') }}
{{ vssrlrni('wu', 'd') }}
{{ vssrlrni('d', 'q') }}
{{ vssrlrni('du', 'q') }}

{{ vrotr('b') }}
{{ vrotr('h') }}
{{ vrotr('w') }}
{{ vrotr('d') }}

{{ vrotri('b') }}
{{ vrotri('h') }}
{{ vrotri('w') }}
{{ vrotri('d') }}
