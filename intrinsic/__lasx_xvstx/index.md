# \_\_lasx_xvstx

LASX intrinsic · group: [Memory Load & Store](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/memory/index.md)

### Synopsis

```
void __lasx_xvstx (__m256i data, void * addr, long int offset)
#include <lasxintrin.h>
Instruction: xvstx xr, r, r
CPU Flags: LASX
```

### Description

Write whole vector data in `data` to memory address `addr + offset`. Address misalignment does not raise an exception.

### Operation

```
memory_store(256, data, addr + offset);
```

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
