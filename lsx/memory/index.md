# Memory Load & Store

## \_\_m128i \_\_lsx_vld (void * addr, imm_n2048_2047 offset)

### Synopsis

```
__m128i __lsx_vld (void * addr, imm_n2048_2047 offset)
#include <lsxintrin.h>
Instruction: vld vr, r, imm
CPU Flags: LSX
```

### Description

Read whole vector from memory address `addr + offset`, save the data into `dst`. Note that you can use this intrinsic to load floating point vectors, even though the return type represents integer vectors.

### Operation

```
dst = memory_load(128, addr + offset);
```

## \_\_m128i \_\_lsx_vldx (void * addr, long int offset)

### Synopsis

```
__m128i __lsx_vldx (void * addr, long int offset)
#include <lsxintrin.h>
Instruction: vldx vr, r, r
CPU Flags: LSX
```

### Description

Read whole vector from memory address `addr + offset`, save the data into `dst`. Note that you can use this intrinsic to load floating point vectors, even though the return type represents integer vectors.

### Operation

```
dst = memory_load(128, addr + offset);
```

## \_\_m128i \_\_lsx_vldrepl_b (void * addr, imm_n2048_2047 offset)

### Synopsis

```
__m128i __lsx_vldrepl_b (void * addr, imm_n2048_2047 offset)
#include <lsxintrin.h>
Instruction: vldrepl.b vr, r, imm
CPU Flags: LSX
```

### Description

Read 8-bit data from memory address `addr + (offset << 0)`, replicate the data to all vector lanes and save into `dst`.

### Operation

```
u8 data = memory_load(8, addr + offset);
for (int i = 0; i < 16; i++) {
  dst.byte[i] = data;
}
```

## \_\_m128i \_\_lsx_vldrepl_h (void * addr, imm_n1024_1023 offset)

### Synopsis

```
__m128i __lsx_vldrepl_h (void * addr, imm_n1024_1023 offset)
#include <lsxintrin.h>
Instruction: vldrepl.h vr, r, imm
CPU Flags: LSX
```

### Description

Read 16-bit data from memory address `addr + (offset << 1)`, replicate the data to all vector lanes and save into `dst`.

### Operation

```
u16 data = memory_load(16, addr + (offset << 1));
for (int i = 0; i < 8; i++) {
  dst.half[i] = data;
}
```

## \_\_m128i \_\_lsx_vldrepl_w (void * addr, imm_n512_511 offset)

### Synopsis

```
__m128i __lsx_vldrepl_w (void * addr, imm_n512_511 offset)
#include <lsxintrin.h>
Instruction: vldrepl.w vr, r, imm
CPU Flags: LSX
```

### Description

Read 32-bit data from memory address `addr + (offset << 2)`, replicate the data to all vector lanes and save into `dst`.

### Operation

```
u32 data = memory_load(32, addr + (offset << 2));
for (int i = 0; i < 4; i++) {
  dst.word[i] = data;
}
```

## \_\_m128i \_\_lsx_vldrepl_d (void * addr, imm_n256_255 offset)

### Synopsis

```
__m128i __lsx_vldrepl_d (void * addr, imm_n256_255 offset)
#include <lsxintrin.h>
Instruction: vldrepl.d vr, r, imm
CPU Flags: LSX
```

### Description

Read 64-bit data from memory address `addr + (offset << 3)`, replicate the data to all vector lanes and save into `dst`.

### Operation

```
u64 data = memory_load(64, addr + (offset << 3));
for (int i = 0; i < 2; i++) {
  dst.dword[i] = data;
}
```

## void \_\_lsx_vst (\_\_m128i data, void * addr, imm_n2048_2047 offset)

### Synopsis

```
void __lsx_vst (__m128i data, void * addr, imm_n2048_2047 offset)
#include <lsxintrin.h>
Instruction: vst vr, r, imm
CPU Flags: LSX
```

### Description

Write whole vector data in `data` to memory address `addr + offset`.

### Operation

```
memory_store(128, data, addr + offset);
```

## void \_\_lsx_vstx (\_\_m128i data, void * addr, long int offset)

### Synopsis

```
void __lsx_vstx (__m128i data, void * addr, long int offset)
#include <lsxintrin.h>
Instruction: vstx vr, r, r
CPU Flags: LSX
```

### Description

Write whole-vector data in `data` to memory address `addr + offset`.

### Operation

```
memory_store(128, data, addr + offset);
```

## void \_\_lsx_vstelm_b (\_\_m128i data, void * addr, imm_n128_127 offset, imm0_15 lane)

### Synopsis

```
void __lsx_vstelm_b (__m128i data, void * addr, imm_n128_127 offset, imm0_15 lane)
#include <lsxintrin.h>
Instruction: vstelm.b vr, r, imm, imm
CPU Flags: LSX
```

### Description

Store the 8-bit element in `data` specified by `lane` to memory address `addr + offset`.

### Operation

```
memory_store(8, data.byte[lane], addr + offset);
```

## void \_\_lsx_vstelm_h (\_\_m128i data, void * addr, imm_n128_127 offset, imm0_7 lane)

### Synopsis

```
void __lsx_vstelm_h (__m128i data, void * addr, imm_n128_127 offset, imm0_7 lane)
#include <lsxintrin.h>
Instruction: vstelm.h vr, r, imm, imm
CPU Flags: LSX
```

### Description

Store the 16-bit element in `data` specified by `lane` to memory address `addr + offset`.

### Operation

```
memory_store(16, data.half[lane], addr + offset);
```

## void \_\_lsx_vstelm_w (\_\_m128i data, void * addr, imm_n128_127 offset, imm0_3 lane)

### Synopsis

```
void __lsx_vstelm_w (__m128i data, void * addr, imm_n128_127 offset, imm0_3 lane)
#include <lsxintrin.h>
Instruction: vstelm.w vr, r, imm, imm
CPU Flags: LSX
```

### Description

Store the 32-bit element in `data` specified by `lane` to memory address `addr + offset`.

### Operation

```
memory_store(32, data.word[lane], addr + offset);
```

## void \_\_lsx_vstelm_d (\_\_m128i data, void * addr, imm_n128_127 offset, imm0_1 lane)

### Synopsis

```
void __lsx_vstelm_d (__m128i data, void * addr, imm_n128_127 offset, imm0_1 lane)
#include <lsxintrin.h>
Instruction: vstelm.d vr, r, imm, imm
CPU Flags: LSX
```

### Description

Store the 64-bit element in `data` specified by `lane` to memory address `addr + offset`.

### Operation

```
memory_store(64, data.dword[lane], addr + offset);
```
