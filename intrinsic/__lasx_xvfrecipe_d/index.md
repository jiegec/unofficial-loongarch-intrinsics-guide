# \_\_lasx_xvfrecipe_d

LASX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_computation/index.md)

### Synopsis

```
__m256d __lasx_xvfrecipe_d (__m256d a)
#include <lasxintrin.h>
Instruction: xvfrecipe.d xr, xr
CPU Flags: LASX
```

### Description

Compute estimated reciprocal of double precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = 1 / a.fp64[i]; // estimated
}
```

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
