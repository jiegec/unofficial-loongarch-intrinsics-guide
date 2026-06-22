# Shift

## \_\_m256i \_\_lasx_xvbsll_v (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvbsll_v (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvbsll.v xr, xr, imm
CPU Flags: LASX
```

### Description

Compute whole vector `a` shifted left by `imm * 8` bits.

### Operation

```
int shift = (imm * 8) % 128;
dst.qword[0] = (u128)a.qword[0] << shift;
dst.qword[1] = (u128)a.qword[1] << shift;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvbsrl_v (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvbsrl_v (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvbsrl.v xr, xr, imm
CPU Flags: LASX
```

### Description

Compute whole vector `a` shifted right by `imm * 8` bits.

### Operation

```
int shift = (imm * 8) % 128;
dst.qword[0] = (u128)a.qword[0] >> shift;
dst.qword[1] = (u128)a.qword[1] >> shift;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsll_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsll_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsll.b xr, xr, xr
CPU Flags: LASX
```

### Description

Logical left shift the unsigned 8-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[i] << (b.byte[i] & 0x7);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsll_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsll_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsll.h xr, xr, xr
CPU Flags: LASX
```

### Description

Logical left shift the unsigned 16-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = a.half[i] << (b.half[i] & 0xf);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsll_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsll_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsll.w xr, xr, xr
CPU Flags: LASX
```

### Description

Logical left shift the unsigned 32-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = a.word[i] << (b.word[i] & 0x1f);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsll_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsll_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsll.d xr, xr, xr
CPU Flags: LASX
```

### Description

Logical left shift the unsigned 64-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = a.dword[i] << (b.dword[i] & 0x3f);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvslli_b (\_\_m256i a, imm0_7 imm)

### Synopsis

```
__m256i __lasx_xvslli_b (__m256i a, imm0_7 imm)
#include <lasxintrin.h>
Instruction: xvslli.b xr, xr, imm
CPU Flags: LASX
```

### Description

Logical left shift the unsigned 8-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[i] << imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvslli_h (\_\_m256i a, imm0_15 imm)

### Synopsis

```
__m256i __lasx_xvslli_h (__m256i a, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvslli.h xr, xr, imm
CPU Flags: LASX
```

### Description

Logical left shift the unsigned 16-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = a.half[i] << imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvslli_w (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvslli_w (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvslli.w xr, xr, imm
CPU Flags: LASX
```

### Description

Logical left shift the unsigned 32-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = a.word[i] << imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvslli_d (\_\_m256i a, imm0_63 imm)

### Synopsis

```
__m256i __lasx_xvslli_d (__m256i a, imm0_63 imm)
#include <lasxintrin.h>
Instruction: xvslli.d xr, xr, imm
CPU Flags: LASX
```

### Description

Logical left shift the unsigned 64-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = a.dword[i] << imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsllwil_h_b (\_\_m256i a, imm0_7 imm)

### Synopsis

```
__m256i __lasx_xvsllwil_h_b (__m256i a, imm0_7 imm)
#include <lasxintrin.h>
Instruction: xvsllwil.h.b xr, xr, imm
CPU Flags: LASX
```

### Description

Extend and shift signed 8-bit elements in `a` by `imm` to signed 16-bit result.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (s16)(s8)a.byte[i] << imm;
}
for (int i = 8; i < 16; i++) {
  dst.half[i] = (s16)(s8)a.byte[i + 8] << imm;
}

// Expands to:

if (0) {
  dst.half[0] = ((s16)((s8)a.byte[0])) << imm;
  dst.half[1] = ((s16)((s8)a.byte[1])) << imm;
  dst.half[2] = ((s16)((s8)a.byte[2])) << imm;
  dst.half[3] = ((s16)((s8)a.byte[3])) << imm;
  dst.half[4] = ((s16)((s8)a.byte[4])) << imm;
  dst.half[5] = ((s16)((s8)a.byte[5])) << imm;
  dst.half[6] = ((s16)((s8)a.byte[6])) << imm;
  dst.half[7] = ((s16)((s8)a.byte[7])) << imm;
  dst.half[8] = ((s16)((s8)a.byte[16])) << imm;
  dst.half[9] = ((s16)((s8)a.byte[17])) << imm;
  dst.half[10] = ((s16)((s8)a.byte[18])) << imm;
  dst.half[11] = ((s16)((s8)a.byte[19])) << imm;
  dst.half[12] = ((s16)((s8)a.byte[20])) << imm;
  dst.half[13] = ((s16)((s8)a.byte[21])) << imm;
  dst.half[14] = ((s16)((s8)a.byte[22])) << imm;
  dst.half[15] = ((s16)((s8)a.byte[23])) << imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsllwil_hu_bu (\_\_m256i a, imm0_7 imm)

### Synopsis

```
__m256i __lasx_xvsllwil_hu_bu (__m256i a, imm0_7 imm)
#include <lasxintrin.h>
Instruction: xvsllwil.hu.bu xr, xr, imm
CPU Flags: LASX
```

### Description

Extend and shift unsigned 8-bit elements in `a` by `imm` to unsigned 16-bit result.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (u16)(u8)a.byte[i] << imm;
}
for (int i = 8; i < 16; i++) {
  dst.half[i] = (u16)(u8)a.byte[i + 8] << imm;
}

// Expands to:

if (0) {
  dst.half[0] = ((u16)((u8)a.byte[0])) << imm;
  dst.half[1] = ((u16)((u8)a.byte[1])) << imm;
  dst.half[2] = ((u16)((u8)a.byte[2])) << imm;
  dst.half[3] = ((u16)((u8)a.byte[3])) << imm;
  dst.half[4] = ((u16)((u8)a.byte[4])) << imm;
  dst.half[5] = ((u16)((u8)a.byte[5])) << imm;
  dst.half[6] = ((u16)((u8)a.byte[6])) << imm;
  dst.half[7] = ((u16)((u8)a.byte[7])) << imm;
  dst.half[8] = ((u16)((u8)a.byte[16])) << imm;
  dst.half[9] = ((u16)((u8)a.byte[17])) << imm;
  dst.half[10] = ((u16)((u8)a.byte[18])) << imm;
  dst.half[11] = ((u16)((u8)a.byte[19])) << imm;
  dst.half[12] = ((u16)((u8)a.byte[20])) << imm;
  dst.half[13] = ((u16)((u8)a.byte[21])) << imm;
  dst.half[14] = ((u16)((u8)a.byte[22])) << imm;
  dst.half[15] = ((u16)((u8)a.byte[23])) << imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsllwil_w_h (\_\_m256i a, imm0_15 imm)

### Synopsis

```
__m256i __lasx_xvsllwil_w_h (__m256i a, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvsllwil.w.h xr, xr, imm
CPU Flags: LASX
```

### Description

Extend and shift signed 16-bit elements in `a` by `imm` to signed 32-bit result.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (s32)(s16)a.half[i] << imm;
}
for (int i = 4; i < 8; i++) {
  dst.word[i] = (s32)(s16)a.half[i + 4] << imm;
}

// Expands to:

if (0) {
  dst.word[0] = ((s32)((s16)a.half[0])) << imm;
  dst.word[1] = ((s32)((s16)a.half[1])) << imm;
  dst.word[2] = ((s32)((s16)a.half[2])) << imm;
  dst.word[3] = ((s32)((s16)a.half[3])) << imm;
  dst.word[4] = ((s32)((s16)a.half[8])) << imm;
  dst.word[5] = ((s32)((s16)a.half[9])) << imm;
  dst.word[6] = ((s32)((s16)a.half[10])) << imm;
  dst.word[7] = ((s32)((s16)a.half[11])) << imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsllwil_wu_hu (\_\_m256i a, imm0_15 imm)

### Synopsis

```
__m256i __lasx_xvsllwil_wu_hu (__m256i a, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvsllwil.wu.hu xr, xr, imm
CPU Flags: LASX
```

### Description

Extend and shift unsigned 16-bit elements in `a` by `imm` to unsigned 32-bit result.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (u32)(u16)a.half[i] << imm;
}
for (int i = 4; i < 8; i++) {
  dst.word[i] = (u32)(u16)a.half[i + 4] << imm;
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[0])) << imm;
  dst.word[1] = ((u32)((u16)a.half[1])) << imm;
  dst.word[2] = ((u32)((u16)a.half[2])) << imm;
  dst.word[3] = ((u32)((u16)a.half[3])) << imm;
  dst.word[4] = ((u32)((u16)a.half[8])) << imm;
  dst.word[5] = ((u32)((u16)a.half[9])) << imm;
  dst.word[6] = ((u32)((u16)a.half[10])) << imm;
  dst.word[7] = ((u32)((u16)a.half[11])) << imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsllwil_d_w (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvsllwil_d_w (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvsllwil.d.w xr, xr, imm
CPU Flags: LASX
```

### Description

Extend and shift signed 32-bit elements in `a` by `imm` to signed 64-bit result.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)(s32)a.word[i] << imm;
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = (s64)(s32)a.word[i + 2] << imm;
}

// Expands to:

if (0) {
  dst.dword[0] = ((s64)((s32)a.word[0])) << imm;
  dst.dword[1] = ((s64)((s32)a.word[1])) << imm;
  dst.dword[2] = ((s64)((s32)a.word[4])) << imm;
  dst.dword[3] = ((s64)((s32)a.word[5])) << imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsllwil_du_wu (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvsllwil_du_wu (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvsllwil.du.wu xr, xr, imm
CPU Flags: LASX
```

### Description

Extend and shift unsigned 32-bit elements in `a` by `imm` to unsigned 64-bit result.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (u64)(u32)a.word[i] << imm;
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = (u64)(u32)a.word[i + 2] << imm;
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[0])) << imm;
  dst.dword[1] = ((u64)((u32)a.word[1])) << imm;
  dst.dword[2] = ((u64)((u32)a.word[4])) << imm;
  dst.dword[3] = ((u64)((u32)a.word[5])) << imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsra_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsra_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsra.b xr, xr, xr
CPU Flags: LASX
```

### Description

Arithmetic right shift the signed 8-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = ((s8)a.byte[i]) >> (b.byte[i] & 0x7);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsra_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsra_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsra.h xr, xr, xr
CPU Flags: LASX
```

### Description

Arithmetic right shift the signed 16-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = ((s16)a.half[i]) >> (b.half[i] & 0xf);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsra_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsra_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsra.w xr, xr, xr
CPU Flags: LASX
```

### Description

Arithmetic right shift the signed 32-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = ((s32)a.word[i]) >> (b.word[i] & 0x1f);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsra_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsra_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsra.d xr, xr, xr
CPU Flags: LASX
```

### Description

Arithmetic right shift the signed 64-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = ((s64)a.dword[i]) >> (b.dword[i] & 0x3f);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsrai_b (\_\_m256i a, imm0_7 imm)

### Synopsis

```
__m256i __lasx_xvsrai_b (__m256i a, imm0_7 imm)
#include <lasxintrin.h>
Instruction: xvsrai.b xr, xr, imm
CPU Flags: LASX
```

### Description

Arithmetic right shift the signed 8-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = ((s8)a.byte[i]) >> imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsrai_h (\_\_m256i a, imm0_15 imm)

### Synopsis

```
__m256i __lasx_xvsrai_h (__m256i a, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvsrai.h xr, xr, imm
CPU Flags: LASX
```

### Description

Arithmetic right shift the signed 16-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = ((s16)a.half[i]) >> imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsrai_w (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvsrai_w (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvsrai.w xr, xr, imm
CPU Flags: LASX
```

### Description

Arithmetic right shift the signed 32-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = ((s32)a.word[i]) >> imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsrai_d (\_\_m256i a, imm0_63 imm)

### Synopsis

```
__m256i __lasx_xvsrai_d (__m256i a, imm0_63 imm)
#include <lasxintrin.h>
Instruction: xvsrai.d xr, xr, imm
CPU Flags: LASX
```

### Description

Arithmetic right shift the signed 64-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = ((s64)a.dword[i]) >> imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsran_b_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsran_b_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsran.b.h xr, xr, xr
CPU Flags: LASX
```

### Description

Arithmetic right shift the signed 16-bit elements in `a` by elements in `b`, truncate to 8-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (i < 8) ? (s8)((s16)a.half[i] >> (b.half[i] & 15)) : 0;
}
for (int i = 16; i < 32; i++) {
  dst.byte[i] = (i < 24) ? (s8)((s16)a.half[i - 8] >> (b.half[i - 8] & 15)) : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsran_h_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsran_h_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsran.h.w xr, xr, xr
CPU Flags: LASX
```

### Description

Arithmetic right shift the signed 32-bit elements in `a` by elements in `b`, truncate to 16-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (i < 4) ? (s16)((s32)a.word[i] >> (b.word[i] & 31)) : 0;
}
for (int i = 8; i < 16; i++) {
  dst.half[i] =
      (i < 12) ? (s16)((s32)a.word[i - 4] >> (b.word[i - 4] & 31)) : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsran_w_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsran_w_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsran.w.d xr, xr, xr
CPU Flags: LASX
```

### Description

Arithmetic right shift the signed 64-bit elements in `a` by elements in `b`, truncate to 32-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2) ? (s32)((s64)a.dword[i] >> (b.dword[i] & 63)) : 0;
}
for (int i = 4; i < 8; i++) {
  dst.word[i] =
      (i < 6) ? (s32)((s64)a.dword[i - 2] >> (b.dword[i - 2] & 63)) : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsrani_b_h (\_\_m256i a, \_\_m256i b, imm0_15 imm)

### Synopsis

```
__m256i __lasx_xvsrani_b_h (__m256i a, __m256i b, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvsrani.b.h xr, xr, imm
CPU Flags: LASX
```

### Description

Arithmetic right shift the signed 16-bit elements in `a` and `b` by `imm`, truncate to 8-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] =
      (i < 8) ? (s8)((s16)b.half[i] >> imm) : (s8)((s16)a.half[i - 8] >> imm);
}
for (int i = 16; i < 32; i++) {
  dst.byte[i] = (i < 24) ? (s8)((s16)b.half[i - 8] >> imm)
                         : (s8)((s16)a.half[i - 16] >> imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvsrani_h_w (\_\_m256i a, \_\_m256i b, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvsrani_h_w (__m256i a, __m256i b, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvsrani.h.w xr, xr, imm
CPU Flags: LASX
```

### Description

Arithmetic right shift the signed 32-bit elements in `a` and `b` by `imm`, truncate to 16-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] =
      (i < 4) ? (s16)((s32)b.word[i] >> imm) : (s16)((s32)a.word[i - 4] >> imm);
}
for (int i = 8; i < 16; i++) {
  dst.half[i] = (i < 12) ? (s16)((s32)b.word[i - 4] >> imm)
                         : (s16)((s32)a.word[i - 8] >> imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvsrani_w_d (\_\_m256i a, \_\_m256i b, imm0_63 imm)

### Synopsis

```
__m256i __lasx_xvsrani_w_d (__m256i a, __m256i b, imm0_63 imm)
#include <lasxintrin.h>
Instruction: xvsrani.w.d xr, xr, imm
CPU Flags: LASX
```

### Description

Arithmetic right shift the signed 64-bit elements in `a` and `b` by `imm`, truncate to 32-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2) ? (s32)((s64)b.dword[i] >> imm)
                        : (s32)((s64)a.dword[i - 2] >> imm);
}
for (int i = 4; i < 8; i++) {
  dst.word[i] = (i < 6) ? (s32)((s64)b.dword[i - 2] >> imm)
                        : (s32)((s64)a.dword[i - 4] >> imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvsrani_d_q (\_\_m256i a, \_\_m256i b, imm0_127 imm)

### Synopsis

```
__m256i __lasx_xvsrani_d_q (__m256i a, __m256i b, imm0_127 imm)
#include <lasxintrin.h>
Instruction: xvsrani.d.q xr, xr, imm
CPU Flags: LASX
```

### Description

Arithmetic right shift the signed 128-bit elements in `a` and `b` by `imm`, truncate to 64-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (i < 1) ? (s64)((s128)b.qword[i] >> imm)
                         : (s64)((s128)a.qword[i - 1] >> imm);
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = (i < 3) ? (s64)((s128)b.qword[i - 1] >> imm)
                         : (s64)((s128)a.qword[i - 2] >> imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvsrar_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsrar_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsrar.b xr, xr, xr
CPU Flags: LASX
```

### Description

Arithmetic right shift (with rounding) the signed 8-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
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

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvsrar_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsrar_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsrar.h xr, xr, xr
CPU Flags: LASX
```

### Description

Arithmetic right shift (with rounding) the signed 16-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
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

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvsrar_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsrar_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsrar.w xr, xr, xr
CPU Flags: LASX
```

### Description

Arithmetic right shift (with rounding) the signed 32-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
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

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvsrar_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsrar_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsrar.d xr, xr, xr
CPU Flags: LASX
```

### Description

Arithmetic right shift (with rounding) the signed 64-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
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

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvsrari_b (\_\_m256i a, imm0_7 imm)

### Synopsis

```
__m256i __lasx_xvsrari_b (__m256i a, imm0_7 imm)
#include <lasxintrin.h>
Instruction: xvsrari.b xr, xr, imm
CPU Flags: LASX
```

### Description

Arithmetic right shift (with rounding) the signed 8-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  if (imm == 0) {
    dst.byte[i] = a.byte[i];
  } else {
    dst.byte[i] = ((s8)a.byte[i] >> imm) + (((s8)a.byte[i] >> (imm - 1)) & 0x1);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvsrari_h (\_\_m256i a, imm0_15 imm)

### Synopsis

```
__m256i __lasx_xvsrari_h (__m256i a, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvsrari.h xr, xr, imm
CPU Flags: LASX
```

### Description

Arithmetic right shift (with rounding) the signed 16-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
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

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvsrari_w (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvsrari_w (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvsrari.w xr, xr, imm
CPU Flags: LASX
```

### Description

Arithmetic right shift (with rounding) the signed 32-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
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

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvsrari_d (\_\_m256i a, imm0_63 imm)

### Synopsis

```
__m256i __lasx_xvsrari_d (__m256i a, imm0_63 imm)
#include <lasxintrin.h>
Instruction: xvsrari.d xr, xr, imm
CPU Flags: LASX
```

### Description

Arithmetic right shift (with rounding) the signed 64-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
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

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvsrarn_b_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsrarn_b_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsrarn.b.h xr, xr, xr
CPU Flags: LASX
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
for (int i = 16; i < 32; i++) {
  if (i < 24) {
    u8 shift = (b.half[i - 8] & 15);
    if (shift == 0) {
      dst.byte[i] = (s8)(s16)a.half[i - 8];
    } else {
      dst.byte[i] = (s8)(((s16)a.half[i - 8] >> shift) +
                         (((s16)a.half[i - 8] >> (shift - 1)) & 0x1));
    }
  } else {
    dst.byte[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvsrarn_h_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsrarn_h_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsrarn.h.w xr, xr, xr
CPU Flags: LASX
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
for (int i = 8; i < 16; i++) {
  if (i < 12) {
    u8 shift = (b.word[i - 4] & 31);
    if (shift == 0) {
      dst.half[i] = (s16)(s32)a.word[i - 4];
    } else {
      dst.half[i] = (s16)(((s32)a.word[i - 4] >> shift) +
                          (((s32)a.word[i - 4] >> (shift - 1)) & 0x1));
    }
  } else {
    dst.half[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvsrarn_w_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsrarn_w_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsrarn.w.d xr, xr, xr
CPU Flags: LASX
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
for (int i = 4; i < 8; i++) {
  if (i < 6) {
    u8 shift = (b.dword[i - 2] & 63);
    if (shift == 0) {
      dst.word[i] = (s32)(s64)a.dword[i - 2];
    } else {
      dst.word[i] = (s32)(((s64)a.dword[i - 2] >> shift) +
                          (((s64)a.dword[i - 2] >> (shift - 1)) & 0x1));
    }
  } else {
    dst.word[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvsrarni_b_h (\_\_m256i a, \_\_m256i b, imm0_15 imm)

### Synopsis

```
__m256i __lasx_xvsrarni_b_h (__m256i a, __m256i b, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvsrarni.b.h xr, xr, imm
CPU Flags: LASX
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
for (int i = 16; i < 32; i++) {
  if (i < 24) {
    if (imm == 0) {
      dst.byte[i] = (s8)(s16)b.half[i - 8];
    } else {
      dst.byte[i] = (s8)(((s16)b.half[i - 8] >> imm) +
                         (((s16)b.half[i - 8] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.byte[i] = (s8)(s16)a.half[i - 16];
    } else {
      dst.byte[i] = (s8)(((s16)a.half[i - 16] >> imm) +
                         (((s16)a.half[i - 16] >> (imm - 1)) & 0x1));
    }
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvsrarni_h_w (\_\_m256i a, \_\_m256i b, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvsrarni_h_w (__m256i a, __m256i b, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvsrarni.h.w xr, xr, imm
CPU Flags: LASX
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
for (int i = 8; i < 16; i++) {
  if (i < 12) {
    if (imm == 0) {
      dst.half[i] = (s16)(s32)b.word[i - 4];
    } else {
      dst.half[i] = (s16)(((s32)b.word[i - 4] >> imm) +
                          (((s32)b.word[i - 4] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.half[i] = (s16)(s32)a.word[i - 8];
    } else {
      dst.half[i] = (s16)(((s32)a.word[i - 8] >> imm) +
                          (((s32)a.word[i - 8] >> (imm - 1)) & 0x1));
    }
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvsrarni_w_d (\_\_m256i a, \_\_m256i b, imm0_63 imm)

### Synopsis

```
__m256i __lasx_xvsrarni_w_d (__m256i a, __m256i b, imm0_63 imm)
#include <lasxintrin.h>
Instruction: xvsrarni.w.d xr, xr, imm
CPU Flags: LASX
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
for (int i = 4; i < 8; i++) {
  if (i < 6) {
    if (imm == 0) {
      dst.word[i] = (s32)(s64)b.dword[i - 2];
    } else {
      dst.word[i] = (s32)(((s64)b.dword[i - 2] >> imm) +
                          (((s64)b.dword[i - 2] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.word[i] = (s32)(s64)a.dword[i - 4];
    } else {
      dst.word[i] = (s32)(((s64)a.dword[i - 4] >> imm) +
                          (((s64)a.dword[i - 4] >> (imm - 1)) & 0x1));
    }
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvsrarni_d_q (\_\_m256i a, \_\_m256i b, imm0_127 imm)

### Synopsis

```
__m256i __lasx_xvsrarni_d_q (__m256i a, __m256i b, imm0_127 imm)
#include <lasxintrin.h>
Instruction: xvsrarni.d.q xr, xr, imm
CPU Flags: LASX
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
for (int i = 2; i < 4; i++) {
  if (i < 3) {
    if (imm == 0) {
      dst.dword[i] = (s64)(s128)b.qword[i - 1];
    } else {
      dst.dword[i] = (s64)(((s128)b.qword[i - 1] >> imm) +
                           (((s128)b.qword[i - 1] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.dword[i] = (s64)(s128)a.qword[i - 2];
    } else {
      dst.dword[i] = (s64)(((s128)a.qword[i - 2] >> imm) +
                           (((s128)a.qword[i - 2] >> (imm - 1)) & 0x1));
    }
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvsrl_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsrl_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsrl.b xr, xr, xr
CPU Flags: LASX
```

### Description

Logical right shift the unsigned 8-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[i] >> (b.byte[i] & 0x7);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsrl_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsrl_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsrl.h xr, xr, xr
CPU Flags: LASX
```

### Description

Logical right shift the unsigned 16-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = a.half[i] >> (b.half[i] & 0xf);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsrl_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsrl_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsrl.w xr, xr, xr
CPU Flags: LASX
```

### Description

Logical right shift the unsigned 32-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = a.word[i] >> (b.word[i] & 0x1f);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsrl_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsrl_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsrl.d xr, xr, xr
CPU Flags: LASX
```

### Description

Logical right shift the unsigned 64-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = a.dword[i] >> (b.dword[i] & 0x3f);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsrli_b (\_\_m256i a, imm0_7 imm)

### Synopsis

```
__m256i __lasx_xvsrli_b (__m256i a, imm0_7 imm)
#include <lasxintrin.h>
Instruction: xvsrli.b xr, xr, imm
CPU Flags: LASX
```

### Description

Logical right shift the unsigned 8-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[i] >> imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsrli_h (\_\_m256i a, imm0_15 imm)

### Synopsis

```
__m256i __lasx_xvsrli_h (__m256i a, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvsrli.h xr, xr, imm
CPU Flags: LASX
```

### Description

Logical right shift the unsigned 16-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = a.half[i] >> imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsrli_w (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvsrli_w (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvsrli.w xr, xr, imm
CPU Flags: LASX
```

### Description

Logical right shift the unsigned 32-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = a.word[i] >> imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsrli_d (\_\_m256i a, imm0_63 imm)

### Synopsis

```
__m256i __lasx_xvsrli_d (__m256i a, imm0_63 imm)
#include <lasxintrin.h>
Instruction: xvsrli.d xr, xr, imm
CPU Flags: LASX
```

### Description

Logical right shift the unsigned 64-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = a.dword[i] >> imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsrln_b_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsrln_b_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsrln.b.h xr, xr, xr
CPU Flags: LASX
```

### Description

Logical right shift the unsigned 16-bit elements in `a` by elements in `b`, truncate to 8-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (i < 8) ? (u8)((u16)a.half[i] >> (b.half[i] & 15)) : 0;
}
for (int i = 16; i < 32; i++) {
  dst.byte[i] = (i < 24) ? (u8)((u16)a.half[i - 8] >> (b.half[i - 8] & 15)) : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsrln_h_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsrln_h_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsrln.h.w xr, xr, xr
CPU Flags: LASX
```

### Description

Logical right shift the unsigned 32-bit elements in `a` by elements in `b`, truncate to 16-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (i < 4) ? (u16)((u32)a.word[i] >> (b.word[i] & 31)) : 0;
}
for (int i = 8; i < 16; i++) {
  dst.half[i] =
      (i < 12) ? (u16)((u32)a.word[i - 4] >> (b.word[i - 4] & 31)) : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsrln_w_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsrln_w_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsrln.w.d xr, xr, xr
CPU Flags: LASX
```

### Description

Logical right shift the unsigned 64-bit elements in `a` by elements in `b`, truncate to 32-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2) ? (u32)((u64)a.dword[i] >> (b.dword[i] & 63)) : 0;
}
for (int i = 4; i < 8; i++) {
  dst.word[i] =
      (i < 6) ? (u32)((u64)a.dword[i - 2] >> (b.dword[i - 2] & 63)) : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 1                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsrlni_b_h (\_\_m256i a, \_\_m256i b, imm0_15 imm)

### Synopsis

```
__m256i __lasx_xvsrlni_b_h (__m256i a, __m256i b, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvsrlni.b.h xr, xr, imm
CPU Flags: LASX
```

### Description

Logical right shift the unsigned 16-bit elements in `a` and `b` by `imm`, truncate to 8-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] =
      (i < 8) ? (u8)((u16)b.half[i] >> imm) : (u8)((u16)a.half[i - 8] >> imm);
}
for (int i = 16; i < 32; i++) {
  dst.byte[i] = (i < 24) ? (u8)((u16)b.half[i - 8] >> imm)
                         : (u8)((u16)a.half[i - 16] >> imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvsrlni_h_w (\_\_m256i a, \_\_m256i b, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvsrlni_h_w (__m256i a, __m256i b, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvsrlni.h.w xr, xr, imm
CPU Flags: LASX
```

### Description

Logical right shift the unsigned 32-bit elements in `a` and `b` by `imm`, truncate to 16-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] =
      (i < 4) ? (u16)((u32)b.word[i] >> imm) : (u16)((u32)a.word[i - 4] >> imm);
}
for (int i = 8; i < 16; i++) {
  dst.half[i] = (i < 12) ? (u16)((u32)b.word[i - 4] >> imm)
                         : (u16)((u32)a.word[i - 8] >> imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvsrlni_w_d (\_\_m256i a, \_\_m256i b, imm0_63 imm)

### Synopsis

```
__m256i __lasx_xvsrlni_w_d (__m256i a, __m256i b, imm0_63 imm)
#include <lasxintrin.h>
Instruction: xvsrlni.w.d xr, xr, imm
CPU Flags: LASX
```

### Description

Logical right shift the unsigned 64-bit elements in `a` and `b` by `imm`, truncate to 32-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2) ? (u32)((u64)b.dword[i] >> imm)
                        : (u32)((u64)a.dword[i - 2] >> imm);
}
for (int i = 4; i < 8; i++) {
  dst.word[i] = (i < 6) ? (u32)((u64)b.dword[i - 2] >> imm)
                        : (u32)((u64)a.dword[i - 4] >> imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvsrlni_d_q (\_\_m256i a, \_\_m256i b, imm0_127 imm)

### Synopsis

```
__m256i __lasx_xvsrlni_d_q (__m256i a, __m256i b, imm0_127 imm)
#include <lasxintrin.h>
Instruction: xvsrlni.d.q xr, xr, imm
CPU Flags: LASX
```

### Description

Logical right shift the unsigned 128-bit elements in `a` and `b` by `imm`, truncate to 64-bit and store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (i < 1) ? (u64)((u128)b.qword[i] >> imm)
                         : (u64)((u128)a.qword[i - 1] >> imm);
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = (i < 3) ? (u64)((u128)b.qword[i - 1] >> imm)
                         : (u64)((u128)a.qword[i - 2] >> imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvsrlr_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsrlr_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsrlr.b xr, xr, xr
CPU Flags: LASX
```

### Description

Logical right shift (with rounding) the unsigned 8-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
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

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvsrlr_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsrlr_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsrlr.h xr, xr, xr
CPU Flags: LASX
```

### Description

Logical right shift (with rounding) the unsigned 16-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
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

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvsrlr_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsrlr_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsrlr.w xr, xr, xr
CPU Flags: LASX
```

### Description

Logical right shift (with rounding) the unsigned 32-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
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

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvsrlr_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsrlr_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsrlr.d xr, xr, xr
CPU Flags: LASX
```

### Description

Logical right shift (with rounding) the unsigned 64-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
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

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvsrlri_b (\_\_m256i a, imm0_7 imm)

### Synopsis

```
__m256i __lasx_xvsrlri_b (__m256i a, imm0_7 imm)
#include <lasxintrin.h>
Instruction: xvsrlri.b xr, xr, imm
CPU Flags: LASX
```

### Description

Logical right shift (with rounding) the unsigned 8-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  if (imm == 0) {
    dst.byte[i] = a.byte[i];
  } else {
    dst.byte[i] = (a.byte[i] >> imm) + ((a.byte[i] >> (imm - 1)) & 0x1);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvsrlri_h (\_\_m256i a, imm0_15 imm)

### Synopsis

```
__m256i __lasx_xvsrlri_h (__m256i a, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvsrlri.h xr, xr, imm
CPU Flags: LASX
```

### Description

Logical right shift (with rounding) the unsigned 16-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  if (imm == 0) {
    dst.half[i] = a.half[i];
  } else {
    dst.half[i] = (a.half[i] >> imm) + ((a.half[i] >> (imm - 1)) & 0x1);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvsrlri_w (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvsrlri_w (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvsrlri.w xr, xr, imm
CPU Flags: LASX
```

### Description

Logical right shift (with rounding) the unsigned 32-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (imm == 0) {
    dst.word[i] = a.word[i];
  } else {
    dst.word[i] = (a.word[i] >> imm) + ((a.word[i] >> (imm - 1)) & 0x1);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvsrlri_d (\_\_m256i a, imm0_63 imm)

### Synopsis

```
__m256i __lasx_xvsrlri_d (__m256i a, imm0_63 imm)
#include <lasxintrin.h>
Instruction: xvsrlri.d xr, xr, imm
CPU Flags: LASX
```

### Description

Logical right shift (with rounding) the unsigned 64-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (imm == 0) {
    dst.dword[i] = a.dword[i];
  } else {
    dst.dword[i] = (a.dword[i] >> imm) + ((a.dword[i] >> (imm - 1)) & 0x1);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvsrlrn_b_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsrlrn_b_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsrlrn.b.h xr, xr, xr
CPU Flags: LASX
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
for (int i = 16; i < 32; i++) {
  if (i < 24) {
    u8 shift = (b.half[i - 8] & 15);
    if (shift == 0) {
      dst.byte[i] = (u8)(u16)a.half[i - 8];
    } else {
      dst.byte[i] = (u8)(((u16)a.half[i - 8] >> shift) +
                         (((u16)a.half[i - 8] >> (shift - 1)) & 0x1));
    }
  } else {
    dst.byte[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvsrlrn_h_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsrlrn_h_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsrlrn.h.w xr, xr, xr
CPU Flags: LASX
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
for (int i = 8; i < 16; i++) {
  if (i < 12) {
    u8 shift = (b.word[i - 4] & 31);
    if (shift == 0) {
      dst.half[i] = (u16)(u32)a.word[i - 4];
    } else {
      dst.half[i] = (u16)(((u32)a.word[i - 4] >> shift) +
                          (((u32)a.word[i - 4] >> (shift - 1)) & 0x1));
    }
  } else {
    dst.half[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvsrlrn_w_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsrlrn_w_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsrlrn.w.d xr, xr, xr
CPU Flags: LASX
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
for (int i = 4; i < 8; i++) {
  if (i < 6) {
    u8 shift = (b.dword[i - 2] & 63);
    if (shift == 0) {
      dst.word[i] = (u32)(u64)a.dword[i - 2];
    } else {
      dst.word[i] = (u32)(((u64)a.dword[i - 2] >> shift) +
                          (((u64)a.dword[i - 2] >> (shift - 1)) & 0x1));
    }
  } else {
    dst.word[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvsrlrni_b_h (\_\_m256i a, \_\_m256i b, imm0_15 imm)

### Synopsis

```
__m256i __lasx_xvsrlrni_b_h (__m256i a, __m256i b, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvsrlrni.b.h xr, xr, imm
CPU Flags: LASX
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
for (int i = 16; i < 32; i++) {
  if (i < 24) {
    if (imm == 0) {
      dst.byte[i] = (u8)(u16)b.half[i - 8];
    } else {
      dst.byte[i] = (u8)(((u16)b.half[i - 8] >> imm) +
                         (((u16)b.half[i - 8] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.byte[i] = (u8)(u16)a.half[i - 16];
    } else {
      dst.byte[i] = (u8)(((u16)a.half[i - 16] >> imm) +
                         (((u16)a.half[i - 16] >> (imm - 1)) & 0x1));
    }
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvsrlrni_h_w (\_\_m256i a, \_\_m256i b, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvsrlrni_h_w (__m256i a, __m256i b, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvsrlrni.h.w xr, xr, imm
CPU Flags: LASX
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
for (int i = 8; i < 16; i++) {
  if (i < 12) {
    if (imm == 0) {
      dst.half[i] = (u16)(u32)b.word[i - 4];
    } else {
      dst.half[i] = (u16)(((u32)b.word[i - 4] >> imm) +
                          (((u32)b.word[i - 4] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.half[i] = (u16)(u32)a.word[i - 8];
    } else {
      dst.half[i] = (u16)(((u32)a.word[i - 8] >> imm) +
                          (((u32)a.word[i - 8] >> (imm - 1)) & 0x1));
    }
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvsrlrni_w_d (\_\_m256i a, \_\_m256i b, imm0_63 imm)

### Synopsis

```
__m256i __lasx_xvsrlrni_w_d (__m256i a, __m256i b, imm0_63 imm)
#include <lasxintrin.h>
Instruction: xvsrlrni.w.d xr, xr, imm
CPU Flags: LASX
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
for (int i = 4; i < 8; i++) {
  if (i < 6) {
    if (imm == 0) {
      dst.word[i] = (u32)(u64)b.dword[i - 2];
    } else {
      dst.word[i] = (u32)(((u64)b.dword[i - 2] >> imm) +
                          (((u64)b.dword[i - 2] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.word[i] = (u32)(u64)a.dword[i - 4];
    } else {
      dst.word[i] = (u32)(((u64)a.dword[i - 4] >> imm) +
                          (((u64)a.dword[i - 4] >> (imm - 1)) & 0x1));
    }
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvsrlrni_d_q (\_\_m256i a, \_\_m256i b, imm0_127 imm)

### Synopsis

```
__m256i __lasx_xvsrlrni_d_q (__m256i a, __m256i b, imm0_127 imm)
#include <lasxintrin.h>
Instruction: xvsrlrni.d.q xr, xr, imm
CPU Flags: LASX
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
for (int i = 2; i < 4; i++) {
  if (i < 3) {
    if (imm == 0) {
      dst.dword[i] = (u64)(u128)b.qword[i - 1];
    } else {
      dst.dword[i] = (u64)(((u128)b.qword[i - 1] >> imm) +
                           (((u128)b.qword[i - 1] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.dword[i] = (u64)(u128)a.qword[i - 2];
    } else {
      dst.dword[i] = (u64)(((u128)a.qword[i - 2] >> imm) +
                           (((u128)a.qword[i - 2] >> (imm - 1)) & 0x1));
    }
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvssran_b_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssran_b_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssran.b.h xr, xr, xr
CPU Flags: LASX
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
for (int i = 16; i < 32; i++) {
  if (i < 24) {
    s16 temp = (s16)a.half[i - 8] >> (b.half[i - 8] & 15);
    dst.byte[i] = clamp<s16>(temp, -128, 127);
  } else {
    dst.byte[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssran_bu_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssran_bu_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssran.bu.h xr, xr, xr
CPU Flags: LASX
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
for (int i = 16; i < 32; i++) {
  if (i < 24) {
    s16 temp = (s16)a.half[i - 8] >> (b.half[i - 8] & 15);
    dst.byte[i] = clamp<s16>(temp, 0, 255);
  } else {
    dst.byte[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssran_h_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssran_h_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssran.h.w xr, xr, xr
CPU Flags: LASX
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
for (int i = 8; i < 16; i++) {
  if (i < 12) {
    s32 temp = (s32)a.word[i - 4] >> (b.word[i - 4] & 31);
    dst.half[i] = clamp<s32>(temp, -32768, 32767);
  } else {
    dst.half[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssran_hu_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssran_hu_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssran.hu.w xr, xr, xr
CPU Flags: LASX
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
for (int i = 8; i < 16; i++) {
  if (i < 12) {
    s32 temp = (s32)a.word[i - 4] >> (b.word[i - 4] & 31);
    dst.half[i] = clamp<s32>(temp, 0, 65535);
  } else {
    dst.half[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssran_w_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssran_w_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssran.w.d xr, xr, xr
CPU Flags: LASX
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
for (int i = 4; i < 8; i++) {
  if (i < 6) {
    s64 temp = (s64)a.dword[i - 2] >> (b.dword[i - 2] & 63);
    dst.word[i] = clamp<s64>(temp, -2147483648, 2147483647);
  } else {
    dst.word[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssran_wu_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssran_wu_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssran.wu.d xr, xr, xr
CPU Flags: LASX
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
for (int i = 4; i < 8; i++) {
  if (i < 6) {
    s64 temp = (s64)a.dword[i - 2] >> (b.dword[i - 2] & 63);
    dst.word[i] = clamp<s64>(temp, 0, 4294967295);
  } else {
    dst.word[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrani_b_h (\_\_m256i a, \_\_m256i b, imm0_15 imm)

### Synopsis

```
__m256i __lasx_xvssrani_b_h (__m256i a, __m256i b, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvssrani.b.h xr, xr, imm
CPU Flags: LASX
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
for (int i = 16; i < 32; i++) {
  if (i < 24) {
    s16 temp = (s16)b.half[i - 8] >> imm;
    dst.byte[i] = clamp<s16>(temp, -128, 127);
  } else {
    s16 temp = (s16)a.half[i - 16] >> imm;
    dst.byte[i] = clamp<s16>(temp, -128, 127);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrani_bu_h (\_\_m256i a, \_\_m256i b, imm0_15 imm)

### Synopsis

```
__m256i __lasx_xvssrani_bu_h (__m256i a, __m256i b, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvssrani.bu.h xr, xr, imm
CPU Flags: LASX
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
for (int i = 16; i < 32; i++) {
  if (i < 24) {
    s16 temp = (s16)b.half[i - 8] >> imm;
    dst.byte[i] = clamp<s16>(temp, 0, 255);
  } else {
    s16 temp = (s16)a.half[i - 16] >> imm;
    dst.byte[i] = clamp<s16>(temp, 0, 255);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrani_h_w (\_\_m256i a, \_\_m256i b, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvssrani_h_w (__m256i a, __m256i b, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvssrani.h.w xr, xr, imm
CPU Flags: LASX
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
for (int i = 8; i < 16; i++) {
  if (i < 12) {
    s32 temp = (s32)b.word[i - 4] >> imm;
    dst.half[i] = clamp<s32>(temp, -32768, 32767);
  } else {
    s32 temp = (s32)a.word[i - 8] >> imm;
    dst.half[i] = clamp<s32>(temp, -32768, 32767);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrani_hu_w (\_\_m256i a, \_\_m256i b, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvssrani_hu_w (__m256i a, __m256i b, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvssrani.hu.w xr, xr, imm
CPU Flags: LASX
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
for (int i = 8; i < 16; i++) {
  if (i < 12) {
    s32 temp = (s32)b.word[i - 4] >> imm;
    dst.half[i] = clamp<s32>(temp, 0, 65535);
  } else {
    s32 temp = (s32)a.word[i - 8] >> imm;
    dst.half[i] = clamp<s32>(temp, 0, 65535);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrani_w_d (\_\_m256i a, \_\_m256i b, imm0_63 imm)

### Synopsis

```
__m256i __lasx_xvssrani_w_d (__m256i a, __m256i b, imm0_63 imm)
#include <lasxintrin.h>
Instruction: xvssrani.w.d xr, xr, imm
CPU Flags: LASX
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
for (int i = 4; i < 8; i++) {
  if (i < 6) {
    s64 temp = (s64)b.dword[i - 2] >> imm;
    dst.word[i] = clamp<s64>(temp, -2147483648, 2147483647);
  } else {
    s64 temp = (s64)a.dword[i - 4] >> imm;
    dst.word[i] = clamp<s64>(temp, -2147483648, 2147483647);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrani_wu_d (\_\_m256i a, \_\_m256i b, imm0_63 imm)

### Synopsis

```
__m256i __lasx_xvssrani_wu_d (__m256i a, __m256i b, imm0_63 imm)
#include <lasxintrin.h>
Instruction: xvssrani.wu.d xr, xr, imm
CPU Flags: LASX
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
for (int i = 4; i < 8; i++) {
  if (i < 6) {
    s64 temp = (s64)b.dword[i - 2] >> imm;
    dst.word[i] = clamp<s64>(temp, 0, 4294967295);
  } else {
    s64 temp = (s64)a.dword[i - 4] >> imm;
    dst.word[i] = clamp<s64>(temp, 0, 4294967295);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrani_d_q (\_\_m256i a, \_\_m256i b, imm0_127 imm)

### Synopsis

```
__m256i __lasx_xvssrani_d_q (__m256i a, __m256i b, imm0_127 imm)
#include <lasxintrin.h>
Instruction: xvssrani.d.q xr, xr, imm
CPU Flags: LASX
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
for (int i = 2; i < 4; i++) {
  if (i < 3) {
    s128 temp = (s128)b.qword[i - 1] >> imm;
    dst.dword[i] = clamp<s128>(temp, -9223372036854775808, 9223372036854775807);
  } else {
    s128 temp = (s128)a.qword[i - 2] >> imm;
    dst.dword[i] = clamp<s128>(temp, -9223372036854775808, 9223372036854775807);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvssrani_du_q (\_\_m256i a, \_\_m256i b, imm0_127 imm)

### Synopsis

```
__m256i __lasx_xvssrani_du_q (__m256i a, __m256i b, imm0_127 imm)
#include <lasxintrin.h>
Instruction: xvssrani.du.q xr, xr, imm
CPU Flags: LASX
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
for (int i = 2; i < 4; i++) {
  if (i < 3) {
    s128 temp = (s128)b.qword[i - 1] >> imm;
    dst.dword[i] = clamp<s128>(temp, 0, 18446744073709551615);
  } else {
    s128 temp = (s128)a.qword[i - 2] >> imm;
    dst.dword[i] = clamp<s128>(temp, 0, 18446744073709551615);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvssrarn_b_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssrarn_b_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssrarn.b.h xr, xr, xr
CPU Flags: LASX
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
for (int i = 16; i < 32; i++) {
  if (i < 24) {
    s16 temp;
    if ((b.half[i - 8] & 15) == 0) {
      temp = (s16)a.half[i - 8];
    } else {
      temp = ((s16)a.half[i - 8] >> (b.half[i - 8] & 15)) +
             (((s16)a.half[i - 8] >> ((b.half[i - 8] & 15) - 1)) & 1);
    }
    dst.byte[i] = clamp<s16>(temp, -128, 127);
  } else {
    dst.byte[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrarn_bu_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssrarn_bu_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssrarn.bu.h xr, xr, xr
CPU Flags: LASX
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
for (int i = 16; i < 32; i++) {
  if (i < 24) {
    s16 temp;
    if ((b.half[i - 8] & 15) == 0) {
      temp = (s16)a.half[i - 8];
    } else {
      temp = ((s16)a.half[i - 8] >> (b.half[i - 8] & 15)) +
             (((s16)a.half[i - 8] >> ((b.half[i - 8] & 15) - 1)) & 1);
    }
    dst.byte[i] = clamp<s16>(temp, 0, 255);
  } else {
    dst.byte[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrarn_h_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssrarn_h_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssrarn.h.w xr, xr, xr
CPU Flags: LASX
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
for (int i = 8; i < 16; i++) {
  if (i < 12) {
    s32 temp;
    if ((b.word[i - 4] & 31) == 0) {
      temp = (s32)a.word[i - 4];
    } else {
      temp = ((s32)a.word[i - 4] >> (b.word[i - 4] & 31)) +
             (((s32)a.word[i - 4] >> ((b.word[i - 4] & 31) - 1)) & 1);
    }
    dst.half[i] = clamp<s32>(temp, -32768, 32767);
  } else {
    dst.half[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrarn_hu_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssrarn_hu_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssrarn.hu.w xr, xr, xr
CPU Flags: LASX
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
for (int i = 8; i < 16; i++) {
  if (i < 12) {
    s32 temp;
    if ((b.word[i - 4] & 31) == 0) {
      temp = (s32)a.word[i - 4];
    } else {
      temp = ((s32)a.word[i - 4] >> (b.word[i - 4] & 31)) +
             (((s32)a.word[i - 4] >> ((b.word[i - 4] & 31) - 1)) & 1);
    }
    dst.half[i] = clamp<s32>(temp, 0, 65535);
  } else {
    dst.half[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrarn_w_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssrarn_w_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssrarn.w.d xr, xr, xr
CPU Flags: LASX
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
for (int i = 4; i < 8; i++) {
  if (i < 6) {
    s64 temp;
    if ((b.dword[i - 2] & 63) == 0) {
      temp = (s64)a.dword[i - 2];
    } else {
      temp = ((s64)a.dword[i - 2] >> (b.dword[i - 2] & 63)) +
             (((s64)a.dword[i - 2] >> ((b.dword[i - 2] & 63) - 1)) & 1);
    }
    dst.word[i] = clamp<s64>(temp, -2147483648, 2147483647);
  } else {
    dst.word[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrarn_wu_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssrarn_wu_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssrarn.wu.d xr, xr, xr
CPU Flags: LASX
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
for (int i = 4; i < 8; i++) {
  if (i < 6) {
    s64 temp;
    if ((b.dword[i - 2] & 63) == 0) {
      temp = (s64)a.dword[i - 2];
    } else {
      temp = ((s64)a.dword[i - 2] >> (b.dword[i - 2] & 63)) +
             (((s64)a.dword[i - 2] >> ((b.dword[i - 2] & 63) - 1)) & 1);
    }
    dst.word[i] = clamp<s64>(temp, 0, 4294967295);
  } else {
    dst.word[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrarni_b_h (\_\_m256i a, \_\_m256i b, imm0_15 imm)

### Synopsis

```
__m256i __lasx_xvssrarni_b_h (__m256i a, __m256i b, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvssrarni.b.h xr, xr, imm
CPU Flags: LASX
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
for (int i = 16; i < 32; i++) {
  if (i < 24) {
    s16 temp;
    if (imm == 0) {
      temp = (s16)b.half[i - 8];
    } else {
      temp =
          ((s16)b.half[i - 8] >> imm) + (((s16)b.half[i - 8] >> (imm - 1)) & 1);
    }
    dst.byte[i] = clamp<s16>(temp, -128, 127);
  } else {
    s16 temp;
    if (imm == 0) {
      temp = (s16)a.half[i - 16];
    } else {
      temp = ((s16)a.half[i - 16] >> imm) +
             (((s16)a.half[i - 16] >> (imm - 1)) & 1);
    }
    dst.byte[i] = clamp<s16>(temp, -128, 127);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrarni_bu_h (\_\_m256i a, \_\_m256i b, imm0_15 imm)

### Synopsis

```
__m256i __lasx_xvssrarni_bu_h (__m256i a, __m256i b, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvssrarni.bu.h xr, xr, imm
CPU Flags: LASX
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
for (int i = 16; i < 32; i++) {
  if (i < 24) {
    s16 temp;
    if (imm == 0) {
      temp = (s16)b.half[i - 8];
    } else {
      temp =
          ((s16)b.half[i - 8] >> imm) + (((s16)b.half[i - 8] >> (imm - 1)) & 1);
    }
    dst.byte[i] = clamp<s16>(temp, 0, 255);
  } else {
    s16 temp;
    if (imm == 0) {
      temp = (s16)a.half[i - 16];
    } else {
      temp = ((s16)a.half[i - 16] >> imm) +
             (((s16)a.half[i - 16] >> (imm - 1)) & 1);
    }
    dst.byte[i] = clamp<s16>(temp, 0, 255);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrarni_h_w (\_\_m256i a, \_\_m256i b, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvssrarni_h_w (__m256i a, __m256i b, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvssrarni.h.w xr, xr, imm
CPU Flags: LASX
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
for (int i = 8; i < 16; i++) {
  if (i < 12) {
    s32 temp;
    if (imm == 0) {
      temp = (s32)b.word[i - 4];
    } else {
      temp =
          ((s32)b.word[i - 4] >> imm) + (((s32)b.word[i - 4] >> (imm - 1)) & 1);
    }
    dst.half[i] = clamp<s32>(temp, -32768, 32767);
  } else {
    s32 temp;
    if (imm == 0) {
      temp = (s32)a.word[i - 8];
    } else {
      temp =
          ((s32)a.word[i - 8] >> imm) + (((s32)a.word[i - 8] >> (imm - 1)) & 1);
    }
    dst.half[i] = clamp<s32>(temp, -32768, 32767);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrarni_hu_w (\_\_m256i a, \_\_m256i b, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvssrarni_hu_w (__m256i a, __m256i b, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvssrarni.hu.w xr, xr, imm
CPU Flags: LASX
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
for (int i = 8; i < 16; i++) {
  if (i < 12) {
    s32 temp;
    if (imm == 0) {
      temp = (s32)b.word[i - 4];
    } else {
      temp =
          ((s32)b.word[i - 4] >> imm) + (((s32)b.word[i - 4] >> (imm - 1)) & 1);
    }
    dst.half[i] = clamp<s32>(temp, 0, 65535);
  } else {
    s32 temp;
    if (imm == 0) {
      temp = (s32)a.word[i - 8];
    } else {
      temp =
          ((s32)a.word[i - 8] >> imm) + (((s32)a.word[i - 8] >> (imm - 1)) & 1);
    }
    dst.half[i] = clamp<s32>(temp, 0, 65535);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrarni_w_d (\_\_m256i a, \_\_m256i b, imm0_63 imm)

### Synopsis

```
__m256i __lasx_xvssrarni_w_d (__m256i a, __m256i b, imm0_63 imm)
#include <lasxintrin.h>
Instruction: xvssrarni.w.d xr, xr, imm
CPU Flags: LASX
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
for (int i = 4; i < 8; i++) {
  if (i < 6) {
    s64 temp;
    if (imm == 0) {
      temp = (s64)b.dword[i - 2];
    } else {
      temp = ((s64)b.dword[i - 2] >> imm) +
             (((s64)b.dword[i - 2] >> (imm - 1)) & 1);
    }
    dst.word[i] = clamp<s64>(temp, -2147483648, 2147483647);
  } else {
    s64 temp;
    if (imm == 0) {
      temp = (s64)a.dword[i - 4];
    } else {
      temp = ((s64)a.dword[i - 4] >> imm) +
             (((s64)a.dword[i - 4] >> (imm - 1)) & 1);
    }
    dst.word[i] = clamp<s64>(temp, -2147483648, 2147483647);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrarni_wu_d (\_\_m256i a, \_\_m256i b, imm0_63 imm)

### Synopsis

```
__m256i __lasx_xvssrarni_wu_d (__m256i a, __m256i b, imm0_63 imm)
#include <lasxintrin.h>
Instruction: xvssrarni.wu.d xr, xr, imm
CPU Flags: LASX
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
for (int i = 4; i < 8; i++) {
  if (i < 6) {
    s64 temp;
    if (imm == 0) {
      temp = (s64)b.dword[i - 2];
    } else {
      temp = ((s64)b.dword[i - 2] >> imm) +
             (((s64)b.dword[i - 2] >> (imm - 1)) & 1);
    }
    dst.word[i] = clamp<s64>(temp, 0, 4294967295);
  } else {
    s64 temp;
    if (imm == 0) {
      temp = (s64)a.dword[i - 4];
    } else {
      temp = ((s64)a.dword[i - 4] >> imm) +
             (((s64)a.dword[i - 4] >> (imm - 1)) & 1);
    }
    dst.word[i] = clamp<s64>(temp, 0, 4294967295);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrarni_d_q (\_\_m256i a, \_\_m256i b, imm0_127 imm)

### Synopsis

```
__m256i __lasx_xvssrarni_d_q (__m256i a, __m256i b, imm0_127 imm)
#include <lasxintrin.h>
Instruction: xvssrarni.d.q xr, xr, imm
CPU Flags: LASX
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
for (int i = 2; i < 4; i++) {
  if (i < 3) {
    s128 temp;
    if (imm == 0) {
      temp = (s128)b.qword[i - 1];
    } else {
      temp = ((s128)b.qword[i - 1] >> imm) +
             (((s128)b.qword[i - 1] >> (imm - 1)) & 1);
    }
    dst.dword[i] = clamp<s128>(temp, -9223372036854775808, 9223372036854775807);
  } else {
    s128 temp;
    if (imm == 0) {
      temp = (s128)a.qword[i - 2];
    } else {
      temp = ((s128)a.qword[i - 2] >> imm) +
             (((s128)a.qword[i - 2] >> (imm - 1)) & 1);
    }
    dst.dword[i] = clamp<s128>(temp, -9223372036854775808, 9223372036854775807);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvssrarni_du_q (\_\_m256i a, \_\_m256i b, imm0_127 imm)

### Synopsis

```
__m256i __lasx_xvssrarni_du_q (__m256i a, __m256i b, imm0_127 imm)
#include <lasxintrin.h>
Instruction: xvssrarni.du.q xr, xr, imm
CPU Flags: LASX
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
for (int i = 2; i < 4; i++) {
  if (i < 3) {
    s128 temp;
    if (imm == 0) {
      temp = (s128)b.qword[i - 1];
    } else {
      temp = ((s128)b.qword[i - 1] >> imm) +
             (((s128)b.qword[i - 1] >> (imm - 1)) & 1);
    }
    dst.dword[i] = clamp<s128>(temp, 0, 18446744073709551615);
  } else {
    s128 temp;
    if (imm == 0) {
      temp = (s128)a.qword[i - 2];
    } else {
      temp = ((s128)a.qword[i - 2] >> imm) +
             (((s128)a.qword[i - 2] >> (imm - 1)) & 1);
    }
    dst.dword[i] = clamp<s128>(temp, 0, 18446744073709551615);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvssrln_b_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssrln_b_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssrln.b.h xr, xr, xr
CPU Flags: LASX
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
for (int i = 16; i < 32; i++) {
  if (i < 24) {
    u16 temp = (u16)a.half[i - 8] >> (b.half[i - 8] & 15);
    dst.byte[i] = clamp<u16>(temp, 0, 127);
  } else {
    dst.byte[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrln_bu_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssrln_bu_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssrln.bu.h xr, xr, xr
CPU Flags: LASX
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
for (int i = 16; i < 32; i++) {
  if (i < 24) {
    u16 temp = (u16)a.half[i - 8] >> (b.half[i - 8] & 15);
    dst.byte[i] = clamp<u16>(temp, 0, 255);
  } else {
    dst.byte[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrln_h_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssrln_h_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssrln.h.w xr, xr, xr
CPU Flags: LASX
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
for (int i = 8; i < 16; i++) {
  if (i < 12) {
    u32 temp = (u32)a.word[i - 4] >> (b.word[i - 4] & 31);
    dst.half[i] = clamp<u32>(temp, 0, 32767);
  } else {
    dst.half[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrln_hu_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssrln_hu_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssrln.hu.w xr, xr, xr
CPU Flags: LASX
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
for (int i = 8; i < 16; i++) {
  if (i < 12) {
    u32 temp = (u32)a.word[i - 4] >> (b.word[i - 4] & 31);
    dst.half[i] = clamp<u32>(temp, 0, 65535);
  } else {
    dst.half[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrln_w_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssrln_w_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssrln.w.d xr, xr, xr
CPU Flags: LASX
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
for (int i = 4; i < 8; i++) {
  if (i < 6) {
    u64 temp = (u64)a.dword[i - 2] >> (b.dword[i - 2] & 63);
    dst.word[i] = clamp<u64>(temp, 0, 2147483647);
  } else {
    dst.word[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrln_wu_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssrln_wu_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssrln.wu.d xr, xr, xr
CPU Flags: LASX
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
for (int i = 4; i < 8; i++) {
  if (i < 6) {
    u64 temp = (u64)a.dword[i - 2] >> (b.dword[i - 2] & 63);
    dst.word[i] = clamp<u64>(temp, 0, 4294967295);
  } else {
    dst.word[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrlni_b_h (\_\_m256i a, \_\_m256i b, imm0_15 imm)

### Synopsis

```
__m256i __lasx_xvssrlni_b_h (__m256i a, __m256i b, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvssrlni.b.h xr, xr, imm
CPU Flags: LASX
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
for (int i = 16; i < 32; i++) {
  if (i < 24) {
    u16 temp = (u16)b.half[i - 8] >> imm;
    dst.byte[i] = clamp<u16>(temp, 0, 127);
  } else {
    u16 temp = (u16)a.half[i - 16] >> imm;
    dst.byte[i] = clamp<u16>(temp, 0, 127);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrlni_bu_h (\_\_m256i a, \_\_m256i b, imm0_15 imm)

### Synopsis

```
__m256i __lasx_xvssrlni_bu_h (__m256i a, __m256i b, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvssrlni.bu.h xr, xr, imm
CPU Flags: LASX
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
for (int i = 16; i < 32; i++) {
  if (i < 24) {
    u16 temp = (u16)b.half[i - 8] >> imm;
    dst.byte[i] = clamp<u16>(temp, 0, 255);
  } else {
    u16 temp = (u16)a.half[i - 16] >> imm;
    dst.byte[i] = clamp<u16>(temp, 0, 255);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrlni_h_w (\_\_m256i a, \_\_m256i b, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvssrlni_h_w (__m256i a, __m256i b, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvssrlni.h.w xr, xr, imm
CPU Flags: LASX
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
for (int i = 8; i < 16; i++) {
  if (i < 12) {
    u32 temp = (u32)b.word[i - 4] >> imm;
    dst.half[i] = clamp<u32>(temp, 0, 32767);
  } else {
    u32 temp = (u32)a.word[i - 8] >> imm;
    dst.half[i] = clamp<u32>(temp, 0, 32767);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrlni_hu_w (\_\_m256i a, \_\_m256i b, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvssrlni_hu_w (__m256i a, __m256i b, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvssrlni.hu.w xr, xr, imm
CPU Flags: LASX
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
for (int i = 8; i < 16; i++) {
  if (i < 12) {
    u32 temp = (u32)b.word[i - 4] >> imm;
    dst.half[i] = clamp<u32>(temp, 0, 65535);
  } else {
    u32 temp = (u32)a.word[i - 8] >> imm;
    dst.half[i] = clamp<u32>(temp, 0, 65535);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrlni_w_d (\_\_m256i a, \_\_m256i b, imm0_63 imm)

### Synopsis

```
__m256i __lasx_xvssrlni_w_d (__m256i a, __m256i b, imm0_63 imm)
#include <lasxintrin.h>
Instruction: xvssrlni.w.d xr, xr, imm
CPU Flags: LASX
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
for (int i = 4; i < 8; i++) {
  if (i < 6) {
    u64 temp = (u64)b.dword[i - 2] >> imm;
    dst.word[i] = clamp<u64>(temp, 0, 2147483647);
  } else {
    u64 temp = (u64)a.dword[i - 4] >> imm;
    dst.word[i] = clamp<u64>(temp, 0, 2147483647);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrlni_wu_d (\_\_m256i a, \_\_m256i b, imm0_63 imm)

### Synopsis

```
__m256i __lasx_xvssrlni_wu_d (__m256i a, __m256i b, imm0_63 imm)
#include <lasxintrin.h>
Instruction: xvssrlni.wu.d xr, xr, imm
CPU Flags: LASX
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
for (int i = 4; i < 8; i++) {
  if (i < 6) {
    u64 temp = (u64)b.dword[i - 2] >> imm;
    dst.word[i] = clamp<u64>(temp, 0, 4294967295);
  } else {
    u64 temp = (u64)a.dword[i - 4] >> imm;
    dst.word[i] = clamp<u64>(temp, 0, 4294967295);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrlni_d_q (\_\_m256i a, \_\_m256i b, imm0_127 imm)

### Synopsis

```
__m256i __lasx_xvssrlni_d_q (__m256i a, __m256i b, imm0_127 imm)
#include <lasxintrin.h>
Instruction: xvssrlni.d.q xr, xr, imm
CPU Flags: LASX
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
for (int i = 2; i < 4; i++) {
  if (i < 3) {
    u128 temp = (u128)b.qword[i - 1] >> imm;
    dst.dword[i] = clamp<u128>(temp, 0, 9223372036854775807);
  } else {
    u128 temp = (u128)a.qword[i - 2] >> imm;
    dst.dword[i] = clamp<u128>(temp, 0, 9223372036854775807);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvssrlni_du_q (\_\_m256i a, \_\_m256i b, imm0_127 imm)

### Synopsis

```
__m256i __lasx_xvssrlni_du_q (__m256i a, __m256i b, imm0_127 imm)
#include <lasxintrin.h>
Instruction: xvssrlni.du.q xr, xr, imm
CPU Flags: LASX
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
for (int i = 2; i < 4; i++) {
  if (i < 3) {
    u128 temp = (u128)b.qword[i - 1] >> imm;
    dst.dword[i] = clamp<u128>(temp, 0, 18446744073709551615);
  } else {
    u128 temp = (u128)a.qword[i - 2] >> imm;
    dst.dword[i] = clamp<u128>(temp, 0, 18446744073709551615);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvssrlrn_b_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssrlrn_b_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssrlrn.b.h xr, xr, xr
CPU Flags: LASX
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
for (int i = 16; i < 32; i++) {
  if (i < 24) {
    u16 temp;
    if ((b.half[i - 8] & 15) == 0) {
      temp = (u16)a.half[i - 8];
    } else {
      temp = ((u16)a.half[i - 8] >> (b.half[i - 8] & 15)) +
             (((u16)a.half[i - 8] >> ((b.half[i - 8] & 15) - 1)) & 1);
    }
    dst.byte[i] = clamp<u16>(temp, 0, 127);
  } else {
    dst.byte[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrlrn_bu_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssrlrn_bu_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssrlrn.bu.h xr, xr, xr
CPU Flags: LASX
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
for (int i = 16; i < 32; i++) {
  if (i < 24) {
    u16 temp;
    if ((b.half[i - 8] & 15) == 0) {
      temp = (u16)a.half[i - 8];
    } else {
      temp = ((u16)a.half[i - 8] >> (b.half[i - 8] & 15)) +
             (((u16)a.half[i - 8] >> ((b.half[i - 8] & 15) - 1)) & 1);
    }
    dst.byte[i] = clamp<u16>(temp, 0, 255);
  } else {
    dst.byte[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrlrn_h_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssrlrn_h_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssrlrn.h.w xr, xr, xr
CPU Flags: LASX
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
for (int i = 8; i < 16; i++) {
  if (i < 12) {
    u32 temp;
    if ((b.word[i - 4] & 31) == 0) {
      temp = (u32)a.word[i - 4];
    } else {
      temp = ((u32)a.word[i - 4] >> (b.word[i - 4] & 31)) +
             (((u32)a.word[i - 4] >> ((b.word[i - 4] & 31) - 1)) & 1);
    }
    dst.half[i] = clamp<u32>(temp, 0, 32767);
  } else {
    dst.half[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrlrn_hu_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssrlrn_hu_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssrlrn.hu.w xr, xr, xr
CPU Flags: LASX
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
for (int i = 8; i < 16; i++) {
  if (i < 12) {
    u32 temp;
    if ((b.word[i - 4] & 31) == 0) {
      temp = (u32)a.word[i - 4];
    } else {
      temp = ((u32)a.word[i - 4] >> (b.word[i - 4] & 31)) +
             (((u32)a.word[i - 4] >> ((b.word[i - 4] & 31) - 1)) & 1);
    }
    dst.half[i] = clamp<u32>(temp, 0, 65535);
  } else {
    dst.half[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrlrn_w_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssrlrn_w_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssrlrn.w.d xr, xr, xr
CPU Flags: LASX
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
for (int i = 4; i < 8; i++) {
  if (i < 6) {
    u64 temp;
    if ((b.dword[i - 2] & 63) == 0) {
      temp = (u64)a.dword[i - 2];
    } else {
      temp = ((u64)a.dword[i - 2] >> (b.dword[i - 2] & 63)) +
             (((u64)a.dword[i - 2] >> ((b.dword[i - 2] & 63) - 1)) & 1);
    }
    dst.word[i] = clamp<u64>(temp, 0, 2147483647);
  } else {
    dst.word[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrlrn_wu_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssrlrn_wu_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssrlrn.wu.d xr, xr, xr
CPU Flags: LASX
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
for (int i = 4; i < 8; i++) {
  if (i < 6) {
    u64 temp;
    if ((b.dword[i - 2] & 63) == 0) {
      temp = (u64)a.dword[i - 2];
    } else {
      temp = ((u64)a.dword[i - 2] >> (b.dword[i - 2] & 63)) +
             (((u64)a.dword[i - 2] >> ((b.dword[i - 2] & 63) - 1)) & 1);
    }
    dst.word[i] = clamp<u64>(temp, 0, 4294967295);
  } else {
    dst.word[i] = 0;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrlrni_b_h (\_\_m256i a, \_\_m256i b, imm0_15 imm)

### Synopsis

```
__m256i __lasx_xvssrlrni_b_h (__m256i a, __m256i b, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvssrlrni.b.h xr, xr, imm
CPU Flags: LASX
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
for (int i = 16; i < 32; i++) {
  if (i < 24) {
    u16 temp;
    if (imm == 0) {
      temp = (u16)b.half[i - 8];
    } else {
      temp =
          ((u16)b.half[i - 8] >> imm) + (((u16)b.half[i - 8] >> (imm - 1)) & 1);
    }
    dst.byte[i] = clamp<u16>(temp, 0, 127);
  } else {
    u16 temp;
    if (imm == 0) {
      temp = (u16)a.half[i - 16];
    } else {
      temp = ((u16)a.half[i - 16] >> imm) +
             (((u16)a.half[i - 16] >> (imm - 1)) & 1);
    }
    dst.byte[i] = clamp<u16>(temp, 0, 127);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrlrni_bu_h (\_\_m256i a, \_\_m256i b, imm0_15 imm)

### Synopsis

```
__m256i __lasx_xvssrlrni_bu_h (__m256i a, __m256i b, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvssrlrni.bu.h xr, xr, imm
CPU Flags: LASX
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
for (int i = 16; i < 32; i++) {
  if (i < 24) {
    u16 temp;
    if (imm == 0) {
      temp = (u16)b.half[i - 8];
    } else {
      temp =
          ((u16)b.half[i - 8] >> imm) + (((u16)b.half[i - 8] >> (imm - 1)) & 1);
    }
    dst.byte[i] = clamp<u16>(temp, 0, 255);
  } else {
    u16 temp;
    if (imm == 0) {
      temp = (u16)a.half[i - 16];
    } else {
      temp = ((u16)a.half[i - 16] >> imm) +
             (((u16)a.half[i - 16] >> (imm - 1)) & 1);
    }
    dst.byte[i] = clamp<u16>(temp, 0, 255);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrlrni_h_w (\_\_m256i a, \_\_m256i b, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvssrlrni_h_w (__m256i a, __m256i b, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvssrlrni.h.w xr, xr, imm
CPU Flags: LASX
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
for (int i = 8; i < 16; i++) {
  if (i < 12) {
    u32 temp;
    if (imm == 0) {
      temp = (u32)b.word[i - 4];
    } else {
      temp =
          ((u32)b.word[i - 4] >> imm) + (((u32)b.word[i - 4] >> (imm - 1)) & 1);
    }
    dst.half[i] = clamp<u32>(temp, 0, 32767);
  } else {
    u32 temp;
    if (imm == 0) {
      temp = (u32)a.word[i - 8];
    } else {
      temp =
          ((u32)a.word[i - 8] >> imm) + (((u32)a.word[i - 8] >> (imm - 1)) & 1);
    }
    dst.half[i] = clamp<u32>(temp, 0, 32767);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrlrni_hu_w (\_\_m256i a, \_\_m256i b, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvssrlrni_hu_w (__m256i a, __m256i b, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvssrlrni.hu.w xr, xr, imm
CPU Flags: LASX
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
for (int i = 8; i < 16; i++) {
  if (i < 12) {
    u32 temp;
    if (imm == 0) {
      temp = (u32)b.word[i - 4];
    } else {
      temp =
          ((u32)b.word[i - 4] >> imm) + (((u32)b.word[i - 4] >> (imm - 1)) & 1);
    }
    dst.half[i] = clamp<u32>(temp, 0, 65535);
  } else {
    u32 temp;
    if (imm == 0) {
      temp = (u32)a.word[i - 8];
    } else {
      temp =
          ((u32)a.word[i - 8] >> imm) + (((u32)a.word[i - 8] >> (imm - 1)) & 1);
    }
    dst.half[i] = clamp<u32>(temp, 0, 65535);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrlrni_w_d (\_\_m256i a, \_\_m256i b, imm0_63 imm)

### Synopsis

```
__m256i __lasx_xvssrlrni_w_d (__m256i a, __m256i b, imm0_63 imm)
#include <lasxintrin.h>
Instruction: xvssrlrni.w.d xr, xr, imm
CPU Flags: LASX
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
for (int i = 4; i < 8; i++) {
  if (i < 6) {
    u64 temp;
    if (imm == 0) {
      temp = (u64)b.dword[i - 2];
    } else {
      temp = ((u64)b.dword[i - 2] >> imm) +
             (((u64)b.dword[i - 2] >> (imm - 1)) & 1);
    }
    dst.word[i] = clamp<u64>(temp, 0, 2147483647);
  } else {
    u64 temp;
    if (imm == 0) {
      temp = (u64)a.dword[i - 4];
    } else {
      temp = ((u64)a.dword[i - 4] >> imm) +
             (((u64)a.dword[i - 4] >> (imm - 1)) & 1);
    }
    dst.word[i] = clamp<u64>(temp, 0, 2147483647);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrlrni_wu_d (\_\_m256i a, \_\_m256i b, imm0_63 imm)

### Synopsis

```
__m256i __lasx_xvssrlrni_wu_d (__m256i a, __m256i b, imm0_63 imm)
#include <lasxintrin.h>
Instruction: xvssrlrni.wu.d xr, xr, imm
CPU Flags: LASX
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
for (int i = 4; i < 8; i++) {
  if (i < 6) {
    u64 temp;
    if (imm == 0) {
      temp = (u64)b.dword[i - 2];
    } else {
      temp = ((u64)b.dword[i - 2] >> imm) +
             (((u64)b.dword[i - 2] >> (imm - 1)) & 1);
    }
    dst.word[i] = clamp<u64>(temp, 0, 4294967295);
  } else {
    u64 temp;
    if (imm == 0) {
      temp = (u64)a.dword[i - 4];
    } else {
      temp = ((u64)a.dword[i - 4] >> imm) +
             (((u64)a.dword[i - 4] >> (imm - 1)) & 1);
    }
    dst.word[i] = clamp<u64>(temp, 0, 4294967295);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 1                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvssrlrni_d_q (\_\_m256i a, \_\_m256i b, imm0_127 imm)

### Synopsis

```
__m256i __lasx_xvssrlrni_d_q (__m256i a, __m256i b, imm0_127 imm)
#include <lasxintrin.h>
Instruction: xvssrlrni.d.q xr, xr, imm
CPU Flags: LASX
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
for (int i = 2; i < 4; i++) {
  if (i < 3) {
    u128 temp;
    if (imm == 0) {
      temp = (u128)b.qword[i - 1];
    } else {
      temp = ((u128)b.qword[i - 1] >> imm) +
             (((u128)b.qword[i - 1] >> (imm - 1)) & 1);
    }
    dst.dword[i] = clamp<u128>(temp, 0, 9223372036854775807);
  } else {
    u128 temp;
    if (imm == 0) {
      temp = (u128)a.qword[i - 2];
    } else {
      temp = ((u128)a.qword[i - 2] >> imm) +
             (((u128)a.qword[i - 2] >> (imm - 1)) & 1);
    }
    dst.dword[i] = clamp<u128>(temp, 0, 9223372036854775807);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvssrlrni_du_q (\_\_m256i a, \_\_m256i b, imm0_127 imm)

### Synopsis

```
__m256i __lasx_xvssrlrni_du_q (__m256i a, __m256i b, imm0_127 imm)
#include <lasxintrin.h>
Instruction: xvssrlrni.du.q xr, xr, imm
CPU Flags: LASX
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
for (int i = 2; i < 4; i++) {
  if (i < 3) {
    u128 temp;
    if (imm == 0) {
      temp = (u128)b.qword[i - 1];
    } else {
      temp = ((u128)b.qword[i - 1] >> imm) +
             (((u128)b.qword[i - 1] >> (imm - 1)) & 1);
    }
    dst.dword[i] = clamp<u128>(temp, 0, 18446744073709551615);
  } else {
    u128 temp;
    if (imm == 0) {
      temp = (u128)a.qword[i - 2];
    } else {
      temp = ((u128)a.qword[i - 2] >> imm) +
             (((u128)a.qword[i - 2] >> (imm - 1)) & 1);
    }
    dst.dword[i] = clamp<u128>(temp, 0, 18446744073709551615);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvrotr_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvrotr_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvrotr.b xr, xr, xr
CPU Flags: LASX
```

### Description

Rotate right the unsigned 8-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] =
      (a.byte[i] >> (b.byte[i] & 0x7)) | (a.byte[i] << (8 - (b.byte[i] & 0x7)));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvrotr_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvrotr_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvrotr.h xr, xr, xr
CPU Flags: LASX
```

### Description

Rotate right the unsigned 16-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = (a.half[i] >> (b.half[i] & 0xf)) |
                (a.half[i] << (16 - (b.half[i] & 0xf)));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvrotr_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvrotr_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvrotr.w xr, xr, xr
CPU Flags: LASX
```

### Description

Rotate right the unsigned 32-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (a.word[i] >> (b.word[i] & 0x1f)) |
                (a.word[i] << (32 - (b.word[i] & 0x1f)));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvrotr_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvrotr_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvrotr.d xr, xr, xr
CPU Flags: LASX
```

### Description

Rotate right the unsigned 64-bit elements in `a` by elements in `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (a.dword[i] >> (b.dword[i] & 0x3f)) |
                 (a.dword[i] << (64 - (b.dword[i] & 0x3f)));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvrotri_b (\_\_m256i a, imm0_7 imm)

### Synopsis

```
__m256i __lasx_xvrotri_b (__m256i a, imm0_7 imm)
#include <lasxintrin.h>
Instruction: xvrotri.b xr, xr, imm
CPU Flags: LASX
```

### Description

Rotate right the unsigned 8-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = (a.byte[i] >> imm) | (a.byte[i] << (8 - imm));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvrotri_h (\_\_m256i a, imm0_15 imm)

### Synopsis

```
__m256i __lasx_xvrotri_h (__m256i a, imm0_15 imm)
#include <lasxintrin.h>
Instruction: xvrotri.h xr, xr, imm
CPU Flags: LASX
```

### Description

Rotate right the unsigned 16-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = (a.half[i] >> imm) | (a.half[i] << (16 - imm));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvrotri_w (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvrotri_w (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvrotri.w xr, xr, imm
CPU Flags: LASX
```

### Description

Rotate right the unsigned 32-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (a.word[i] >> imm) | (a.word[i] << (32 - imm));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvrotri_d (\_\_m256i a, imm0_63 imm)

### Synopsis

```
__m256i __lasx_xvrotri_d (__m256i a, imm0_63 imm)
#include <lasxintrin.h>
Instruction: xvrotri.d xr, xr, imm
CPU Flags: LASX
```

### Description

Rotate right the unsigned 64-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (a.dword[i] >> imm) | (a.dword[i] << (64 - imm));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |
