# \_\_lasx_xvldrepl_b

LASX intrinsic · group: [Memory Load & Store](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/memory/index.md)

### Synopsis

```
__m256i __lasx_xvldrepl_b (void * addr, imm_n2048_2047 offset)
#include <lasxintrin.h>
Instruction: xvldrepl.b xr, r, imm
CPU Flags: LASX
```

### Description

Read 8-bit data from memory address `addr + offset`, replicate the data to all vector lanes and save into `dst`. Address misalignment does not raise an exception. The `offset` operand is a byte offset, matching the assembly immediate; the encoded instruction stores it as a scaled immediate for wider elements.

### Operation

```
u8 data = memory_load(8, addr + offset);
for (int i = 0; i < 32; i++) {
  dst.byte[i] = data;
}
```

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
