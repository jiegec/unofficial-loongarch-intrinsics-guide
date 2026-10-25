# \_\_lsx_vfrsqrte_s

LSX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_computation/index.md)

### Synopsis

```
__m128 __lsx_vfrsqrte_s (__m128 a)
#include <lsxintrin.h>
Instruction: vfrsqrte.s vr, vr
CPU Flags: LSX
```

### Description

Compute estimated reciprocal of square root of single precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = 1.0 / sqrt(a.fp32[i]); // estimated
}
```

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
