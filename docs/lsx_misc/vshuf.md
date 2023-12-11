# Shuffling

## __m128i __lsx_vshuf_b (__m128i a, __m128i b, __m128i c)

### Synopsis

```c++
__m128i __lsx_vshuf_b (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vshuf.b vr, vr, vr
CPU Flags: LSX
```

### Description

Shuffle bytes from `a` and `b` with indices from `c`.

Caveat: the indices are placed in `c`, while in other `vshuf` intrinsics they are placed in `a`.

### Operation

```c++
for (int i = 0;i < 16;i++) {
    if ((c.byte[i] % 32) < 16) {
        dst.byte[i] = b.byte[c.byte[i] % 16];
    } else {
        dst.byte[i] = a.byte[c.byte[i] % 16];
    }
}
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
for (int i = 0;i < 8;i++) {
    if ((c.half[i] % 16) < 8) {
        dst.half[i] = c.half[a.half[i] % 8];
    } else {
        dst.half[i] = b.half[a.half[i] % 8];
    }
}
```
