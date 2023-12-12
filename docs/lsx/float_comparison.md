# Floating Point Comparison

## __m128i __lsx_vfcmp_cond_s (__m128 a, __m128 b)

### Synopsis

```c++
__m128i __lsx_vfcmp_cond_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfcmp.cond.s vr, vr
CPU Flags: LSX
```

`cond` can be one of:

- `caf`: Quiet Always False
- `ceq`: Quiet Equal
- `cle`: Quiet Less than or Equal
- `clt`: Quiet Less than
- `cne`: Quiet Not Equal
- `cor`: Quiet Ordered
- `cueq`: Quiet Unordered Equal
- `cule`: Quiet Unordered Less than or Equal
- `cult`: Quiet Unordered Less than
- `cun`: Quiet Unordered
- `cune`: Quiet Unordered Not Equal
- `saf`: Signaling Always False
- `seq`: Signaling Equal
- `sle`: Signaling Less than or Equal
- `slt`: Signaling Less than
- `sne`: Signaling Not Equal
- `sor`: Signaling Ordered
- `sueq`: Signaling Unordered Equal
- `sule`: Signaling Unordered Less than or Equal
- `sult`: Signaling Unordered Less than
- `sun`: Signaling Unordered
- `sune`: Signaling Unordered Not Equal

### Description

Compare single precision floating point elements from `a` and `b`, save the comparison result into `dst`.

### Operation

```c++
for (int i = 0;i < 4;i++) {
    if (fp_compare(cond, a.fp32[i], b.fp32[i])) {
        dst.word[i] = 0xFFFFFFFF;
    } else {
        dst.word[i] = 0;
    }
}
```

## __m128i __lsx_vfcmp_cond_d (__m128d a, __m128d b)

### Synopsis

```c++
__m128i __lsx_vfcmp_cond_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfcmp.cond.d vr, vr
CPU Flags: LSX
```

`cond` can be one of:

- `caf`: Quiet Always False
- `ceq`: Quiet Equal
- `cle`: Quiet Less than or Equal
- `clt`: Quiet Less than
- `cne`: Quiet Not Equal
- `cor`: Quiet Ordered
- `cueq`: Quiet Unordered Equal
- `cule`: Quiet Unordered Less than or Equal
- `cult`: Quiet Unordered Less than
- `cun`: Quiet Unordered
- `cune`: Quiet Unordered Not Equal
- `saf`: Signaling Always False
- `seq`: Signaling Equal
- `sle`: Signaling Less than or Equal
- `slt`: Signaling Less than
- `sne`: Signaling Not Equal
- `sor`: Signaling Ordered
- `sueq`: Signaling Unordered Equal
- `sule`: Signaling Unordered Less than or Equal
- `sult`: Signaling Unordered Less than
- `sun`: Signaling Unordered
- `sune`: Signaling Unordered Not Equal

### Description

Compare double precision floating point elements from `a` and `b`, save the comparison result into `dst`.

### Operation

```c++
for (int i = 0;i < 2;i++) {
    if (fp_compare(cond, a.fp64[i], b.fp64[i])) {
        dst.word[i] = 0xFFFFFFFFFFFFFFFF;
    } else {
        dst.word[i] = 0;
    }
}
```