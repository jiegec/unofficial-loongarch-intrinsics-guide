# \_\_lasx_xvld

LASX intrinsic · group: [Memory Load & Store](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/memory/index.md)

### Synopsis

```
__m256i __lasx_xvld (void * addr, imm_n2048_2047 offset)
#include <lasxintrin.h>
Instruction: xvld xr, r, imm
CPU Flags: LASX
```

### Description

Read whole vector from memory address `addr + offset`, save the data into `dst`. Address misalignment does not raise an exception. Note that you can use this intrinsic to load floating point vectors, even though the return type represents integer vectors.

### Operation

```
dst = memory_load(256, addr + offset);
```

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
