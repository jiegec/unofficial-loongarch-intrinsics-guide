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
for (int i = 0;i < 16;i++) {
    if (c.byte[i] >= 64) {
        // Caveat: observed in 3C5000, but not in QEMU
        dst.byte[i] = 0;
    } else if ((c.byte[i] % 32) < 16) {
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
    if (a.half[i] >= 64) {
        // Caveat: observed in 3C5000, but not in QEMU
        dst.byte[i] = 0;
    } else if ((a.half[i] % 16) < 8) {
        dst.half[i] = c.half[a.half[i] % 8];
    } else {
        dst.half[i] = b.half[a.half[i] % 8];
    }
}
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
for (int i = 0;i < 4;i++) {
    if (a.word[i] >= 64) {
        // Caveat: observed in 3C5000, but not in QEMU
        dst.word[i] = 0;
    } else if ((a.word[i] % 8) < 4) {
        dst.word[i] = c.word[a.word[i] % 4];
    } else {
        dst.word[i] = b.word[a.word[i] % 4];
    }
}
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
for (int i = 0;i < 2;i++) {
    if (a.dword[i] >= 64) {
        // Caveat: observed in 3C5000, but not in QEMU
        dst.dword[i] = 0;
    } else if ((a.dword[i] % 4) < 2) {
        dst.dword[i] = c.dword[a.dword[i] % 2];
    } else {
        dst.dword[i] = b.dword[a.dword[i] % 2];
    }
}
```
