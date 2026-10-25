# \_\_lsx_vstelm_w

LSX intrinsic · group: [Memory Load & Store](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/memory/index.md)

### Synopsis

```
void __lsx_vstelm_w (__m128i data, void * addr, imm_n128_127 offset, imm0_3 lane)
#include <lsxintrin.h>
Instruction: vstelm.w vr, r, imm, imm
CPU Flags: LSX
```

### Description

Store element `lane` of `data` to memory address `addr + offset`. Address misalignment does not raise an exception. The `offset` operand is a byte offset; the encoded instruction stores it as a scaled immediate for wider elements.

### Operation

```
memory_store(32, data.word[lane], addr + offset);
```

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
