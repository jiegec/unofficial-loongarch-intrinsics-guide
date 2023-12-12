# Add/Subtract/Absolute Difference

## __m128i __lsx_vadd_b (__m128i a, __m128i b)

### Synopsis

```c++
__m128i __lsx_vadd_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vadd.b vr, vr, vr
CPU Flags: LSX
```

### Description

Add 8-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```c++
{% include 'vadd_b.h' %}
```

## __m128i __lsx_vadd_h (__m128i a, __m128i b)

### Synopsis

```c++
__m128i __lsx_vadd_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vadd.h vr, vr, vr
CPU Flags: LSX
```

### Description

Add 16-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```c++
{% include 'vadd_h.h' %}
```

## __m128i __lsx_vadd_w (__m128i a, __m128i b)

### Synopsis

```c++
__m128i __lsx_vadd_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vadd.w vr, vr, vr
CPU Flags: LSX
```

### Description

Add 32-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```c++
{% include 'vadd_w.h' %}
```

## __m128i __lsx_vadd_d (__m128i a, __m128i b)

### Synopsis

```c++
__m128i __lsx_vadd_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vadd.d vr, vr, vr
CPU Flags: LSX
```

### Description

Add 64-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```c++
{% include 'vadd_d.h' %}
```

## __m128i __lsx_vadd_q (__m128i a, __m128i b)

### Synopsis

```c++
__m128i __lsx_vadd_q (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vadd.q vr, vr, vr
CPU Flags: LSX
```

### Description

Add 128-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```c++
{% include 'vadd_q.h' %}
```

## __m128i __lsx_vabsd_b (__m128i a, __m128i b)

### Synopsis

```c++
__m128i __lsx_vabsd_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vabsd.b vr, vr, vr
CPU Flags: LSX
```

### Description

Compute absolute difference of signed 8-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```c++
{% include 'vabsd_b.h' %}
```

## __m128i __lsx_vabsd_bu (__m128i a, __m128i b)

### Synopsis

```c++
__m128i __lsx_vabsd_bu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vabsd.bu vr, vr, vr
CPU Flags: LSX
```

### Description

Compute absolute difference of unsigned 8-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```c++
{% include 'vabsd_bu.h' %}
```

## __m128i __lsx_vabsd_h (__m128i a, __m128i b)

### Synopsis

```c++
__m128i __lsx_vabsd_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vabsd.h vr, vr, vr
CPU Flags: LSX
```

### Description

Compute absolute difference of signed 16-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```c++
{% include 'vabsd_h.h' %}
```

## __m128i __lsx_vabsd_hu (__m128i a, __m128i b)

### Synopsis

```c++
__m128i __lsx_vabsd_hu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vabsd.hu vr, vr, vr
CPU Flags: LSX
```

### Description

Compute absolute difference of unsigned 16-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```c++
{% include 'vabsd_hu.h' %}
```

## __m128i __lsx_vabsd_w (__m128i a, __m128i b)

### Synopsis

```c++
__m128i __lsx_vabsd_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vabsd.w vr, vr, vr
CPU Flags: LSX
```

### Description

Compute absolute difference of signed 32-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```c++
{% include 'vabsd_w.h' %}
```

## __m128i __lsx_vabsd_wu (__m128i a, __m128i b)

### Synopsis

```c++
__m128i __lsx_vabsd_wu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vabsd.wu vr, vr, vr
CPU Flags: LSX
```

### Description

Compute absolute difference of unsigned 32-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```c++
{% include 'vabsd_wu.h' %}
```

## __m128i __lsx_vabsd_d (__m128i a, __m128i b)

### Synopsis

```c++
__m128i __lsx_vabsd_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vabsd.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compute absolute difference of signed 64-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```c++
{% include 'vabsd_d.h' %}
```

## __m128i __lsx_vabsd_du (__m128i a, __m128i b)

### Synopsis

```c++
__m128i __lsx_vabsd_du (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vabsd.du vr, vr, vr
CPU Flags: LSX
```

### Description

Compute absolute difference of unsigned 64-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```c++
{% include 'vabsd_du.h' %}
```
