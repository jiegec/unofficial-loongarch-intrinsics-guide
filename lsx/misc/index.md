# Misc

## \_\_m128i \_\_lsx_vexth_h_b (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vexth_h_b (__m128i a)
#include <lsxintrin.h>
Instruction: vexth.h.b vr, vr
CPU Flags: LSX
```

### Description

Extend signed 8-bit elements in the higher half of `a` to 16-bit.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (s16)(s8)a.byte[i + 8];
}

// Expands to:

if (0) {
  dst.half[0] = (s16)((s8)a.byte[8]);
  dst.half[1] = (s16)((s8)a.byte[9]);
  dst.half[2] = (s16)((s8)a.byte[10]);
  dst.half[3] = (s16)((s8)a.byte[11]);
  dst.half[4] = (s16)((s8)a.byte[12]);
  dst.half[5] = (s16)((s8)a.byte[13]);
  dst.half[6] = (s16)((s8)a.byte[14]);
  dst.half[7] = (s16)((s8)a.byte[15]);
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

## \_\_m128i \_\_lsx_vexth_hu_bu (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vexth_hu_bu (__m128i a)
#include <lsxintrin.h>
Instruction: vexth.hu.bu vr, vr
CPU Flags: LSX
```

### Description

Extend unsigned 8-bit elements in the higher half of `a` to 16-bit.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (u16)(u8)a.byte[i + 8];
}

// Expands to:

if (0) {
  dst.half[0] = (u16)((u8)a.byte[8]);
  dst.half[1] = (u16)((u8)a.byte[9]);
  dst.half[2] = (u16)((u8)a.byte[10]);
  dst.half[3] = (u16)((u8)a.byte[11]);
  dst.half[4] = (u16)((u8)a.byte[12]);
  dst.half[5] = (u16)((u8)a.byte[13]);
  dst.half[6] = (u16)((u8)a.byte[14]);
  dst.half[7] = (u16)((u8)a.byte[15]);
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

## \_\_m128i \_\_lsx_vexth_w_h (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vexth_w_h (__m128i a)
#include <lsxintrin.h>
Instruction: vexth.w.h vr, vr
CPU Flags: LSX
```

### Description

Extend signed 16-bit elements in the higher half of `a` to 32-bit.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (s32)(s16)a.half[i + 4];
}

// Expands to:

if (0) {
  dst.word[0] = (s32)((s16)a.half[4]);
  dst.word[1] = (s32)((s16)a.half[5]);
  dst.word[2] = (s32)((s16)a.half[6]);
  dst.word[3] = (s32)((s16)a.half[7]);
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

## \_\_m128i \_\_lsx_vexth_wu_hu (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vexth_wu_hu (__m128i a)
#include <lsxintrin.h>
Instruction: vexth.wu.hu vr, vr
CPU Flags: LSX
```

### Description

Extend unsigned 16-bit elements in the higher half of `a` to 32-bit.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (u32)(u16)a.half[i + 4];
}

// Expands to:

if (0) {
  dst.word[0] = (u32)((u16)a.half[4]);
  dst.word[1] = (u32)((u16)a.half[5]);
  dst.word[2] = (u32)((u16)a.half[6]);
  dst.word[3] = (u32)((u16)a.half[7]);
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

## \_\_m128i \_\_lsx_vexth_d_w (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vexth_d_w (__m128i a)
#include <lsxintrin.h>
Instruction: vexth.d.w vr, vr
CPU Flags: LSX
```

### Description

Extend signed 32-bit elements in the higher half of `a` to 64-bit.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)(s32)a.word[i + 2];
}

// Expands to:

if (0) {
  dst.dword[0] = (s64)((s32)a.word[2]);
  dst.dword[1] = (s64)((s32)a.word[3]);
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

## \_\_m128i \_\_lsx_vexth_du_wu (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vexth_du_wu (__m128i a)
#include <lsxintrin.h>
Instruction: vexth.du.wu vr, vr
CPU Flags: LSX
```

### Description

Extend unsigned 32-bit elements in the higher half of `a` to 64-bit.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (u64)(u32)a.word[i + 2];
}

// Expands to:

if (0) {
  dst.dword[0] = (u64)((u32)a.word[2]);
  dst.dword[1] = (u64)((u32)a.word[3]);
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

## \_\_m128i \_\_lsx_vexth_q_d (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vexth_q_d (__m128i a)
#include <lsxintrin.h>
Instruction: vexth.q.d vr, vr
CPU Flags: LSX
```

### Description

Extend signed 64-bit elements in the higher half of `a` to 128-bit.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] = (s128)(s64)a.dword[i + 1];
}

// Expands to:

if (0) {
  dst.qword[0] = (s128)((s64)a.dword[1]);
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

## \_\_m128i \_\_lsx_vexth_qu_du (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vexth_qu_du (__m128i a)
#include <lsxintrin.h>
Instruction: vexth.qu.du vr, vr
CPU Flags: LSX
```

### Description

Extend unsigned 64-bit elements in the higher half of `a` to 128-bit.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] = (u128)(u64)a.dword[i + 1];
}

// Expands to:

if (0) {
  dst.qword[0] = (u128)((u64)a.dword[1]);
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

## \_\_m128i \_\_lsx_vextl_q_d (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vextl_q_d (__m128i a)
#include <lsxintrin.h>
Instruction: vextl.q.d vr, vr
CPU Flags: LSX
```

### Description

Extend signed 64-bit elements in the lower half of `a` to 128-bit.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] = (s128)(s64)a.dword[i];
}

// Expands to:

if (0) {
  dst.qword[0] = (s128)((s64)a.dword[0]);
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

## \_\_m128i \_\_lsx_vextl_qu_du (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vextl_qu_du (__m128i a)
#include <lsxintrin.h>
Instruction: vextl.qu.du vr, vr
CPU Flags: LSX
```

### Description

Extend unsigned 64-bit elements in the lower half of `a` to 128-bit.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] = (u128)(u64)a.dword[i];
}

// Expands to:

if (0) {
  dst.qword[0] = (u128)((u64)a.dword[0]);
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

## \_\_m128i \_\_lsx_vextrins_b (\_\_m128i a, \_\_m128i b, imm0_255 imm)

### Synopsis

```
__m128i __lsx_vextrins_b (__m128i a, __m128i b, imm0_255 imm)
#include <lsxintrin.h>
Instruction: vextrins.b vr, vr, imm
CPU Flags: LSX
```

### Description

Extract one 8-bit element in `b` and insert it to `a` according to `imm`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (i == ((imm >> 4) & 15)) ? b.byte[imm & 15] : a.byte[i];
}

// Expands to:

if (0) {
  dst.byte[0] = (0 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[0]);
  dst.byte[1] = (1 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[1]);
  dst.byte[2] = (2 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[2]);
  dst.byte[3] = (3 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[3]);
  dst.byte[4] = (4 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[4]);
  dst.byte[5] = (5 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[5]);
  dst.byte[6] = (6 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[6]);
  dst.byte[7] = (7 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[7]);
  dst.byte[8] = (8 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[8]);
  dst.byte[9] = (9 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[9]);
  dst.byte[10] = (10 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[10]);
  dst.byte[11] = (11 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[11]);
  dst.byte[12] = (12 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[12]);
  dst.byte[13] = (13 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[13]);
  dst.byte[14] = (14 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[14]);
  dst.byte[15] = (15 == ((imm >> 4) & 15)) ? (b.byte[imm & 15]) : (a.byte[15]);
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

## \_\_m128i \_\_lsx_vextrins_h (\_\_m128i a, \_\_m128i b, imm0_255 imm)

### Synopsis

```
__m128i __lsx_vextrins_h (__m128i a, __m128i b, imm0_255 imm)
#include <lsxintrin.h>
Instruction: vextrins.h vr, vr, imm
CPU Flags: LSX
```

### Description

Extract one 16-bit element in `b` and insert it to `a` according to `imm`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (i == ((imm >> 4) & 7)) ? b.half[imm & 7] : a.half[i];
}

// Expands to:

if (0) {
  dst.half[0] = (0 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[0]);
  dst.half[1] = (1 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[1]);
  dst.half[2] = (2 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[2]);
  dst.half[3] = (3 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[3]);
  dst.half[4] = (4 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[4]);
  dst.half[5] = (5 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[5]);
  dst.half[6] = (6 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[6]);
  dst.half[7] = (7 == ((imm >> 4) & 7)) ? (b.half[imm & 7]) : (a.half[7]);
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

## \_\_m128i \_\_lsx_vextrins_w (\_\_m128i a, \_\_m128i b, imm0_255 imm)

### Synopsis

```
__m128i __lsx_vextrins_w (__m128i a, __m128i b, imm0_255 imm)
#include <lsxintrin.h>
Instruction: vextrins.w vr, vr, imm
CPU Flags: LSX
```

### Description

Extract one 32-bit element in `b` and insert it to `a` according to `imm`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (i == ((imm >> 4) & 3)) ? b.word[imm & 3] : a.word[i];
}

// Expands to:

if (0) {
  dst.word[0] = (0 == ((imm >> 4) & 3)) ? (b.word[imm & 3]) : (a.word[0]);
  dst.word[1] = (1 == ((imm >> 4) & 3)) ? (b.word[imm & 3]) : (a.word[1]);
  dst.word[2] = (2 == ((imm >> 4) & 3)) ? (b.word[imm & 3]) : (a.word[2]);
  dst.word[3] = (3 == ((imm >> 4) & 3)) ? (b.word[imm & 3]) : (a.word[3]);
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

## \_\_m128i \_\_lsx_vextrins_d (\_\_m128i a, \_\_m128i b, imm0_255 imm)

### Synopsis

```
__m128i __lsx_vextrins_d (__m128i a, __m128i b, imm0_255 imm)
#include <lsxintrin.h>
Instruction: vextrins.d vr, vr, imm
CPU Flags: LSX
```

### Description

Extract one 64-bit element in `b` and insert it to `a` according to `imm`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (i == ((imm >> 4) & 1)) ? b.dword[imm & 1] : a.dword[i];
}

// Expands to:

if (0) {
  dst.dword[0] = (0 == ((imm >> 4) & 1)) ? (b.dword[imm & 1]) : (a.dword[0]);
  dst.dword[1] = (1 == ((imm >> 4) & 1)) ? (b.dword[imm & 1]) : (a.dword[1]);
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

## \_\_m128i \_\_lsx_vilvh_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vilvh_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vilvh.b vr, vr, vr
CPU Flags: LSX
```

### Description

Interleave 8-bit elements in higher half of `a` and `b`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (i % 2 == 1) ? a.byte[i / 2 + 8] : b.byte[i / 2 + 8];
}

// Expands to:

if (0) {
  dst.byte[0] = b.byte[8];
  dst.byte[1] = a.byte[8];
  dst.byte[2] = b.byte[9];
  dst.byte[3] = a.byte[9];
  dst.byte[4] = b.byte[10];
  dst.byte[5] = a.byte[10];
  dst.byte[6] = b.byte[11];
  dst.byte[7] = a.byte[11];
  dst.byte[8] = b.byte[12];
  dst.byte[9] = a.byte[12];
  dst.byte[10] = b.byte[13];
  dst.byte[11] = a.byte[13];
  dst.byte[12] = b.byte[14];
  dst.byte[13] = a.byte[14];
  dst.byte[14] = b.byte[15];
  dst.byte[15] = a.byte[15];
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

## \_\_m128i \_\_lsx_vilvh_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vilvh_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vilvh.h vr, vr, vr
CPU Flags: LSX
```

### Description

Interleave 16-bit elements in higher half of `a` and `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (i % 2 == 1) ? a.half[i / 2 + 4] : b.half[i / 2 + 4];
}

// Expands to:

if (0) {
  dst.half[0] = b.half[4];
  dst.half[1] = a.half[4];
  dst.half[2] = b.half[5];
  dst.half[3] = a.half[5];
  dst.half[4] = b.half[6];
  dst.half[5] = a.half[6];
  dst.half[6] = b.half[7];
  dst.half[7] = a.half[7];
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

## \_\_m128i \_\_lsx_vilvh_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vilvh_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vilvh.w vr, vr, vr
CPU Flags: LSX
```

### Description

Interleave 32-bit elements in higher half of `a` and `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (i % 2 == 1) ? a.word[i / 2 + 2] : b.word[i / 2 + 2];
}

// Expands to:

if (0) {
  dst.word[0] = b.word[2];
  dst.word[1] = a.word[2];
  dst.word[2] = b.word[3];
  dst.word[3] = a.word[3];
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

## \_\_m128i \_\_lsx_vilvh_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vilvh_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vilvh.d vr, vr, vr
CPU Flags: LSX
```

### Description

Interleave 64-bit elements in higher half of `a` and `b`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (i % 2 == 1) ? a.dword[i / 2 + 1] : b.dword[i / 2 + 1];
}

// Expands to:

if (0) {
  dst.dword[0] = b.dword[1];
  dst.dword[1] = a.dword[1];
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

## \_\_m128i \_\_lsx_vilvl_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vilvl_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vilvl.b vr, vr, vr
CPU Flags: LSX
```

### Description

Interleave 8-bit elements in lower half of `a` and `b`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (i % 2 == 1) ? a.byte[i / 2] : b.byte[i / 2];
}

// Expands to:

if (0) {
  dst.byte[0] = b.byte[0];
  dst.byte[1] = a.byte[0];
  dst.byte[2] = b.byte[1];
  dst.byte[3] = a.byte[1];
  dst.byte[4] = b.byte[2];
  dst.byte[5] = a.byte[2];
  dst.byte[6] = b.byte[3];
  dst.byte[7] = a.byte[3];
  dst.byte[8] = b.byte[4];
  dst.byte[9] = a.byte[4];
  dst.byte[10] = b.byte[5];
  dst.byte[11] = a.byte[5];
  dst.byte[12] = b.byte[6];
  dst.byte[13] = a.byte[6];
  dst.byte[14] = b.byte[7];
  dst.byte[15] = a.byte[7];
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

## \_\_m128i \_\_lsx_vilvl_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vilvl_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vilvl.h vr, vr, vr
CPU Flags: LSX
```

### Description

Interleave 16-bit elements in lower half of `a` and `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (i % 2 == 1) ? a.half[i / 2] : b.half[i / 2];
}

// Expands to:

if (0) {
  dst.half[0] = b.half[0];
  dst.half[1] = a.half[0];
  dst.half[2] = b.half[1];
  dst.half[3] = a.half[1];
  dst.half[4] = b.half[2];
  dst.half[5] = a.half[2];
  dst.half[6] = b.half[3];
  dst.half[7] = a.half[3];
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

## \_\_m128i \_\_lsx_vilvl_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vilvl_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vilvl.w vr, vr, vr
CPU Flags: LSX
```

### Description

Interleave 32-bit elements in lower half of `a` and `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (i % 2 == 1) ? a.word[i / 2] : b.word[i / 2];
}

// Expands to:

if (0) {
  dst.word[0] = b.word[0];
  dst.word[1] = a.word[0];
  dst.word[2] = b.word[1];
  dst.word[3] = a.word[1];
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

## \_\_m128i \_\_lsx_vilvl_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vilvl_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vilvl.d vr, vr, vr
CPU Flags: LSX
```

### Description

Interleave 64-bit elements in lower half of `a` and `b`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (i % 2 == 1) ? a.dword[i / 2] : b.dword[i / 2];
}

// Expands to:

if (0) {
  dst.dword[0] = b.dword[0];
  dst.dword[1] = a.dword[0];
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

## \_\_m128i \_\_lsx_vinsgr2vr_b (\_\_m128i a, int b, imm0_15 imm)

### Synopsis

```
__m128i __lsx_vinsgr2vr_b (__m128i a, int b, imm0_15 imm)
#include <lsxintrin.h>
Instruction: vinsgr2vr.b vr, r, imm
CPU Flags: LSX
```

### Description

Insert 8-bit element into lane indexed `imm`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (i == imm) ? b : a.byte[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 1                |
| 3A6000   | LA664  | 1       | 1                |
| 3C6000   | LA664  | 1       | 1                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vinsgr2vr_h (\_\_m128i a, int b, imm0_7 imm)

### Synopsis

```
__m128i __lsx_vinsgr2vr_h (__m128i a, int b, imm0_7 imm)
#include <lsxintrin.h>
Instruction: vinsgr2vr.h vr, r, imm
CPU Flags: LSX
```

### Description

Insert 16-bit element into lane indexed `imm`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (i == imm) ? b : a.half[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 1                |
| 3A6000   | LA664  | 1       | 1                |
| 3C6000   | LA664  | 1       | 1                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vinsgr2vr_w (\_\_m128i a, int b, imm0_3 imm)

### Synopsis

```
__m128i __lsx_vinsgr2vr_w (__m128i a, int b, imm0_3 imm)
#include <lsxintrin.h>
Instruction: vinsgr2vr.w vr, r, imm
CPU Flags: LSX
```

### Description

Insert 32-bit element into lane indexed `imm`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (i == imm) ? b : a.word[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 1                |
| 3A6000   | LA664  | 1       | 1                |
| 3C6000   | LA664  | 1       | 1                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vinsgr2vr_d (\_\_m128i a, long int b, imm0_1 imm)

### Synopsis

```
__m128i __lsx_vinsgr2vr_d (__m128i a, long int b, imm0_1 imm)
#include <lsxintrin.h>
Instruction: vinsgr2vr.d vr, r, imm
CPU Flags: LSX
```

### Description

Insert 64-bit element into lane indexed `imm`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (i == imm) ? b : a.dword[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 1                |
| 3A6000   | LA664  | 1       | 1                |
| 3C6000   | LA664  | 1       | 1                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vfrstp_b (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vfrstp_b (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vfrstp.b vr, vr, vr
CPU Flags: LSX
```

### Description

Find the first negative 8-bit element in `b`, set the index of the element to the lane of `a` specified by `c`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[i];
}
int i;
for (i = 0; i < 16; i++) {
  if ((s8)b.byte[i] < 0) {
    break;
  }
}
dst.byte[c.byte[0] % 16] = i;
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vfrstp_h (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vfrstp_h (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vfrstp.h vr, vr, vr
CPU Flags: LSX
```

### Description

Find the first negative 16-bit element in `b`, set the index of the element to the lane of `a` specified by `c`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = a.half[i];
}
int i;
for (i = 0; i < 8; i++) {
  if ((s16)b.half[i] < 0) {
    break;
  }
}
dst.half[c.half[0] % 8] = i;
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vfrstpi_b (\_\_m128i a, \_\_m128i b, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vfrstpi_b (__m128i a, __m128i b, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vfrstpi.b vr, vr, imm
CPU Flags: LSX
```

### Description

Find the first negative 8-bit element in `b`, set the index of the element to the lane of `a` specified by `imm`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[i];
}
int i;
for (i = 0; i < 16; i++) {
  if ((s8)b.byte[i] < 0) {
    break;
  }
}
dst.byte[imm % 16] = i;
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vfrstpi_h (\_\_m128i a, \_\_m128i b, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vfrstpi_h (__m128i a, __m128i b, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vfrstpi.h vr, vr, imm
CPU Flags: LSX
```

### Description

Find the first negative 16-bit element in `b`, set the index of the element to the lane of `a` specified by `imm`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = a.half[i];
}
int i;
for (i = 0; i < 8; i++) {
  if ((s16)b.half[i] < 0) {
    break;
  }
}
dst.half[imm % 8] = i;
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vmskgez_b (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vmskgez_b (__m128i a)
#include <lsxintrin.h>
Instruction: vmskgez.b vr, vr
CPU Flags: LSX
```

### Description

For each 8-bit element in `a`, if the element is greater than or equal to zero, set one bit in `dst`, otherwise clear it.

### Examples

```
__m128i __lsx_vmskgez_b(__m128i{0x1122334455667788, 0x99aabbccddeeff00})
= 0x00000000000001fe 0x0000000000000000
__m128i __lsx_vmskgez_b(__m128i{0x0000808000000000, 0x0081000081716151})
= 0x000000000000b7cf 0x0000000000000000
```

### Operation

```
u64 m = 0x8080808080808080;
u64 c = m & a.dword[0];
c |= c << 7;
c |= c << 14;
c |= c << 28;
c >>= 56;
dst.dword[0] = c;
c = m & a.dword[1];
c |= c << 7;
c |= c << 14;
c |= c << 28;
c >>= 56;
dst.dword[0] |= c << 8;
dst.dword[0] = (u16)~dst.dword[0];
dst.dword[1] = 0;
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

## \_\_m128i \_\_lsx_vmskltz_b (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vmskltz_b (__m128i a)
#include <lsxintrin.h>
Instruction: vmskltz.b vr, vr
CPU Flags: LSX
```

### Description

For each 8-bit element in `a`, if the element is less than zero, set one bit in `dst`, otherwise clear it.

### Examples

```
__m128i __lsx_vmskltz_b(__m128i{0x1122334455667788, 0x99aabbccddeeff00})
= 0x000000000000fe01 0x0000000000000000
__m128i __lsx_vmskltz_b(__m128i{0x0000808000000000, 0x0081000081716151})
= 0x0000000000004830 0x0000000000000000
```

### Operation

```
u64 m = 0x8080808080808080;
u64 c = m & a.dword[0];
c |= c << 7;
c |= c << 14;
c |= c << 28;
c >>= 56;
dst.dword[0] = c;
c = m & a.dword[1];
c |= c << 7;
c |= c << 14;
c |= c << 28;
c >>= 56;
dst.dword[0] |= c << 8;
dst.dword[1] = 0;
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

## \_\_m128i \_\_lsx_vmskltz_h (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vmskltz_h (__m128i a)
#include <lsxintrin.h>
Instruction: vmskltz.h vr, vr
CPU Flags: LSX
```

### Description

For each 16-bit element in `a`, if the element is less than zero, set one bit in `dst`, otherwise clear it.

### Examples

```
__m128i __lsx_vmskltz_h(__m128i{0x1122334455667788, 0x99aabbccddeeff00})
= 0x00000000000000f0 0x0000000000000000
__m128i __lsx_vmskltz_h(__m128i{0x0000808000000000, 0x0081000081716151})
= 0x0000000000000024 0x0000000000000000
```

### Operation

```
u64 m = 0x8000800080008000;
u64 c = m & a.dword[0];
c |= c << 15;
c |= c << 30;
c >>= 60;
dst.dword[0] = c;
c = m & a.dword[1];
c |= c << 15;
c |= c << 30;
c >>= 60;
dst.dword[0] |= c << 4;
dst.dword[1] = 0;
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

## \_\_m128i \_\_lsx_vmskltz_w (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vmskltz_w (__m128i a)
#include <lsxintrin.h>
Instruction: vmskltz.w vr, vr
CPU Flags: LSX
```

### Description

For each 32-bit element in `a`, if the element is less than zero, set one bit in `dst`, otherwise clear it.

### Examples

```
__m128i __lsx_vmskltz_w(__m128i{0x1122334455667788, 0x99aabbccddeeff00})
= 0x000000000000000c 0x0000000000000000
__m128i __lsx_vmskltz_w(__m128i{0x0000808000000000, 0x0081000081716151})
= 0x0000000000000004 0x0000000000000000
```

### Operation

```
u64 m = 0x8000000080000000;
u64 c = m & a.dword[0];
c |= c << 31;
c >>= 62;
dst.dword[0] = c;
c = m & a.dword[1];
c |= c << 31;
c >>= 62;
dst.dword[0] |= c << 2;
dst.dword[1] = 0;
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

## \_\_m128i \_\_lsx_vmskltz_d (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vmskltz_d (__m128i a)
#include <lsxintrin.h>
Instruction: vmskltz.d vr, vr
CPU Flags: LSX
```

### Description

For each 64-bit element in `a`, if the element is less than zero, set one bit in `dst`, otherwise clear it.

### Examples

```
__m128i __lsx_vmskltz_d(__m128i{0x1122334455667788, 0x99aabbccddeeff00})
= 0x0000000000000002 0x0000000000000000
__m128i __lsx_vmskltz_d(__m128i{0x0000808000000000, 0x0081000081716151})
= 0x0000000000000000 0x0000000000000000
```

### Operation

```
u64 m = 0x8000000000000000;
u64 c = m & a.dword[0];
c >>= 63;
dst.dword[0] = c;
c = m & a.dword[1];
c >>= 63;
dst.dword[0] |= c << 1;
dst.dword[1] = 0;
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

## \_\_m128i \_\_lsx_vmsknz_b (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vmsknz_b (__m128i a)
#include <lsxintrin.h>
Instruction: vmsknz.b vr, vr
CPU Flags: LSX
```

### Description

For each 8-bit element in `a`, if the element is non-zero, set one bit in `dst`, otherwise clear it.

### Examples

```
__m128i __lsx_vmsknz_b(__m128i{0x1122334455667788, 0x99aabbccddeeff00})
= 0x000000000000feff 0x0000000000000000
__m128i __lsx_vmsknz_b(__m128i{0x0000111100000000, 0x0011000011111111})
= 0x0000000000004f30 0x0000000000000000
```

### Operation

```
u64 m = 0x7F7F7F7F7F7F7F7F;
u64 c = ~(((a.dword[0] & m) + m) | a.dword[0] | m);
c |= c << 7;
c |= c << 14;
c |= c << 28;
c >>= 56;
dst.dword[0] = c;
c = ~(((a.dword[1] & m) + m) | a.dword[1] | m);
c |= c << 7;
c |= c << 14;
c |= c << 28;
c >>= 56;
dst.dword[0] |= c << 8;
dst.dword[0] = (u16)~dst.dword[0];
dst.dword[1] = 0;
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

## \_\_m128i \_\_lsx_vpackev_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vpackev_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vpackev.b vr, vr, vr
CPU Flags: LSX
```

### Description

Collect and pack even-positioned 8-bit elements in `a` and `b` and store `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (i % 2 == 1) ? a.byte[i - 1] : b.byte[i];
}

// Expands to:

if (0) {
  dst.byte[0] = b.byte[0];
  dst.byte[1] = a.byte[0];
  dst.byte[2] = b.byte[2];
  dst.byte[3] = a.byte[2];
  dst.byte[4] = b.byte[4];
  dst.byte[5] = a.byte[4];
  dst.byte[6] = b.byte[6];
  dst.byte[7] = a.byte[6];
  dst.byte[8] = b.byte[8];
  dst.byte[9] = a.byte[8];
  dst.byte[10] = b.byte[10];
  dst.byte[11] = a.byte[10];
  dst.byte[12] = b.byte[12];
  dst.byte[13] = a.byte[12];
  dst.byte[14] = b.byte[14];
  dst.byte[15] = a.byte[14];
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

## \_\_m128i \_\_lsx_vpackev_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vpackev_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vpackev.h vr, vr, vr
CPU Flags: LSX
```

### Description

Collect and pack even-positioned 16-bit elements in `a` and `b` and store `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (i % 2 == 1) ? a.half[i - 1] : b.half[i];
}

// Expands to:

if (0) {
  dst.half[0] = b.half[0];
  dst.half[1] = a.half[0];
  dst.half[2] = b.half[2];
  dst.half[3] = a.half[2];
  dst.half[4] = b.half[4];
  dst.half[5] = a.half[4];
  dst.half[6] = b.half[6];
  dst.half[7] = a.half[6];
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

## \_\_m128i \_\_lsx_vpackev_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vpackev_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vpackev.w vr, vr, vr
CPU Flags: LSX
```

### Description

Collect and pack even-positioned 32-bit elements in `a` and `b` and store `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (i % 2 == 1) ? a.word[i - 1] : b.word[i];
}

// Expands to:

if (0) {
  dst.word[0] = b.word[0];
  dst.word[1] = a.word[0];
  dst.word[2] = b.word[2];
  dst.word[3] = a.word[2];
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

## \_\_m128i \_\_lsx_vpackev_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vpackev_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vpackev.d vr, vr, vr
CPU Flags: LSX
```

### Description

Collect and pack even-positioned 64-bit elements in `a` and `b` and store `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (i % 2 == 1) ? a.dword[i - 1] : b.dword[i];
}

// Expands to:

if (0) {
  dst.dword[0] = b.dword[0];
  dst.dword[1] = a.dword[0];
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

## \_\_m128i \_\_lsx_vpackod_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vpackod_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vpackod.b vr, vr, vr
CPU Flags: LSX
```

### Description

Collect and pack odd-positioned 8-bit elements in `a` and `b` and store `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (i % 2 == 1) ? a.byte[i] : b.byte[i + 1];
}

// Expands to:

if (0) {
  dst.byte[0] = b.byte[1];
  dst.byte[1] = a.byte[1];
  dst.byte[2] = b.byte[3];
  dst.byte[3] = a.byte[3];
  dst.byte[4] = b.byte[5];
  dst.byte[5] = a.byte[5];
  dst.byte[6] = b.byte[7];
  dst.byte[7] = a.byte[7];
  dst.byte[8] = b.byte[9];
  dst.byte[9] = a.byte[9];
  dst.byte[10] = b.byte[11];
  dst.byte[11] = a.byte[11];
  dst.byte[12] = b.byte[13];
  dst.byte[13] = a.byte[13];
  dst.byte[14] = b.byte[15];
  dst.byte[15] = a.byte[15];
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

## \_\_m128i \_\_lsx_vpackod_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vpackod_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vpackod.h vr, vr, vr
CPU Flags: LSX
```

### Description

Collect and pack odd-positioned 16-bit elements in `a` and `b` and store `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (i % 2 == 1) ? a.half[i] : b.half[i + 1];
}

// Expands to:

if (0) {
  dst.half[0] = b.half[1];
  dst.half[1] = a.half[1];
  dst.half[2] = b.half[3];
  dst.half[3] = a.half[3];
  dst.half[4] = b.half[5];
  dst.half[5] = a.half[5];
  dst.half[6] = b.half[7];
  dst.half[7] = a.half[7];
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

## \_\_m128i \_\_lsx_vpackod_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vpackod_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vpackod.w vr, vr, vr
CPU Flags: LSX
```

### Description

Collect and pack odd-positioned 32-bit elements in `a` and `b` and store `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (i % 2 == 1) ? a.word[i] : b.word[i + 1];
}

// Expands to:

if (0) {
  dst.word[0] = b.word[1];
  dst.word[1] = a.word[1];
  dst.word[2] = b.word[3];
  dst.word[3] = a.word[3];
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

## \_\_m128i \_\_lsx_vpackod_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vpackod_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vpackod.d vr, vr, vr
CPU Flags: LSX
```

### Description

Collect and pack odd-positioned 64-bit elements in `a` and `b` and store `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (i % 2 == 1) ? a.dword[i] : b.dword[i + 1];
}

// Expands to:

if (0) {
  dst.dword[0] = b.dword[1];
  dst.dword[1] = a.dword[1];
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

## \_\_m128i \_\_lsx_vpickev_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vpickev_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vpickev.b vr, vr, vr
CPU Flags: LSX
```

### Description

Pick even-positioned 8-bit elements in `b` first, then pick even-positioned 8-bit elements in `a`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (i < 8) ? b.byte[i * 2] : a.byte[(i - 8) * 2];
}

// Expands to:

if (0) {
  dst.byte[0] = b.byte[0];
  dst.byte[1] = b.byte[2];
  dst.byte[2] = b.byte[4];
  dst.byte[3] = b.byte[6];
  dst.byte[4] = b.byte[8];
  dst.byte[5] = b.byte[10];
  dst.byte[6] = b.byte[12];
  dst.byte[7] = b.byte[14];
  dst.byte[8] = a.byte[0];
  dst.byte[9] = a.byte[2];
  dst.byte[10] = a.byte[4];
  dst.byte[11] = a.byte[6];
  dst.byte[12] = a.byte[8];
  dst.byte[13] = a.byte[10];
  dst.byte[14] = a.byte[12];
  dst.byte[15] = a.byte[14];
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

## \_\_m128i \_\_lsx_vpickev_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vpickev_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vpickev.h vr, vr, vr
CPU Flags: LSX
```

### Description

Pick even-positioned 16-bit elements in `b` first, then pick even-positioned 16-bit elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (i < 4) ? b.half[i * 2] : a.half[(i - 4) * 2];
}

// Expands to:

if (0) {
  dst.half[0] = b.half[0];
  dst.half[1] = b.half[2];
  dst.half[2] = b.half[4];
  dst.half[3] = b.half[6];
  dst.half[4] = a.half[0];
  dst.half[5] = a.half[2];
  dst.half[6] = a.half[4];
  dst.half[7] = a.half[6];
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

## \_\_m128i \_\_lsx_vpickev_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vpickev_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vpickev.w vr, vr, vr
CPU Flags: LSX
```

### Description

Pick even-positioned 32-bit elements in `b` first, then pick even-positioned 32-bit elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2) ? b.word[i * 2] : a.word[(i - 2) * 2];
}

// Expands to:

if (0) {
  dst.word[0] = b.word[0];
  dst.word[1] = b.word[2];
  dst.word[2] = a.word[0];
  dst.word[3] = a.word[2];
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

## \_\_m128i \_\_lsx_vpickev_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vpickev_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vpickev.d vr, vr, vr
CPU Flags: LSX
```

### Description

Pick even-positioned 64-bit elements in `b` first, then pick even-positioned 64-bit elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (i < 1) ? b.dword[i * 2] : a.dword[(i - 1) * 2];
}

// Expands to:

if (0) {
  dst.dword[0] = b.dword[0];
  dst.dword[1] = a.dword[0];
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

## int \_\_lsx_vpickve2gr_b (\_\_m128i a, imm0_15 idx)

### Synopsis

```
int __lsx_vpickve2gr_b (__m128i a, imm0_15 idx)
#include <lsxintrin.h>
Instruction: vpickve2gr.b r, vr, imm
CPU Flags: LSX
```

### Description

Pick the `lane` specified by `idx` from `a` and store into `dst`.

### Operation

```
dst = (s8)a.byte[idx];
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 1                |
| 3A6000   | LA664  | 1       | 1                |
| 3C6000   | LA664  | 1       | 1                |
| 2K1000LA | LA264  | 2       | 0.5(1/2)         |
| 2K3000   | LA364E | 2       | 0.5(1/2)         |

## unsigned int \_\_lsx_vpickve2gr_bu (\_\_m128i a, imm0_15 idx)

### Synopsis

```
unsigned int __lsx_vpickve2gr_bu (__m128i a, imm0_15 idx)
#include <lsxintrin.h>
Instruction: vpickve2gr.bu r, vr, imm
CPU Flags: LSX
```

### Description

Pick the `lane` specified by `idx` from `a` and store into `dst`.

### Operation

```
dst = (u8)a.byte[idx];
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 1                |
| 3A6000   | LA664  | 1       | 1                |
| 3C6000   | LA664  | 1       | 1                |
| 2K1000LA | LA264  | 2       | 0.5(1/2)         |
| 2K3000   | LA364E | 2       | 0.5(1/2)         |

## int \_\_lsx_vpickve2gr_h (\_\_m128i a, imm0_7 idx)

### Synopsis

```
int __lsx_vpickve2gr_h (__m128i a, imm0_7 idx)
#include <lsxintrin.h>
Instruction: vpickve2gr.h r, vr, imm
CPU Flags: LSX
```

### Description

Pick the `lane` specified by `idx` from `a` and store into `dst`.

### Operation

```
dst = (s16)a.half[idx];
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 1                |
| 3A6000   | LA664  | 1       | 1                |
| 3C6000   | LA664  | 1       | 1                |
| 2K1000LA | LA264  | 2       | 0.5(1/2)         |
| 2K3000   | LA364E | 2       | 0.5(1/2)         |

## unsigned int \_\_lsx_vpickve2gr_hu (\_\_m128i a, imm0_7 idx)

### Synopsis

```
unsigned int __lsx_vpickve2gr_hu (__m128i a, imm0_7 idx)
#include <lsxintrin.h>
Instruction: vpickve2gr.hu r, vr, imm
CPU Flags: LSX
```

### Description

Pick the `lane` specified by `idx` from `a` and store into `dst`.

### Operation

```
dst = (u16)a.half[idx];
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 1                |
| 3A6000   | LA664  | 1       | 1                |
| 3C6000   | LA664  | 1       | 1                |
| 2K1000LA | LA264  | 2       | 0.5(1/2)         |
| 2K3000   | LA364E | 2       | 0.5(1/2)         |

## int \_\_lsx_vpickve2gr_w (\_\_m128i a, imm0_3 idx)

### Synopsis

```
int __lsx_vpickve2gr_w (__m128i a, imm0_3 idx)
#include <lsxintrin.h>
Instruction: vpickve2gr.w r, vr, imm
CPU Flags: LSX
```

### Description

Pick the `lane` specified by `idx` from `a` and store into `dst`.

### Operation

```
dst = (s32)a.word[idx];
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 1                |
| 3A6000   | LA664  | 1       | 1                |
| 3C6000   | LA664  | 1       | 1                |
| 2K1000LA | LA264  | 2       | 0.5(1/2)         |
| 2K3000   | LA364E | 2       | 0.5(1/2)         |

## unsigned int \_\_lsx_vpickve2gr_wu (\_\_m128i a, imm0_3 idx)

### Synopsis

```
unsigned int __lsx_vpickve2gr_wu (__m128i a, imm0_3 idx)
#include <lsxintrin.h>
Instruction: vpickve2gr.wu r, vr, imm
CPU Flags: LSX
```

### Description

Pick the `lane` specified by `idx` from `a` and store into `dst`.

### Operation

```
dst = (u32)a.word[idx];
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 1                |
| 3A6000   | LA664  | 1       | 1                |
| 3C6000   | LA664  | 1       | 1                |
| 2K1000LA | LA264  | 2       | 0.5(1/2)         |
| 2K3000   | LA364E | 2       | 0.5(1/2)         |

## long int \_\_lsx_vpickve2gr_d (\_\_m128i a, imm0_1 idx)

### Synopsis

```
long int __lsx_vpickve2gr_d (__m128i a, imm0_1 idx)
#include <lsxintrin.h>
Instruction: vpickve2gr.d r, vr, imm
CPU Flags: LSX
```

### Description

Pick the `lane` specified by `idx` from `a` and store into `dst`.

### Operation

```
dst = (s64)a.dword[idx];
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 1                |
| 3A6000   | LA664  | 1       | 1                |
| 3C6000   | LA664  | 1       | 1                |
| 2K1000LA | LA264  | 2       | 0.5(1/2)         |
| 2K3000   | LA364E | 2       | 0.5(1/2)         |

## unsigned long int \_\_lsx_vpickve2gr_du (\_\_m128i a, imm0_1 idx)

### Synopsis

```
unsigned long int __lsx_vpickve2gr_du (__m128i a, imm0_1 idx)
#include <lsxintrin.h>
Instruction: vpickve2gr.du r, vr, imm
CPU Flags: LSX
```

### Description

Pick the `lane` specified by `idx` from `a` and store into `dst`.

### Operation

```
dst = (u64)a.dword[idx];
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 1                |
| 3A6000   | LA664  | 1       | 1                |
| 3C6000   | LA664  | 1       | 1                |
| 2K1000LA | LA264  | 2       | 0.5(1/2)         |
| 2K3000   | LA364E | 2       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vpickod_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vpickod_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vpickod.b vr, vr, vr
CPU Flags: LSX
```

### Description

Pick odd-positioned 8-bit elements in `b` first, then pick odd-positioned 8-bit elements in `a`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (i < 8) ? b.byte[i * 2 + 1] : a.byte[(i - 8) * 2 + 1];
}

// Expands to:

if (0) {
  dst.byte[0] = b.byte[1];
  dst.byte[1] = b.byte[3];
  dst.byte[2] = b.byte[5];
  dst.byte[3] = b.byte[7];
  dst.byte[4] = b.byte[9];
  dst.byte[5] = b.byte[11];
  dst.byte[6] = b.byte[13];
  dst.byte[7] = b.byte[15];
  dst.byte[8] = a.byte[1];
  dst.byte[9] = a.byte[3];
  dst.byte[10] = a.byte[5];
  dst.byte[11] = a.byte[7];
  dst.byte[12] = a.byte[9];
  dst.byte[13] = a.byte[11];
  dst.byte[14] = a.byte[13];
  dst.byte[15] = a.byte[15];
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

## \_\_m128i \_\_lsx_vpickod_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vpickod_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vpickod.h vr, vr, vr
CPU Flags: LSX
```

### Description

Pick odd-positioned 16-bit elements in `b` first, then pick odd-positioned 16-bit elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (i < 4) ? b.half[i * 2 + 1] : a.half[(i - 4) * 2 + 1];
}

// Expands to:

if (0) {
  dst.half[0] = b.half[1];
  dst.half[1] = b.half[3];
  dst.half[2] = b.half[5];
  dst.half[3] = b.half[7];
  dst.half[4] = a.half[1];
  dst.half[5] = a.half[3];
  dst.half[6] = a.half[5];
  dst.half[7] = a.half[7];
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

## \_\_m128i \_\_lsx_vpickod_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vpickod_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vpickod.w vr, vr, vr
CPU Flags: LSX
```

### Description

Pick odd-positioned 32-bit elements in `b` first, then pick odd-positioned 32-bit elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2) ? b.word[i * 2 + 1] : a.word[(i - 2) * 2 + 1];
}

// Expands to:

if (0) {
  dst.word[0] = b.word[1];
  dst.word[1] = b.word[3];
  dst.word[2] = a.word[1];
  dst.word[3] = a.word[3];
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

## \_\_m128i \_\_lsx_vpickod_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vpickod_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vpickod.d vr, vr, vr
CPU Flags: LSX
```

### Description

Pick odd-positioned 64-bit elements in `b` first, then pick odd-positioned 64-bit elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (i < 1) ? b.dword[i * 2 + 1] : a.dword[(i - 1) * 2 + 1];
}

// Expands to:

if (0) {
  dst.dword[0] = b.dword[1];
  dst.dword[1] = a.dword[1];
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

## \_\_m128i \_\_lsx_vrepli_b (imm_n512_511 imm)

### Synopsis

```
__m128i __lsx_vrepli_b (imm_n512_511 imm)
#include <lsxintrin.h>
Instruction: vldi vr, imm
CPU Flags: LSX
```

### Description

Repeat `imm` to fill whole vector.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = imm;
}
```

Tested on real machine.

## \_\_m128i \_\_lsx_vrepli_h (imm_n512_511 imm)

### Synopsis

```
__m128i __lsx_vrepli_h (imm_n512_511 imm)
#include <lsxintrin.h>
Instruction: vldi vr, imm
CPU Flags: LSX
```

### Description

Repeat `imm` to fill whole vector.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = imm;
}
```

Tested on real machine.

## \_\_m128i \_\_lsx_vrepli_w (imm_n512_511 imm)

### Synopsis

```
__m128i __lsx_vrepli_w (imm_n512_511 imm)
#include <lsxintrin.h>
Instruction: vldi vr, imm
CPU Flags: LSX
```

### Description

Repeat `imm` to fill whole vector.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = imm;
}
```

Tested on real machine.

## \_\_m128i \_\_lsx_vrepli_d (imm_n512_511 imm)

### Synopsis

```
__m128i __lsx_vrepli_d (imm_n512_511 imm)
#include <lsxintrin.h>
Instruction: vldi vr, imm
CPU Flags: LSX
```

### Description

Repeat `imm` to fill whole vector.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = imm;
}
```

Tested on real machine.

## \_\_m128i \_\_lsx_vreplgr2vr_b (int val)

### Synopsis

```
__m128i __lsx_vreplgr2vr_b (int val)
#include <lsxintrin.h>
Instruction: vreplgr2vr.b vr, r
CPU Flags: LSX
```

### Description

Repeat `val` to whole vector.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = val;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | N/A     | 1                |
| 3A6000   | LA664  | N/A     | 1                |
| 3C6000   | LA664  | N/A     | 1                |
| 2K1000LA | LA264  | N/A     | 0.5(1/2)         |
| 2K3000   | LA364E | N/A     | 1                |

## \_\_m128i \_\_lsx_vreplgr2vr_h (int val)

### Synopsis

```
__m128i __lsx_vreplgr2vr_h (int val)
#include <lsxintrin.h>
Instruction: vreplgr2vr.h vr, r
CPU Flags: LSX
```

### Description

Repeat `val` to whole vector.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = val;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | N/A     | 1                |
| 3A6000   | LA664  | N/A     | 1                |
| 3C6000   | LA664  | N/A     | 1                |
| 2K1000LA | LA264  | N/A     | 0.5(1/2)         |
| 2K3000   | LA364E | N/A     | 1                |

## \_\_m128i \_\_lsx_vreplgr2vr_w (int val)

### Synopsis

```
__m128i __lsx_vreplgr2vr_w (int val)
#include <lsxintrin.h>
Instruction: vreplgr2vr.w vr, r
CPU Flags: LSX
```

### Description

Repeat `val` to whole vector.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = val;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | N/A     | 1                |
| 3A6000   | LA664  | N/A     | 1                |
| 3C6000   | LA664  | N/A     | 1                |
| 2K1000LA | LA264  | N/A     | 1                |
| 2K3000   | LA364E | N/A     | 1                |

## \_\_m128i \_\_lsx_vreplgr2vr_d (long int val)

### Synopsis

```
__m128i __lsx_vreplgr2vr_d (long int val)
#include <lsxintrin.h>
Instruction: vreplgr2vr.d vr, r
CPU Flags: LSX
```

### Description

Repeat `val` to whole vector.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = val;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | N/A     | 1                |
| 3A6000   | LA664  | N/A     | 1                |
| 3C6000   | LA664  | N/A     | 1                |
| 2K1000LA | LA264  | N/A     | 1                |
| 2K3000   | LA364E | N/A     | 1                |

## \_\_m128i \_\_lsx_vreplve_b (\_\_m128i a, int idx)

### Synopsis

```
__m128i __lsx_vreplve_b (__m128i a, int idx)
#include <lsxintrin.h>
Instruction: vreplve.b vr, vr, r
CPU Flags: LSX
```

### Description

Repeat the element in lane `idx` of `a` to fill whole vector.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[idx % 16];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 1                |
| 3A6000   | LA664  | 1       | 1                |
| 3C6000   | LA664  | 1       | 1                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vreplve_h (\_\_m128i a, int idx)

### Synopsis

```
__m128i __lsx_vreplve_h (__m128i a, int idx)
#include <lsxintrin.h>
Instruction: vreplve.h vr, vr, r
CPU Flags: LSX
```

### Description

Repeat the element in lane `idx` of `a` to fill whole vector.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = a.half[idx % 8];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 1                |
| 3A6000   | LA664  | 1       | 1                |
| 3C6000   | LA664  | 1       | 1                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vreplve_w (\_\_m128i a, int idx)

### Synopsis

```
__m128i __lsx_vreplve_w (__m128i a, int idx)
#include <lsxintrin.h>
Instruction: vreplve.w vr, vr, r
CPU Flags: LSX
```

### Description

Repeat the element in lane `idx` of `a` to fill whole vector.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = a.word[idx % 4];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 1                |
| 3A6000   | LA664  | 1       | 1                |
| 3C6000   | LA664  | 1       | 1                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vreplve_d (\_\_m128i a, int idx)

### Synopsis

```
__m128i __lsx_vreplve_d (__m128i a, int idx)
#include <lsxintrin.h>
Instruction: vreplve.d vr, vr, r
CPU Flags: LSX
```

### Description

Repeat the element in lane `idx` of `a` to fill whole vector.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = a.dword[idx % 2];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 1                |
| 3A6000   | LA664  | 1       | 1                |
| 3C6000   | LA664  | 1       | 1                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vreplvei_b (\_\_m128i a, imm0_15 idx)

### Synopsis

```
__m128i __lsx_vreplvei_b (__m128i a, imm0_15 idx)
#include <lsxintrin.h>
Instruction: vreplvei.b vr, vr, imm
CPU Flags: LSX
```

### Description

Repeat the element in lane `idx` of `a` to fill whole vector.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[idx];
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

## \_\_m128i \_\_lsx_vreplvei_h (\_\_m128i a, imm0_7 idx)

### Synopsis

```
__m128i __lsx_vreplvei_h (__m128i a, imm0_7 idx)
#include <lsxintrin.h>
Instruction: vreplvei.h vr, vr, imm
CPU Flags: LSX
```

### Description

Repeat the element in lane `idx` of `a` to fill whole vector.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = a.half[idx];
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

## \_\_m128i \_\_lsx_vreplvei_w (\_\_m128i a, imm0_3 idx)

### Synopsis

```
__m128i __lsx_vreplvei_w (__m128i a, imm0_3 idx)
#include <lsxintrin.h>
Instruction: vreplvei.w vr, vr, imm
CPU Flags: LSX
```

### Description

Repeat the element in lane `idx` of `a` to fill whole vector.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = a.word[idx];
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

## \_\_m128i \_\_lsx_vreplvei_d (\_\_m128i a, imm0_1 idx)

### Synopsis

```
__m128i __lsx_vreplvei_d (__m128i a, imm0_1 idx)
#include <lsxintrin.h>
Instruction: vreplvei.d vr, vr, imm
CPU Flags: LSX
```

### Description

Repeat the element in lane `idx` of `a` to fill whole vector.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = a.dword[idx];
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

## \_\_m128i \_\_lsx_vsat_b (\_\_m128i a, imm0_7 imm)

### Synopsis

```
__m128i __lsx_vsat_b (__m128i a, imm0_7 imm)
#include <lsxintrin.h>
Instruction: vsat.b vr, vr, imm
CPU Flags: LSX
```

### Description

Clamp signed 8-bit elements in `a` to range specified by `imm`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = clamp<s8>(a.byte[i], -(1 << imm), (1 << imm) - 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vsat_bu (\_\_m128i a, imm0_7 imm)

### Synopsis

```
__m128i __lsx_vsat_bu (__m128i a, imm0_7 imm)
#include <lsxintrin.h>
Instruction: vsat.bu vr, vr, imm
CPU Flags: LSX
```

### Description

Clamp unsigned 8-bit elements in `a` to range specified by `imm`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = clamp<u8>(a.byte[i], 0, (1 << (imm + 1)) - 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vsat_h (\_\_m128i a, imm0_15 imm)

### Synopsis

```
__m128i __lsx_vsat_h (__m128i a, imm0_15 imm)
#include <lsxintrin.h>
Instruction: vsat.h vr, vr, imm
CPU Flags: LSX
```

### Description

Clamp signed 16-bit elements in `a` to range specified by `imm`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = clamp<s16>(a.half[i], -(1 << imm), (1 << imm) - 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vsat_hu (\_\_m128i a, imm0_15 imm)

### Synopsis

```
__m128i __lsx_vsat_hu (__m128i a, imm0_15 imm)
#include <lsxintrin.h>
Instruction: vsat.hu vr, vr, imm
CPU Flags: LSX
```

### Description

Clamp unsigned 16-bit elements in `a` to range specified by `imm`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = clamp<u16>(a.half[i], 0, (1 << (imm + 1)) - 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vsat_w (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vsat_w (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vsat.w vr, vr, imm
CPU Flags: LSX
```

### Description

Clamp signed 32-bit elements in `a` to range specified by `imm`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = clamp<s32>(a.word[i], -(1 << imm), (1 << imm) - 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vsat_wu (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vsat_wu (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vsat.wu vr, vr, imm
CPU Flags: LSX
```

### Description

Clamp unsigned 32-bit elements in `a` to range specified by `imm`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = clamp<u32>(a.word[i], 0, (1 << (imm + 1)) - 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vsat_d (\_\_m128i a, imm0_63 imm)

### Synopsis

```
__m128i __lsx_vsat_d (__m128i a, imm0_63 imm)
#include <lsxintrin.h>
Instruction: vsat.d vr, vr, imm
CPU Flags: LSX
```

### Description

Clamp signed 64-bit elements in `a` to range specified by `imm`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = clamp<s64>(a.dword[i], -(1 << imm), (1 << imm) - 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vsat_du (\_\_m128i a, imm0_63 imm)

### Synopsis

```
__m128i __lsx_vsat_du (__m128i a, imm0_63 imm)
#include <lsxintrin.h>
Instruction: vsat.du vr, vr, imm
CPU Flags: LSX
```

### Description

Clamp unsigned 64-bit elements in `a` to range specified by `imm`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = clamp<u64>(a.dword[i], 0, (1 << (imm + 1)) - 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vsigncov_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsigncov_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsigncov.b vr, vr, vr
CPU Flags: LSX
```

### Description

If the 8-bit element in `a` equals to zero, set the result to zero. If the signed 8-bit element in `a` is positive, copy element in `b` to result. Otherwise, copy negated element in `b` to result. If `a` and `b` are the same vectors, it is equivalent to computing absolute value.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] =
      (a.byte[i] == 0) ? 0 : ((s8)a.byte[i] > 0 ? b.byte[i] : -b.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 2                |
| 3C6000   | LA664  | 1       | 2                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vsigncov_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsigncov_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsigncov.h vr, vr, vr
CPU Flags: LSX
```

### Description

If the 16-bit element in `a` equals to zero, set the result to zero. If the signed 16-bit element in `a` is positive, copy element in `b` to result. Otherwise, copy negated element in `b` to result. If `a` and `b` are the same vectors, it is equivalent to computing absolute value.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] =
      (a.half[i] == 0) ? 0 : ((s16)a.half[i] > 0 ? b.half[i] : -b.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 2                |
| 3C6000   | LA664  | 1       | 2                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vsigncov_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsigncov_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsigncov.w vr, vr, vr
CPU Flags: LSX
```

### Description

If the 32-bit element in `a` equals to zero, set the result to zero. If the signed 32-bit element in `a` is positive, copy element in `b` to result. Otherwise, copy negated element in `b` to result. If `a` and `b` are the same vectors, it is equivalent to computing absolute value.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] =
      (a.word[i] == 0) ? 0 : ((s32)a.word[i] > 0 ? b.word[i] : -b.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 2                |
| 3C6000   | LA664  | 1       | 2                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vsigncov_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsigncov_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsigncov.d vr, vr, vr
CPU Flags: LSX
```

### Description

If the 64-bit element in `a` equals to zero, set the result to zero. If the signed 64-bit element in `a` is positive, copy element in `b` to result. Otherwise, copy negated element in `b` to result. If `a` and `b` are the same vectors, it is equivalent to computing absolute value.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] =
      (a.dword[i] == 0) ? 0 : ((s64)a.dword[i] > 0 ? b.dword[i] : -b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 2                |
| 3C6000   | LA664  | 1       | 2                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vldi (imm_n1024_1023 imm)

### Synopsis

```
__m128i __lsx_vldi (imm_n1024_1023 imm)
#include <lsxintrin.h>
Instruction: vldi vr, imm
CPU Flags: LSX
```

### Description

Initialize `dst` using predefined patterns:

- `imm[12:10]=0b000`: broadcast `imm[7:0]` as 8-bit elements to all lanes
- `imm[12:10]=0b001`: broadcast sign-extended `imm[9:0]` as 16-bit elements to all lanes
- `imm[12:10]=0b010`: broadcast sign-extended `imm[9:0]` as 32-bit elements to all lanes
- `imm[12:10]=0b011`: broadcast sign-extended `imm[9:0]` as 64-bit elements to all lanes
- `imm[12:8]=0b10000`: broadcast `imm[7:0]` as 32-bit elements to all lanes
- `imm[12:8]=0b10001`: broadcast `imm[7:0] << 8` as 32-bit elements to all lanes
- `imm[12:8]=0b10010`: broadcast `imm[7:0] << 16` as 32-bit elements to all lanes
- `imm[12:8]=0b10011`: broadcast `imm[7:0] << 24` as 32-bit elements to all lanes
- `imm[12:8]=0b10100`: broadcast `imm[7:0]` as 16-bit elements to all lanes
- `imm[12:8]=0b10101`: broadcast `imm[7:0] << 8` as 16-bit elements to all lanes
- `imm[12:8]=0b10110`: broadcast `(imm[7:0] << 8) | 0xFF` as 32-bit elements to all lanes
- `imm[12:8]=0b10111`: broadcast `(imm[7:0] << 16) | 0xFFFF` as 32-bit elements to all lanes
- `imm[12:8]=0b11000`: broadcast `imm[7:0]` as 8-bit elements to all lanes
- `imm[12:8]=0b11001`: repeat each bit of `imm[7:0]` eight times, and broadcast the result as 64-bit elements to all lanes
- `imm[12:8]=0b11010`: broadcast `(imm[7] << 31) | ((1-imm[6]) << 30) | ((imm[6] * 0x1F) << 25) | (imm[5:0] << 19)` as 32-bit elements to all lanes
- `imm[12:8]=0b11011`: broadcast `(imm[7] << 31) | ((1-imm[6]) << 30) | ((imm[6] * 0x1F) << 25) | (imm[5:0] << 19)` as 64-bit elements to all lanes
- `imm[12:8]=0b11100`: broadcast `(imm[7] << 63) | ((1-imm[6]) << 62) | ((imm[6] * 0xFF) << 54) | (imm[5:0] << 48)` as 64-bit elements to all lanes

### Operation

```
u64 imm12_10 = (imm >> 10) & 0b111;
u64 imm12_8 = (imm >> 8) & 0b11111;
u64 imm9_0 = imm & 0x3FF;
s64 simm9_0 = ((s64)imm9_0 << 54) >> 54;
u64 imm7_0 = imm & 0xFF;
u64 imm7 = (imm >> 7) & 0x1;
u64 imm6 = (imm >> 6) & 0x1;
u64 imm5 = (imm >> 5) & 0x1;
u64 imm5_0 = imm & 0x3F;
u64 imm4 = (imm >> 4) & 0x1;
u64 imm3 = (imm >> 3) & 0x1;
u64 imm2 = (imm >> 2) & 0x1;
u64 imm1 = (imm >> 1) & 0x1;
u64 imm0 = imm & 0x1;

u64 broadcast_value;
u64 broadcast_width;
if (imm12_10 == 0b000) {
  broadcast_value = imm7_0;
  broadcast_width = 8;
} else if (imm12_10 == 0b001) {
  broadcast_value = simm9_0;
  broadcast_width = 16;
} else if (imm12_10 == 0b010) {
  broadcast_value = simm9_0;
  broadcast_width = 32;
} else if (imm12_10 == 0b011) {
  broadcast_value = simm9_0;
  broadcast_width = 64;
} else if (imm12_8 == 0b10000) {
  broadcast_value = imm7_0;
  broadcast_width = 32;
} else if (imm12_8 == 0b10001) {
  broadcast_value = imm7_0 << 8;
  broadcast_width = 32;
} else if (imm12_8 == 0b10010) {
  broadcast_value = imm7_0 << 16;
  broadcast_width = 32;
} else if (imm12_8 == 0b10011) {
  broadcast_value = imm7_0 << 24;
  broadcast_width = 32;
} else if (imm12_8 == 0b10100) {
  broadcast_value = imm7_0;
  broadcast_width = 16;
} else if (imm12_8 == 0b10101) {
  broadcast_value = imm7_0 << 8;
  broadcast_width = 16;
} else if (imm12_8 == 0b10110) {
  broadcast_value = (imm7_0 << 8) | 0xFF;
  broadcast_width = 32;
} else if (imm12_8 == 0b10111) {
  broadcast_value = (imm7_0 << 16) | 0xFFFF;
  broadcast_width = 32;
} else if (imm12_8 == 0b11000) {
  broadcast_value = imm7_0;
  broadcast_width = 8;
} else if (imm12_8 == 0b11001) {
  broadcast_value = imm0 * 0xFF + imm1 * 0xFF00 + imm2 * 0xFF0000 +
                    imm3 * 0xFF000000 + imm4 * 0xFF00000000 +
                    imm5 * 0xFF0000000000 + imm6 * 0xFF000000000000 +
                    imm7 * 0xFF00000000000000;
  broadcast_width = 64;
} else if (imm12_8 == 0b11010) {
  broadcast_value = (imm7 << 31) | ((1 - imm6) << 30) | ((imm6 * 0x1F) << 25) |
                    (imm5_0 << 19);
  broadcast_width = 32;
} else if (imm12_8 == 0b11011) {
  broadcast_value = (imm7 << 31) | ((1 - imm6) << 30) | ((imm6 * 0x1F) << 25) |
                    (imm5_0 << 19);
  broadcast_width = 64;
} else if (imm12_8 == 0b11100) {
  broadcast_value = (imm7 << 63) | ((1 - imm6) << 62) | ((imm6 * 0xFF) << 54) |
                    (imm5_0 << 48);
  broadcast_width = 64;
}

if (broadcast_width == 8) {
  for (int i = 0; i < 16; i++) {
    dst.byte[i] = broadcast_value;
  }
} else if (broadcast_width == 16) {
  for (int i = 0; i < 8; i++) {
    dst.half[i] = broadcast_value;
  }
} else if (broadcast_width == 32) {
  for (int i = 0; i < 4; i++) {
    dst.word[i] = broadcast_value;
  }
} else if (broadcast_width == 64) {
  for (int i = 0; i < 2; i++) {
    dst.dword[i] = broadcast_value;
  }
}
```

Tested on real machine.
