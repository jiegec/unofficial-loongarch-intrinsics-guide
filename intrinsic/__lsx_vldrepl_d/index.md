# \_\_lsx_vldrepl_d

LSX intrinsic · group: [Memory Load & Store](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/memory/index.md)

### Synopsis

```
__m128i __lsx_vldrepl_d (void * addr, imm_n256_255 offset)
#include <lsxintrin.h>
Instruction: vldrepl.d vr, r, imm
CPU Flags: LSX
```

### Description

Read 64-bit data from memory address `addr + offset`, replicate the data to all vector lanes and save into `dst`. Address misalignment does not raise an exception. The `offset` operand is a byte offset, matching the assembly immediate; the encoded instruction stores it as a scaled immediate for wider elements.

### Operation

```
u64 data = memory_load(64, addr + offset);
for (int i = 0; i < 2; i++) {
  dst.dword[i] = data;
}
```

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
