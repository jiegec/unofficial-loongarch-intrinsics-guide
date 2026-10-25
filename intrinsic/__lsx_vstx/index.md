# \_\_lsx_vstx

LSX intrinsic · group: [Memory Load & Store](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/memory/index.md)

### Synopsis

```
void __lsx_vstx (__m128i data, void * addr, long int offset)
#include <lsxintrin.h>
Instruction: vstx vr, r, r
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
