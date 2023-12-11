# Memory Load

## __m128i __lsx_vld (void * addr, imm_n2048_2047 offset)

### Synopsis

```c++
__m128i __lsx_vld (void * addr, imm_n2048_2047 offset)
#include <lsxintrin.h>
Instruction: vld vr, r, imm
CPU Flags: LSX
```

### Description

Read 128-bit data from memory address `addr + offset`, save the data into `dst`.

### Operation

```c++
dst = memory_load(128, addr + offset);
```

## __m128i __lsx_vldx (void * addr, long int offset)

### Synopsis

```c++
__m128i __lsx_vldx (void * addr, long int offset);
#include <lsxintrin.h>
Instruction: vldx vr, r, r
CPU Flags: LSX
```

### Description

Read 128-bit data from memory address `addr + offset`, save the data into `dst`.

### Operation

```c++
dst = memory_load(128, addr + offset);
```

## __m128i __lsx_vldrepl_b (void * addr, imm_n2048_2047 offset)

### Synopsis

```c++
__m128i __lsx_vldrepl_b (void * addr, imm_n2048_2047 offset)
#include <lsxintrin.h>
Instruction: vldrepl.b vr, r, imm
CPU Flags: LSX
```

### Description

Read 8-bit data from memory address `addr + offset`, replicate the data to all vector lanes and save into `dst`.

### Operation

```c++
u8 data = memory_load(8, addr + offset);
for (int i = 0;i < 16;i++) {
    dst.byte[i] = data;
}
```

## __m128i __lsx_vldrepl_h (void * addr, imm_n1024_1023 offset)

### Synopsis

```c++
__m128i __lsx_vldrepl_h (void * addr, imm_n1024_1023 offset)
#include <lsxintrin.h>
Instruction: vldrepl.h vr, r, imm
CPU Flags: LSX
```

### Description

Read 16-bit data from memory address `addr + (offset << 1)`, replicate the data to all vector lanes and save into `dst`.

### Operation

```c++
u16 data = memory_load(16, addr + (offset << 1));
for (int i = 0;i < 8;i++) {
    dst.half[i] = data;
}
```

## __m128i __lsx_vldrepl_w (void * addr, imm_n512_511 offset)

### Synopsis

```c++
__m128i __lsx_vldrepl_w (void * addr, imm_n512_511 offset)
#include <lsxintrin.h>
Instruction: vldrepl.w vr, r, imm
CPU Flags: LSX
```

### Description

Read 32-bit data from memory address `addr + (offset << 2)`, replicate the data to all vector lanes and save into `dst`.

### Operation

```c++
u32 data = memory_load(32, addr + (offset << 2));
for (int i = 0;i < 4;i++) {
    dst.word[i] = data;
}
```

## __m128i __lsx_vldrepl_d (void * addr, imm_n256_255 offset)

### Synopsis

```c++
__m128i __lsx_vldrepl_d (void * addr, imm_n256_255 offset)
#include <lsxintrin.h>
Instruction: vldrepl.d vr, r, imm
CPU Flags: LSX
```

### Description

Read 64-bit data from memory address `addr + (offset << 3)`, replicate the data to all vector lanes and save into `dst`.

### Operation

```c++
u64 data = memory_load(64, addr + (offset << 3));
for (int i = 0;i < 2;i++) {
    dst.dword[i] = data;
}
```