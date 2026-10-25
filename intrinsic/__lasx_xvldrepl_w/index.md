# \_\_lasx_xvldrepl_w

LASX intrinsic · group: [Memory Load & Store](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/memory/index.md)

### Synopsis

```
__m256i __lasx_xvldrepl_w (void * addr, imm_n512_511 offset)
#include <lasxintrin.h>
Instruction: xvldrepl.w xr, r, imm
CPU Flags: LASX
```

### Description

Read 32-bit data from memory address `addr + offset`, replicate the data to all vector lanes and save into `dst`. Address misalignment does not raise an exception. The `offset` operand is a byte offset, matching the assembly immediate; the encoded instruction stores it as a scaled immediate for wider elements.

### Operation

```
u32 data = memory_load(32, addr + offset);
for (int i = 0; i < 8; i++) {
  dst.word[i] = data;
}
```

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
