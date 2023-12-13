# Floating Point Conversion

## __m128d __lsx_vfcvth_d_s (__m128 a)

### Synopsis

```c++
__m128d __lsx_vfcvth_d_s (__m128 a)
#include <lsxintrin.h>
Instruction: vfcvth.d.s vr, vr
CPU Flags: LSX
```

### Description

Convert single precision floating point elements in higher half of `a` to double precision.

### Operation

```c++
{% include('vfcvth_d_s.h') %}
```

## __m128d __lsx_vfcvtl_d_s (__m128 a)

### Synopsis

```c++
__m128d __lsx_vfcvtl_d_s (__m128 a)
#include <lsxintrin.h>
Instruction: vfcvtl.d.s vr, vr
CPU Flags: LSX
```

### Description

Convert single precision floating point elements in lower half of `a` to double precision.

### Operation

```c++
{% include('vfcvtl_d_s.h') %}
```


## __m128 __lsx_vfcvt_s_d (__m128d a, __m128d b)

### Synopsis

```c++
__m128 __lsx_vfcvt_s_d (__m128a, __m128d b)
#include <lsxintrin.h>
Instruction: vfcvt.s.d vr, vr, vr
CPU Flags: LSX
```

### Description

Convert double precision floating point elements in `a` and `b` to double precision.

### Operation

```c++
{% include('vfcvt_s_d.h') %}
```

{{ vftint_l_s('', 'l') }}
{{ vftint_l_s('', 'h') }}
{{ vftint_l_s('rm', 'l') }}
{{ vftint_l_s('rm', 'h') }}
{{ vftint_l_s('rp', 'l') }}
{{ vftint_l_s('rp', 'h') }}
{{ vftint_l_s('rz', 'l') }}
{{ vftint_l_s('rz', 'h') }}
{{ vftint_l_s('rne', 'l') }}
{{ vftint_l_s('rne', 'h') }}
