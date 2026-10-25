# \_\_lasx_concat_128_s

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256 __lasx_concat_128_s (__m128 lo, __m128 hi)
#include <lsxintrin.h>
#include <lasxintrin.h>
CPU Flags: LASX
```

### Description

Concatenate two 128-bit single precision vectors. `lo` becomes the low 128-bit half and `hi` becomes the high 128-bit half.

This helper intrinsic does not name a distinct LASX instruction. Depending on the operation and optimization, it may lower to a no-op, a register move, or an existing LASX permutation instruction such as `xvpermi.q`.

### Operation

```
for (int i = 0;i < 4;i++) {
  dst.fp32[i] = lo.fp32[i];
  dst.fp32[i + 4] = hi.fp32[i];
}
```

### GCC Compatibility

Supported by GCC 16 and later.

GCC 14 and GCC 15 do not provide this intrinsic. The fallback below uses a non-volatile GNU inline assembly register alias where the older compiler has no 128/256-bit cast operator, and otherwise uses regular LASX intrinsics. It is intended for source compatibility; the native GCC 16 intrinsic may still generate better code.

```
#if defined(__loongarch_asx) && !defined(__loongarch_asx_sx_conv)
static inline __m256 __lasx_concat_128_s (__m128 lo, __m128 hi) {
  __m256 l;
  __m256 h;
  __asm__("" : "=f"(l) : "0"(lo));
  __asm__("" : "=f"(h) : "0"(hi));
  return (__m256)__lasx_xvpermi_q((__m256i)l, (__m256i)h, 0x02);
}
#endif
```

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
