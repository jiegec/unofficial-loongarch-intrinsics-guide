# \_\_lasx_insert_128_lo_d

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256d __lasx_insert_128_lo_d (__m256d a, __m128d b)
#include <lsxintrin.h>
#include <lasxintrin.h>
CPU Flags: LASX
```

### Description

Replace the low 128-bit half of `a` with the double precision vector `b`.

This helper intrinsic does not name a distinct LASX instruction. Depending on the operation and optimization, it may lower to a no-op, a register move, or an existing LASX permutation instruction such as `xvpermi.q`.

### Operation

```
for (int i = 0;i < 2;i++) {
  dst.fp64[i] = b.fp64[i];
  dst.fp64[i + 2] = a.fp64[i + 2];
}
```

### GCC Compatibility

Supported by GCC 16 and later.

GCC 14 and GCC 15 do not provide this intrinsic. The fallback below uses a non-volatile GNU inline assembly register alias where the older compiler has no 128/256-bit cast operator, and otherwise uses regular LASX intrinsics. It is intended for source compatibility; the native GCC 16 intrinsic may still generate better code.

```
#if defined(__loongarch_asx) && !defined(__loongarch_asx_sx_conv)
static inline __m256d __lasx_insert_128_lo_d (__m256d a, __m128d b) {
  __m256d bb;
  __asm__("" : "=f"(bb) : "0"(b));
  return (__m256d)__lasx_xvpermi_q((__m256i)bb, (__m256i)a, 0x12);
}
#endif
```

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
