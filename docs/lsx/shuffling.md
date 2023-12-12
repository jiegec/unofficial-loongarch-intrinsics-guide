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

{{ vshuf_hwd('h') }}
{{ vshuf_hwd('w') }}
{{ vshuf_hwd('d') }}

{{ vshuf4i('b') }}
{{ vshuf4i('h') }}
{{ vshuf4i('w') }}
{{ vshuf4i('d') }}
