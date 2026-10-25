# \_\_lsx_vfrecipe_d

LSX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_computation/index.md)

### Synopsis

```
__m128d __lsx_vfrecipe_d (__m128d a)
#include <lsxintrin.h>
Instruction: vfrecipe.d vr, vr
CPU Flags: LSX
```

### Description

Compute estimated reciprocal of double precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = 1 / a.fp64[i]; // estimated
}
```

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
