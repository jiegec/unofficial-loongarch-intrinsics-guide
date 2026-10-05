# Misc

## \_\_m256i \_\_lasx_cast_128 (\_\_m128i a)

### Synopsis

```
__m256i __lasx_cast_128 (__m128i a)
#include <lsxintrin.h>
#include <lasxintrin.h>
CPU Flags: LASX
```

### Description

Return a 256-bit integer vector whose low 128-bit half contains `a`. The high 128-bit half is unspecified.

This helper intrinsic does not name a distinct LASX instruction. Depending on the operation and optimization, it may lower to a no-op, a register move, or an existing LASX permutation instruction such as `xvpermi.q`.

### Operation

```
for (int i = 0;i < 2;i++) {
  dst.dword[i] = a.dword[i];
}
for (int i = 2;i < 4;i++) {
  dst.dword[i] = unspecified;
}
```

### GCC Compatibility

Supported by GCC 16 and later.

GCC 14 and GCC 15 do not provide this intrinsic. The fallback below uses a non-volatile GNU inline assembly register alias where the older compiler has no 128/256-bit cast operator, and otherwise uses regular LASX intrinsics. It is intended for source compatibility; the native GCC 16 intrinsic may still generate better code.

```
#if defined(__loongarch_asx) && !defined(__loongarch_asx_sx_conv)
static inline __m256i __lasx_cast_128 (__m128i a) {
  __m256i r;
  __asm__("" : "=f"(r) : "0"(a));
  return r;
}
#endif
```

## \_\_m256d \_\_lasx_cast_128_d (\_\_m128d a)

### Synopsis

```
__m256d __lasx_cast_128_d (__m128d a)
#include <lsxintrin.h>
#include <lasxintrin.h>
CPU Flags: LASX
```

### Description

Return a 256-bit double precision vector whose low 128-bit half contains `a`. The high 128-bit half is unspecified.

This helper intrinsic does not name a distinct LASX instruction. Depending on the operation and optimization, it may lower to a no-op, a register move, or an existing LASX permutation instruction such as `xvpermi.q`.

### Operation

```
for (int i = 0;i < 2;i++) {
  dst.fp64[i] = a.fp64[i];
}
for (int i = 2;i < 4;i++) {
  dst.fp64[i] = unspecified;
}
```

### GCC Compatibility

Supported by GCC 16 and later.

GCC 14 and GCC 15 do not provide this intrinsic. The fallback below uses a non-volatile GNU inline assembly register alias where the older compiler has no 128/256-bit cast operator, and otherwise uses regular LASX intrinsics. It is intended for source compatibility; the native GCC 16 intrinsic may still generate better code.

```
#if defined(__loongarch_asx) && !defined(__loongarch_asx_sx_conv)
static inline __m256d __lasx_cast_128_d (__m128d a) {
  __m256d r;
  __asm__("" : "=f"(r) : "0"(a));
  return r;
}
#endif
```

## \_\_m256 \_\_lasx_cast_128_s (\_\_m128 a)

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

## \_\_m256i \_\_lasx_concat_128 (\_\_m128i lo, \_\_m128i hi)

### Synopsis

```
__m256i __lasx_concat_128 (__m128i lo, __m128i hi)
#include <lsxintrin.h>
#include <lasxintrin.h>
CPU Flags: LASX
```

### Description

Concatenate two 128-bit integer vectors. `lo` becomes the low 128-bit half and `hi` becomes the high 128-bit half.

This helper intrinsic does not name a distinct LASX instruction. Depending on the operation and optimization, it may lower to a no-op, a register move, or an existing LASX permutation instruction such as `xvpermi.q`.

### Operation

```
for (int i = 0;i < 2;i++) {
  dst.dword[i] = lo.dword[i];
  dst.dword[i + 2] = hi.dword[i];
}
```

### GCC Compatibility

Supported by GCC 16 and later.

GCC 14 and GCC 15 do not provide this intrinsic. The fallback below uses a non-volatile GNU inline assembly register alias where the older compiler has no 128/256-bit cast operator, and otherwise uses regular LASX intrinsics. It is intended for source compatibility; the native GCC 16 intrinsic may still generate better code.

```
#if defined(__loongarch_asx) && !defined(__loongarch_asx_sx_conv)
static inline __m256i __lasx_concat_128 (__m128i lo, __m128i hi) {
  __m256i l;
  __m256i h;
  __asm__("" : "=f"(l) : "0"(lo));
  __asm__("" : "=f"(h) : "0"(hi));
  return __lasx_xvpermi_q(l, h, 0x02);
}
#endif
```

## \_\_m256d \_\_lasx_concat_128_d (\_\_m128d lo, \_\_m128d hi)

### Synopsis

```
__m256d __lasx_concat_128_d (__m128d lo, __m128d hi)
#include <lsxintrin.h>
#include <lasxintrin.h>
CPU Flags: LASX
```

### Description

Concatenate two 128-bit double precision vectors. `lo` becomes the low 128-bit half and `hi` becomes the high 128-bit half.

This helper intrinsic does not name a distinct LASX instruction. Depending on the operation and optimization, it may lower to a no-op, a register move, or an existing LASX permutation instruction such as `xvpermi.q`.

### Operation

```
for (int i = 0;i < 2;i++) {
  dst.fp64[i] = lo.fp64[i];
  dst.fp64[i + 2] = hi.fp64[i];
}
```

### GCC Compatibility

Supported by GCC 16 and later.

GCC 14 and GCC 15 do not provide this intrinsic. The fallback below uses a non-volatile GNU inline assembly register alias where the older compiler has no 128/256-bit cast operator, and otherwise uses regular LASX intrinsics. It is intended for source compatibility; the native GCC 16 intrinsic may still generate better code.

```
#if defined(__loongarch_asx) && !defined(__loongarch_asx_sx_conv)
static inline __m256d __lasx_concat_128_d (__m128d lo, __m128d hi) {
  __m256d l;
  __m256d h;
  __asm__("" : "=f"(l) : "0"(lo));
  __asm__("" : "=f"(h) : "0"(hi));
  return (__m256d)__lasx_xvpermi_q((__m256i)l, (__m256i)h, 0x02);
}
#endif
```

## \_\_m256 \_\_lasx_concat_128_s (\_\_m128 lo, \_\_m128 hi)

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

## \_\_m128i \_\_lasx_extract_128_hi (\_\_m256i a)

### Synopsis

```
__m128i __lasx_extract_128_hi (__m256i a)
#include <lsxintrin.h>
#include <lasxintrin.h>
CPU Flags: LASX
```

### Description

Extract the high 128-bit half of `a` as an integer vector.

This helper intrinsic does not name a distinct LASX instruction. Depending on the operation and optimization, it may lower to a no-op, a register move, or an existing LASX permutation instruction such as `xvpermi.q`.

### Operation

```
for (int i = 0;i < 2;i++) {
  dst.dword[i] = a.dword[i + 2];
}
```

### GCC Compatibility

Supported by GCC 16 and later.

GCC 14 and GCC 15 do not provide this intrinsic. The fallback below uses a non-volatile GNU inline assembly register alias where the older compiler has no 128/256-bit cast operator, and otherwise uses regular LASX intrinsics. It is intended for source compatibility; the native GCC 16 intrinsic may still generate better code.

```
#if defined(__loongarch_asx) && !defined(__loongarch_asx_sx_conv)
static inline __m128i __lasx_extract_128_hi (__m256i a) {
  __m256i t = __lasx_xvpermi_d(a, 0x0e);
  __m128i r;
  __asm__("" : "=f"(r) : "0"(t));
  return r;
}
#endif
```

## \_\_m128d \_\_lasx_extract_128_hi_d (\_\_m256d a)

### Synopsis

```
__m128d __lasx_extract_128_hi_d (__m256d a)
#include <lsxintrin.h>
#include <lasxintrin.h>
CPU Flags: LASX
```

### Description

Extract the high 128-bit half of `a` as a double precision vector.

This helper intrinsic does not name a distinct LASX instruction. Depending on the operation and optimization, it may lower to a no-op, a register move, or an existing LASX permutation instruction such as `xvpermi.q`.

### Operation

```
for (int i = 0;i < 2;i++) {
  dst.fp64[i] = a.fp64[i + 2];
}
```

### GCC Compatibility

Supported by GCC 16 and later.

GCC 14 and GCC 15 do not provide this intrinsic. The fallback below uses a non-volatile GNU inline assembly register alias where the older compiler has no 128/256-bit cast operator, and otherwise uses regular LASX intrinsics. It is intended for source compatibility; the native GCC 16 intrinsic may still generate better code.

```
#if defined(__loongarch_asx) && !defined(__loongarch_asx_sx_conv)
static inline __m128d __lasx_extract_128_hi_d (__m256d a) {
  __m256i t = __lasx_xvpermi_d((__m256i)a, 0x0e);
  __m128d r;
  __asm__("" : "=f"(r) : "0"(t));
  return r;
}
#endif
```

## \_\_m128 \_\_lasx_extract_128_hi_s (\_\_m256 a)

### Synopsis

```
__m128 __lasx_extract_128_hi_s (__m256 a)
#include <lsxintrin.h>
#include <lasxintrin.h>
CPU Flags: LASX
```

### Description

Extract the high 128-bit half of `a` as a single precision vector.

This helper intrinsic does not name a distinct LASX instruction. Depending on the operation and optimization, it may lower to a no-op, a register move, or an existing LASX permutation instruction such as `xvpermi.q`.

### Operation

```
for (int i = 0;i < 4;i++) {
  dst.fp32[i] = a.fp32[i + 4];
}
```

### GCC Compatibility

Supported by GCC 16 and later.

GCC 14 and GCC 15 do not provide this intrinsic. The fallback below uses a non-volatile GNU inline assembly register alias where the older compiler has no 128/256-bit cast operator, and otherwise uses regular LASX intrinsics. It is intended for source compatibility; the native GCC 16 intrinsic may still generate better code.

```
#if defined(__loongarch_asx) && !defined(__loongarch_asx_sx_conv)
static inline __m128 __lasx_extract_128_hi_s (__m256 a) {
  __m256i t = __lasx_xvpermi_d((__m256i)a, 0x0e);
  __m128 r;
  __asm__("" : "=f"(r) : "0"(t));
  return r;
}
#endif
```

## \_\_m128i \_\_lasx_extract_128_lo (\_\_m256i a)

### Synopsis

```
__m128i __lasx_extract_128_lo (__m256i a)
#include <lsxintrin.h>
#include <lasxintrin.h>
CPU Flags: LASX
```

### Description

Extract the low 128-bit half of `a` as an integer vector.

This helper intrinsic does not name a distinct LASX instruction. Depending on the operation and optimization, it may lower to a no-op, a register move, or an existing LASX permutation instruction such as `xvpermi.q`.

### Operation

```
for (int i = 0;i < 2;i++) {
  dst.dword[i] = a.dword[i];
}
```

### GCC Compatibility

Supported by GCC 16 and later.

GCC 14 and GCC 15 do not provide this intrinsic. The fallback below uses a non-volatile GNU inline assembly register alias where the older compiler has no 128/256-bit cast operator, and otherwise uses regular LASX intrinsics. It is intended for source compatibility; the native GCC 16 intrinsic may still generate better code.

```
#if defined(__loongarch_asx) && !defined(__loongarch_asx_sx_conv)
static inline __m128i __lasx_extract_128_lo (__m256i a) {
  __m128i r;
  __asm__("" : "=f"(r) : "0"(a));
  return r;
}
#endif
```

## \_\_m128d \_\_lasx_extract_128_lo_d (\_\_m256d a)

### Synopsis

```
__m128d __lasx_extract_128_lo_d (__m256d a)
#include <lsxintrin.h>
#include <lasxintrin.h>
CPU Flags: LASX
```

### Description

Extract the low 128-bit half of `a` as a double precision vector.

This helper intrinsic does not name a distinct LASX instruction. Depending on the operation and optimization, it may lower to a no-op, a register move, or an existing LASX permutation instruction such as `xvpermi.q`.

### Operation

```
for (int i = 0;i < 2;i++) {
  dst.fp64[i] = a.fp64[i];
}
```

### GCC Compatibility

Supported by GCC 16 and later.

GCC 14 and GCC 15 do not provide this intrinsic. The fallback below uses a non-volatile GNU inline assembly register alias where the older compiler has no 128/256-bit cast operator, and otherwise uses regular LASX intrinsics. It is intended for source compatibility; the native GCC 16 intrinsic may still generate better code.

```
#if defined(__loongarch_asx) && !defined(__loongarch_asx_sx_conv)
static inline __m128d __lasx_extract_128_lo_d (__m256d a) {
  __m128d r;
  __asm__("" : "=f"(r) : "0"(a));
  return r;
}
#endif
```

## \_\_m128 \_\_lasx_extract_128_lo_s (\_\_m256 a)

### Synopsis

```
__m128 __lasx_extract_128_lo_s (__m256 a)
#include <lsxintrin.h>
#include <lasxintrin.h>
CPU Flags: LASX
```

### Description

Extract the low 128-bit half of `a` as a single precision vector.

This helper intrinsic does not name a distinct LASX instruction. Depending on the operation and optimization, it may lower to a no-op, a register move, or an existing LASX permutation instruction such as `xvpermi.q`.

### Operation

```
for (int i = 0;i < 4;i++) {
  dst.fp32[i] = a.fp32[i];
}
```

### GCC Compatibility

Supported by GCC 16 and later.

GCC 14 and GCC 15 do not provide this intrinsic. The fallback below uses a non-volatile GNU inline assembly register alias where the older compiler has no 128/256-bit cast operator, and otherwise uses regular LASX intrinsics. It is intended for source compatibility; the native GCC 16 intrinsic may still generate better code.

```
#if defined(__loongarch_asx) && !defined(__loongarch_asx_sx_conv)
static inline __m128 __lasx_extract_128_lo_s (__m256 a) {
  __m128 r;
  __asm__("" : "=f"(r) : "0"(a));
  return r;
}
#endif
```

## \_\_m256i \_\_lasx_insert_128_hi (\_\_m256i a, \_\_m128i b)

### Synopsis

```
__m256i __lasx_insert_128_hi (__m256i a, __m128i b)
#include <lsxintrin.h>
#include <lasxintrin.h>
CPU Flags: LASX
```

### Description

Replace the high 128-bit half of `a` with the integer vector `b`.

This helper intrinsic does not name a distinct LASX instruction. Depending on the operation and optimization, it may lower to a no-op, a register move, or an existing LASX permutation instruction such as `xvpermi.q`.

### Operation

```
for (int i = 0;i < 2;i++) {
  dst.dword[i] = a.dword[i];
  dst.dword[i + 2] = b.dword[i];
}
```

### GCC Compatibility

Supported by GCC 16 and later.

GCC 14 and GCC 15 do not provide this intrinsic. The fallback below uses a non-volatile GNU inline assembly register alias where the older compiler has no 128/256-bit cast operator, and otherwise uses regular LASX intrinsics. It is intended for source compatibility; the native GCC 16 intrinsic may still generate better code.

```
#if defined(__loongarch_asx) && !defined(__loongarch_asx_sx_conv)
static inline __m256i __lasx_insert_128_hi (__m256i a, __m128i b) {
  __m256i bb;
  __asm__("" : "=f"(bb) : "0"(b));
  return __lasx_xvpermi_q(a, bb, 0x02);
}
#endif
```

## \_\_m256d \_\_lasx_insert_128_hi_d (\_\_m256d a, \_\_m128d b)

### Synopsis

```
__m256d __lasx_insert_128_hi_d (__m256d a, __m128d b)
#include <lsxintrin.h>
#include <lasxintrin.h>
CPU Flags: LASX
```

### Description

Replace the high 128-bit half of `a` with the double precision vector `b`.

This helper intrinsic does not name a distinct LASX instruction. Depending on the operation and optimization, it may lower to a no-op, a register move, or an existing LASX permutation instruction such as `xvpermi.q`.

### Operation

```
for (int i = 0;i < 2;i++) {
  dst.fp64[i] = a.fp64[i];
  dst.fp64[i + 2] = b.fp64[i];
}
```

### GCC Compatibility

Supported by GCC 16 and later.

GCC 14 and GCC 15 do not provide this intrinsic. The fallback below uses a non-volatile GNU inline assembly register alias where the older compiler has no 128/256-bit cast operator, and otherwise uses regular LASX intrinsics. It is intended for source compatibility; the native GCC 16 intrinsic may still generate better code.

```
#if defined(__loongarch_asx) && !defined(__loongarch_asx_sx_conv)
static inline __m256d __lasx_insert_128_hi_d (__m256d a, __m128d b) {
  __m256d bb;
  __asm__("" : "=f"(bb) : "0"(b));
  return (__m256d)__lasx_xvpermi_q((__m256i)a, (__m256i)bb, 0x02);
}
#endif
```

## \_\_m256 \_\_lasx_insert_128_hi_s (\_\_m256 a, \_\_m128 b)

### Synopsis

```
__m256 __lasx_insert_128_hi_s (__m256 a, __m128 b)
#include <lsxintrin.h>
#include <lasxintrin.h>
CPU Flags: LASX
```

### Description

Replace the high 128-bit half of `a` with the single precision vector `b`.

This helper intrinsic does not name a distinct LASX instruction. Depending on the operation and optimization, it may lower to a no-op, a register move, or an existing LASX permutation instruction such as `xvpermi.q`.

### Operation

```
for (int i = 0;i < 4;i++) {
  dst.fp32[i] = a.fp32[i];
  dst.fp32[i + 4] = b.fp32[i];
}
```

### GCC Compatibility

Supported by GCC 16 and later.

GCC 14 and GCC 15 do not provide this intrinsic. The fallback below uses a non-volatile GNU inline assembly register alias where the older compiler has no 128/256-bit cast operator, and otherwise uses regular LASX intrinsics. It is intended for source compatibility; the native GCC 16 intrinsic may still generate better code.

```
#if defined(__loongarch_asx) && !defined(__loongarch_asx_sx_conv)
static inline __m256 __lasx_insert_128_hi_s (__m256 a, __m128 b) {
  __m256 bb;
  __asm__("" : "=f"(bb) : "0"(b));
  return (__m256)__lasx_xvpermi_q((__m256i)a, (__m256i)bb, 0x02);
}
#endif
```

## \_\_m256i \_\_lasx_insert_128_lo (\_\_m256i a, \_\_m128i b)

### Synopsis

```
__m256i __lasx_insert_128_lo (__m256i a, __m128i b)
#include <lsxintrin.h>
#include <lasxintrin.h>
CPU Flags: LASX
```

### Description

Replace the low 128-bit half of `a` with the integer vector `b`.

This helper intrinsic does not name a distinct LASX instruction. Depending on the operation and optimization, it may lower to a no-op, a register move, or an existing LASX permutation instruction such as `xvpermi.q`.

### Operation

```
for (int i = 0;i < 2;i++) {
  dst.dword[i] = b.dword[i];
  dst.dword[i + 2] = a.dword[i + 2];
}
```

### GCC Compatibility

Supported by GCC 16 and later.

GCC 14 and GCC 15 do not provide this intrinsic. The fallback below uses a non-volatile GNU inline assembly register alias where the older compiler has no 128/256-bit cast operator, and otherwise uses regular LASX intrinsics. It is intended for source compatibility; the native GCC 16 intrinsic may still generate better code.

```
#if defined(__loongarch_asx) && !defined(__loongarch_asx_sx_conv)
static inline __m256i __lasx_insert_128_lo (__m256i a, __m128i b) {
  __m256i bb;
  __asm__("" : "=f"(bb) : "0"(b));
  return __lasx_xvpermi_q(bb, a, 0x12);
}
#endif
```

## \_\_m256d \_\_lasx_insert_128_lo_d (\_\_m256d a, \_\_m128d b)

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

## \_\_m256 \_\_lasx_insert_128_lo_s (\_\_m256 a, \_\_m128 b)

### Synopsis

```
__m256 __lasx_insert_128_lo_s (__m256 a, __m128 b)
#include <lsxintrin.h>
#include <lasxintrin.h>
CPU Flags: LASX
```

### Description

Replace the low 128-bit half of `a` with the single precision vector `b`.

This helper intrinsic does not name a distinct LASX instruction. Depending on the operation and optimization, it may lower to a no-op, a register move, or an existing LASX permutation instruction such as `xvpermi.q`.

### Operation

```
for (int i = 0;i < 4;i++) {
  dst.fp32[i] = b.fp32[i];
  dst.fp32[i + 4] = a.fp32[i + 4];
}
```

### GCC Compatibility

Supported by GCC 16 and later.

GCC 14 and GCC 15 do not provide this intrinsic. The fallback below uses a non-volatile GNU inline assembly register alias where the older compiler has no 128/256-bit cast operator, and otherwise uses regular LASX intrinsics. It is intended for source compatibility; the native GCC 16 intrinsic may still generate better code.

```
#if defined(__loongarch_asx) && !defined(__loongarch_asx_sx_conv)
static inline __m256 __lasx_insert_128_lo_s (__m256 a, __m128 b) {
  __m256 bb;
  __asm__("" : "=f"(bb) : "0"(b));
  return (__m256)__lasx_xvpermi_q((__m256i)bb, (__m256i)a, 0x12);
}
#endif
```

## \_\_m256i \_\_lasx_xvexth_h_b (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvexth_h_b (__m256i a)
#include <lasxintrin.h>
Instruction: xvexth.h.b xr, xr
CPU Flags: LASX
```

### Description

Extend signed 8-bit elements in the higher half of `a` to 16-bit.

### Operation

```
int i;
for (i = 0; i < 8; i++) {
  dst.half[i] = (s16)(s8)a.byte[i + 8];
}
for (; i < 16; i++) {
  dst.half[i] = (s16)(s8)a.byte[i + 16];
}

// Expands to:

if (0) {
  dst.half[0] = (s16)((s8)a.byte[8]);
  dst.half[1] = (s16)((s8)a.byte[9]);
  dst.half[2] = (s16)((s8)a.byte[10]);
  dst.half[3] = (s16)((s8)a.byte[11]);
  dst.half[4] = (s16)((s8)a.byte[12]);
  dst.half[5] = (s16)((s8)a.byte[13]);
  dst.half[6] = (s16)((s8)a.byte[14]);
  dst.half[7] = (s16)((s8)a.byte[15]);
  dst.half[8] = (s16)((s8)a.byte[24]);
  dst.half[9] = (s16)((s8)a.byte[25]);
  dst.half[10] = (s16)((s8)a.byte[26]);
  dst.half[11] = (s16)((s8)a.byte[27]);
  dst.half[12] = (s16)((s8)a.byte[28]);
  dst.half[13] = (s16)((s8)a.byte[29]);
  dst.half[14] = (s16)((s8)a.byte[30]);
  dst.half[15] = (s16)((s8)a.byte[31]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvexth_hu_bu (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvexth_hu_bu (__m256i a)
#include <lasxintrin.h>
Instruction: xvexth.hu.bu xr, xr
CPU Flags: LASX
```

### Description

Extend unsigned 8-bit elements in the higher half of `a` to 16-bit.

### Operation

```
int i;
for (i = 0; i < 8; i++) {
  dst.half[i] = (u16)(u8)a.byte[i + 8];
}
for (; i < 16; i++) {
  dst.half[i] = (u16)(u8)a.byte[i + 16];
}

// Expands to:

if (0) {
  dst.half[0] = (u16)((u8)a.byte[8]);
  dst.half[1] = (u16)((u8)a.byte[9]);
  dst.half[2] = (u16)((u8)a.byte[10]);
  dst.half[3] = (u16)((u8)a.byte[11]);
  dst.half[4] = (u16)((u8)a.byte[12]);
  dst.half[5] = (u16)((u8)a.byte[13]);
  dst.half[6] = (u16)((u8)a.byte[14]);
  dst.half[7] = (u16)((u8)a.byte[15]);
  dst.half[8] = (u16)((u8)a.byte[24]);
  dst.half[9] = (u16)((u8)a.byte[25]);
  dst.half[10] = (u16)((u8)a.byte[26]);
  dst.half[11] = (u16)((u8)a.byte[27]);
  dst.half[12] = (u16)((u8)a.byte[28]);
  dst.half[13] = (u16)((u8)a.byte[29]);
  dst.half[14] = (u16)((u8)a.byte[30]);
  dst.half[15] = (u16)((u8)a.byte[31]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvexth_w_h (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvexth_w_h (__m256i a)
#include <lasxintrin.h>
Instruction: xvexth.w.h xr, xr
CPU Flags: LASX
```

### Description

Extend signed 16-bit elements in the higher half of `a` to 32-bit.

### Operation

```
int i;
for (i = 0; i < 4; i++) {
  dst.word[i] = (s32)(s16)a.half[i + 4];
}
for (; i < 8; i++) {
  dst.word[i] = (s32)(s16)a.half[i + 8];
}

// Expands to:

if (0) {
  dst.word[0] = (s32)((s16)a.half[4]);
  dst.word[1] = (s32)((s16)a.half[5]);
  dst.word[2] = (s32)((s16)a.half[6]);
  dst.word[3] = (s32)((s16)a.half[7]);
  dst.word[4] = (s32)((s16)a.half[12]);
  dst.word[5] = (s32)((s16)a.half[13]);
  dst.word[6] = (s32)((s16)a.half[14]);
  dst.word[7] = (s32)((s16)a.half[15]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvexth_wu_hu (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvexth_wu_hu (__m256i a)
#include <lasxintrin.h>
Instruction: xvexth.wu.hu xr, xr
CPU Flags: LASX
```

### Description

Extend unsigned 16-bit elements in the higher half of `a` to 32-bit.

### Operation

```
int i;
for (i = 0; i < 4; i++) {
  dst.word[i] = (u32)(u16)a.half[i + 4];
}
for (; i < 8; i++) {
  dst.word[i] = (u32)(u16)a.half[i + 8];
}

// Expands to:

if (0) {
  dst.word[0] = (u32)((u16)a.half[4]);
  dst.word[1] = (u32)((u16)a.half[5]);
  dst.word[2] = (u32)((u16)a.half[6]);
  dst.word[3] = (u32)((u16)a.half[7]);
  dst.word[4] = (u32)((u16)a.half[12]);
  dst.word[5] = (u32)((u16)a.half[13]);
  dst.word[6] = (u32)((u16)a.half[14]);
  dst.word[7] = (u32)((u16)a.half[15]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvexth_d_w (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvexth_d_w (__m256i a)
#include <lasxintrin.h>
Instruction: xvexth.d.w xr, xr
CPU Flags: LASX
```

### Description

Extend signed 32-bit elements in the higher half of `a` to 64-bit.

### Operation

```
int i;
for (i = 0; i < 2; i++) {
  dst.dword[i] = (s64)(s32)a.word[i + 2];
}
for (; i < 4; i++) {
  dst.dword[i] = (s64)(s32)a.word[i + 4];
}

// Expands to:

if (0) {
  dst.dword[0] = (s64)((s32)a.word[2]);
  dst.dword[1] = (s64)((s32)a.word[3]);
  dst.dword[2] = (s64)((s32)a.word[6]);
  dst.dword[3] = (s64)((s32)a.word[7]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvexth_du_wu (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvexth_du_wu (__m256i a)
#include <lasxintrin.h>
Instruction: xvexth.du.wu xr, xr
CPU Flags: LASX
```

### Description

Extend unsigned 32-bit elements in the higher half of `a` to 64-bit.

### Operation

```
int i;
for (i = 0; i < 2; i++) {
  dst.dword[i] = (u64)(u32)a.word[i + 2];
}
for (; i < 4; i++) {
  dst.dword[i] = (u64)(u32)a.word[i + 4];
}

// Expands to:

if (0) {
  dst.dword[0] = (u64)((u32)a.word[2]);
  dst.dword[1] = (u64)((u32)a.word[3]);
  dst.dword[2] = (u64)((u32)a.word[6]);
  dst.dword[3] = (u64)((u32)a.word[7]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvexth_q_d (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvexth_q_d (__m256i a)
#include <lasxintrin.h>
Instruction: xvexth.q.d xr, xr
CPU Flags: LASX
```

### Description

Extend signed 64-bit elements in the higher half of `a` to 128-bit.

### Operation

```
int i;
for (i = 0; i < 1; i++) {
  dst.qword[i] = (s128)(s64)a.dword[i + 1];
}
for (; i < 2; i++) {
  dst.qword[i] = (s128)(s64)a.dword[i + 2];
}

// Expands to:

if (0) {
  dst.qword[0] = (s128)((s64)a.dword[1]);
  dst.qword[1] = (s128)((s64)a.dword[3]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvexth_qu_du (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvexth_qu_du (__m256i a)
#include <lasxintrin.h>
Instruction: xvexth.qu.du xr, xr
CPU Flags: LASX
```

### Description

Extend unsigned 64-bit elements in the higher half of `a` to 128-bit.

### Operation

```
int i;
for (i = 0; i < 1; i++) {
  dst.qword[i] = (u128)(u64)a.dword[i + 1];
}
for (; i < 2; i++) {
  dst.qword[i] = (u128)(u64)a.dword[i + 2];
}

// Expands to:

if (0) {
  dst.qword[0] = (u128)((u64)a.dword[1]);
  dst.qword[1] = (u128)((u64)a.dword[3]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvextl_q_d (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvextl_q_d (__m256i a)
#include <lasxintrin.h>
Instruction: xvextl.q.d xr, xr
CPU Flags: LASX
```

### Description

Extend signed 64-bit elements in the lower half of `a` to 128-bit.

### Operation

```
int i;
for (i = 0; i < 1; i++) {
  dst.qword[i] = (s128)(s64)a.dword[i];
}
for (; i < 2; i++) {
  dst.qword[i] = (s128)(s64)a.dword[i + 1];
}

// Expands to:

if (0) {
  dst.qword[0] = (s128)((s64)a.dword[0]);
  dst.qword[1] = (s128)((s64)a.dword[2]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvextl_qu_du (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvextl_qu_du (__m256i a)
#include <lasxintrin.h>
Instruction: xvextl.qu.du xr, xr
CPU Flags: LASX
```

### Description

Extend unsigned 64-bit elements in the lower half of `a` to 128-bit.

### Operation

```
int i;
for (i = 0; i < 1; i++) {
  dst.qword[i] = (u128)(u64)a.dword[i];
}
for (; i < 2; i++) {
  dst.qword[i] = (u128)(u64)a.dword[i + 1];
}

// Expands to:

if (0) {
  dst.qword[0] = (u128)((u64)a.dword[0]);
  dst.qword[1] = (u128)((u64)a.dword[2]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvextrins_b (\_\_m256i a, \_\_m256i b, imm0_255 imm)

### Synopsis

```
__m256i __lasx_xvextrins_b (__m256i a, __m256i b, imm0_255 imm)
#include <lasxintrin.h>
Instruction: xvextrins.b xr, xr, imm
CPU Flags: LASX
```

### Description

For each 128-bit half, extract one 8-bit element in `b` and insert it to `a` according to `imm`.

### Operation

```
int i;
for (i = 0; i < 16; i++) {
  dst.byte[i] = (i == ((imm >> 4) & 15)) ? b.byte[imm & 15] : a.byte[i];
}
for (; i < 32; i++) {
  dst.byte[i] =
      (i - 16 == ((imm >> 4) & 15)) ? b.byte[(imm & 15) + 16] : a.byte[i];
}

// Expands to:

if (0) {
  dst.byte[0] = (0 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[0]);
  dst.byte[1] = (1 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[1]);
  dst.byte[2] = (2 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[2]);
  dst.byte[3] = (3 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[3]);
  dst.byte[4] = (4 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[4]);
  dst.byte[5] = (5 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[5]);
  dst.byte[6] = (6 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[6]);
  dst.byte[7] = (7 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[7]);
  dst.byte[8] = (8 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[8]);
  dst.byte[9] = (9 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[9]);
  dst.byte[10] = (10 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[10]);
  dst.byte[11] = (11 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[11]);
  dst.byte[12] = (12 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[12]);
  dst.byte[13] = (13 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[13]);
  dst.byte[14] = (14 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[14]);
  dst.byte[15] = (15 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[15]);
  dst.byte[16] =
      (0 == ((imm >> 4) & 15)) ? (b.byte[(imm & 15) + 16]) : (a.byte[16]);
  dst.byte[17] =
      (1 == ((imm >> 4) & 15)) ? (b.byte[(imm & 15) + 16]) : (a.byte[17]);
  dst.byte[18] =
      (2 == ((imm >> 4) & 15)) ? (b.byte[(imm & 15) + 16]) : (a.byte[18]);
  dst.byte[19] =
      (3 == ((imm >> 4) & 15)) ? (b.byte[(imm & 15) + 16]) : (a.byte[19]);
  dst.byte[20] =
      (4 == ((imm >> 4) & 15)) ? (b.byte[(imm & 15) + 16]) : (a.byte[20]);
  dst.byte[21] =
      (5 == ((imm >> 4) & 15)) ? (b.byte[(imm & 15) + 16]) : (a.byte[21]);
  dst.byte[22] =
      (6 == ((imm >> 4) & 15)) ? (b.byte[(imm & 15) + 16]) : (a.byte[22]);
  dst.byte[23] =
      (7 == ((imm >> 4) & 15)) ? (b.byte[(imm & 15) + 16]) : (a.byte[23]);
  dst.byte[24] =
      (8 == ((imm >> 4) & 15)) ? (b.byte[(imm & 15) + 16]) : (a.byte[24]);
  dst.byte[25] =
      (9 == ((imm >> 4) & 15)) ? (b.byte[(imm & 15) + 16]) : (a.byte[25]);
  dst.byte[26] =
      (10 == ((imm >> 4) & 15)) ? (b.byte[(imm & 15) + 16]) : (a.byte[26]);
  dst.byte[27] =
      (11 == ((imm >> 4) & 15)) ? (b.byte[(imm & 15) + 16]) : (a.byte[27]);
  dst.byte[28] =
      (12 == ((imm >> 4) & 15)) ? (b.byte[(imm & 15) + 16]) : (a.byte[28]);
  dst.byte[29] =
      (13 == ((imm >> 4) & 15)) ? (b.byte[(imm & 15) + 16]) : (a.byte[29]);
  dst.byte[30] =
      (14 == ((imm >> 4) & 15)) ? (b.byte[(imm & 15) + 16]) : (a.byte[30]);
  dst.byte[31] =
      (15 == ((imm >> 4) & 15)) ? (b.byte[(imm & 15) + 16]) : (a.byte[31]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvextrins_h (\_\_m256i a, \_\_m256i b, imm0_255 imm)

### Synopsis

```
__m256i __lasx_xvextrins_h (__m256i a, __m256i b, imm0_255 imm)
#include <lasxintrin.h>
Instruction: xvextrins.h xr, xr, imm
CPU Flags: LASX
```

### Description

For each 128-bit half, extract one 16-bit element in `b` and insert it to `a` according to `imm`.

### Operation

```
int i;
for (i = 0; i < 8; i++) {
  dst.half[i] = (i == ((imm >> 4) & 7)) ? b.half[imm & 7] : a.half[i];
}
for (; i < 16; i++) {
  dst.half[i] = (i - 8 == ((imm >> 4) & 7)) ? b.half[(imm & 7) + 8] : a.half[i];
}

// Expands to:

if (0) {
  dst.half[0] = (0 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[0]);
  dst.half[1] = (1 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[1]);
  dst.half[2] = (2 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[2]);
  dst.half[3] = (3 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[3]);
  dst.half[4] = (4 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[4]);
  dst.half[5] = (5 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[5]);
  dst.half[6] = (6 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[6]);
  dst.half[7] = (7 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[7]);
  dst.half[8] = (0 == ((imm >> 4) & 7)) ? (b.half[(imm & 7) + 8]) : (a.half[8]);
  dst.half[9] = (1 == ((imm >> 4) & 7)) ? (b.half[(imm & 7) + 8]) : (a.half[9]);
  dst.half[10] =
      (2 == ((imm >> 4) & 7)) ? (b.half[(imm & 7) + 8]) : (a.half[10]);
  dst.half[11] =
      (3 == ((imm >> 4) & 7)) ? (b.half[(imm & 7) + 8]) : (a.half[11]);
  dst.half[12] =
      (4 == ((imm >> 4) & 7)) ? (b.half[(imm & 7) + 8]) : (a.half[12]);
  dst.half[13] =
      (5 == ((imm >> 4) & 7)) ? (b.half[(imm & 7) + 8]) : (a.half[13]);
  dst.half[14] =
      (6 == ((imm >> 4) & 7)) ? (b.half[(imm & 7) + 8]) : (a.half[14]);
  dst.half[15] =
      (7 == ((imm >> 4) & 7)) ? (b.half[(imm & 7) + 8]) : (a.half[15]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvextrins_w (\_\_m256i a, \_\_m256i b, imm0_255 imm)

### Synopsis

```
__m256i __lasx_xvextrins_w (__m256i a, __m256i b, imm0_255 imm)
#include <lasxintrin.h>
Instruction: xvextrins.w xr, xr, imm
CPU Flags: LASX
```

### Description

For each 128-bit half, extract one 32-bit element in `b` and insert it to `a` according to `imm`.

### Operation

```
int i;
for (i = 0; i < 4; i++) {
  dst.word[i] = (i == ((imm >> 4) & 3)) ? b.word[imm & 3] : a.word[i];
}
for (; i < 8; i++) {
  dst.word[i] = (i - 4 == ((imm >> 4) & 3)) ? b.word[(imm & 3) + 4] : a.word[i];
}

// Expands to:

if (0) {
  dst.word[0] = (0 == ((imm >> 4) & 3)) ? (b.word[imm & 3]) : (a.word[0]);
  dst.word[1] = (1 == ((imm >> 4) & 3)) ? (b.word[imm & 3]) : (a.word[1]);
  dst.word[2] = (2 == ((imm >> 4) & 3)) ? (b.word[imm & 3]) : (a.word[2]);
  dst.word[3] = (3 == ((imm >> 4) & 3)) ? (b.word[imm & 3]) : (a.word[3]);
  dst.word[4] = (0 == ((imm >> 4) & 3)) ? (b.word[(imm & 3) + 4]) : (a.word[4]);
  dst.word[5] = (1 == ((imm >> 4) & 3)) ? (b.word[(imm & 3) + 4]) : (a.word[5]);
  dst.word[6] = (2 == ((imm >> 4) & 3)) ? (b.word[(imm & 3) + 4]) : (a.word[6]);
  dst.word[7] = (3 == ((imm >> 4) & 3)) ? (b.word[(imm & 3) + 4]) : (a.word[7]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvextrins_d (\_\_m256i a, \_\_m256i b, imm0_255 imm)

### Synopsis

```
__m256i __lasx_xvextrins_d (__m256i a, __m256i b, imm0_255 imm)
#include <lasxintrin.h>
Instruction: xvextrins.d xr, xr, imm
CPU Flags: LASX
```

### Description

For each 128-bit half, extract one 64-bit element in `b` and insert it to `a` according to `imm`.

### Operation

```
int i;
for (i = 0; i < 2; i++) {
  dst.dword[i] = (i == ((imm >> 4) & 1)) ? b.dword[imm & 1] : a.dword[i];
}
for (; i < 4; i++) {
  dst.dword[i] =
      (i - 2 == ((imm >> 4) & 1)) ? b.dword[(imm & 1) + 2] : a.dword[i];
}

// Expands to:

if (0) {
  dst.dword[0] = (0 == ((imm >> 4) & 1)) ? (b.dword[imm & 1]) : (a.dword[0]);
  dst.dword[1] = (1 == ((imm >> 4) & 1)) ? (b.dword[imm & 1]) : (a.dword[1]);
  dst.dword[2] =
      (0 == ((imm >> 4) & 1)) ? (b.dword[(imm & 1) + 2]) : (a.dword[2]);
  dst.dword[3] =
      (1 == ((imm >> 4) & 1)) ? (b.dword[(imm & 1) + 2]) : (a.dword[3]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_vext2xv_h_b (\_\_m256i a)

### Synopsis

```
__m256i __lasx_vext2xv_h_b (__m256i a)
#include <lsxintrin.h>
Instruction: vext2xv.h.b xr, xr
CPU Flags: LSX
```

### Description

Extend signed 8-bit lane of `a` to signed 16-bit elements.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = (s16)(s8)a.byte[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

## \_\_m256i \_\_lasx_vext2xv_hu_bu (\_\_m256i a)

### Synopsis

```
__m256i __lasx_vext2xv_hu_bu (__m256i a)
#include <lsxintrin.h>
Instruction: vext2xv.hu.bu xr, xr
CPU Flags: LSX
```

### Description

Extend unsigned 8-bit lane of `a` to unsigned 16-bit elements.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = (u16)(u8)a.byte[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

## \_\_m256i \_\_lasx_vext2xv_w_b (\_\_m256i a)

### Synopsis

```
__m256i __lasx_vext2xv_w_b (__m256i a)
#include <lsxintrin.h>
Instruction: vext2xv.w.b xr, xr
CPU Flags: LSX
```

### Description

Extend signed 8-bit lane of `a` to signed 32-bit elements.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (s32)(s8)a.byte[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

## \_\_m256i \_\_lasx_vext2xv_wu_bu (\_\_m256i a)

### Synopsis

```
__m256i __lasx_vext2xv_wu_bu (__m256i a)
#include <lsxintrin.h>
Instruction: vext2xv.wu.bu xr, xr
CPU Flags: LSX
```

### Description

Extend unsigned 8-bit lane of `a` to unsigned 32-bit elements.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (u32)(u8)a.byte[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

## \_\_m256i \_\_lasx_vext2xv_w_h (\_\_m256i a)

### Synopsis

```
__m256i __lasx_vext2xv_w_h (__m256i a)
#include <lsxintrin.h>
Instruction: vext2xv.w.h xr, xr
CPU Flags: LSX
```

### Description

Extend signed 16-bit lane of `a` to signed 32-bit elements.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (s32)(s16)a.half[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

## \_\_m256i \_\_lasx_vext2xv_wu_hu (\_\_m256i a)

### Synopsis

```
__m256i __lasx_vext2xv_wu_hu (__m256i a)
#include <lsxintrin.h>
Instruction: vext2xv.wu.hu xr, xr
CPU Flags: LSX
```

### Description

Extend unsigned 16-bit lane of `a` to unsigned 32-bit elements.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (u32)(u16)a.half[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

## \_\_m256i \_\_lasx_vext2xv_d_b (\_\_m256i a)

### Synopsis

```
__m256i __lasx_vext2xv_d_b (__m256i a)
#include <lsxintrin.h>
Instruction: vext2xv.d.b xr, xr
CPU Flags: LSX
```

### Description

Extend signed 8-bit lane of `a` to signed 64-bit elements.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (s64)(s8)a.byte[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

## \_\_m256i \_\_lasx_vext2xv_du_bu (\_\_m256i a)

### Synopsis

```
__m256i __lasx_vext2xv_du_bu (__m256i a)
#include <lsxintrin.h>
Instruction: vext2xv.du.bu xr, xr
CPU Flags: LSX
```

### Description

Extend unsigned 8-bit lane of `a` to unsigned 64-bit elements.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (u64)(u8)a.byte[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

## \_\_m256i \_\_lasx_vext2xv_d_h (\_\_m256i a)

### Synopsis

```
__m256i __lasx_vext2xv_d_h (__m256i a)
#include <lsxintrin.h>
Instruction: vext2xv.d.h xr, xr
CPU Flags: LSX
```

### Description

Extend signed 16-bit lane of `a` to signed 64-bit elements.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (s64)(s16)a.half[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

## \_\_m256i \_\_lasx_vext2xv_du_hu (\_\_m256i a)

### Synopsis

```
__m256i __lasx_vext2xv_du_hu (__m256i a)
#include <lsxintrin.h>
Instruction: vext2xv.du.hu xr, xr
CPU Flags: LSX
```

### Description

Extend unsigned 16-bit lane of `a` to unsigned 64-bit elements.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (u64)(u16)a.half[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

## \_\_m256i \_\_lasx_vext2xv_d_w (\_\_m256i a)

### Synopsis

```
__m256i __lasx_vext2xv_d_w (__m256i a)
#include <lsxintrin.h>
Instruction: vext2xv.d.w xr, xr
CPU Flags: LSX
```

### Description

Extend signed 32-bit lane of `a` to signed 64-bit elements.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (s64)(s32)a.word[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

## \_\_m256i \_\_lasx_vext2xv_du_wu (\_\_m256i a)

### Synopsis

```
__m256i __lasx_vext2xv_du_wu (__m256i a)
#include <lsxintrin.h>
Instruction: vext2xv.du.wu xr, xr
CPU Flags: LSX
```

### Description

Extend unsigned 32-bit lane of `a` to unsigned 64-bit elements.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (u64)(u32)a.word[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

## \_\_m256i \_\_lasx_xvilvh_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvilvh_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvilvh.b xr, xr, xr
CPU Flags: LASX
```

### Description

Interleave 8-bit elements in higher half of `a` and `b`.

### Operation

```
int i;
for (i = 0; i < 16; i++) {
  dst.byte[i] = (i % 2 == 1) ? a.byte[i / 2 + 8] : b.byte[i / 2 + 8];
}
for (; i < 32; i++) {
  dst.byte[i] = (i % 2 == 1) ? a.byte[i / 2 + 16] : b.byte[i / 2 + 16];
}

// Expands to:

if (0) {
  dst.byte[0] = b.byte[8];
  dst.byte[1] = a.byte[8];
  dst.byte[2] = b.byte[9];
  dst.byte[3] = a.byte[9];
  dst.byte[4] = b.byte[10];
  dst.byte[5] = a.byte[10];
  dst.byte[6] = b.byte[11];
  dst.byte[7] = a.byte[11];
  dst.byte[8] = b.byte[12];
  dst.byte[9] = a.byte[12];
  dst.byte[10] = b.byte[13];
  dst.byte[11] = a.byte[13];
  dst.byte[12] = b.byte[14];
  dst.byte[13] = a.byte[14];
  dst.byte[14] = b.byte[15];
  dst.byte[15] = a.byte[15];
  dst.byte[16] = b.byte[24];
  dst.byte[17] = a.byte[24];
  dst.byte[18] = b.byte[25];
  dst.byte[19] = a.byte[25];
  dst.byte[20] = b.byte[26];
  dst.byte[21] = a.byte[26];
  dst.byte[22] = b.byte[27];
  dst.byte[23] = a.byte[27];
  dst.byte[24] = b.byte[28];
  dst.byte[25] = a.byte[28];
  dst.byte[26] = b.byte[29];
  dst.byte[27] = a.byte[29];
  dst.byte[28] = b.byte[30];
  dst.byte[29] = a.byte[30];
  dst.byte[30] = b.byte[31];
  dst.byte[31] = a.byte[31];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvilvh_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvilvh_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvilvh.h xr, xr, xr
CPU Flags: LASX
```

### Description

Interleave 16-bit elements in higher half of `a` and `b`.

### Operation

```
int i;
for (i = 0; i < 8; i++) {
  dst.half[i] = (i % 2 == 1) ? a.half[i / 2 + 4] : b.half[i / 2 + 4];
}
for (; i < 16; i++) {
  dst.half[i] = (i % 2 == 1) ? a.half[i / 2 + 8] : b.half[i / 2 + 8];
}

// Expands to:

if (0) {
  dst.half[0] = b.half[4];
  dst.half[1] = a.half[4];
  dst.half[2] = b.half[5];
  dst.half[3] = a.half[5];
  dst.half[4] = b.half[6];
  dst.half[5] = a.half[6];
  dst.half[6] = b.half[7];
  dst.half[7] = a.half[7];
  dst.half[8] = b.half[12];
  dst.half[9] = a.half[12];
  dst.half[10] = b.half[13];
  dst.half[11] = a.half[13];
  dst.half[12] = b.half[14];
  dst.half[13] = a.half[14];
  dst.half[14] = b.half[15];
  dst.half[15] = a.half[15];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvilvh_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvilvh_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvilvh.w xr, xr, xr
CPU Flags: LASX
```

### Description

Interleave 32-bit elements in higher half of `a` and `b`.

### Operation

```
int i;
for (i = 0; i < 4; i++) {
  dst.word[i] = (i % 2 == 1) ? a.word[i / 2 + 2] : b.word[i / 2 + 2];
}
for (; i < 8; i++) {
  dst.word[i] = (i % 2 == 1) ? a.word[i / 2 + 4] : b.word[i / 2 + 4];
}

// Expands to:

if (0) {
  dst.word[0] = b.word[2];
  dst.word[1] = a.word[2];
  dst.word[2] = b.word[3];
  dst.word[3] = a.word[3];
  dst.word[4] = b.word[6];
  dst.word[5] = a.word[6];
  dst.word[6] = b.word[7];
  dst.word[7] = a.word[7];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvilvh_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvilvh_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvilvh.d xr, xr, xr
CPU Flags: LASX
```

### Description

Interleave 64-bit elements in higher half of `a` and `b`.

### Operation

```
int i;
for (i = 0; i < 2; i++) {
  dst.dword[i] = (i % 2 == 1) ? a.dword[i / 2 + 1] : b.dword[i / 2 + 1];
}
for (; i < 4; i++) {
  dst.dword[i] = (i % 2 == 1) ? a.dword[i / 2 + 2] : b.dword[i / 2 + 2];
}

// Expands to:

if (0) {
  dst.dword[0] = b.dword[1];
  dst.dword[1] = a.dword[1];
  dst.dword[2] = b.dword[3];
  dst.dword[3] = a.dword[3];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvilvl_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvilvl_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvilvl.b xr, xr, xr
CPU Flags: LASX
```

### Description

Interleave 8-bit elements in lower half of `a` and `b`.

### Operation

```
int i;
for (i = 0; i < 16; i++) {
  dst.byte[i] = (i % 2 == 1) ? a.byte[i / 2] : b.byte[i / 2];
}
for (; i < 32; i++) {
  dst.byte[i] = (i % 2 == 1) ? a.byte[i / 2 + 8] : b.byte[i / 2 + 8];
}

// Expands to:

if (0) {
  dst.byte[0] = b.byte[0];
  dst.byte[1] = a.byte[0];
  dst.byte[2] = b.byte[1];
  dst.byte[3] = a.byte[1];
  dst.byte[4] = b.byte[2];
  dst.byte[5] = a.byte[2];
  dst.byte[6] = b.byte[3];
  dst.byte[7] = a.byte[3];
  dst.byte[8] = b.byte[4];
  dst.byte[9] = a.byte[4];
  dst.byte[10] = b.byte[5];
  dst.byte[11] = a.byte[5];
  dst.byte[12] = b.byte[6];
  dst.byte[13] = a.byte[6];
  dst.byte[14] = b.byte[7];
  dst.byte[15] = a.byte[7];
  dst.byte[16] = b.byte[16];
  dst.byte[17] = a.byte[16];
  dst.byte[18] = b.byte[17];
  dst.byte[19] = a.byte[17];
  dst.byte[20] = b.byte[18];
  dst.byte[21] = a.byte[18];
  dst.byte[22] = b.byte[19];
  dst.byte[23] = a.byte[19];
  dst.byte[24] = b.byte[20];
  dst.byte[25] = a.byte[20];
  dst.byte[26] = b.byte[21];
  dst.byte[27] = a.byte[21];
  dst.byte[28] = b.byte[22];
  dst.byte[29] = a.byte[22];
  dst.byte[30] = b.byte[23];
  dst.byte[31] = a.byte[23];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvilvl_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvilvl_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvilvl.h xr, xr, xr
CPU Flags: LASX
```

### Description

Interleave 16-bit elements in lower half of `a` and `b`.

### Operation

```
int i;
for (i = 0; i < 8; i++) {
  dst.half[i] = (i % 2 == 1) ? a.half[i / 2] : b.half[i / 2];
}
for (; i < 16; i++) {
  dst.half[i] = (i % 2 == 1) ? a.half[i / 2 + 4] : b.half[i / 2 + 4];
}

// Expands to:

if (0) {
  dst.half[0] = b.half[0];
  dst.half[1] = a.half[0];
  dst.half[2] = b.half[1];
  dst.half[3] = a.half[1];
  dst.half[4] = b.half[2];
  dst.half[5] = a.half[2];
  dst.half[6] = b.half[3];
  dst.half[7] = a.half[3];
  dst.half[8] = b.half[8];
  dst.half[9] = a.half[8];
  dst.half[10] = b.half[9];
  dst.half[11] = a.half[9];
  dst.half[12] = b.half[10];
  dst.half[13] = a.half[10];
  dst.half[14] = b.half[11];
  dst.half[15] = a.half[11];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvilvl_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvilvl_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvilvl.w xr, xr, xr
CPU Flags: LASX
```

### Description

Interleave 32-bit elements in lower half of `a` and `b`.

### Operation

```
int i;
for (i = 0; i < 4; i++) {
  dst.word[i] = (i % 2 == 1) ? a.word[i / 2] : b.word[i / 2];
}
for (; i < 8; i++) {
  dst.word[i] = (i % 2 == 1) ? a.word[i / 2 + 2] : b.word[i / 2 + 2];
}

// Expands to:

if (0) {
  dst.word[0] = b.word[0];
  dst.word[1] = a.word[0];
  dst.word[2] = b.word[1];
  dst.word[3] = a.word[1];
  dst.word[4] = b.word[4];
  dst.word[5] = a.word[4];
  dst.word[6] = b.word[5];
  dst.word[7] = a.word[5];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvilvl_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvilvl_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvilvl.d xr, xr, xr
CPU Flags: LASX
```

### Description

Interleave 64-bit elements in lower half of `a` and `b`.

### Operation

```
int i;
for (i = 0; i < 2; i++) {
  dst.dword[i] = (i % 2 == 1) ? a.dword[i / 2] : b.dword[i / 2];
}
for (; i < 4; i++) {
  dst.dword[i] = (i % 2 == 1) ? a.dword[i / 2 + 1] : b.dword[i / 2 + 1];
}

// Expands to:

if (0) {
  dst.dword[0] = b.dword[0];
  dst.dword[1] = a.dword[0];
  dst.dword[2] = b.dword[2];
  dst.dword[3] = a.dword[2];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvinsgr2vr_w (\_\_m256i a, int b, imm0_7 imm)

### Synopsis

```
__m256i __lasx_xvinsgr2vr_w (__m256i a, int b, imm0_7 imm)
#include <lasxintrin.h>
Instruction: xvinsgr2vr.w xr, r, imm
CPU Flags: LASX
```

### Description

Insert 32-bit element into lane indexed `imm`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (i == imm) ? b : a.word[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 1                |
| 3A6000 | LA664 | 1       | 1                |
| 3C6000 | LA664 | 1       | 1                |

## \_\_m256i \_\_lasx_xvinsgr2vr_d (\_\_m256i a, long int b, imm0_3 imm)

### Synopsis

```
__m256i __lasx_xvinsgr2vr_d (__m256i a, long int b, imm0_3 imm)
#include <lasxintrin.h>
Instruction: xvinsgr2vr.d xr, r, imm
CPU Flags: LASX
```

### Description

Insert 64-bit element into lane indexed `imm`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (i == imm) ? b : a.dword[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 1                |
| 3A6000 | LA664 | 1       | 1                |
| 3C6000 | LA664 | 1       | 1                |

## \_\_m256i \_\_lasx_xvinsve0_w (\_\_m256i a, \_\_m256i b, imm0_7 imm)

### Synopsis

```
__m256i __lasx_xvinsve0_w (__m256i a, __m256i b, imm0_7 imm)
#include <lasxintrin.h>
Instruction: xvinsve0.w xr, xr, imm
CPU Flags: LASX
```

### Description

Insert the first 32-bit lane of `b` into lane indexed `imm` of `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (i == imm) ? b.word[0] : a.word[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvinsve0_d (\_\_m256i a, \_\_m256i b, imm0_3 imm)

### Synopsis

```
__m256i __lasx_xvinsve0_d (__m256i a, __m256i b, imm0_3 imm)
#include <lasxintrin.h>
Instruction: xvinsve0.d xr, xr, imm
CPU Flags: LASX
```

### Description

Insert the first 64-bit lane of `b` into lane indexed `imm` of `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (i == imm) ? b.dword[0] : a.dword[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvfrstp_b (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvfrstp_b (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvfrstp.b xr, xr, xr
CPU Flags: LASX
```

### Description

Find the first negative 8-bit element in `b`, set the index of the element to the lane of `a` specified by `c`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[i];
}
int i;
for (i = 0; i < 16; i++) {
  if ((s8)b.byte[i] < 0) {
    break;
  }
}
dst.byte[c.byte[0] % 16] = i;
for (i = 16; i < 32; i++) {
  if ((s8)b.byte[i] < 0) {
    break;
  }
}
dst.byte[(c.byte[16] % 16) + 16] = i - 16;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvfrstp_h (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvfrstp_h (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvfrstp.h xr, xr, xr
CPU Flags: LASX
```

### Description

Find the first negative 16-bit element in `b`, set the index of the element to the lane of `a` specified by `c`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = a.half[i];
}
int i;
for (i = 0; i < 8; i++) {
  if ((s16)b.half[i] < 0) {
    break;
  }
}
dst.half[c.half[0] % 8] = i;
for (i = 8; i < 16; i++) {
  if ((s16)b.half[i] < 0) {
    break;
  }
}
dst.half[(c.half[8] % 8) + 8] = i - 8;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvfrstpi_b (\_\_m256i a, \_\_m256i b, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvfrstpi_b (__m256i a, __m256i b, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvfrstpi.b xr, xr, imm
CPU Flags: LASX
```

### Description

Find the first negative 8-bit element in `b`, set the index of the element to the lane of `a` specified by `imm`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[i];
}
int i;
for (i = 0; i < 16; i++) {
  if ((s8)b.byte[i] < 0) {
    break;
  }
}
dst.byte[imm % 16] = i;
for (i = 16; i < 32; i++) {
  if ((s8)b.byte[i] < 0) {
    break;
  }
}
dst.byte[(imm % 16) + 16] = i - 16;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvfrstpi_h (\_\_m256i a, \_\_m256i b, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvfrstpi_h (__m256i a, __m256i b, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvfrstpi.h xr, xr, imm
CPU Flags: LASX
```

### Description

Find the first negative 16-bit element in `b`, set the index of the element to the lane of `a` specified by `imm`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = a.half[i];
}
int i;
for (i = 0; i < 8; i++) {
  if ((s16)b.half[i] < 0) {
    break;
  }
}
dst.half[imm % 8] = i;
for (i = 8; i < 16; i++) {
  if ((s16)b.half[i] < 0) {
    break;
  }
}
dst.half[(imm % 8) + 8] = i - 8;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvmskgez_b (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvmskgez_b (__m256i a)
#include <lasxintrin.h>
Instruction: xvmskgez.b xr, xr
CPU Flags: LASX
```

### Description

For each 8-bit element in `a`, if the element is greater than or equal to zero, set one bit in `dst`, otherwise clear it.

### Examples

```
__m256i __lasx_xvmskgez_b(__m256i{0x1122334455667788, 0x99aabbccddeeff00, 0xabababab12121212, 0x1234567812345678})
= 0x00000000000001fe 0x0000000000000000 0x000000000000ff0f 0x0000000000000000
__m256i __lasx_xvmskgez_b(__m256i{0x0000191100000000, 0x00a1000011b11c11, 0x1181000008010101, 0x0000000000000000})
= 0x000000000000bbff 0x0000000000000000 0x000000000000ffbf 0x0000000000000000
```

### Operation

```
u64 m = 0x8080808080808080;
u64 c = m & a.dword[0];
c |= c << 7;
c |= c << 14;
c |= c << 28;
c >>= 56;
dst.dword[0] = c;
c = m & a.dword[1];
c |= c << 7;
c |= c << 14;
c |= c << 28;
c >>= 56;
dst.dword[0] |= c << 8;
dst.dword[0] = (u16)~dst.dword[0];
dst.dword[1] = 0;

c = m & a.dword[2];
c |= c << 7;
c |= c << 14;
c |= c << 28;
c >>= 56;
dst.dword[2] = c;
c = m & a.dword[3];
c |= c << 7;
c |= c << 14;
c |= c << 28;
c >>= 56;
dst.dword[2] |= c << 8;
dst.dword[2] = (u16)~dst.dword[2];
dst.dword[3] = 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvmskltz_b (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvmskltz_b (__m256i a)
#include <lasxintrin.h>
Instruction: xvmskltz.b xr, xr
CPU Flags: LASX
```

### Description

For each 8-bit element in `a`, if the element is less than zero, set one bit in `dst`, otherwise clear it.

### Examples

```
__m256i __lasx_xvmskltz_b(__m256i{0x1122334455667788, 0x99aabbccddeeff00, 0xabababab12121212, 0x1234567812345678})
= 0x000000000000fe01 0x0000000000000000 0x00000000000000f0 0x0000000000000000
__m256i __lasx_xvmskltz_b(__m256i{0x0000118100000000, 0x0081000081111118, 0x1181000001010801, 0x0000000000000000})
= 0x0000000000004810 0x0000000000000000 0x0000000000000040 0x0000000000000000
```

### Operation

```
u64 m = 0x8080808080808080;
u64 c = m & a.dword[0];
c |= c << 7;
c |= c << 14;
c |= c << 28;
c >>= 56;
dst.dword[0] = c;
c = m & a.dword[1];
c |= c << 7;
c |= c << 14;
c |= c << 28;
c >>= 56;
dst.dword[0] |= c << 8;
dst.dword[1] = 0;

c = m & a.dword[2];
c |= c << 7;
c |= c << 14;
c |= c << 28;
c >>= 56;
dst.dword[2] = c;
c = m & a.dword[3];
c |= c << 7;
c |= c << 14;
c |= c << 28;
c >>= 56;
dst.dword[2] |= c << 8;
dst.dword[3] = 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvmskltz_h (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvmskltz_h (__m256i a)
#include <lasxintrin.h>
Instruction: xvmskltz.h xr, xr
CPU Flags: LASX
```

### Description

For each 16-bit element in `a`, if the element is less than zero, set one bit in `dst`, otherwise clear it.

### Examples

```
__m256i __lasx_xvmskltz_h(__m256i{0x1122334455667788, 0x99aabbccddeeff00, 0xabababab12121212, 0x1234567812345678})
= 0x00000000000000f0 0x0000000000000000 0x000000000000000c 0x0000000000000000
__m256i __lasx_xvmskltz_h(__m256i{0x0000818100000000, 0x0018000018181881, 0x1181000008080808, 0x0000000000000000})
= 0x0000000000000004 0x0000000000000000 0x0000000000000000 0x0000000000000000
```

### Operation

```
u64 m = 0x8000800080008000;
u64 c = m & a.dword[0];
c |= c << 15;
c |= c << 30;
c >>= 60;
dst.dword[0] = c;
c = m & a.dword[1];
c |= c << 15;
c |= c << 30;
c >>= 60;
dst.dword[0] |= c << 4;
dst.dword[1] = 0;

c = m & a.dword[2];
c |= c << 15;
c |= c << 30;
c >>= 60;
dst.dword[2] = c;
c = m & a.dword[3];
c |= c << 15;
c |= c << 30;
c >>= 60;
dst.dword[2] |= c << 4;
dst.dword[3] = 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvmskltz_w (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvmskltz_w (__m256i a)
#include <lasxintrin.h>
Instruction: xvmskltz.w xr, xr
CPU Flags: LASX
```

### Description

For each 32-bit element in `a`, if the element is less than zero, set one bit in `dst`, otherwise clear it.

### Examples

```
__m256i __lasx_xvmskltz_w(__m256i{0x1122334455667788, 0x99aabbccddeeff00, 0xabababab12121212, 0x1234567812345678})
= 0x000000000000000c 0x0000000000000000 0x0000000000000002 0x0000000000000000
__m256i __lasx_xvmskltz_w(__m256i{0x0000811100000000, 0x0018000081111111, 0x8111000001010108, 0x0000000000000000})
= 0x0000000000000004 0x0000000000000000 0x0000000000000002 0x0000000000000000
```

### Operation

```
u64 m = 0x8000000080000000;
u64 c = m & a.dword[0];
c |= c << 31;
c >>= 62;
dst.dword[0] = c;
c = m & a.dword[1];
c |= c << 31;
c >>= 62;
dst.dword[0] |= c << 2;
dst.dword[1] = 0;

c = m & a.dword[2];
c |= c << 31;
c >>= 62;
dst.dword[2] = c;
c = m & a.dword[3];
c |= c << 31;
c >>= 62;
dst.dword[2] |= c << 2;
dst.dword[3] = 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvmskltz_d (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvmskltz_d (__m256i a)
#include <lasxintrin.h>
Instruction: xvmskltz.d xr, xr
CPU Flags: LASX
```

### Description

For each 64-bit element in `a`, if the element is less than zero, set one bit in `dst`, otherwise clear it.

### Examples

```
__m256i __lasx_xvmskltz_d(__m256i{0x1122334455667788, 0x99aabbccddeeff00, 0xabababab12121212, 0x1234567812345678})
= 0x0000000000000002 0x0000000000000000 0x0000000000000001 0x0000000000000000
__m256i __lasx_xvmskltz_d(__m256i{0x0000111800000000, 0x0081000081111111, 0x8111000008010101, 0x0000000000000000})
= 0x0000000000000000 0x0000000000000000 0x0000000000000001 0x0000000000000000
```

### Operation

```
u64 m = 0x8000000000000000;
u64 c = m & a.dword[0];
c >>= 63;
dst.dword[0] = c;
c = m & a.dword[1];
c >>= 63;
dst.dword[0] |= c << 1;
dst.dword[1] = 0;

c = m & a.dword[2];
c >>= 63;
dst.dword[2] = c;
c = m & a.dword[3];
c >>= 63;
dst.dword[2] |= c << 1;
dst.dword[3] = 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvmsknz_b (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvmsknz_b (__m256i a)
#include <lasxintrin.h>
Instruction: xvmsknz.b xr, xr
CPU Flags: LASX
```

### Description

For each 8-bit element in `a`, if the element is non-zero, set one bit in `dst`, otherwise clear it.

### Examples

```
__m256i __lasx_xvmsknz_b(__m256i{0x1122334455667788, 0x99aabbccddeeff00, 0xabababab12121212, 0x1234567812345678})
= 0x000000000000feff 0x0000000000000000 0x000000000000ffff 0x0000000000000000
__m256i __lasx_xvmsknz_b(__m256i{0x0000111100000000, 0x0011000011111111, 0x1111000001010101, 0x0000000000000000})
= 0x0000000000004f30 0x0000000000000000 0x00000000000000cf 0x0000000000000000
```

### Operation

```
u64 m = 0x7F7F7F7F7F7F7F7F;
u64 c = ~(((a.dword[0] & m) + m) | a.dword[0] | m);
c |= c << 7;
c |= c << 14;
c |= c << 28;
c >>= 56;
dst.dword[0] = c;
c = ~(((a.dword[1] & m) + m) | a.dword[1] | m);
c |= c << 7;
c |= c << 14;
c |= c << 28;
c >>= 56;
dst.dword[0] |= c << 8;
dst.dword[0] = (u16)~dst.dword[0];
dst.dword[1] = 0;

c = ~(((a.dword[2] & m) + m) | a.dword[2] | m);
c |= c << 7;
c |= c << 14;
c |= c << 28;
c >>= 56;
dst.dword[2] = c;
c = ~(((a.dword[3] & m) + m) | a.dword[3] | m);
c |= c << 7;
c |= c << 14;
c |= c << 28;
c >>= 56;
dst.dword[2] |= c << 8;
dst.dword[2] = (u16)~dst.dword[2];
dst.dword[3] = 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvpackev_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvpackev_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvpackev.b xr, xr, xr
CPU Flags: LASX
```

### Description

Collect and pack even-positioned 8-bit elements in `a` and `b` and store `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = (i % 2 == 1) ? a.byte[i - 1] : b.byte[i];
}

// Expands to:

if (0) {
  dst.byte[0] = b.byte[0];
  dst.byte[1] = a.byte[0];
  dst.byte[2] = b.byte[2];
  dst.byte[3] = a.byte[2];
  dst.byte[4] = b.byte[4];
  dst.byte[5] = a.byte[4];
  dst.byte[6] = b.byte[6];
  dst.byte[7] = a.byte[6];
  dst.byte[8] = b.byte[8];
  dst.byte[9] = a.byte[8];
  dst.byte[10] = b.byte[10];
  dst.byte[11] = a.byte[10];
  dst.byte[12] = b.byte[12];
  dst.byte[13] = a.byte[12];
  dst.byte[14] = b.byte[14];
  dst.byte[15] = a.byte[14];
  dst.byte[16] = b.byte[16];
  dst.byte[17] = a.byte[16];
  dst.byte[18] = b.byte[18];
  dst.byte[19] = a.byte[18];
  dst.byte[20] = b.byte[20];
  dst.byte[21] = a.byte[20];
  dst.byte[22] = b.byte[22];
  dst.byte[23] = a.byte[22];
  dst.byte[24] = b.byte[24];
  dst.byte[25] = a.byte[24];
  dst.byte[26] = b.byte[26];
  dst.byte[27] = a.byte[26];
  dst.byte[28] = b.byte[28];
  dst.byte[29] = a.byte[28];
  dst.byte[30] = b.byte[30];
  dst.byte[31] = a.byte[30];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvpackev_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvpackev_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvpackev.h xr, xr, xr
CPU Flags: LASX
```

### Description

Collect and pack even-positioned 16-bit elements in `a` and `b` and store `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = (i % 2 == 1) ? a.half[i - 1] : b.half[i];
}

// Expands to:

if (0) {
  dst.half[0] = b.half[0];
  dst.half[1] = a.half[0];
  dst.half[2] = b.half[2];
  dst.half[3] = a.half[2];
  dst.half[4] = b.half[4];
  dst.half[5] = a.half[4];
  dst.half[6] = b.half[6];
  dst.half[7] = a.half[6];
  dst.half[8] = b.half[8];
  dst.half[9] = a.half[8];
  dst.half[10] = b.half[10];
  dst.half[11] = a.half[10];
  dst.half[12] = b.half[12];
  dst.half[13] = a.half[12];
  dst.half[14] = b.half[14];
  dst.half[15] = a.half[14];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvpackev_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvpackev_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvpackev.w xr, xr, xr
CPU Flags: LASX
```

### Description

Collect and pack even-positioned 32-bit elements in `a` and `b` and store `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (i % 2 == 1) ? a.word[i - 1] : b.word[i];
}

// Expands to:

if (0) {
  dst.word[0] = b.word[0];
  dst.word[1] = a.word[0];
  dst.word[2] = b.word[2];
  dst.word[3] = a.word[2];
  dst.word[4] = b.word[4];
  dst.word[5] = a.word[4];
  dst.word[6] = b.word[6];
  dst.word[7] = a.word[6];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvpackev_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvpackev_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvpackev.d xr, xr, xr
CPU Flags: LASX
```

### Description

Collect and pack even-positioned 64-bit elements in `a` and `b` and store `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (i % 2 == 1) ? a.dword[i - 1] : b.dword[i];
}

// Expands to:

if (0) {
  dst.dword[0] = b.dword[0];
  dst.dword[1] = a.dword[0];
  dst.dword[2] = b.dword[2];
  dst.dword[3] = a.dword[2];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvpackod_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvpackod_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvpackod.b xr, xr, xr
CPU Flags: LASX
```

### Description

Collect and pack odd-positioned 8-bit elements in `a` and `b` and store `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = (i % 2 == 1) ? a.byte[i] : b.byte[i + 1];
}

// Expands to:

if (0) {
  dst.byte[0] = b.byte[1];
  dst.byte[1] = a.byte[1];
  dst.byte[2] = b.byte[3];
  dst.byte[3] = a.byte[3];
  dst.byte[4] = b.byte[5];
  dst.byte[5] = a.byte[5];
  dst.byte[6] = b.byte[7];
  dst.byte[7] = a.byte[7];
  dst.byte[8] = b.byte[9];
  dst.byte[9] = a.byte[9];
  dst.byte[10] = b.byte[11];
  dst.byte[11] = a.byte[11];
  dst.byte[12] = b.byte[13];
  dst.byte[13] = a.byte[13];
  dst.byte[14] = b.byte[15];
  dst.byte[15] = a.byte[15];
  dst.byte[16] = b.byte[17];
  dst.byte[17] = a.byte[17];
  dst.byte[18] = b.byte[19];
  dst.byte[19] = a.byte[19];
  dst.byte[20] = b.byte[21];
  dst.byte[21] = a.byte[21];
  dst.byte[22] = b.byte[23];
  dst.byte[23] = a.byte[23];
  dst.byte[24] = b.byte[25];
  dst.byte[25] = a.byte[25];
  dst.byte[26] = b.byte[27];
  dst.byte[27] = a.byte[27];
  dst.byte[28] = b.byte[29];
  dst.byte[29] = a.byte[29];
  dst.byte[30] = b.byte[31];
  dst.byte[31] = a.byte[31];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvpackod_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvpackod_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvpackod.h xr, xr, xr
CPU Flags: LASX
```

### Description

Collect and pack odd-positioned 16-bit elements in `a` and `b` and store `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = (i % 2 == 1) ? a.half[i] : b.half[i + 1];
}

// Expands to:

if (0) {
  dst.half[0] = b.half[1];
  dst.half[1] = a.half[1];
  dst.half[2] = b.half[3];
  dst.half[3] = a.half[3];
  dst.half[4] = b.half[5];
  dst.half[5] = a.half[5];
  dst.half[6] = b.half[7];
  dst.half[7] = a.half[7];
  dst.half[8] = b.half[9];
  dst.half[9] = a.half[9];
  dst.half[10] = b.half[11];
  dst.half[11] = a.half[11];
  dst.half[12] = b.half[13];
  dst.half[13] = a.half[13];
  dst.half[14] = b.half[15];
  dst.half[15] = a.half[15];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvpackod_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvpackod_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvpackod.w xr, xr, xr
CPU Flags: LASX
```

### Description

Collect and pack odd-positioned 32-bit elements in `a` and `b` and store `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (i % 2 == 1) ? a.word[i] : b.word[i + 1];
}

// Expands to:

if (0) {
  dst.word[0] = b.word[1];
  dst.word[1] = a.word[1];
  dst.word[2] = b.word[3];
  dst.word[3] = a.word[3];
  dst.word[4] = b.word[5];
  dst.word[5] = a.word[5];
  dst.word[6] = b.word[7];
  dst.word[7] = a.word[7];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvpackod_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvpackod_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvpackod.d xr, xr, xr
CPU Flags: LASX
```

### Description

Collect and pack odd-positioned 64-bit elements in `a` and `b` and store `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (i % 2 == 1) ? a.dword[i] : b.dword[i + 1];
}

// Expands to:

if (0) {
  dst.dword[0] = b.dword[1];
  dst.dword[1] = a.dword[1];
  dst.dword[2] = b.dword[3];
  dst.dword[3] = a.dword[3];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvpickev_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvpickev_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvpickev.b xr, xr, xr
CPU Flags: LASX
```

### Description

Pick even-positioned 8-bit elements in `b` first, then pick even-positioned 8-bit elements in `a`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (i < 8) ? b.byte[i * 2] : a.byte[(i - 8) * 2];
}
for (int i = 16; i < 32; i++) {
  dst.byte[i] = (i < 24) ? b.byte[(i - 8) * 2] : a.byte[(i - 16) * 2];
}

// Expands to:

if (0) {
  dst.byte[0] = b.byte[0];
  dst.byte[1] = b.byte[2];
  dst.byte[2] = b.byte[4];
  dst.byte[3] = b.byte[6];
  dst.byte[4] = b.byte[8];
  dst.byte[5] = b.byte[10];
  dst.byte[6] = b.byte[12];
  dst.byte[7] = b.byte[14];
  dst.byte[8] = a.byte[0];
  dst.byte[9] = a.byte[2];
  dst.byte[10] = a.byte[4];
  dst.byte[11] = a.byte[6];
  dst.byte[12] = a.byte[8];
  dst.byte[13] = a.byte[10];
  dst.byte[14] = a.byte[12];
  dst.byte[15] = a.byte[14];
  dst.byte[16] = b.byte[16];
  dst.byte[17] = b.byte[18];
  dst.byte[18] = b.byte[20];
  dst.byte[19] = b.byte[22];
  dst.byte[20] = b.byte[24];
  dst.byte[21] = b.byte[26];
  dst.byte[22] = b.byte[28];
  dst.byte[23] = b.byte[30];
  dst.byte[24] = a.byte[16];
  dst.byte[25] = a.byte[18];
  dst.byte[26] = a.byte[20];
  dst.byte[27] = a.byte[22];
  dst.byte[28] = a.byte[24];
  dst.byte[29] = a.byte[26];
  dst.byte[30] = a.byte[28];
  dst.byte[31] = a.byte[30];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvpickev_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvpickev_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvpickev.h xr, xr, xr
CPU Flags: LASX
```

### Description

Pick even-positioned 16-bit elements in `b` first, then pick even-positioned 16-bit elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (i < 4) ? b.half[i * 2] : a.half[(i - 4) * 2];
}
for (int i = 8; i < 16; i++) {
  dst.half[i] = (i < 12) ? b.half[(i - 4) * 2] : a.half[(i - 8) * 2];
}

// Expands to:

if (0) {
  dst.half[0] = b.half[0];
  dst.half[1] = b.half[2];
  dst.half[2] = b.half[4];
  dst.half[3] = b.half[6];
  dst.half[4] = a.half[0];
  dst.half[5] = a.half[2];
  dst.half[6] = a.half[4];
  dst.half[7] = a.half[6];
  dst.half[8] = b.half[8];
  dst.half[9] = b.half[10];
  dst.half[10] = b.half[12];
  dst.half[11] = b.half[14];
  dst.half[12] = a.half[8];
  dst.half[13] = a.half[10];
  dst.half[14] = a.half[12];
  dst.half[15] = a.half[14];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvpickev_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvpickev_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvpickev.w xr, xr, xr
CPU Flags: LASX
```

### Description

Pick even-positioned 32-bit elements in `b` first, then pick even-positioned 32-bit elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2) ? b.word[i * 2] : a.word[(i - 2) * 2];
}
for (int i = 4; i < 8; i++) {
  dst.word[i] = (i < 6) ? b.word[(i - 2) * 2] : a.word[(i - 4) * 2];
}

// Expands to:

if (0) {
  dst.word[0] = b.word[0];
  dst.word[1] = b.word[2];
  dst.word[2] = a.word[0];
  dst.word[3] = a.word[2];
  dst.word[4] = b.word[4];
  dst.word[5] = b.word[6];
  dst.word[6] = a.word[4];
  dst.word[7] = a.word[6];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvpickev_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvpickev_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvpickev.d xr, xr, xr
CPU Flags: LASX
```

### Description

Pick even-positioned 64-bit elements in `b` first, then pick even-positioned 64-bit elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (i < 1) ? b.dword[i * 2] : a.dword[(i - 1) * 2];
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = (i < 3) ? b.dword[(i - 1) * 2] : a.dword[(i - 2) * 2];
}

// Expands to:

if (0) {
  dst.dword[0] = b.dword[0];
  dst.dword[1] = a.dword[0];
  dst.dword[2] = b.dword[2];
  dst.dword[3] = a.dword[2];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvpickve_w (\_\_m256i a, imm0_7 imm)

### Synopsis

```
__m256i __lasx_xvpickve_w (__m256i a, imm0_7 imm)
#include <lasxintrin.h>
Instruction: xvpickve.w xr, xr, imm
CPU Flags: LASX
```

### Description

Copy one 32-bit lane from `a` specified by `imm` to the first lane of `dst`, and set the other lanes to zero.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (i == 0) ? a.word[imm] : 0;
}

// Expands to:

if (0) {
  dst.word[0] = a.word[imm];
  dst.word[1] = 0;
  dst.word[2] = 0;
  dst.word[3] = 0;
  dst.word[4] = 0;
  dst.word[5] = 0;
  dst.word[6] = 0;
  dst.word[7] = 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

## \_\_m256i \_\_lasx_xvpickve_d (\_\_m256i a, imm0_3 imm)

### Synopsis

```
__m256i __lasx_xvpickve_d (__m256i a, imm0_3 imm)
#include <lasxintrin.h>
Instruction: xvpickve.d xr, xr, imm
CPU Flags: LASX
```

### Description

Copy one 64-bit lane from `a` specified by `imm` to the first lane of `dst`, and set the other lanes to zero.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (i == 0) ? a.dword[imm] : 0;
}

// Expands to:

if (0) {
  dst.dword[0] = a.dword[imm];
  dst.dword[1] = 0;
  dst.dword[2] = 0;
  dst.dword[3] = 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

## \_\_m256 \_\_lasx_xvpickve_w_f (\_\_m256 a, imm0_7 imm)

### Synopsis

```
__m256 __lasx_xvpickve_w_f (__m256 a, imm0_7 imm)
#include <lasxintrin.h>
Instruction: xvpickve.w xr, xr, imm
CPU Flags: LASX
```

### Description

Copy one 32-bit lane from `a` specified by `imm` to the first lane of `dst`, and set the other lanes to zero.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (i == 0) ? a.word[imm] : 0;
}

// Expands to:

if (0) {
  dst.word[0] = a.word[imm];
  dst.word[1] = 0;
  dst.word[2] = 0;
  dst.word[3] = 0;
  dst.word[4] = 0;
  dst.word[5] = 0;
  dst.word[6] = 0;
  dst.word[7] = 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

## \_\_m256d \_\_lasx_xvpickve_d_f (\_\_m256d a, imm0_3 imm)

### Synopsis

```
__m256d __lasx_xvpickve_d_f (__m256d a, imm0_3 imm)
#include <lasxintrin.h>
Instruction: xvpickve.d xr, xr, imm
CPU Flags: LASX
```

### Description

Copy one 64-bit lane from `a` specified by `imm` to the first lane of `dst`, and set the other lanes to zero.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (i == 0) ? a.dword[imm] : 0;
}

// Expands to:

if (0) {
  dst.dword[0] = a.dword[imm];
  dst.dword[1] = 0;
  dst.dword[2] = 0;
  dst.dword[3] = 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

## int \_\_lasx_xvpickve2gr_w (\_\_m256i a, imm0_7 idx)

### Synopsis

```
int __lasx_xvpickve2gr_w (__m256i a, imm0_7 idx)
#include <lasxintrin.h>
Instruction: xvpickve2gr.w r, xr, imm
CPU Flags: LASX
```

### Description

Pick the `lane` specified by `idx` from `a` and store into `dst`.

### Operation

```
dst = (s32)a.word[idx];
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 1                |
| 3A6000 | LA664 | 1       | 1                |
| 3C6000 | LA664 | 1       | 1                |

## unsigned int \_\_lasx_xvpickve2gr_wu (\_\_m256i a, imm0_7 idx)

### Synopsis

```
unsigned int __lasx_xvpickve2gr_wu (__m256i a, imm0_7 idx)
#include <lasxintrin.h>
Instruction: xvpickve2gr.wu r, xr, imm
CPU Flags: LASX
```

### Description

Pick the `lane` specified by `idx` from `a` and store into `dst`.

### Operation

```
dst = (u32)a.word[idx];
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 1                |
| 3A6000 | LA664 | 1       | 1                |
| 3C6000 | LA664 | 1       | 1                |

## long int \_\_lasx_xvpickve2gr_d (\_\_m256i a, imm0_3 idx)

### Synopsis

```
long int __lasx_xvpickve2gr_d (__m256i a, imm0_3 idx)
#include <lasxintrin.h>
Instruction: xvpickve2gr.d r, xr, imm
CPU Flags: LASX
```

### Description

Pick the `lane` specified by `idx` from `a` and store into `dst`.

### Operation

```
dst = (s64)a.dword[idx];
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 1                |
| 3A6000 | LA664 | 1       | 1                |
| 3C6000 | LA664 | 1       | 1                |

## unsigned long int \_\_lasx_xvpickve2gr_du (\_\_m256i a, imm0_3 idx)

### Synopsis

```
unsigned long int __lasx_xvpickve2gr_du (__m256i a, imm0_3 idx)
#include <lasxintrin.h>
Instruction: xvpickve2gr.du r, xr, imm
CPU Flags: LASX
```

### Description

Pick the `lane` specified by `idx` from `a` and store into `dst`.

### Operation

```
dst = (u64)a.dword[idx];
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 1                |
| 3A6000 | LA664 | 1       | 1                |
| 3C6000 | LA664 | 1       | 1                |

## \_\_m256i \_\_lasx_xvpickod_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvpickod_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvpickod.b xr, xr, xr
CPU Flags: LASX
```

### Description

Pick odd-positioned 8-bit elements in `b` first, then pick odd-positioned 8-bit elements in `a`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (i < 8) ? b.byte[i * 2 + 1] : a.byte[(i - 8) * 2 + 1];
}
for (int i = 16; i < 32; i++) {
  dst.byte[i] = (i < 24) ? b.byte[(i - 8) * 2 + 1] : a.byte[(i - 16) * 2 + 1];
}

// Expands to:

if (0) {
  dst.byte[0] = b.byte[1];
  dst.byte[1] = b.byte[3];
  dst.byte[2] = b.byte[5];
  dst.byte[3] = b.byte[7];
  dst.byte[4] = b.byte[9];
  dst.byte[5] = b.byte[11];
  dst.byte[6] = b.byte[13];
  dst.byte[7] = b.byte[15];
  dst.byte[8] = a.byte[1];
  dst.byte[9] = a.byte[3];
  dst.byte[10] = a.byte[5];
  dst.byte[11] = a.byte[7];
  dst.byte[12] = a.byte[9];
  dst.byte[13] = a.byte[11];
  dst.byte[14] = a.byte[13];
  dst.byte[15] = a.byte[15];
  dst.byte[16] = b.byte[17];
  dst.byte[17] = b.byte[19];
  dst.byte[18] = b.byte[21];
  dst.byte[19] = b.byte[23];
  dst.byte[20] = b.byte[25];
  dst.byte[21] = b.byte[27];
  dst.byte[22] = b.byte[29];
  dst.byte[23] = b.byte[31];
  dst.byte[24] = a.byte[17];
  dst.byte[25] = a.byte[19];
  dst.byte[26] = a.byte[21];
  dst.byte[27] = a.byte[23];
  dst.byte[28] = a.byte[25];
  dst.byte[29] = a.byte[27];
  dst.byte[30] = a.byte[29];
  dst.byte[31] = a.byte[31];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvpickod_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvpickod_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvpickod.h xr, xr, xr
CPU Flags: LASX
```

### Description

Pick odd-positioned 16-bit elements in `b` first, then pick odd-positioned 16-bit elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (i < 4) ? b.half[i * 2 + 1] : a.half[(i - 4) * 2 + 1];
}
for (int i = 8; i < 16; i++) {
  dst.half[i] = (i < 12) ? b.half[(i - 4) * 2 + 1] : a.half[(i - 8) * 2 + 1];
}

// Expands to:

if (0) {
  dst.half[0] = b.half[1];
  dst.half[1] = b.half[3];
  dst.half[2] = b.half[5];
  dst.half[3] = b.half[7];
  dst.half[4] = a.half[1];
  dst.half[5] = a.half[3];
  dst.half[6] = a.half[5];
  dst.half[7] = a.half[7];
  dst.half[8] = b.half[9];
  dst.half[9] = b.half[11];
  dst.half[10] = b.half[13];
  dst.half[11] = b.half[15];
  dst.half[12] = a.half[9];
  dst.half[13] = a.half[11];
  dst.half[14] = a.half[13];
  dst.half[15] = a.half[15];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvpickod_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvpickod_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvpickod.w xr, xr, xr
CPU Flags: LASX
```

### Description

Pick odd-positioned 32-bit elements in `b` first, then pick odd-positioned 32-bit elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2) ? b.word[i * 2 + 1] : a.word[(i - 2) * 2 + 1];
}
for (int i = 4; i < 8; i++) {
  dst.word[i] = (i < 6) ? b.word[(i - 2) * 2 + 1] : a.word[(i - 4) * 2 + 1];
}

// Expands to:

if (0) {
  dst.word[0] = b.word[1];
  dst.word[1] = b.word[3];
  dst.word[2] = a.word[1];
  dst.word[3] = a.word[3];
  dst.word[4] = b.word[5];
  dst.word[5] = b.word[7];
  dst.word[6] = a.word[5];
  dst.word[7] = a.word[7];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvpickod_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvpickod_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvpickod.d xr, xr, xr
CPU Flags: LASX
```

### Description

Pick odd-positioned 64-bit elements in `b` first, then pick odd-positioned 64-bit elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (i < 1) ? b.dword[i * 2 + 1] : a.dword[(i - 1) * 2 + 1];
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = (i < 3) ? b.dword[(i - 1) * 2 + 1] : a.dword[(i - 2) * 2 + 1];
}

// Expands to:

if (0) {
  dst.dword[0] = b.dword[1];
  dst.dword[1] = a.dword[1];
  dst.dword[2] = b.dword[3];
  dst.dword[3] = a.dword[3];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvrepli_b (imm_n512_511 imm)

### Synopsis

```
__m256i __lasx_xvrepli_b (imm_n512_511 imm)
#include <lasxintrin.h>
Instruction: xvldi xr, imm
CPU Flags: LASX
```

### Description

Repeat `imm` to fill whole vector.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = imm;
}
```

Tested on real machine.

## \_\_m256i \_\_lasx_xvrepli_h (imm_n512_511 imm)

### Synopsis

```
__m256i __lasx_xvrepli_h (imm_n512_511 imm)
#include <lasxintrin.h>
Instruction: xvldi xr, imm
CPU Flags: LASX
```

### Description

Repeat `imm` to fill whole vector.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = imm;
}
```

Tested on real machine.

## \_\_m256i \_\_lasx_xvrepli_w (imm_n512_511 imm)

### Synopsis

```
__m256i __lasx_xvrepli_w (imm_n512_511 imm)
#include <lasxintrin.h>
Instruction: xvldi xr, imm
CPU Flags: LASX
```

### Description

Repeat `imm` to fill whole vector.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = imm;
}
```

Tested on real machine.

## \_\_m256i \_\_lasx_xvrepli_d (imm_n512_511 imm)

### Synopsis

```
__m256i __lasx_xvrepli_d (imm_n512_511 imm)
#include <lasxintrin.h>
Instruction: xvldi xr, imm
CPU Flags: LASX
```

### Description

Repeat `imm` to fill whole vector.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = imm;
}
```

Tested on real machine.

## \_\_m256i \_\_lasx_xvreplgr2vr_b (int val)

### Synopsis

```
__m256i __lasx_xvreplgr2vr_b (int val)
#include <lasxintrin.h>
Instruction: xvreplgr2vr.b xr, r
CPU Flags: LASX
```

### Description

Repeat `val` to whole vector.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = val;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 1                |
| 3A6000 | LA664 | N/A     | 1                |
| 3C6000 | LA664 | N/A     | 1                |

## \_\_m256i \_\_lasx_xvreplgr2vr_h (int val)

### Synopsis

```
__m256i __lasx_xvreplgr2vr_h (int val)
#include <lasxintrin.h>
Instruction: xvreplgr2vr.h xr, r
CPU Flags: LASX
```

### Description

Repeat `val` to whole vector.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = val;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 1                |
| 3A6000 | LA664 | N/A     | 1                |
| 3C6000 | LA664 | N/A     | 1                |

## \_\_m256i \_\_lasx_xvreplgr2vr_w (int val)

### Synopsis

```
__m256i __lasx_xvreplgr2vr_w (int val)
#include <lasxintrin.h>
Instruction: xvreplgr2vr.w xr, r
CPU Flags: LASX
```

### Description

Repeat `val` to whole vector.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = val;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 1                |
| 3A6000 | LA664 | N/A     | 1                |
| 3C6000 | LA664 | N/A     | 1                |

## \_\_m256i \_\_lasx_xvreplgr2vr_d (long int val)

### Synopsis

```
__m256i __lasx_xvreplgr2vr_d (long int val)
#include <lasxintrin.h>
Instruction: xvreplgr2vr.d xr, r
CPU Flags: LASX
```

### Description

Repeat `val` to whole vector.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = val;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 1                |
| 3A6000 | LA664 | N/A     | 1                |
| 3C6000 | LA664 | N/A     | 1                |

## \_\_m256i \_\_lasx_xvreplve_b (\_\_m256i a, int idx)

### Synopsis

```
__m256i __lasx_xvreplve_b (__m256i a, int idx)
#include <lasxintrin.h>
Instruction: xvreplve.b xr, xr, r
CPU Flags: LASX
```

### Description

Repeat the element in lane `idx` of `a` to fill whole vector.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[idx % 16];
}
for (int i = 16; i < 32; i++) {
  dst.byte[i] = a.byte[(idx % 16) + 16];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 1                |
| 3A6000 | LA664 | 1       | 1                |
| 3C6000 | LA664 | 1       | 1                |

## \_\_m256i \_\_lasx_xvreplve_h (\_\_m256i a, int idx)

### Synopsis

```
__m256i __lasx_xvreplve_h (__m256i a, int idx)
#include <lasxintrin.h>
Instruction: xvreplve.h xr, xr, r
CPU Flags: LASX
```

### Description

Repeat the element in lane `idx` of `a` to fill whole vector.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = a.half[idx % 8];
}
for (int i = 8; i < 16; i++) {
  dst.half[i] = a.half[(idx % 8) + 8];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 1                |
| 3A6000 | LA664 | 1       | 1                |
| 3C6000 | LA664 | 1       | 1                |

## \_\_m256i \_\_lasx_xvreplve_w (\_\_m256i a, int idx)

### Synopsis

```
__m256i __lasx_xvreplve_w (__m256i a, int idx)
#include <lasxintrin.h>
Instruction: xvreplve.w xr, xr, r
CPU Flags: LASX
```

### Description

Repeat the element in lane `idx` of `a` to fill whole vector.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = a.word[idx % 4];
}
for (int i = 4; i < 8; i++) {
  dst.word[i] = a.word[(idx % 4) + 4];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 1                |
| 3A6000 | LA664 | 1       | 1                |
| 3C6000 | LA664 | 1       | 1                |

## \_\_m256i \_\_lasx_xvreplve_d (\_\_m256i a, int idx)

### Synopsis

```
__m256i __lasx_xvreplve_d (__m256i a, int idx)
#include <lasxintrin.h>
Instruction: xvreplve.d xr, xr, r
CPU Flags: LASX
```

### Description

Repeat the element in lane `idx` of `a` to fill whole vector.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = a.dword[idx % 2];
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = a.dword[(idx % 2) + 2];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 1                |
| 3A6000 | LA664 | 1       | 1                |
| 3C6000 | LA664 | 1       | 1                |

## \_\_m256i \_\_lasx_xvreplve0_b (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvreplve0_b (__m256i a)
#include <lasxintrin.h>
Instruction: xvreplve0.b xr, xr
CPU Flags: LASX
```

### Description

Repeat the first 8-bit lane from `a` to all lanes of `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[0];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

## \_\_m256i \_\_lasx_xvreplve0_h (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvreplve0_h (__m256i a)
#include <lasxintrin.h>
Instruction: xvreplve0.h xr, xr
CPU Flags: LASX
```

### Description

Repeat the first 16-bit lane from `a` to all lanes of `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = a.half[0];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

## \_\_m256i \_\_lasx_xvreplve0_w (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvreplve0_w (__m256i a)
#include <lasxintrin.h>
Instruction: xvreplve0.w xr, xr
CPU Flags: LASX
```

### Description

Repeat the first 32-bit lane from `a` to all lanes of `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = a.word[0];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

## \_\_m256i \_\_lasx_xvreplve0_d (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvreplve0_d (__m256i a)
#include <lasxintrin.h>
Instruction: xvreplve0.d xr, xr
CPU Flags: LASX
```

### Description

Repeat the first 64-bit lane from `a` to all lanes of `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = a.dword[0];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

## \_\_m256i \_\_lasx_xvreplve0_q (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvreplve0_q (__m256i a)
#include <lasxintrin.h>
Instruction: xvreplve0.q xr, xr
CPU Flags: LASX
```

### Description

Repeat the first 128-bit lane from `a` to all lanes of `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] = a.qword[0];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

## \_\_m256i \_\_lasx_xvrepl128vei_b (\_\_m256i a, imm0_15 idx)

### Synopsis

```
__m256i __lasx_xvrepl128vei_b (__m256i a, imm0_15 idx)
#include <lasxintrin.h>
Instruction: xvrepl128vei.b xr, xr, imm
CPU Flags: LASX
```

### Description

Repeat the element in lane `idx` of `a` to fill whole vector.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[idx];
}
for (int i = 16; i < 32; i++) {
  dst.byte[i] = a.byte[idx + 16];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvrepl128vei_h (\_\_m256i a, imm0_7 idx)

### Synopsis

```
__m256i __lasx_xvrepl128vei_h (__m256i a, imm0_7 idx)
#include <lasxintrin.h>
Instruction: xvrepl128vei.h xr, xr, imm
CPU Flags: LASX
```

### Description

Repeat the element in lane `idx` of `a` to fill whole vector.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = a.half[idx];
}
for (int i = 8; i < 16; i++) {
  dst.half[i] = a.half[idx + 8];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvrepl128vei_w (\_\_m256i a, imm0_3 idx)

### Synopsis

```
__m256i __lasx_xvrepl128vei_w (__m256i a, imm0_3 idx)
#include <lasxintrin.h>
Instruction: xvrepl128vei.w xr, xr, imm
CPU Flags: LASX
```

### Description

Repeat the element in lane `idx` of `a` to fill whole vector.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = a.word[idx];
}
for (int i = 4; i < 8; i++) {
  dst.word[i] = a.word[idx + 4];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvrepl128vei_d (\_\_m256i a, imm0_1 idx)

### Synopsis

```
__m256i __lasx_xvrepl128vei_d (__m256i a, imm0_1 idx)
#include <lasxintrin.h>
Instruction: xvrepl128vei.d xr, xr, imm
CPU Flags: LASX
```

### Description

Repeat the element in lane `idx` of `a` to fill whole vector.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = a.dword[idx];
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = a.dword[idx + 2];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsat_b (\_\_m256i a, imm0_7 imm)

### Synopsis

```
__m256i __lasx_xvsat_b (__m256i a, imm0_7 imm)
#include <lasxintrin.h>
Instruction: xvsat.b xr, xr, imm
CPU Flags: LASX
```

### Description

Clamp signed 8-bit elements in `a` to range specified by `imm`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = clamp<s8>(a.byte[i], -(1 << imm), (1 << imm) - 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsat_bu (\_\_m256i a, imm0_7 imm)

### Synopsis

```
__m256i __lasx_xvsat_bu (__m256i a, imm0_7 imm)
#include <lasxintrin.h>
Instruction: xvsat.bu xr, xr, imm
CPU Flags: LASX
```

### Description

Clamp unsigned 8-bit elements in `a` to range specified by `imm`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = clamp<u8>(a.byte[i], 0, (1 << (imm + 1)) - 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsat_h (\_\_m256i a, imm0_15 imm)

### Synopsis

```
__m256i __lasx_xvsat_h (__m256i a, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvsat.h xr, xr, imm
CPU Flags: LASX
```

### Description

Clamp signed 16-bit elements in `a` to range specified by `imm`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = clamp<s16>(a.half[i], -(1 << imm), (1 << imm) - 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsat_hu (\_\_m256i a, imm0_15 imm)

### Synopsis

```
__m256i __lasx_xvsat_hu (__m256i a, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvsat.hu xr, xr, imm
CPU Flags: LASX
```

### Description

Clamp unsigned 16-bit elements in `a` to range specified by `imm`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = clamp<u16>(a.half[i], 0, (1 << (imm + 1)) - 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsat_w (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvsat_w (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvsat.w xr, xr, imm
CPU Flags: LASX
```

### Description

Clamp signed 32-bit elements in `a` to range specified by `imm`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = clamp<s32>(a.word[i], -(1 << imm), (1 << imm) - 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsat_wu (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvsat_wu (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvsat.wu xr, xr, imm
CPU Flags: LASX
```

### Description

Clamp unsigned 32-bit elements in `a` to range specified by `imm`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = clamp<u32>(a.word[i], 0, (1 << (imm + 1)) - 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsat_d (\_\_m256i a, imm0_63 imm)

### Synopsis

```
__m256i __lasx_xvsat_d (__m256i a, imm0_63 imm)
#include <lasxintrin.h>
Instruction: xvsat.d xr, xr, imm
CPU Flags: LASX
```

### Description

Clamp signed 64-bit elements in `a` to range specified by `imm`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = clamp<s64>(a.dword[i], -(1 << imm), (1 << imm) - 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsat_du (\_\_m256i a, imm0_63 imm)

### Synopsis

```
__m256i __lasx_xvsat_du (__m256i a, imm0_63 imm)
#include <lasxintrin.h>
Instruction: xvsat.du xr, xr, imm
CPU Flags: LASX
```

### Description

Clamp unsigned 64-bit elements in `a` to range specified by `imm`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = clamp<u64>(a.dword[i], 0, (1 << (imm + 1)) - 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsigncov_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsigncov_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsigncov.b xr, xr, xr
CPU Flags: LASX
```

### Description

If the 8-bit element in `a` equals to zero, set the result to zero. If the signed 8-bit element in `a` is positive, copy element in `b` to result. Otherwise, copy negated element in `b` to result. If `a` and `b` are the same vectors, it is equivalent to computing absolute value.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] =
      (a.byte[i] == 0) ? 0 : ((s8)a.byte[i] > 0 ? b.byte[i] : -b.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 2                |
| 3C6000 | LA664 | 1       | 2                |

## \_\_m256i \_\_lasx_xvsigncov_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsigncov_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsigncov.h xr, xr, xr
CPU Flags: LASX
```

### Description

If the 16-bit element in `a` equals to zero, set the result to zero. If the signed 16-bit element in `a` is positive, copy element in `b` to result. Otherwise, copy negated element in `b` to result. If `a` and `b` are the same vectors, it is equivalent to computing absolute value.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] =
      (a.half[i] == 0) ? 0 : ((s16)a.half[i] > 0 ? b.half[i] : -b.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 2                |
| 3C6000 | LA664 | 1       | 2                |

## \_\_m256i \_\_lasx_xvsigncov_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsigncov_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsigncov.w xr, xr, xr
CPU Flags: LASX
```

### Description

If the 32-bit element in `a` equals to zero, set the result to zero. If the signed 32-bit element in `a` is positive, copy element in `b` to result. Otherwise, copy negated element in `b` to result. If `a` and `b` are the same vectors, it is equivalent to computing absolute value.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] =
      (a.word[i] == 0) ? 0 : ((s32)a.word[i] > 0 ? b.word[i] : -b.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 2                |
| 3C6000 | LA664 | 1       | 2                |

## \_\_m256i \_\_lasx_xvsigncov_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsigncov_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsigncov.d xr, xr, xr
CPU Flags: LASX
```

### Description

If the 64-bit element in `a` equals to zero, set the result to zero. If the signed 64-bit element in `a` is positive, copy element in `b` to result. Otherwise, copy negated element in `b` to result. If `a` and `b` are the same vectors, it is equivalent to computing absolute value.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] =
      (a.dword[i] == 0) ? 0 : ((s64)a.dword[i] > 0 ? b.dword[i] : -b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 2                |
| 3C6000 | LA664 | 1       | 2                |

## \_\_m256i \_\_lasx_xvldi (imm_n1024_1023 imm)

### Synopsis

```
__m256i __lasx_xvldi (imm_n1024_1023 imm)
#include <lasxintrin.h>
Instruction: xvldi xr, imm
CPU Flags: LASX
```

### Description

Initialize `dst` using predefined patterns:

- `imm[12:10]=0b000`: broadcast `imm[7:0]` as 8-bit elements to all lanes
- `imm[12:10]=0b001`: broadcast sign-extended `imm[9:0]` as 16-bit elements to all lanes
- `imm[12:10]=0b010`: broadcast sign-extended `imm[9:0]` as 32-bit elements to all lanes
- `imm[12:10]=0b011`: broadcast sign-extended `imm[9:0]` as 64-bit elements to all lanes
- `imm[12:8]=0b10000`: broadcast `imm[7:0]` as 32-bit elements to all lanes
- `imm[12:8]=0b10001`: broadcast `imm[7:0] << 8` as 32-bit elements to all lanes
- `imm[12:8]=0b10010`: broadcast `imm[7:0] << 16` as 32-bit elements to all lanes
- `imm[12:8]=0b10011`: broadcast `imm[7:0] << 24` as 32-bit elements to all lanes
- `imm[12:8]=0b10100`: broadcast `imm[7:0]` as 16-bit elements to all lanes
- `imm[12:8]=0b10101`: broadcast `imm[7:0] << 8` as 16-bit elements to all lanes
- `imm[12:8]=0b10110`: broadcast `(imm[7:0] << 8) | 0xFF` as 32-bit elements to all lanes
- `imm[12:8]=0b10111`: broadcast `(imm[7:0] << 16) | 0xFFFF` as 32-bit elements to all lanes
- `imm[12:8]=0b11000`: broadcast `imm[7:0]` as 8-bit elements to all lanes
- `imm[12:8]=0b11001`: repeat each bit of `imm[7:0]` eight times, and broadcast the result as 64-bit elements to all lanes
- `imm[12:8]=0b11010`: broadcast `(imm[7] << 31) | ((1-imm[6]) << 30) | ((imm[6] * 0x1F) << 25) | (imm[5:0] << 19)` as 32-bit elements to all lanes
- `imm[12:8]=0b11011`: broadcast `(imm[7] << 31) | ((1-imm[6]) << 30) | ((imm[6] * 0x1F) << 25) | (imm[5:0] << 19)` as 64-bit elements to all lanes
- `imm[12:8]=0b11100`: broadcast `(imm[7] << 63) | ((1-imm[6]) << 62) | ((imm[6] * 0xFF) << 54) | (imm[5:0] << 48)` as 64-bit elements to all lanes

### Operation

```
u64 imm12_10 = (imm >> 10) & 0b111;
u64 imm12_8 = (imm >> 8) & 0b11111;
u64 imm9_0 = imm & 0x3FF;
s64 simm9_0 = ((s64)imm9_0 << 54) >> 54;
u64 imm7_0 = imm & 0xFF;
u64 imm7 = (imm >> 7) & 0x1;
u64 imm6 = (imm >> 6) & 0x1;
u64 imm5 = (imm >> 5) & 0x1;
u64 imm5_0 = imm & 0x3F;
u64 imm4 = (imm >> 4) & 0x1;
u64 imm3 = (imm >> 3) & 0x1;
u64 imm2 = (imm >> 2) & 0x1;
u64 imm1 = (imm >> 1) & 0x1;
u64 imm0 = imm & 0x1;

u64 broadcast_value;
u64 broadcast_width;
if (imm12_10 == 0b000) {
  broadcast_value = imm7_0;
  broadcast_width = 8;
} else if (imm12_10 == 0b001) {
  broadcast_value = simm9_0;
  broadcast_width = 16;
} else if (imm12_10 == 0b010) {
  broadcast_value = simm9_0;
  broadcast_width = 32;
} else if (imm12_10 == 0b011) {
  broadcast_value = simm9_0;
  broadcast_width = 64;
} else if (imm12_8 == 0b10000) {
  broadcast_value = imm7_0;
  broadcast_width = 32;
} else if (imm12_8 == 0b10001) {
  broadcast_value = imm7_0 << 8;
  broadcast_width = 32;
} else if (imm12_8 == 0b10010) {
  broadcast_value = imm7_0 << 16;
  broadcast_width = 32;
} else if (imm12_8 == 0b10011) {
  broadcast_value = imm7_0 << 24;
  broadcast_width = 32;
} else if (imm12_8 == 0b10100) {
  broadcast_value = imm7_0;
  broadcast_width = 16;
} else if (imm12_8 == 0b10101) {
  broadcast_value = imm7_0 << 8;
  broadcast_width = 16;
} else if (imm12_8 == 0b10110) {
  broadcast_value = (imm7_0 << 8) | 0xFF;
  broadcast_width = 32;
} else if (imm12_8 == 0b10111) {
  broadcast_value = (imm7_0 << 16) | 0xFFFF;
  broadcast_width = 32;
} else if (imm12_8 == 0b11000) {
  broadcast_value = imm7_0;
  broadcast_width = 8;
} else if (imm12_8 == 0b11001) {
  broadcast_value = imm0 * 0xFF + imm1 * 0xFF00 + imm2 * 0xFF0000 +
                    imm3 * 0xFF000000 + imm4 * 0xFF00000000 +
                    imm5 * 0xFF0000000000 + imm6 * 0xFF000000000000 +
                    imm7 * 0xFF00000000000000;
  broadcast_width = 64;
} else if (imm12_8 == 0b11010) {
  broadcast_value = (imm7 << 31) | ((1 - imm6) << 30) | ((imm6 * 0x1F) << 25) |
                    (imm5_0 << 19);
  broadcast_width = 32;
} else if (imm12_8 == 0b11011) {
  broadcast_value = (imm7 << 31) | ((1 - imm6) << 30) | ((imm6 * 0x1F) << 25) |
                    (imm5_0 << 19);
  broadcast_width = 64;
} else if (imm12_8 == 0b11100) {
  broadcast_value = (imm7 << 63) | ((1 - imm6) << 62) | ((imm6 * 0xFF) << 54) |
                    (imm5_0 << 48);
  broadcast_width = 64;
}

if (broadcast_width == 8) {
  for (int i = 0; i < 32; i++) {
    dst.byte[i] = broadcast_value;
  }
} else if (broadcast_width == 16) {
  for (int i = 0; i < 16; i++) {
    dst.half[i] = broadcast_value;
  }
} else if (broadcast_width == 32) {
  for (int i = 0; i < 8; i++) {
    dst.word[i] = broadcast_value;
  }
} else if (broadcast_width == 64) {
  for (int i = 0; i < 4; i++) {
    dst.dword[i] = broadcast_value;
  }
}
```

Tested on real machine.
