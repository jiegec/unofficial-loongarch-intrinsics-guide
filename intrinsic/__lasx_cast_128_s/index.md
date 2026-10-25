# \_\_lasx_cast_128_s

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256 __lasx_cast_128_s (__m128 a)
#include <lsxintrin.h>
#include <lasxintrin.h>
CPU Flags: LASX
```

### Description

Return a 256-bit single precision vector whose low 128-bit half contains `a`. The high 128-bit half is unspecified.

This helper intrinsic does not name a distinct LASX instruction. Depending on the operation and optimization, it may lower to a no-op, a register move, or an existing LASX permutation instruction such as `xvpermi.q`.

### Operation

```
for (int i = 0;i < 4;i++) {
  dst.fp32[i] = a.fp32[i];
}
for (int i = 4;i < 8;i++) {
  dst.fp32[i] = unspecified;
}
```

### GCC Compatibility

Supported by GCC 16 and later.

GCC 14 and GCC 15 do not provide this intrinsic. The fallback below uses a non-volatile GNU inline assembly register alias where the older compiler has no 128/256-bit cast operator, and otherwise uses regular LASX intrinsics. It is intended for source compatibility; the native GCC 16 intrinsic may still generate better code.

```
#if defined(__loongarch_asx) && !defined(__loongarch_asx_sx_conv)
static inline __m256 __lasx_cast_128_s (__m128 a) {
  __m256 r;
  __asm__("" : "=f"(r) : "0"(a));
  return r;
}
#endif
```

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
