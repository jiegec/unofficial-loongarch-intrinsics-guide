# \_\_lsx_vrepli_d

LSX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/misc/index.md)

### Synopsis

```
__m128i __lsx_vrepli_d (imm_n512_511 imm)
#include <lsxintrin.h>
Instruction: vldi vr, imm
CPU Flags: LSX
```

### Description

Repeat `imm` to fill whole vector.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = imm;
}
```

Tested on real machine.

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
