# \_\_lasx_xvstelm_w

LASX intrinsic · group: [Memory Load & Store](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/memory/index.md)

### Synopsis

```
void __lasx_xvstelm_w (__m256i data, void * addr, imm_n128_127 offset, imm0_7 lane)
#include <lasxintrin.h>
Instruction: xvstelm.w xr, r, imm, imm
CPU Flags: LASX
```

### Description

Store element `lane` of `data` to memory address `addr + offset`. Address misalignment does not raise an exception. The `offset` operand is a byte offset; the encoded instruction stores it as a scaled immediate for wider elements.

### Operation

```
memory_store(32, data.word[lane], addr + offset);
```

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
