# \_\_lsx_vst

LSX intrinsic · group: [Memory Load & Store](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/memory/index.md)

### Synopsis

```
void __lsx_vst (__m128i data, void * addr, imm_n2048_2047 offset)
#include <lsxintrin.h>
Instruction: vst vr, r, imm
CPU Flags: LSX
```

### Description

Write whole vector data in `data` to memory address `addr + offset`. Address misalignment does not raise an exception.

### Operation

```
memory_store(128, data, addr + offset);
```

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
