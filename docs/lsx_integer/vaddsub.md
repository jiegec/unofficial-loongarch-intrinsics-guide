# Add/Subtract

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
