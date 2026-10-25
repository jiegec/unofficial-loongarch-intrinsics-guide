# \_\_lasx_xvrepli_d

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

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

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
