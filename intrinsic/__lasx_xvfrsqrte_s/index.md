# \_\_lasx_xvfrsqrte_s

LASX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_computation/index.md)

### Synopsis

```
__m256 __lasx_xvfrsqrte_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvfrsqrte.s xr, xr
CPU Flags: LASX
```

### Description

Compute estimated reciprocal of square root of single precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = 1.0 / sqrt(a.fp32[i]); // estimated
}
```

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
