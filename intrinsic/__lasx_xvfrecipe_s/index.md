# \_\_lasx_xvfrecipe_s

LASX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_computation/index.md)

### Synopsis

```
__m256 __lasx_xvfrecipe_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvfrecipe.s xr, xr
CPU Flags: LASX
```

### Description

Compute estimated reciprocal of single precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = 1 / a.fp32[i]; // estimated
}
```

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
