# \_\_lsx_vld

LSX intrinsic · group: [Memory Load & Store](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/memory/index.md)

### Synopsis

```
__m128i __lsx_vld (void * addr, imm_n2048_2047 offset)
#include <lsxintrin.h>
Instruction: vld vr, r, imm
CPU Flags: LSX
```

### Description

Read whole vector from memory address `addr + offset`, save the data into `dst`. Address misalignment does not raise an exception. Note that you can use this intrinsic to load floating point vectors, even though the return type represents integer vectors.

### Operation

```
dst = memory_load(128, addr + offset);
```

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
