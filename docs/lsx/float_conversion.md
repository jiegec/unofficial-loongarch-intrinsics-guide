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

## __m128 __lsx_vfcvth_s_h (__m128i a)

### Synopsis

```c++
__m128 __lsx_vfcvth_s_h (__m128i a)
#include <lsxintrin.h>
Instruction: vfcvth.s.h vr, vr
CPU Flags: LSX
```

### Description

Convert half precision floating point elements in higher half of `a` to single precision.

### Operation

```c++
{% include('vfcvth_s_h.h') %}
```

## __m128 __lsx_vfcvtl_s_h (__m128i a)

### Synopsis

```c++
__m128 __lsx_vfcvtl_s_h (__m128i a)
#include <lsxintrin.h>
Instruction: vfcvtl.s.h vr, vr
CPU Flags: LSX
```

### Description

Convert half precision floating point elements in lower half of `a` to single precision.

### Operation

```c++
{% include('vfcvtl_s_h.h') %}
```


## __m128i __lsx_vfcvt_h_s (__m128 a, __m128 b)

### Synopsis

```c++
__m128i __lsx_vfcvt_h_s (__m128, __m128 b)
#include <lsxintrin.h>
Instruction: vfcvt.h.s vr, vr, vr
CPU Flags: LSX
```

### Description

Convert single precision floating point elements in `a` and `b` to half precision.

### Operation

```c++
{% include('vfcvt_h_s.h') %}
```

{{ vffint_d_w('h') }}
{{ vffint_d_w('l') }}

{{ vffint('d', 'l') }}
{{ vffint('d', 'lu') }}
{{ vffint('s', 'w') }}
{{ vffint('s', 'wu') }}

{{ vffint_s_l() }}

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

{{ vftint('', 'l', 'd') }}
{{ vftint('', 'w', 's') }}
{{ vftint('rm', 'l', 'd') }}
{{ vftint('rm', 'w', 's') }}
{{ vftint('rp', 'l', 'd') }}
{{ vftint('rp', 'w', 's') }}
{{ vftint('rz', 'l', 'd') }}
{{ vftint('rz', 'w', 's') }}
{{ vftint('rne', 'l', 'd') }}
{{ vftint('rne', 'w', 's') }}

{{ vftint('', 'lu', 'd') }}
{{ vftint('', 'wu', 's') }}
{{ vftint('rz', 'lu', 'd') }}
{{ vftint('rz', 'wu', 's') }}

{{ vftint_w_d('') }}
{{ vftint_w_d('rm') }}
{{ vftint_w_d('rp') }}
{{ vftint_w_d('rz') }}
{{ vftint_w_d('rne') }}