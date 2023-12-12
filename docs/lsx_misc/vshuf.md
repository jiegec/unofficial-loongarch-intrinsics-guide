# Shuffling

## __m128i __lsx_vshuf_b (__m128i a, __m128i b, __m128i c)

### Synopsis

```c++
__m128i __lsx_vshuf_b (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vshuf.b vr, vr, vr, vr
CPU Flags: LSX
```

### Description

Shuffle bytes from `a` and `b` with indices from `c`.

Caveat: the indices are placed in `c`, while in other `vshuf` intrinsics they are placed in `a`.

### Operation

```c++
{% include 'vshuf_b.h' %}
```

## __m128i __lsx_vshuf_h (__m128i a, __m128i b, __m128i c)

### Synopsis

```c++
__m128i __lsx_vshuf_h (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vshuf.h vr, vr, vr
CPU Flags: LSX
```

### Description

Shuffle half words from `b` and `c` with indices from `a`.

### Operation

```c++
{% include 'vshuf_h.h' %}
```

## __m128i __lsx_vshuf_w (__m128i a, __m128i b, __m128i c)

### Synopsis

```c++
__m128i __lsx_vshuf_w (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vshuf.w vr, vr, vr
CPU Flags: LSX
```

### Description

Shuffle words from `b` and `c` with indices from `a`.

### Operation

```c++
{% include 'vshuf_w.h' %}
```

## __m128i __lsx_vshuf_d (__m128i a, __m128i b, __m128i c)

### Synopsis

```c++
__m128i __lsx_vshuf_d (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vshuf.d vr, vr, vr
CPU Flags: LSX
```

### Description

Shuffle double words from `b` and `c` with indices from `a`.

### Operation

```c++
{% include 'vshuf_d.h' %}
```
