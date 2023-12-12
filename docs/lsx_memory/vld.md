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

{{ vldrepl('b') }}
{{ vldrepl('h') }}
{{ vldrepl('w') }}
{{ vldrepl('d') }}
