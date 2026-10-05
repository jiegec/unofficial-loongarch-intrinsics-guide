# Shift

## \_\_m128i \_\_lsx_vbsll_v (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vbsll_v (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vbsll.v vr, vr, imm
CPU Flags: LSX
```

### Description

Compute whole vector `a` shifted left by `imm * 8` bits.

### Operation

```
int shift = (imm * 8) % 128;
dst.qword[0] = (u128)a.qword[0] << shift;
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vbsrl_v (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vbsrl_v (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vbsrl.v vr, vr, imm
CPU Flags: LSX
```

### Description

Compute whole vector `a` shifted right by `imm * 8` bits.

### Operation

```
int shift = (imm * 8) % 128;
dst.qword[0] = (u128)a.qword[0] >> shift;
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vsll_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsll_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsll.b vr, vr, vr
CPU Flags: LSX
```

### Description

Logical left shift the unsigned 8-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[i] << (b.byte[i] & 0x7);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vsll_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsll_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsll.h vr, vr, vr
CPU Flags: LSX
```

### Description

Logical left shift the unsigned 16-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = a.half[i] << (b.half[i] & 0xf);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vsll_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsll_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsll.w vr, vr, vr
CPU Flags: LSX
```

### Description

Logical left shift the unsigned 32-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = a.word[i] << (b.word[i] & 0x1f);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vsll_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsll_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsll.d vr, vr, vr
CPU Flags: LSX
```

### Description

Logical left shift the unsigned 64-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = a.dword[i] << (b.dword[i] & 0x3f);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vslli_b (\_\_m128i a, imm0_7 imm)

### Synopsis

```
__m128i __lsx_vslli_b (__m128i a, imm0_7 imm)
#include <lsxintrin.h>
Instruction: vslli.b vr, vr, imm
CPU Flags: LSX
```

### Description

Logical left shift the unsigned 8-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[i] << imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vslli_h (\_\_m128i a, imm0_15 imm)

### Synopsis

```
__m128i __lsx_vslli_h (__m128i a, imm0_15 imm)
#include <lsxintrin.h>
Instruction: vslli.h vr, vr, imm
CPU Flags: LSX
```

### Description

Logical left shift the unsigned 16-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = a.half[i] << imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vslli_w (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vslli_w (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vslli.w vr, vr, imm
CPU Flags: LSX
```

### Description

Logical left shift the unsigned 32-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = a.word[i] << imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vslli_d (\_\_m128i a, imm0_63 imm)

### Synopsis

```
__m128i __lsx_vslli_d (__m128i a, imm0_63 imm)
#include <lsxintrin.h>
Instruction: vslli.d vr, vr, imm
CPU Flags: LSX
```

### Description

Logical left shift the unsigned 64-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = a.dword[i] << imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vsllwil_h_b (\_\_m128i a, imm0_7 imm)

### Synopsis

```
__m128i __lsx_vsllwil_h_b (__m128i a, imm0_7 imm)
#include <lsxintrin.h>
Instruction: vsllwil.h.b vr, vr, imm
CPU Flags: LSX
```

### Description

Extend and shift signed 8-bit elements in `a` by `imm` to signed 16-bit result.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (s16)(s8)a.byte[i] << imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 1                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 0.5(1/2)         |
| 2K3000   | LA364E | 2       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vsllwil_hu_bu (\_\_m128i a, imm0_7 imm)

### Synopsis

```
__m128i __lsx_vsllwil_hu_bu (__m128i a, imm0_7 imm)
#include <lsxintrin.h>
Instruction: vsllwil.hu.bu vr, vr, imm
CPU Flags: LSX
```

### Description

Extend and shift unsigned 8-bit elements in `a` by `imm` to unsigned 16-bit result.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (u16)(u8)a.byte[i] << imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 1                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 0.5(1/2)         |
| 2K3000   | LA364E | 2       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vsllwil_w_h (\_\_m128i a, imm0_15 imm)

### Synopsis

```
__m128i __lsx_vsllwil_w_h (__m128i a, imm0_15 imm)
#include <lsxintrin.h>
Instruction: vsllwil.w.h vr, vr, imm
CPU Flags: LSX
```

### Description

Extend and shift signed 16-bit elements in `a` by `imm` to signed 32-bit result.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (s32)(s16)a.half[i] << imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 1                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 0.5(1/2)         |
| 2K3000   | LA364E | 2       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vsllwil_wu_hu (\_\_m128i a, imm0_15 imm)

### Synopsis

```
__m128i __lsx_vsllwil_wu_hu (__m128i a, imm0_15 imm)
#include <lsxintrin.h>
Instruction: vsllwil.wu.hu vr, vr, imm
CPU Flags: LSX
```

### Description

Extend and shift unsigned 16-bit elements in `a` by `imm` to unsigned 32-bit result.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (u32)(u16)a.half[i] << imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 1                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 0.5(1/2)         |
| 2K3000   | LA364E | 2       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vsllwil_d_w (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vsllwil_d_w (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vsllwil.d.w vr, vr, imm
CPU Flags: LSX
```

### Description

Extend and shift signed 32-bit elements in `a` by `imm` to signed 64-bit result.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)(s32)a.word[i] << imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 1                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 0.5(1/2)         |
| 2K3000   | LA364E | 2       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vsllwil_du_wu (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vsllwil_du_wu (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vsllwil.du.wu vr, vr, imm
CPU Flags: LSX
```

### Description

Extend and shift unsigned 32-bit elements in `a` by `imm` to unsigned 64-bit result.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (u64)(u32)a.word[i] << imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 1                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 0.5(1/2)         |
| 2K3000   | LA364E | 2       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vsra_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsra_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsra.b vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 8-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = ((s8)a.byte[i]) >> (b.byte[i] & 0x7);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vsra_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsra_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsra.h vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 16-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = ((s16)a.half[i]) >> (b.half[i] & 0xf);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vsra_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsra_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsra.w vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 32-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = ((s32)a.word[i]) >> (b.word[i] & 0x1f);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vsra_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsra_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsra.d vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 64-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = ((s64)a.dword[i]) >> (b.dword[i] & 0x3f);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vsrai_b (\_\_m128i a, imm0_7 imm)

### Synopsis

```
__m128i __lsx_vsrai_b (__m128i a, imm0_7 imm)
#include <lsxintrin.h>
Instruction: vsrai.b vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 8-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = ((s8)a.byte[i]) >> imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vsrai_h (\_\_m128i a, imm0_15 imm)

### Synopsis

```
__m128i __lsx_vsrai_h (__m128i a, imm0_15 imm)
#include <lsxintrin.h>
Instruction: vsrai.h vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 16-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = ((s16)a.half[i]) >> imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vsrai_w (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vsrai_w (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vsrai.w vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 32-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = ((s32)a.word[i]) >> imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vsrai_d (\_\_m128i a, imm0_63 imm)

### Synopsis

```
__m128i __lsx_vsrai_d (__m128i a, imm0_63 imm)
#include <lsxintrin.h>
Instruction: vsrai.d vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 64-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = ((s64)a.dword[i]) >> imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vsran_b_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsran_b_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsran.b.h vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 16-bit elements in `a` by elements in `b`, truncate to 8-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (i < 8) ? (s8)((s16)a.half[i] >> (b.half[i] & 15)) : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 1                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 0.5(1/2)         |
| 2K3000   | LA364E | 2       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vsran_h_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsran_h_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsran.h.w vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 32-bit elements in `a` by elements in `b`, truncate to 16-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (i < 4) ? (s16)((s32)a.word[i] >> (b.word[i] & 31)) : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 1                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 0.5(1/2)         |
| 2K3000   | LA364E | 2       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vsran_w_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsran_w_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsran.w.d vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 64-bit elements in `a` by elements in `b`, truncate to 32-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2) ? (s32)((s64)a.dword[i] >> (b.dword[i] & 63)) : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 1                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 0.5(1/2)         |
| 2K3000   | LA364E | 2       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vsrani_b_h (\_\_m128i a, \_\_m128i b, imm0_15 imm)

### Synopsis

```
__m128i __lsx_vsrani_b_h (__m128i a, __m128i b, imm0_15 imm)
#include <lsxintrin.h>
Instruction: vsrani.b.h vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 16-bit elements in `a` and `b` by `imm`, truncate to 8-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] =
      (i < 8) ? (s8)((s16)b.half[i] >> imm) : (s8)((s16)a.half[i - 8] >> imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vsrani_h_w (\_\_m128i a, \_\_m128i b, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vsrani_h_w (__m128i a, __m128i b, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vsrani.h.w vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 32-bit elements in `a` and `b` by `imm`, truncate to 16-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] =
      (i < 4) ? (s16)((s32)b.word[i] >> imm) : (s16)((s32)a.word[i - 4] >> imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vsrani_w_d (\_\_m128i a, \_\_m128i b, imm0_63 imm)

### Synopsis

```
__m128i __lsx_vsrani_w_d (__m128i a, __m128i b, imm0_63 imm)
#include <lsxintrin.h>
Instruction: vsrani.w.d vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 64-bit elements in `a` and `b` by `imm`, truncate to 32-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2) ? (s32)((s64)b.dword[i] >> imm)
                        : (s32)((s64)a.dword[i - 2] >> imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vsrani_d_q (\_\_m128i a, \_\_m128i b, imm0_127 imm)

### Synopsis

```
__m128i __lsx_vsrani_d_q (__m128i a, __m128i b, imm0_127 imm)
#include <lsxintrin.h>
Instruction: vsrani.d.q vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 128-bit elements in `a` and `b` by `imm`, truncate to 64-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (i < 1) ? (s64)((s128)b.qword[i] >> imm)
                         : (s64)((s128)a.qword[i - 1] >> imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vsrar_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsrar_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsrar.b vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift (with rounding) the signed 8-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  if ((b.byte[i] & 0x7) == 0) {
    dst.byte[i] = a.byte[i];
  } else {
    dst.byte[i] = ((s8)a.byte[i] >> (b.byte[i] & 0x7)) +
                  (((s8)a.byte[i] >> ((b.byte[i] & 0x7) - 1)) & 0x1);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vsrar_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsrar_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsrar.h vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift (with rounding) the signed 16-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  if ((b.half[i] & 0xf) == 0) {
    dst.half[i] = a.half[i];
  } else {
    dst.half[i] = ((s16)a.half[i] >> (b.half[i] & 0xf)) +
                  (((s16)a.half[i] >> ((b.half[i] & 0xf) - 1)) & 0x1);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vsrar_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsrar_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsrar.w vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift (with rounding) the signed 32-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  if ((b.word[i] & 0x1f) == 0) {
    dst.word[i] = a.word[i];
  } else {
    dst.word[i] = ((s32)a.word[i] >> (b.word[i] & 0x1f)) +
                  (((s32)a.word[i] >> ((b.word[i] & 0x1f) - 1)) & 0x1);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vsrar_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsrar_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsrar.d vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift (with rounding) the signed 64-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  if ((b.dword[i] & 0x3f) == 0) {
    dst.dword[i] = a.dword[i];
  } else {
    dst.dword[i] = ((s64)a.dword[i] >> (b.dword[i] & 0x3f)) +
                   (((s64)a.dword[i] >> ((b.dword[i] & 0x3f) - 1)) & 0x1);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vsrari_b (\_\_m128i a, imm0_7 imm)

### Synopsis

```
__m128i __lsx_vsrari_b (__m128i a, imm0_7 imm)
#include <lsxintrin.h>
Instruction: vsrari.b vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift (with rounding) the signed 8-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (imm == 0) {
    dst.byte[i] = a.byte[i];
  } else {
    dst.byte[i] = ((s8)a.byte[i] >> imm) + (((s8)a.byte[i] >> (imm - 1)) & 0x1);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vsrari_h (\_\_m128i a, imm0_15 imm)

### Synopsis

```
__m128i __lsx_vsrari_h (__m128i a, imm0_15 imm)
#include <lsxintrin.h>
Instruction: vsrari.h vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift (with rounding) the signed 16-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (imm == 0) {
    dst.half[i] = a.half[i];
  } else {
    dst.half[i] =
        ((s16)a.half[i] >> imm) + (((s16)a.half[i] >> (imm - 1)) & 0x1);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vsrari_w (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vsrari_w (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vsrari.w vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift (with rounding) the signed 32-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (imm == 0) {
    dst.word[i] = a.word[i];
  } else {
    dst.word[i] =
        ((s32)a.word[i] >> imm) + (((s32)a.word[i] >> (imm - 1)) & 0x1);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vsrari_d (\_\_m128i a, imm0_63 imm)

### Synopsis

```
__m128i __lsx_vsrari_d (__m128i a, imm0_63 imm)
#include <lsxintrin.h>
Instruction: vsrari.d vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift (with rounding) the signed 64-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (imm == 0) {
    dst.dword[i] = a.dword[i];
  } else {
    dst.dword[i] =
        ((s64)a.dword[i] >> imm) + (((s64)a.dword[i] >> (imm - 1)) & 0x1);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vsrarn_b_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsrarn_b_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsrarn.b.h vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift (with rounding) the signed 16-bit elements in `a` by elements in `b`, truncate to 8-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (i < 8) {
    u8 shift = (b.half[i] & 15);
    if (shift == 0) {
      dst.byte[i] = (s8)(s16)a.half[i];
    } else {
      dst.byte[i] = (s8)(((s16)a.half[i] >> shift) +
                         (((s16)a.half[i] >> (shift - 1)) & 0x1));
    }
  } else {
    dst.byte[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vsrarn_h_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsrarn_h_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsrarn.h.w vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift (with rounding) the signed 32-bit elements in `a` by elements in `b`, truncate to 16-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (i < 4) {
    u8 shift = (b.word[i] & 31);
    if (shift == 0) {
      dst.half[i] = (s16)(s32)a.word[i];
    } else {
      dst.half[i] = (s16)(((s32)a.word[i] >> shift) +
                          (((s32)a.word[i] >> (shift - 1)) & 0x1));
    }
  } else {
    dst.half[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vsrarn_w_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsrarn_w_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsrarn.w.d vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift (with rounding) the signed 64-bit elements in `a` by elements in `b`, truncate to 32-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (i < 2) {
    u8 shift = (b.dword[i] & 63);
    if (shift == 0) {
      dst.word[i] = (s32)(s64)a.dword[i];
    } else {
      dst.word[i] = (s32)(((s64)a.dword[i] >> shift) +
                          (((s64)a.dword[i] >> (shift - 1)) & 0x1));
    }
  } else {
    dst.word[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vsrarni_b_h (\_\_m128i a, \_\_m128i b, imm0_15 imm)

### Synopsis

```
__m128i __lsx_vsrarni_b_h (__m128i a, __m128i b, imm0_15 imm)
#include <lsxintrin.h>
Instruction: vsrarni.b.h vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift (with rounding) the signed 16-bit elements in `a` and `b` by `imm`, truncate to 8-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (i < 8) {
    if (imm == 0) {
      dst.byte[i] = (s8)(s16)b.half[i];
    } else {
      dst.byte[i] =
          (s8)(((s16)b.half[i] >> imm) + (((s16)b.half[i] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.byte[i] = (s8)(s16)a.half[i - 8];
    } else {
      dst.byte[i] = (s8)(((s16)a.half[i - 8] >> imm) +
                         (((s16)a.half[i - 8] >> (imm - 1)) & 0x1));
    }
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vsrarni_h_w (\_\_m128i a, \_\_m128i b, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vsrarni_h_w (__m128i a, __m128i b, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vsrarni.h.w vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift (with rounding) the signed 32-bit elements in `a` and `b` by `imm`, truncate to 16-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (i < 4) {
    if (imm == 0) {
      dst.half[i] = (s16)(s32)b.word[i];
    } else {
      dst.half[i] = (s16)(((s32)b.word[i] >> imm) +
                          (((s32)b.word[i] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.half[i] = (s16)(s32)a.word[i - 4];
    } else {
      dst.half[i] = (s16)(((s32)a.word[i - 4] >> imm) +
                          (((s32)a.word[i - 4] >> (imm - 1)) & 0x1));
    }
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vsrarni_w_d (\_\_m128i a, \_\_m128i b, imm0_63 imm)

### Synopsis

```
__m128i __lsx_vsrarni_w_d (__m128i a, __m128i b, imm0_63 imm)
#include <lsxintrin.h>
Instruction: vsrarni.w.d vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift (with rounding) the signed 64-bit elements in `a` and `b` by `imm`, truncate to 32-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (i < 2) {
    if (imm == 0) {
      dst.word[i] = (s32)(s64)b.dword[i];
    } else {
      dst.word[i] = (s32)(((s64)b.dword[i] >> imm) +
                          (((s64)b.dword[i] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.word[i] = (s32)(s64)a.dword[i - 2];
    } else {
      dst.word[i] = (s32)(((s64)a.dword[i - 2] >> imm) +
                          (((s64)a.dword[i - 2] >> (imm - 1)) & 0x1));
    }
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vsrarni_d_q (\_\_m128i a, \_\_m128i b, imm0_127 imm)

### Synopsis

```
__m128i __lsx_vsrarni_d_q (__m128i a, __m128i b, imm0_127 imm)
#include <lsxintrin.h>
Instruction: vsrarni.d.q vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift (with rounding) the signed 128-bit elements in `a` and `b` by `imm`, truncate to 64-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (i < 1) {
    if (imm == 0) {
      dst.dword[i] = (s64)(s128)b.qword[i];
    } else {
      dst.dword[i] = (s64)(((s128)b.qword[i] >> imm) +
                           (((s128)b.qword[i] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.dword[i] = (s64)(s128)a.qword[i - 1];
    } else {
      dst.dword[i] = (s64)(((s128)a.qword[i - 1] >> imm) +
                           (((s128)a.qword[i - 1] >> (imm - 1)) & 0x1));
    }
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vsrl_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsrl_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsrl.b vr, vr, vr
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 8-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[i] >> (b.byte[i] & 0x7);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vsrl_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsrl_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsrl.h vr, vr, vr
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 16-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = a.half[i] >> (b.half[i] & 0xf);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vsrl_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsrl_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsrl.w vr, vr, vr
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 32-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = a.word[i] >> (b.word[i] & 0x1f);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vsrl_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsrl_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsrl.d vr, vr, vr
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 64-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = a.dword[i] >> (b.dword[i] & 0x3f);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vsrli_b (\_\_m128i a, imm0_7 imm)

### Synopsis

```
__m128i __lsx_vsrli_b (__m128i a, imm0_7 imm)
#include <lsxintrin.h>
Instruction: vsrli.b vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 8-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[i] >> imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vsrli_h (\_\_m128i a, imm0_15 imm)

### Synopsis

```
__m128i __lsx_vsrli_h (__m128i a, imm0_15 imm)
#include <lsxintrin.h>
Instruction: vsrli.h vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 16-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = a.half[i] >> imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vsrli_w (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vsrli_w (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vsrli.w vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 32-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = a.word[i] >> imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vsrli_d (\_\_m128i a, imm0_63 imm)

### Synopsis

```
__m128i __lsx_vsrli_d (__m128i a, imm0_63 imm)
#include <lsxintrin.h>
Instruction: vsrli.d vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 64-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = a.dword[i] >> imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vsrln_b_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsrln_b_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsrln.b.h vr, vr, vr
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 16-bit elements in `a` by elements in `b`, truncate to 8-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (i < 8) ? (u8)((u16)a.half[i] >> (b.half[i] & 15)) : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 1                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 0.5(1/2)         |
| 2K3000   | LA364E | 2       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vsrln_h_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsrln_h_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsrln.h.w vr, vr, vr
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 32-bit elements in `a` by elements in `b`, truncate to 16-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (i < 4) ? (u16)((u32)a.word[i] >> (b.word[i] & 31)) : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 1                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 0.5(1/2)         |
| 2K3000   | LA364E | 2       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vsrln_w_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsrln_w_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsrln.w.d vr, vr, vr
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 64-bit elements in `a` by elements in `b`, truncate to 32-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2) ? (u32)((u64)a.dword[i] >> (b.dword[i] & 63)) : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 1                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 0.5(1/2)         |
| 2K3000   | LA364E | 2       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vsrlni_b_h (\_\_m128i a, \_\_m128i b, imm0_15 imm)

### Synopsis

```
__m128i __lsx_vsrlni_b_h (__m128i a, __m128i b, imm0_15 imm)
#include <lsxintrin.h>
Instruction: vsrlni.b.h vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 16-bit elements in `a` and `b` by `imm`, truncate to 8-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] =
      (i < 8) ? (u8)((u16)b.half[i] >> imm) : (u8)((u16)a.half[i - 8] >> imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vsrlni_h_w (\_\_m128i a, \_\_m128i b, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vsrlni_h_w (__m128i a, __m128i b, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vsrlni.h.w vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 32-bit elements in `a` and `b` by `imm`, truncate to 16-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] =
      (i < 4) ? (u16)((u32)b.word[i] >> imm) : (u16)((u32)a.word[i - 4] >> imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vsrlni_w_d (\_\_m128i a, \_\_m128i b, imm0_63 imm)

### Synopsis

```
__m128i __lsx_vsrlni_w_d (__m128i a, __m128i b, imm0_63 imm)
#include <lsxintrin.h>
Instruction: vsrlni.w.d vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 64-bit elements in `a` and `b` by `imm`, truncate to 32-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2) ? (u32)((u64)b.dword[i] >> imm)
                        : (u32)((u64)a.dword[i - 2] >> imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vsrlni_d_q (\_\_m128i a, \_\_m128i b, imm0_127 imm)

### Synopsis

```
__m128i __lsx_vsrlni_d_q (__m128i a, __m128i b, imm0_127 imm)
#include <lsxintrin.h>
Instruction: vsrlni.d.q vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 128-bit elements in `a` and `b` by `imm`, truncate to 64-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (i < 1) ? (u64)((u128)b.qword[i] >> imm)
                         : (u64)((u128)a.qword[i - 1] >> imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vsrlr_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsrlr_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsrlr.b vr, vr, vr
CPU Flags: LSX
```

### Description

Logical right shift (with rounding) the unsigned 8-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  if ((b.byte[i] & 0x7) == 0) {
    dst.byte[i] = a.byte[i];
  } else {
    dst.byte[i] = (a.byte[i] >> (b.byte[i] & 0x7)) +
                  ((a.byte[i] >> ((b.byte[i] & 0x7) - 1)) & 0x1);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vsrlr_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsrlr_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsrlr.h vr, vr, vr
CPU Flags: LSX
```

### Description

Logical right shift (with rounding) the unsigned 16-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  if ((b.half[i] & 0xf) == 0) {
    dst.half[i] = a.half[i];
  } else {
    dst.half[i] = (a.half[i] >> (b.half[i] & 0xf)) +
                  ((a.half[i] >> ((b.half[i] & 0xf) - 1)) & 0x1);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vsrlr_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsrlr_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsrlr.w vr, vr, vr
CPU Flags: LSX
```

### Description

Logical right shift (with rounding) the unsigned 32-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  if ((b.word[i] & 0x1f) == 0) {
    dst.word[i] = a.word[i];
  } else {
    dst.word[i] = (a.word[i] >> (b.word[i] & 0x1f)) +
                  ((a.word[i] >> ((b.word[i] & 0x1f) - 1)) & 0x1);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vsrlr_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsrlr_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsrlr.d vr, vr, vr
CPU Flags: LSX
```

### Description

Logical right shift (with rounding) the unsigned 64-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  if ((b.dword[i] & 0x3f) == 0) {
    dst.dword[i] = a.dword[i];
  } else {
    dst.dword[i] = (a.dword[i] >> (b.dword[i] & 0x3f)) +
                   ((a.dword[i] >> ((b.dword[i] & 0x3f) - 1)) & 0x1);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vsrlri_b (\_\_m128i a, imm0_7 imm)

### Synopsis

```
__m128i __lsx_vsrlri_b (__m128i a, imm0_7 imm)
#include <lsxintrin.h>
Instruction: vsrlri.b vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift (with rounding) the unsigned 8-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (imm == 0) {
    dst.byte[i] = a.byte[i];
  } else {
    dst.byte[i] = (a.byte[i] >> imm) + ((a.byte[i] >> (imm - 1)) & 0x1);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vsrlri_h (\_\_m128i a, imm0_15 imm)

### Synopsis

```
__m128i __lsx_vsrlri_h (__m128i a, imm0_15 imm)
#include <lsxintrin.h>
Instruction: vsrlri.h vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift (with rounding) the unsigned 16-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (imm == 0) {
    dst.half[i] = a.half[i];
  } else {
    dst.half[i] = (a.half[i] >> imm) + ((a.half[i] >> (imm - 1)) & 0x1);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vsrlri_w (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vsrlri_w (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vsrlri.w vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift (with rounding) the unsigned 32-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (imm == 0) {
    dst.word[i] = a.word[i];
  } else {
    dst.word[i] = (a.word[i] >> imm) + ((a.word[i] >> (imm - 1)) & 0x1);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vsrlri_d (\_\_m128i a, imm0_63 imm)

### Synopsis

```
__m128i __lsx_vsrlri_d (__m128i a, imm0_63 imm)
#include <lsxintrin.h>
Instruction: vsrlri.d vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift (with rounding) the unsigned 64-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (imm == 0) {
    dst.dword[i] = a.dword[i];
  } else {
    dst.dword[i] = (a.dword[i] >> imm) + ((a.dword[i] >> (imm - 1)) & 0x1);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vsrlrn_b_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsrlrn_b_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsrlrn.b.h vr, vr, vr
CPU Flags: LSX
```

### Description

Logical right shift (with rounding) the unsigned 16-bit elements in `a` by elements in `b`, truncate to 8-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (i < 8) {
    u8 shift = (b.half[i] & 15);
    if (shift == 0) {
      dst.byte[i] = (u8)(u16)a.half[i];
    } else {
      dst.byte[i] = (u8)(((u16)a.half[i] >> shift) +
                         (((u16)a.half[i] >> (shift - 1)) & 0x1));
    }
  } else {
    dst.byte[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vsrlrn_h_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsrlrn_h_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsrlrn.h.w vr, vr, vr
CPU Flags: LSX
```

### Description

Logical right shift (with rounding) the unsigned 32-bit elements in `a` by elements in `b`, truncate to 16-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (i < 4) {
    u8 shift = (b.word[i] & 31);
    if (shift == 0) {
      dst.half[i] = (u16)(u32)a.word[i];
    } else {
      dst.half[i] = (u16)(((u32)a.word[i] >> shift) +
                          (((u32)a.word[i] >> (shift - 1)) & 0x1));
    }
  } else {
    dst.half[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vsrlrn_w_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsrlrn_w_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsrlrn.w.d vr, vr, vr
CPU Flags: LSX
```

### Description

Logical right shift (with rounding) the unsigned 64-bit elements in `a` by elements in `b`, truncate to 32-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (i < 2) {
    u8 shift = (b.dword[i] & 63);
    if (shift == 0) {
      dst.word[i] = (u32)(u64)a.dword[i];
    } else {
      dst.word[i] = (u32)(((u64)a.dword[i] >> shift) +
                          (((u64)a.dword[i] >> (shift - 1)) & 0x1));
    }
  } else {
    dst.word[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vsrlrni_b_h (\_\_m128i a, \_\_m128i b, imm0_15 imm)

### Synopsis

```
__m128i __lsx_vsrlrni_b_h (__m128i a, __m128i b, imm0_15 imm)
#include <lsxintrin.h>
Instruction: vsrlrni.b.h vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift (with rounding) the unsigned 16-bit elements in `a` and `b` by `imm`, truncate to 8-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (i < 8) {
    if (imm == 0) {
      dst.byte[i] = (u8)(u16)b.half[i];
    } else {
      dst.byte[i] =
          (u8)(((u16)b.half[i] >> imm) + (((u16)b.half[i] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.byte[i] = (u8)(u16)a.half[i - 8];
    } else {
      dst.byte[i] = (u8)(((u16)a.half[i - 8] >> imm) +
                         (((u16)a.half[i - 8] >> (imm - 1)) & 0x1));
    }
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vsrlrni_h_w (\_\_m128i a, \_\_m128i b, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vsrlrni_h_w (__m128i a, __m128i b, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vsrlrni.h.w vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift (with rounding) the unsigned 32-bit elements in `a` and `b` by `imm`, truncate to 16-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (i < 4) {
    if (imm == 0) {
      dst.half[i] = (u16)(u32)b.word[i];
    } else {
      dst.half[i] = (u16)(((u32)b.word[i] >> imm) +
                          (((u32)b.word[i] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.half[i] = (u16)(u32)a.word[i - 4];
    } else {
      dst.half[i] = (u16)(((u32)a.word[i - 4] >> imm) +
                          (((u32)a.word[i - 4] >> (imm - 1)) & 0x1));
    }
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vsrlrni_w_d (\_\_m128i a, \_\_m128i b, imm0_63 imm)

### Synopsis

```
__m128i __lsx_vsrlrni_w_d (__m128i a, __m128i b, imm0_63 imm)
#include <lsxintrin.h>
Instruction: vsrlrni.w.d vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift (with rounding) the unsigned 64-bit elements in `a` and `b` by `imm`, truncate to 32-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (i < 2) {
    if (imm == 0) {
      dst.word[i] = (u32)(u64)b.dword[i];
    } else {
      dst.word[i] = (u32)(((u64)b.dword[i] >> imm) +
                          (((u64)b.dword[i] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.word[i] = (u32)(u64)a.dword[i - 2];
    } else {
      dst.word[i] = (u32)(((u64)a.dword[i - 2] >> imm) +
                          (((u64)a.dword[i - 2] >> (imm - 1)) & 0x1));
    }
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vsrlrni_d_q (\_\_m128i a, \_\_m128i b, imm0_127 imm)

### Synopsis

```
__m128i __lsx_vsrlrni_d_q (__m128i a, __m128i b, imm0_127 imm)
#include <lsxintrin.h>
Instruction: vsrlrni.d.q vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift (with rounding) the unsigned 128-bit elements in `a` and `b` by `imm`, truncate to 64-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (i < 1) {
    if (imm == 0) {
      dst.dword[i] = (u64)(u128)b.qword[i];
    } else {
      dst.dword[i] = (u64)(((u128)b.qword[i] >> imm) +
                           (((u128)b.qword[i] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.dword[i] = (u64)(u128)a.qword[i - 1];
    } else {
      dst.dword[i] = (u64)(((u128)a.qword[i - 1] >> imm) +
                           (((u128)a.qword[i - 1] >> (imm - 1)) & 0x1));
    }
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vssran_b_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssran_b_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssran.b.h vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 16-bit elements in `a` by elements in `b`, clamp to fit in signed 8-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (i < 8) {
    s16 temp = (s16)a.half[i] >> (b.half[i] & 15);
    dst.byte[i] = clamp<s16>(temp, -128, 127);
  } else {
    dst.byte[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssran_bu_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssran_bu_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssran.bu.h vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 16-bit elements in `a` by elements in `b`, clamp to fit in unsigned 8-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (i < 8) {
    s16 temp = (s16)a.half[i] >> (b.half[i] & 15);
    dst.byte[i] = clamp<s16>(temp, 0, 255);
  } else {
    dst.byte[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssran_h_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssran_h_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssran.h.w vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 32-bit elements in `a` by elements in `b`, clamp to fit in signed 16-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (i < 4) {
    s32 temp = (s32)a.word[i] >> (b.word[i] & 31);
    dst.half[i] = clamp<s32>(temp, -32768, 32767);
  } else {
    dst.half[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssran_hu_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssran_hu_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssran.hu.w vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 32-bit elements in `a` by elements in `b`, clamp to fit in unsigned 16-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (i < 4) {
    s32 temp = (s32)a.word[i] >> (b.word[i] & 31);
    dst.half[i] = clamp<s32>(temp, 0, 65535);
  } else {
    dst.half[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssran_w_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssran_w_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssran.w.d vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 64-bit elements in `a` by elements in `b`, clamp to fit in signed 32-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (i < 2) {
    s64 temp = (s64)a.dword[i] >> (b.dword[i] & 63);
    dst.word[i] = clamp<s64>(temp, -2147483648, 2147483647);
  } else {
    dst.word[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssran_wu_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssran_wu_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssran.wu.d vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 64-bit elements in `a` by elements in `b`, clamp to fit in unsigned 32-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (i < 2) {
    s64 temp = (s64)a.dword[i] >> (b.dword[i] & 63);
    dst.word[i] = clamp<s64>(temp, 0, 4294967295);
  } else {
    dst.word[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrani_b_h (\_\_m128i a, \_\_m128i b, imm0_15 imm)

### Synopsis

```
__m128i __lsx_vssrani_b_h (__m128i a, __m128i b, imm0_15 imm)
#include <lsxintrin.h>
Instruction: vssrani.b.h vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 16-bit elements in `a` and `b` by `imm`, clamp to fit in signed 8-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (i < 8) {
    s16 temp = (s16)b.half[i] >> imm;
    dst.byte[i] = clamp<s16>(temp, -128, 127);
  } else {
    s16 temp = (s16)a.half[i - 8] >> imm;
    dst.byte[i] = clamp<s16>(temp, -128, 127);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrani_bu_h (\_\_m128i a, \_\_m128i b, imm0_15 imm)

### Synopsis

```
__m128i __lsx_vssrani_bu_h (__m128i a, __m128i b, imm0_15 imm)
#include <lsxintrin.h>
Instruction: vssrani.bu.h vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 16-bit elements in `a` and `b` by `imm`, clamp to fit in unsigned 8-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (i < 8) {
    s16 temp = (s16)b.half[i] >> imm;
    dst.byte[i] = clamp<s16>(temp, 0, 255);
  } else {
    s16 temp = (s16)a.half[i - 8] >> imm;
    dst.byte[i] = clamp<s16>(temp, 0, 255);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrani_h_w (\_\_m128i a, \_\_m128i b, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vssrani_h_w (__m128i a, __m128i b, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vssrani.h.w vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 32-bit elements in `a` and `b` by `imm`, clamp to fit in signed 16-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (i < 4) {
    s32 temp = (s32)b.word[i] >> imm;
    dst.half[i] = clamp<s32>(temp, -32768, 32767);
  } else {
    s32 temp = (s32)a.word[i - 4] >> imm;
    dst.half[i] = clamp<s32>(temp, -32768, 32767);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrani_hu_w (\_\_m128i a, \_\_m128i b, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vssrani_hu_w (__m128i a, __m128i b, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vssrani.hu.w vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 32-bit elements in `a` and `b` by `imm`, clamp to fit in unsigned 16-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (i < 4) {
    s32 temp = (s32)b.word[i] >> imm;
    dst.half[i] = clamp<s32>(temp, 0, 65535);
  } else {
    s32 temp = (s32)a.word[i - 4] >> imm;
    dst.half[i] = clamp<s32>(temp, 0, 65535);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrani_w_d (\_\_m128i a, \_\_m128i b, imm0_63 imm)

### Synopsis

```
__m128i __lsx_vssrani_w_d (__m128i a, __m128i b, imm0_63 imm)
#include <lsxintrin.h>
Instruction: vssrani.w.d vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 64-bit elements in `a` and `b` by `imm`, clamp to fit in signed 32-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (i < 2) {
    s64 temp = (s64)b.dword[i] >> imm;
    dst.word[i] = clamp<s64>(temp, -2147483648, 2147483647);
  } else {
    s64 temp = (s64)a.dword[i - 2] >> imm;
    dst.word[i] = clamp<s64>(temp, -2147483648, 2147483647);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrani_wu_d (\_\_m128i a, \_\_m128i b, imm0_63 imm)

### Synopsis

```
__m128i __lsx_vssrani_wu_d (__m128i a, __m128i b, imm0_63 imm)
#include <lsxintrin.h>
Instruction: vssrani.wu.d vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 64-bit elements in `a` and `b` by `imm`, clamp to fit in unsigned 32-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (i < 2) {
    s64 temp = (s64)b.dword[i] >> imm;
    dst.word[i] = clamp<s64>(temp, 0, 4294967295);
  } else {
    s64 temp = (s64)a.dword[i - 2] >> imm;
    dst.word[i] = clamp<s64>(temp, 0, 4294967295);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrani_d_q (\_\_m128i a, \_\_m128i b, imm0_127 imm)

### Synopsis

```
__m128i __lsx_vssrani_d_q (__m128i a, __m128i b, imm0_127 imm)
#include <lsxintrin.h>
Instruction: vssrani.d.q vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 128-bit elements in `a` and `b` by `imm`, clamp to fit in signed 64-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (i < 1) {
    s128 temp = (s128)b.qword[i] >> imm;
    dst.dword[i] = clamp<s128>(temp, -9223372036854775808, 9223372036854775807);
  } else {
    s128 temp = (s128)a.qword[i - 1] >> imm;
    dst.dword[i] = clamp<s128>(temp, -9223372036854775808, 9223372036854775807);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vssrani_du_q (\_\_m128i a, \_\_m128i b, imm0_127 imm)

### Synopsis

```
__m128i __lsx_vssrani_du_q (__m128i a, __m128i b, imm0_127 imm)
#include <lsxintrin.h>
Instruction: vssrani.du.q vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift the signed 128-bit elements in `a` and `b` by `imm`, clamp to fit in unsigned 64-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (i < 1) {
    s128 temp = (s128)b.qword[i] >> imm;
    dst.dword[i] = clamp<s128>(temp, 0, 18446744073709551615);
  } else {
    s128 temp = (s128)a.qword[i - 1] >> imm;
    dst.dword[i] = clamp<s128>(temp, 0, 18446744073709551615);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vssrarn_b_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssrarn_b_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssrarn.b.h vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift (with rounding) the signed 16-bit elements in `a` by elements in `b`, clamp to fit in signed 8-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (i < 8) {
    s16 temp;
    if ((b.half[i] & 15) == 0) {
      temp = (s16)a.half[i];
    } else {
      temp = ((s16)a.half[i] >> (b.half[i] & 15)) +
             (((s16)a.half[i] >> ((b.half[i] & 15) - 1)) & 1);
    }
    dst.byte[i] = clamp<s16>(temp, -128, 127);
  } else {
    dst.byte[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrarn_bu_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssrarn_bu_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssrarn.bu.h vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift (with rounding) the signed 16-bit elements in `a` by elements in `b`, clamp to fit in unsigned 8-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (i < 8) {
    s16 temp;
    if ((b.half[i] & 15) == 0) {
      temp = (s16)a.half[i];
    } else {
      temp = ((s16)a.half[i] >> (b.half[i] & 15)) +
             (((s16)a.half[i] >> ((b.half[i] & 15) - 1)) & 1);
    }
    dst.byte[i] = clamp<s16>(temp, 0, 255);
  } else {
    dst.byte[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrarn_h_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssrarn_h_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssrarn.h.w vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift (with rounding) the signed 32-bit elements in `a` by elements in `b`, clamp to fit in signed 16-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (i < 4) {
    s32 temp;
    if ((b.word[i] & 31) == 0) {
      temp = (s32)a.word[i];
    } else {
      temp = ((s32)a.word[i] >> (b.word[i] & 31)) +
             (((s32)a.word[i] >> ((b.word[i] & 31) - 1)) & 1);
    }
    dst.half[i] = clamp<s32>(temp, -32768, 32767);
  } else {
    dst.half[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrarn_hu_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssrarn_hu_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssrarn.hu.w vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift (with rounding) the signed 32-bit elements in `a` by elements in `b`, clamp to fit in unsigned 16-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (i < 4) {
    s32 temp;
    if ((b.word[i] & 31) == 0) {
      temp = (s32)a.word[i];
    } else {
      temp = ((s32)a.word[i] >> (b.word[i] & 31)) +
             (((s32)a.word[i] >> ((b.word[i] & 31) - 1)) & 1);
    }
    dst.half[i] = clamp<s32>(temp, 0, 65535);
  } else {
    dst.half[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrarn_w_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssrarn_w_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssrarn.w.d vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift (with rounding) the signed 64-bit elements in `a` by elements in `b`, clamp to fit in signed 32-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (i < 2) {
    s64 temp;
    if ((b.dword[i] & 63) == 0) {
      temp = (s64)a.dword[i];
    } else {
      temp = ((s64)a.dword[i] >> (b.dword[i] & 63)) +
             (((s64)a.dword[i] >> ((b.dword[i] & 63) - 1)) & 1);
    }
    dst.word[i] = clamp<s64>(temp, -2147483648, 2147483647);
  } else {
    dst.word[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrarn_wu_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssrarn_wu_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssrarn.wu.d vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift (with rounding) the signed 64-bit elements in `a` by elements in `b`, clamp to fit in unsigned 32-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (i < 2) {
    s64 temp;
    if ((b.dword[i] & 63) == 0) {
      temp = (s64)a.dword[i];
    } else {
      temp = ((s64)a.dword[i] >> (b.dword[i] & 63)) +
             (((s64)a.dword[i] >> ((b.dword[i] & 63) - 1)) & 1);
    }
    dst.word[i] = clamp<s64>(temp, 0, 4294967295);
  } else {
    dst.word[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrarni_b_h (\_\_m128i a, \_\_m128i b, imm0_15 imm)

### Synopsis

```
__m128i __lsx_vssrarni_b_h (__m128i a, __m128i b, imm0_15 imm)
#include <lsxintrin.h>
Instruction: vssrarni.b.h vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift (with rounding) the signed 16-bit elements in `a` and `b` by `imm`, clamp to fit in signed 8-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (i < 8) {
    s16 temp;
    if (imm == 0) {
      temp = (s16)b.half[i];
    } else {
      temp = ((s16)b.half[i] >> imm) + (((s16)b.half[i] >> (imm - 1)) & 1);
    }
    dst.byte[i] = clamp<s16>(temp, -128, 127);
  } else {
    s16 temp;
    if (imm == 0) {
      temp = (s16)a.half[i - 8];
    } else {
      temp =
          ((s16)a.half[i - 8] >> imm) + (((s16)a.half[i - 8] >> (imm - 1)) & 1);
    }
    dst.byte[i] = clamp<s16>(temp, -128, 127);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrarni_bu_h (\_\_m128i a, \_\_m128i b, imm0_15 imm)

### Synopsis

```
__m128i __lsx_vssrarni_bu_h (__m128i a, __m128i b, imm0_15 imm)
#include <lsxintrin.h>
Instruction: vssrarni.bu.h vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift (with rounding) the signed 16-bit elements in `a` and `b` by `imm`, clamp to fit in unsigned 8-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (i < 8) {
    s16 temp;
    if (imm == 0) {
      temp = (s16)b.half[i];
    } else {
      temp = ((s16)b.half[i] >> imm) + (((s16)b.half[i] >> (imm - 1)) & 1);
    }
    dst.byte[i] = clamp<s16>(temp, 0, 255);
  } else {
    s16 temp;
    if (imm == 0) {
      temp = (s16)a.half[i - 8];
    } else {
      temp =
          ((s16)a.half[i - 8] >> imm) + (((s16)a.half[i - 8] >> (imm - 1)) & 1);
    }
    dst.byte[i] = clamp<s16>(temp, 0, 255);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrarni_h_w (\_\_m128i a, \_\_m128i b, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vssrarni_h_w (__m128i a, __m128i b, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vssrarni.h.w vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift (with rounding) the signed 32-bit elements in `a` and `b` by `imm`, clamp to fit in signed 16-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (i < 4) {
    s32 temp;
    if (imm == 0) {
      temp = (s32)b.word[i];
    } else {
      temp = ((s32)b.word[i] >> imm) + (((s32)b.word[i] >> (imm - 1)) & 1);
    }
    dst.half[i] = clamp<s32>(temp, -32768, 32767);
  } else {
    s32 temp;
    if (imm == 0) {
      temp = (s32)a.word[i - 4];
    } else {
      temp =
          ((s32)a.word[i - 4] >> imm) + (((s32)a.word[i - 4] >> (imm - 1)) & 1);
    }
    dst.half[i] = clamp<s32>(temp, -32768, 32767);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrarni_hu_w (\_\_m128i a, \_\_m128i b, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vssrarni_hu_w (__m128i a, __m128i b, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vssrarni.hu.w vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift (with rounding) the signed 32-bit elements in `a` and `b` by `imm`, clamp to fit in unsigned 16-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (i < 4) {
    s32 temp;
    if (imm == 0) {
      temp = (s32)b.word[i];
    } else {
      temp = ((s32)b.word[i] >> imm) + (((s32)b.word[i] >> (imm - 1)) & 1);
    }
    dst.half[i] = clamp<s32>(temp, 0, 65535);
  } else {
    s32 temp;
    if (imm == 0) {
      temp = (s32)a.word[i - 4];
    } else {
      temp =
          ((s32)a.word[i - 4] >> imm) + (((s32)a.word[i - 4] >> (imm - 1)) & 1);
    }
    dst.half[i] = clamp<s32>(temp, 0, 65535);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrarni_w_d (\_\_m128i a, \_\_m128i b, imm0_63 imm)

### Synopsis

```
__m128i __lsx_vssrarni_w_d (__m128i a, __m128i b, imm0_63 imm)
#include <lsxintrin.h>
Instruction: vssrarni.w.d vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift (with rounding) the signed 64-bit elements in `a` and `b` by `imm`, clamp to fit in signed 32-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (i < 2) {
    s64 temp;
    if (imm == 0) {
      temp = (s64)b.dword[i];
    } else {
      temp = ((s64)b.dword[i] >> imm) + (((s64)b.dword[i] >> (imm - 1)) & 1);
    }
    dst.word[i] = clamp<s64>(temp, -2147483648, 2147483647);
  } else {
    s64 temp;
    if (imm == 0) {
      temp = (s64)a.dword[i - 2];
    } else {
      temp = ((s64)a.dword[i - 2] >> imm) +
             (((s64)a.dword[i - 2] >> (imm - 1)) & 1);
    }
    dst.word[i] = clamp<s64>(temp, -2147483648, 2147483647);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrarni_wu_d (\_\_m128i a, \_\_m128i b, imm0_63 imm)

### Synopsis

```
__m128i __lsx_vssrarni_wu_d (__m128i a, __m128i b, imm0_63 imm)
#include <lsxintrin.h>
Instruction: vssrarni.wu.d vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift (with rounding) the signed 64-bit elements in `a` and `b` by `imm`, clamp to fit in unsigned 32-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (i < 2) {
    s64 temp;
    if (imm == 0) {
      temp = (s64)b.dword[i];
    } else {
      temp = ((s64)b.dword[i] >> imm) + (((s64)b.dword[i] >> (imm - 1)) & 1);
    }
    dst.word[i] = clamp<s64>(temp, 0, 4294967295);
  } else {
    s64 temp;
    if (imm == 0) {
      temp = (s64)a.dword[i - 2];
    } else {
      temp = ((s64)a.dword[i - 2] >> imm) +
             (((s64)a.dword[i - 2] >> (imm - 1)) & 1);
    }
    dst.word[i] = clamp<s64>(temp, 0, 4294967295);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrarni_d_q (\_\_m128i a, \_\_m128i b, imm0_127 imm)

### Synopsis

```
__m128i __lsx_vssrarni_d_q (__m128i a, __m128i b, imm0_127 imm)
#include <lsxintrin.h>
Instruction: vssrarni.d.q vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift (with rounding) the signed 128-bit elements in `a` and `b` by `imm`, clamp to fit in signed 64-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (i < 1) {
    s128 temp;
    if (imm == 0) {
      temp = (s128)b.qword[i];
    } else {
      temp = ((s128)b.qword[i] >> imm) + (((s128)b.qword[i] >> (imm - 1)) & 1);
    }
    dst.dword[i] = clamp<s128>(temp, -9223372036854775808, 9223372036854775807);
  } else {
    s128 temp;
    if (imm == 0) {
      temp = (s128)a.qword[i - 1];
    } else {
      temp = ((s128)a.qword[i - 1] >> imm) +
             (((s128)a.qword[i - 1] >> (imm - 1)) & 1);
    }
    dst.dword[i] = clamp<s128>(temp, -9223372036854775808, 9223372036854775807);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vssrarni_du_q (\_\_m128i a, \_\_m128i b, imm0_127 imm)

### Synopsis

```
__m128i __lsx_vssrarni_du_q (__m128i a, __m128i b, imm0_127 imm)
#include <lsxintrin.h>
Instruction: vssrarni.du.q vr, vr, imm
CPU Flags: LSX
```

### Description

Arithmetic right shift (with rounding) the signed 128-bit elements in `a` and `b` by `imm`, clamp to fit in unsigned 64-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (i < 1) {
    s128 temp;
    if (imm == 0) {
      temp = (s128)b.qword[i];
    } else {
      temp = ((s128)b.qword[i] >> imm) + (((s128)b.qword[i] >> (imm - 1)) & 1);
    }
    dst.dword[i] = clamp<s128>(temp, 0, 18446744073709551615);
  } else {
    s128 temp;
    if (imm == 0) {
      temp = (s128)a.qword[i - 1];
    } else {
      temp = ((s128)a.qword[i - 1] >> imm) +
             (((s128)a.qword[i - 1] >> (imm - 1)) & 1);
    }
    dst.dword[i] = clamp<s128>(temp, 0, 18446744073709551615);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vssrln_b_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssrln_b_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssrln.b.h vr, vr, vr
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 16-bit elements in `a` by elements in `b`, clamp to fit in signed 8-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (i < 8) {
    u16 temp = (u16)a.half[i] >> (b.half[i] & 15);
    dst.byte[i] = clamp<u16>(temp, 0, 127);
  } else {
    dst.byte[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrln_bu_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssrln_bu_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssrln.bu.h vr, vr, vr
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 16-bit elements in `a` by elements in `b`, clamp to fit in unsigned 8-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (i < 8) {
    u16 temp = (u16)a.half[i] >> (b.half[i] & 15);
    dst.byte[i] = clamp<u16>(temp, 0, 255);
  } else {
    dst.byte[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrln_h_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssrln_h_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssrln.h.w vr, vr, vr
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 32-bit elements in `a` by elements in `b`, clamp to fit in signed 16-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (i < 4) {
    u32 temp = (u32)a.word[i] >> (b.word[i] & 31);
    dst.half[i] = clamp<u32>(temp, 0, 32767);
  } else {
    dst.half[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrln_hu_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssrln_hu_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssrln.hu.w vr, vr, vr
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 32-bit elements in `a` by elements in `b`, clamp to fit in unsigned 16-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (i < 4) {
    u32 temp = (u32)a.word[i] >> (b.word[i] & 31);
    dst.half[i] = clamp<u32>(temp, 0, 65535);
  } else {
    dst.half[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrln_w_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssrln_w_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssrln.w.d vr, vr, vr
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 64-bit elements in `a` by elements in `b`, clamp to fit in signed 32-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (i < 2) {
    u64 temp = (u64)a.dword[i] >> (b.dword[i] & 63);
    dst.word[i] = clamp<u64>(temp, 0, 2147483647);
  } else {
    dst.word[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrln_wu_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssrln_wu_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssrln.wu.d vr, vr, vr
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 64-bit elements in `a` by elements in `b`, clamp to fit in unsigned 32-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (i < 2) {
    u64 temp = (u64)a.dword[i] >> (b.dword[i] & 63);
    dst.word[i] = clamp<u64>(temp, 0, 4294967295);
  } else {
    dst.word[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrlni_b_h (\_\_m128i a, \_\_m128i b, imm0_15 imm)

### Synopsis

```
__m128i __lsx_vssrlni_b_h (__m128i a, __m128i b, imm0_15 imm)
#include <lsxintrin.h>
Instruction: vssrlni.b.h vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 16-bit elements in `a` and `b` by `imm`, clamp to fit in signed 8-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (i < 8) {
    u16 temp = (u16)b.half[i] >> imm;
    dst.byte[i] = clamp<u16>(temp, 0, 127);
  } else {
    u16 temp = (u16)a.half[i - 8] >> imm;
    dst.byte[i] = clamp<u16>(temp, 0, 127);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrlni_bu_h (\_\_m128i a, \_\_m128i b, imm0_15 imm)

### Synopsis

```
__m128i __lsx_vssrlni_bu_h (__m128i a, __m128i b, imm0_15 imm)
#include <lsxintrin.h>
Instruction: vssrlni.bu.h vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 16-bit elements in `a` and `b` by `imm`, clamp to fit in unsigned 8-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (i < 8) {
    u16 temp = (u16)b.half[i] >> imm;
    dst.byte[i] = clamp<u16>(temp, 0, 255);
  } else {
    u16 temp = (u16)a.half[i - 8] >> imm;
    dst.byte[i] = clamp<u16>(temp, 0, 255);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrlni_h_w (\_\_m128i a, \_\_m128i b, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vssrlni_h_w (__m128i a, __m128i b, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vssrlni.h.w vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 32-bit elements in `a` and `b` by `imm`, clamp to fit in signed 16-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (i < 4) {
    u32 temp = (u32)b.word[i] >> imm;
    dst.half[i] = clamp<u32>(temp, 0, 32767);
  } else {
    u32 temp = (u32)a.word[i - 4] >> imm;
    dst.half[i] = clamp<u32>(temp, 0, 32767);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrlni_hu_w (\_\_m128i a, \_\_m128i b, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vssrlni_hu_w (__m128i a, __m128i b, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vssrlni.hu.w vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 32-bit elements in `a` and `b` by `imm`, clamp to fit in unsigned 16-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (i < 4) {
    u32 temp = (u32)b.word[i] >> imm;
    dst.half[i] = clamp<u32>(temp, 0, 65535);
  } else {
    u32 temp = (u32)a.word[i - 4] >> imm;
    dst.half[i] = clamp<u32>(temp, 0, 65535);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrlni_w_d (\_\_m128i a, \_\_m128i b, imm0_63 imm)

### Synopsis

```
__m128i __lsx_vssrlni_w_d (__m128i a, __m128i b, imm0_63 imm)
#include <lsxintrin.h>
Instruction: vssrlni.w.d vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 64-bit elements in `a` and `b` by `imm`, clamp to fit in signed 32-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (i < 2) {
    u64 temp = (u64)b.dword[i] >> imm;
    dst.word[i] = clamp<u64>(temp, 0, 2147483647);
  } else {
    u64 temp = (u64)a.dword[i - 2] >> imm;
    dst.word[i] = clamp<u64>(temp, 0, 2147483647);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrlni_wu_d (\_\_m128i a, \_\_m128i b, imm0_63 imm)

### Synopsis

```
__m128i __lsx_vssrlni_wu_d (__m128i a, __m128i b, imm0_63 imm)
#include <lsxintrin.h>
Instruction: vssrlni.wu.d vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 64-bit elements in `a` and `b` by `imm`, clamp to fit in unsigned 32-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (i < 2) {
    u64 temp = (u64)b.dword[i] >> imm;
    dst.word[i] = clamp<u64>(temp, 0, 4294967295);
  } else {
    u64 temp = (u64)a.dword[i - 2] >> imm;
    dst.word[i] = clamp<u64>(temp, 0, 4294967295);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrlni_d_q (\_\_m128i a, \_\_m128i b, imm0_127 imm)

### Synopsis

```
__m128i __lsx_vssrlni_d_q (__m128i a, __m128i b, imm0_127 imm)
#include <lsxintrin.h>
Instruction: vssrlni.d.q vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 128-bit elements in `a` and `b` by `imm`, clamp to fit in signed 64-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (i < 1) {
    u128 temp = (u128)b.qword[i] >> imm;
    dst.dword[i] = clamp<u128>(temp, 0, 9223372036854775807);
  } else {
    u128 temp = (u128)a.qword[i - 1] >> imm;
    dst.dword[i] = clamp<u128>(temp, 0, 9223372036854775807);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vssrlni_du_q (\_\_m128i a, \_\_m128i b, imm0_127 imm)

### Synopsis

```
__m128i __lsx_vssrlni_du_q (__m128i a, __m128i b, imm0_127 imm)
#include <lsxintrin.h>
Instruction: vssrlni.du.q vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift the unsigned 128-bit elements in `a` and `b` by `imm`, clamp to fit in unsigned 64-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (i < 1) {
    u128 temp = (u128)b.qword[i] >> imm;
    dst.dword[i] = clamp<u128>(temp, 0, 18446744073709551615);
  } else {
    u128 temp = (u128)a.qword[i - 1] >> imm;
    dst.dword[i] = clamp<u128>(temp, 0, 18446744073709551615);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vssrlrn_b_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssrlrn_b_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssrlrn.b.h vr, vr, vr
CPU Flags: LSX
```

### Description

Logical right shift (with rounding) the unsigned 16-bit elements in `a` by elements in `b`, clamp to fit in signed 8-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (i < 8) {
    u16 temp;
    if ((b.half[i] & 15) == 0) {
      temp = (u16)a.half[i];
    } else {
      temp = ((u16)a.half[i] >> (b.half[i] & 15)) +
             (((u16)a.half[i] >> ((b.half[i] & 15) - 1)) & 1);
    }
    dst.byte[i] = clamp<u16>(temp, 0, 127);
  } else {
    dst.byte[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrlrn_bu_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssrlrn_bu_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssrlrn.bu.h vr, vr, vr
CPU Flags: LSX
```

### Description

Logical right shift (with rounding) the unsigned 16-bit elements in `a` by elements in `b`, clamp to fit in unsigned 8-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (i < 8) {
    u16 temp;
    if ((b.half[i] & 15) == 0) {
      temp = (u16)a.half[i];
    } else {
      temp = ((u16)a.half[i] >> (b.half[i] & 15)) +
             (((u16)a.half[i] >> ((b.half[i] & 15) - 1)) & 1);
    }
    dst.byte[i] = clamp<u16>(temp, 0, 255);
  } else {
    dst.byte[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrlrn_h_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssrlrn_h_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssrlrn.h.w vr, vr, vr
CPU Flags: LSX
```

### Description

Logical right shift (with rounding) the unsigned 32-bit elements in `a` by elements in `b`, clamp to fit in signed 16-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (i < 4) {
    u32 temp;
    if ((b.word[i] & 31) == 0) {
      temp = (u32)a.word[i];
    } else {
      temp = ((u32)a.word[i] >> (b.word[i] & 31)) +
             (((u32)a.word[i] >> ((b.word[i] & 31) - 1)) & 1);
    }
    dst.half[i] = clamp<u32>(temp, 0, 32767);
  } else {
    dst.half[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrlrn_hu_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssrlrn_hu_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssrlrn.hu.w vr, vr, vr
CPU Flags: LSX
```

### Description

Logical right shift (with rounding) the unsigned 32-bit elements in `a` by elements in `b`, clamp to fit in unsigned 16-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (i < 4) {
    u32 temp;
    if ((b.word[i] & 31) == 0) {
      temp = (u32)a.word[i];
    } else {
      temp = ((u32)a.word[i] >> (b.word[i] & 31)) +
             (((u32)a.word[i] >> ((b.word[i] & 31) - 1)) & 1);
    }
    dst.half[i] = clamp<u32>(temp, 0, 65535);
  } else {
    dst.half[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrlrn_w_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssrlrn_w_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssrlrn.w.d vr, vr, vr
CPU Flags: LSX
```

### Description

Logical right shift (with rounding) the unsigned 64-bit elements in `a` by elements in `b`, clamp to fit in signed 32-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (i < 2) {
    u64 temp;
    if ((b.dword[i] & 63) == 0) {
      temp = (u64)a.dword[i];
    } else {
      temp = ((u64)a.dword[i] >> (b.dword[i] & 63)) +
             (((u64)a.dword[i] >> ((b.dword[i] & 63) - 1)) & 1);
    }
    dst.word[i] = clamp<u64>(temp, 0, 2147483647);
  } else {
    dst.word[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrlrn_wu_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssrlrn_wu_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssrlrn.wu.d vr, vr, vr
CPU Flags: LSX
```

### Description

Logical right shift (with rounding) the unsigned 64-bit elements in `a` by elements in `b`, clamp to fit in unsigned 32-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (i < 2) {
    u64 temp;
    if ((b.dword[i] & 63) == 0) {
      temp = (u64)a.dword[i];
    } else {
      temp = ((u64)a.dword[i] >> (b.dword[i] & 63)) +
             (((u64)a.dword[i] >> ((b.dword[i] & 63) - 1)) & 1);
    }
    dst.word[i] = clamp<u64>(temp, 0, 4294967295);
  } else {
    dst.word[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrlrni_b_h (\_\_m128i a, \_\_m128i b, imm0_15 imm)

### Synopsis

```
__m128i __lsx_vssrlrni_b_h (__m128i a, __m128i b, imm0_15 imm)
#include <lsxintrin.h>
Instruction: vssrlrni.b.h vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift (with rounding) the unsigned 16-bit elements in `a` and `b` by `imm`, clamp to fit in signed 8-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (i < 8) {
    u16 temp;
    if (imm == 0) {
      temp = (u16)b.half[i];
    } else {
      temp = ((u16)b.half[i] >> imm) + (((u16)b.half[i] >> (imm - 1)) & 1);
    }
    dst.byte[i] = clamp<u16>(temp, 0, 127);
  } else {
    u16 temp;
    if (imm == 0) {
      temp = (u16)a.half[i - 8];
    } else {
      temp =
          ((u16)a.half[i - 8] >> imm) + (((u16)a.half[i - 8] >> (imm - 1)) & 1);
    }
    dst.byte[i] = clamp<u16>(temp, 0, 127);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrlrni_bu_h (\_\_m128i a, \_\_m128i b, imm0_15 imm)

### Synopsis

```
__m128i __lsx_vssrlrni_bu_h (__m128i a, __m128i b, imm0_15 imm)
#include <lsxintrin.h>
Instruction: vssrlrni.bu.h vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift (with rounding) the unsigned 16-bit elements in `a` and `b` by `imm`, clamp to fit in unsigned 8-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (i < 8) {
    u16 temp;
    if (imm == 0) {
      temp = (u16)b.half[i];
    } else {
      temp = ((u16)b.half[i] >> imm) + (((u16)b.half[i] >> (imm - 1)) & 1);
    }
    dst.byte[i] = clamp<u16>(temp, 0, 255);
  } else {
    u16 temp;
    if (imm == 0) {
      temp = (u16)a.half[i - 8];
    } else {
      temp =
          ((u16)a.half[i - 8] >> imm) + (((u16)a.half[i - 8] >> (imm - 1)) & 1);
    }
    dst.byte[i] = clamp<u16>(temp, 0, 255);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrlrni_h_w (\_\_m128i a, \_\_m128i b, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vssrlrni_h_w (__m128i a, __m128i b, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vssrlrni.h.w vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift (with rounding) the unsigned 32-bit elements in `a` and `b` by `imm`, clamp to fit in signed 16-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (i < 4) {
    u32 temp;
    if (imm == 0) {
      temp = (u32)b.word[i];
    } else {
      temp = ((u32)b.word[i] >> imm) + (((u32)b.word[i] >> (imm - 1)) & 1);
    }
    dst.half[i] = clamp<u32>(temp, 0, 32767);
  } else {
    u32 temp;
    if (imm == 0) {
      temp = (u32)a.word[i - 4];
    } else {
      temp =
          ((u32)a.word[i - 4] >> imm) + (((u32)a.word[i - 4] >> (imm - 1)) & 1);
    }
    dst.half[i] = clamp<u32>(temp, 0, 32767);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrlrni_hu_w (\_\_m128i a, \_\_m128i b, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vssrlrni_hu_w (__m128i a, __m128i b, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vssrlrni.hu.w vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift (with rounding) the unsigned 32-bit elements in `a` and `b` by `imm`, clamp to fit in unsigned 16-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (i < 4) {
    u32 temp;
    if (imm == 0) {
      temp = (u32)b.word[i];
    } else {
      temp = ((u32)b.word[i] >> imm) + (((u32)b.word[i] >> (imm - 1)) & 1);
    }
    dst.half[i] = clamp<u32>(temp, 0, 65535);
  } else {
    u32 temp;
    if (imm == 0) {
      temp = (u32)a.word[i - 4];
    } else {
      temp =
          ((u32)a.word[i - 4] >> imm) + (((u32)a.word[i - 4] >> (imm - 1)) & 1);
    }
    dst.half[i] = clamp<u32>(temp, 0, 65535);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrlrni_w_d (\_\_m128i a, \_\_m128i b, imm0_63 imm)

### Synopsis

```
__m128i __lsx_vssrlrni_w_d (__m128i a, __m128i b, imm0_63 imm)
#include <lsxintrin.h>
Instruction: vssrlrni.w.d vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift (with rounding) the unsigned 64-bit elements in `a` and `b` by `imm`, clamp to fit in signed 32-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (i < 2) {
    u64 temp;
    if (imm == 0) {
      temp = (u64)b.dword[i];
    } else {
      temp = ((u64)b.dword[i] >> imm) + (((u64)b.dword[i] >> (imm - 1)) & 1);
    }
    dst.word[i] = clamp<u64>(temp, 0, 2147483647);
  } else {
    u64 temp;
    if (imm == 0) {
      temp = (u64)a.dword[i - 2];
    } else {
      temp = ((u64)a.dword[i - 2] >> imm) +
             (((u64)a.dword[i - 2] >> (imm - 1)) & 1);
    }
    dst.word[i] = clamp<u64>(temp, 0, 2147483647);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrlrni_wu_d (\_\_m128i a, \_\_m128i b, imm0_63 imm)

### Synopsis

```
__m128i __lsx_vssrlrni_wu_d (__m128i a, __m128i b, imm0_63 imm)
#include <lsxintrin.h>
Instruction: vssrlrni.wu.d vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift (with rounding) the unsigned 64-bit elements in `a` and `b` by `imm`, clamp to fit in unsigned 32-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (i < 2) {
    u64 temp;
    if (imm == 0) {
      temp = (u64)b.dword[i];
    } else {
      temp = ((u64)b.dword[i] >> imm) + (((u64)b.dword[i] >> (imm - 1)) & 1);
    }
    dst.word[i] = clamp<u64>(temp, 0, 4294967295);
  } else {
    u64 temp;
    if (imm == 0) {
      temp = (u64)a.dword[i - 2];
    } else {
      temp = ((u64)a.dword[i - 2] >> imm) +
             (((u64)a.dword[i - 2] >> (imm - 1)) & 1);
    }
    dst.word[i] = clamp<u64>(temp, 0, 4294967295);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 1                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vssrlrni_d_q (\_\_m128i a, \_\_m128i b, imm0_127 imm)

### Synopsis

```
__m128i __lsx_vssrlrni_d_q (__m128i a, __m128i b, imm0_127 imm)
#include <lsxintrin.h>
Instruction: vssrlrni.d.q vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift (with rounding) the unsigned 128-bit elements in `a` and `b` by `imm`, clamp to fit in signed 64-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (i < 1) {
    u128 temp;
    if (imm == 0) {
      temp = (u128)b.qword[i];
    } else {
      temp = ((u128)b.qword[i] >> imm) + (((u128)b.qword[i] >> (imm - 1)) & 1);
    }
    dst.dword[i] = clamp<u128>(temp, 0, 9223372036854775807);
  } else {
    u128 temp;
    if (imm == 0) {
      temp = (u128)a.qword[i - 1];
    } else {
      temp = ((u128)a.qword[i - 1] >> imm) +
             (((u128)a.qword[i - 1] >> (imm - 1)) & 1);
    }
    dst.dword[i] = clamp<u128>(temp, 0, 9223372036854775807);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vssrlrni_du_q (\_\_m128i a, \_\_m128i b, imm0_127 imm)

### Synopsis

```
__m128i __lsx_vssrlrni_du_q (__m128i a, __m128i b, imm0_127 imm)
#include <lsxintrin.h>
Instruction: vssrlrni.du.q vr, vr, imm
CPU Flags: LSX
```

### Description

Logical right shift (with rounding) the unsigned 128-bit elements in `a` and `b` by `imm`, clamp to fit in unsigned 64-bit integer and store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (i < 1) {
    u128 temp;
    if (imm == 0) {
      temp = (u128)b.qword[i];
    } else {
      temp = ((u128)b.qword[i] >> imm) + (((u128)b.qword[i] >> (imm - 1)) & 1);
    }
    dst.dword[i] = clamp<u128>(temp, 0, 18446744073709551615);
  } else {
    u128 temp;
    if (imm == 0) {
      temp = (u128)a.qword[i - 1];
    } else {
      temp = ((u128)a.qword[i - 1] >> imm) +
             (((u128)a.qword[i - 1] >> (imm - 1)) & 1);
    }
    dst.dword[i] = clamp<u128>(temp, 0, 18446744073709551615);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vrotr_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vrotr_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vrotr.b vr, vr, vr
CPU Flags: LSX
```

### Description

Rotate right the unsigned 8-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] =
      (a.byte[i] >> (b.byte[i] & 0x7)) | (a.byte[i] << (8 - (b.byte[i] & 0x7)));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vrotr_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vrotr_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vrotr.h vr, vr, vr
CPU Flags: LSX
```

### Description

Rotate right the unsigned 16-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (a.half[i] >> (b.half[i] & 0xf)) |
                (a.half[i] << (16 - (b.half[i] & 0xf)));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vrotr_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vrotr_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vrotr.w vr, vr, vr
CPU Flags: LSX
```

### Description

Rotate right the unsigned 32-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (a.word[i] >> (b.word[i] & 0x1f)) |
                (a.word[i] << (32 - (b.word[i] & 0x1f)));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vrotr_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vrotr_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vrotr.d vr, vr, vr
CPU Flags: LSX
```

### Description

Rotate right the unsigned 64-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (a.dword[i] >> (b.dword[i] & 0x3f)) |
                 (a.dword[i] << (64 - (b.dword[i] & 0x3f)));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vrotri_b (\_\_m128i a, imm0_7 imm)

### Synopsis

```
__m128i __lsx_vrotri_b (__m128i a, imm0_7 imm)
#include <lsxintrin.h>
Instruction: vrotri.b vr, vr, imm
CPU Flags: LSX
```

### Description

Rotate right the unsigned 8-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (a.byte[i] >> imm) | (a.byte[i] << (8 - imm));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vrotri_h (\_\_m128i a, imm0_15 imm)

### Synopsis

```
__m128i __lsx_vrotri_h (__m128i a, imm0_15 imm)
#include <lsxintrin.h>
Instruction: vrotri.h vr, vr, imm
CPU Flags: LSX
```

### Description

Rotate right the unsigned 16-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (a.half[i] >> imm) | (a.half[i] << (16 - imm));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vrotri_w (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vrotri_w (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vrotri.w vr, vr, imm
CPU Flags: LSX
```

### Description

Rotate right the unsigned 32-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (a.word[i] >> imm) | (a.word[i] << (32 - imm));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vrotri_d (\_\_m128i a, imm0_63 imm)

### Synopsis

```
__m128i __lsx_vrotri_d (__m128i a, imm0_63 imm)
#include <lsxintrin.h>
Instruction: vrotri.d vr, vr, imm
CPU Flags: LSX
```

### Description

Rotate right the unsigned 64-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (a.dword[i] >> imm) | (a.dword[i] << (64 - imm));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 1       | 1                |
