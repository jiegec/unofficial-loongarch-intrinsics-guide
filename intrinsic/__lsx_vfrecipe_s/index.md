# \_\_lsx_vfrecipe_s

LSX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_computation/index.md)

### Synopsis

```
__m128 __lsx_vfrecipe_s (__m128 a)
#include <lsxintrin.h>
Instruction: vfrecipe.s vr, vr
CPU Flags: LSX
```

### Description

Compute estimated reciprocal of single precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = 1 / a.fp32[i]; // estimated
}
```

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
