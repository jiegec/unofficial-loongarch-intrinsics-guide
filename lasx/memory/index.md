# Memory Load & Store

## \_\_m256i \_\_lasx_xvld (void * addr, imm_n2048_2047 offset)

### Synopsis

```
__m256i __lasx_xvld (void * addr, imm_n2048_2047 offset)
#include <lasxintrin.h>
Instruction: xvld xr, r, imm
CPU Flags: LASX
```

### Description

Read whole vector from memory address `addr + offset`, save the data into `dst`. Note that you can use this intrinsic to load floating point vectors, even though the return type represents integer vectors.

### Operation

```
dst = memory_load(256, addr + offset);
```

## \_\_m256i \_\_lasx_xvldx (void * addr, long int offset)

### Synopsis

```
__m256i __lasx_xvldx (void * addr, long int offset)
#include <lasxintrin.h>
Instruction: xvldx xr, r, r
CPU Flags: LASX
```

### Description

Read whole vector from memory address `addr + offset`, save the data into `dst`. Note that you can use this intrinsic to load floating point vectors, even though the return type represents integer vectors.

### Operation

```
dst = memory_load(256, addr + offset);
```

## \_\_m256i \_\_lasx_xvldrepl_b (void * addr, imm_n2048_2047 offset)

### Synopsis

```
__m256i __lasx_xvldrepl_b (void * addr, imm_n2048_2047 offset)
#include <lasxintrin.h>
Instruction: xvldrepl.b xr, r, imm
CPU Flags: LASX
```

### Description

Read 8-bit data from memory address `addr + (offset << 0)`, replicate the data to all vector lanes and save into `dst`.

### Operation

```
u8 data = memory_load(8, addr + offset);
for (int i = 0; i < 32; i++) {
  dst.byte[i] = data;
}
```

## \_\_m256i \_\_lasx_xvldrepl_h (void * addr, imm_n1024_1023 offset)

### Synopsis

```
__m256i __lasx_xvldrepl_h (void * addr, imm_n1024_1023 offset)
#include <lasxintrin.h>
Instruction: xvldrepl.h xr, r, imm
CPU Flags: LASX
```

### Description

Read 16-bit data from memory address `addr + (offset << 1)`, replicate the data to all vector lanes and save into `dst`.

### Operation

```
u16 data = memory_load(16, addr + (offset << 1));
for (int i = 0; i < 16; i++) {
  dst.half[i] = data;
}
```

## \_\_m256i \_\_lasx_xvldrepl_w (void * addr, imm_n512_511 offset)

### Synopsis

```
__m256i __lasx_xvldrepl_w (void * addr, imm_n512_511 offset)
#include <lasxintrin.h>
Instruction: xvldrepl.w xr, r, imm
CPU Flags: LASX
```

### Description

Read 32-bit data from memory address `addr + (offset << 2)`, replicate the data to all vector lanes and save into `dst`.

### Operation

```
u32 data = memory_load(32, addr + (offset << 2));
for (int i = 0; i < 8; i++) {
  dst.word[i] = data;
}
```

## \_\_m256i \_\_lasx_xvldrepl_d (void * addr, imm_n256_255 offset)

### Synopsis

```
__m256i __lasx_xvldrepl_d (void * addr, imm_n256_255 offset)
#include <lasxintrin.h>
Instruction: xvldrepl.d xr, r, imm
CPU Flags: LASX
```

### Description

Read 64-bit data from memory address `addr + (offset << 3)`, replicate the data to all vector lanes and save into `dst`.

### Operation

```
u64 data = memory_load(64, addr + (offset << 3));
for (int i = 0; i < 4; i++) {
  dst.dword[i] = data;
}
```

## void \_\_lasx_xvst (\_\_m256i data, void * addr, imm_n2048_2047 offset)

### Synopsis

```
void __lasx_xvst (__m256i data, void * addr, imm_n2048_2047 offset)
#include <lasxintrin.h>
Instruction: xvst xr, r, imm
CPU Flags: LASX
```

### Description

Write whole vector data in `data` to memory address `addr + offset`.

### Operation

```
memory_store(256, data, addr + offset);
```

## void \_\_lasx_xvstx (\_\_m256i data, void * addr, long int offset)

### Synopsis

```
void __lasx_xvstx (__m256i data, void * addr, long int offset)
#include <lasxintrin.h>
Instruction: xvstx xr, r, r
CPU Flags: LASX
```

### Description

Write whole-vector data in `data` to memory address `addr + offset`.

### Operation

```
memory_store(256, data, addr + offset);
```

## void \_\_lasx_xvstelm_b (\_\_m256i data, void * addr, imm_n128_127 offset, imm0_31 lane)

### Synopsis

```
void __lasx_xvstelm_b (__m256i data, void * addr, imm_n128_127 offset, imm0_31 lane)
#include <lasxintrin.h>
Instruction: xvstelm.b xr, r, imm, imm
CPU Flags: LASX
```

### Description

Store the 8-bit element in `data` specified by `lane` to memory address `addr + offset`.

### Operation

```
memory_store(8, data.byte[lane], addr + offset);
```

## void \_\_lasx_xvstelm_h (\_\_m256i data, void * addr, imm_n128_127 offset, imm0_15 lane)

### Synopsis

```
void __lasx_xvstelm_h (__m256i data, void * addr, imm_n128_127 offset, imm0_15 lane)
#include <lasxintrin.h>
Instruction: xvstelm.h xr, r, imm, imm
CPU Flags: LASX
```

### Description

Store the 16-bit element in `data` specified by `lane` to memory address `addr + offset`.

### Operation

```
memory_store(16, data.half[lane], addr + offset);
```

## void \_\_lasx_xvstelm_w (\_\_m256i data, void * addr, imm_n128_127 offset, imm0_7 lane)

### Synopsis

```
void __lasx_xvstelm_w (__m256i data, void * addr, imm_n128_127 offset, imm0_7 lane)
#include <lasxintrin.h>
Instruction: xvstelm.w xr, r, imm, imm
CPU Flags: LASX
```

### Description

Store the 32-bit element in `data` specified by `lane` to memory address `addr + offset`.

### Operation

```
memory_store(32, data.word[lane], addr + offset);
```

## void \_\_lasx_xvstelm_d (\_\_m256i data, void * addr, imm_n128_127 offset, imm0_3 lane)

### Synopsis

```
void __lasx_xvstelm_d (__m256i data, void * addr, imm_n128_127 offset, imm0_3 lane)
#include <lasxintrin.h>
Instruction: xvstelm.d xr, r, imm, imm
CPU Flags: LASX
```

### Description

Store the 64-bit element in `data` specified by `lane` to memory address `addr + offset`.

### Operation

```
memory_store(64, data.dword[lane], addr + offset);
```
