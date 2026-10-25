# \_\_lasx_xvldrepl_d

LASX intrinsic · group: [Memory Load & Store](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/memory/index.md)

### Synopsis

```
__m256i __lasx_xvldrepl_d (void * addr, imm_n256_255 offset)
#include <lasxintrin.h>
Instruction: xvldrepl.d xr, r, imm
CPU Flags: LASX
```

### Description

Read 64-bit data from memory address `addr + offset`, replicate the data to all vector lanes and save into `dst`. Address misalignment does not raise an exception. The `offset` operand is a byte offset, matching the assembly immediate; the encoded instruction stores it as a scaled immediate for wider elements.

### Operation

```
u64 data = memory_load(64, addr + offset);
for (int i = 0; i < 4; i++) {
  dst.dword[i] = data;
}
```

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
