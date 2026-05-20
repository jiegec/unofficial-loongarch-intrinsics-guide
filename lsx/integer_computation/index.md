# Integer Computation

## \_\_m128i \_\_lsx_vadd_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vadd_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vadd.b vr, vr, vr
CPU Flags: LSX
```

### Description

Add 8-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[i] + b.byte[i];
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

## \_\_m128i \_\_lsx_vadd_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vadd_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vadd.h vr, vr, vr
CPU Flags: LSX
```

### Description

Add 16-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = a.half[i] + b.half[i];
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

## \_\_m128i \_\_lsx_vadd_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vadd_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vadd.w vr, vr, vr
CPU Flags: LSX
```

### Description

Add 32-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = a.word[i] + b.word[i];
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

## \_\_m128i \_\_lsx_vadd_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vadd_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vadd.d vr, vr, vr
CPU Flags: LSX
```

### Description

Add 64-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = a.dword[i] + b.dword[i];
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

## \_\_m128i \_\_lsx_vadd_q (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vadd_q (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vadd.q vr, vr, vr
CPU Flags: LSX
```

### Description

Add 128-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
dst.qword[0] = a.qword[0] + b.qword[0];
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

## \_\_m128i \_\_lsx_vabsd_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vabsd_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vabsd.b vr, vr, vr
CPU Flags: LSX
```

### Description

Compute absolute difference of signed 8-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = ((s8)a.byte[i] > (s8)b.byte[i]) ? (a.byte[i] - b.byte[i])
                                                : (b.byte[i] - a.byte[i]);
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

## \_\_m128i \_\_lsx_vabsd_bu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vabsd_bu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vabsd.bu vr, vr, vr
CPU Flags: LSX
```

### Description

Compute absolute difference of unsigned 8-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = ((u8)a.byte[i] > (u8)b.byte[i]) ? (a.byte[i] - b.byte[i])
                                                : (b.byte[i] - a.byte[i]);
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

## \_\_m128i \_\_lsx_vabsd_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vabsd_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vabsd.h vr, vr, vr
CPU Flags: LSX
```

### Description

Compute absolute difference of signed 16-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = ((s16)a.half[i] > (s16)b.half[i]) ? (a.half[i] - b.half[i])
                                                  : (b.half[i] - a.half[i]);
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

## \_\_m128i \_\_lsx_vabsd_hu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vabsd_hu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vabsd.hu vr, vr, vr
CPU Flags: LSX
```

### Description

Compute absolute difference of unsigned 16-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = ((u16)a.half[i] > (u16)b.half[i]) ? (a.half[i] - b.half[i])
                                                  : (b.half[i] - a.half[i]);
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

## \_\_m128i \_\_lsx_vabsd_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vabsd_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vabsd.w vr, vr, vr
CPU Flags: LSX
```

### Description

Compute absolute difference of signed 32-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = ((s32)a.word[i] > (s32)b.word[i]) ? (a.word[i] - b.word[i])
                                                  : (b.word[i] - a.word[i]);
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

## \_\_m128i \_\_lsx_vabsd_wu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vabsd_wu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vabsd.wu vr, vr, vr
CPU Flags: LSX
```

### Description

Compute absolute difference of unsigned 32-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = ((u32)a.word[i] > (u32)b.word[i]) ? (a.word[i] - b.word[i])
                                                  : (b.word[i] - a.word[i]);
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

## \_\_m128i \_\_lsx_vabsd_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vabsd_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vabsd.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compute absolute difference of signed 64-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = ((s64)a.dword[i] > (s64)b.dword[i])
                     ? (a.dword[i] - b.dword[i])
                     : (b.dword[i] - a.dword[i]);
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

## \_\_m128i \_\_lsx_vabsd_du (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vabsd_du (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vabsd.du vr, vr, vr
CPU Flags: LSX
```

### Description

Compute absolute difference of unsigned 64-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = ((u64)a.dword[i] > (u64)b.dword[i])
                     ? (a.dword[i] - b.dword[i])
                     : (b.dword[i] - a.dword[i]);
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

## \_\_m128i \_\_lsx_vadda_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vadda_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vadda.b vr, vr, vr
CPU Flags: LSX
```

### Description

Add absolute of 8-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = abs((s8)a.byte[i]) + abs((s8)b.byte[i]);
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

## \_\_m128i \_\_lsx_vadda_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vadda_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vadda.h vr, vr, vr
CPU Flags: LSX
```

### Description

Add absolute of 16-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = abs((s16)a.half[i]) + abs((s16)b.half[i]);
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

## \_\_m128i \_\_lsx_vadda_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vadda_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vadda.w vr, vr, vr
CPU Flags: LSX
```

### Description

Add absolute of 32-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = abs((s32)a.word[i]) + abs((s32)b.word[i]);
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

## \_\_m128i \_\_lsx_vadda_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vadda_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vadda.d vr, vr, vr
CPU Flags: LSX
```

### Description

Add absolute of 64-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = abs((s64)a.dword[i]) + abs((s64)b.dword[i]);
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

## \_\_m128i \_\_lsx_vaddi_bu (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vaddi_bu (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vaddi.bu vr, vr, imm
CPU Flags: LSX
```

### Description

Add 8-bit elements in `a` and `imm`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[i] + imm;
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

## \_\_m128i \_\_lsx_vaddi_hu (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vaddi_hu (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vaddi.hu vr, vr, imm
CPU Flags: LSX
```

### Description

Add 16-bit elements in `a` and `imm`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = a.half[i] + imm;
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

## \_\_m128i \_\_lsx_vaddi_wu (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vaddi_wu (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vaddi.wu vr, vr, imm
CPU Flags: LSX
```

### Description

Add 32-bit elements in `a` and `imm`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = a.word[i] + imm;
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

## \_\_m128i \_\_lsx_vaddi_du (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vaddi_du (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vaddi.du vr, vr, imm
CPU Flags: LSX
```

### Description

Add 64-bit elements in `a` and `imm`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = a.dword[i] + imm;
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

## \_\_m128i \_\_lsx_vaddwev_h_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vaddwev_h_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vaddwev.h.b vr, vr, vr
CPU Flags: LSX
```

### Description

Add even-positioned signed 8-bit elements in `a` and signed elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (s16)(s8)a.byte[2 * i] + (s16)(s8)b.byte[2 * i];
}

// Expands to:

if (0) {
  dst.half[0] = ((s16)((s8)a.byte[0])) + ((s16)((s8)b.byte[0]));
  dst.half[1] = ((s16)((s8)a.byte[2])) + ((s16)((s8)b.byte[2]));
  dst.half[2] = ((s16)((s8)a.byte[4])) + ((s16)((s8)b.byte[4]));
  dst.half[3] = ((s16)((s8)a.byte[6])) + ((s16)((s8)b.byte[6]));
  dst.half[4] = ((s16)((s8)a.byte[8])) + ((s16)((s8)b.byte[8]));
  dst.half[5] = ((s16)((s8)a.byte[10])) + ((s16)((s8)b.byte[10]));
  dst.half[6] = ((s16)((s8)a.byte[12])) + ((s16)((s8)b.byte[12]));
  dst.half[7] = ((s16)((s8)a.byte[14])) + ((s16)((s8)b.byte[14]));
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

## \_\_m128i \_\_lsx_vaddwev_h_bu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vaddwev_h_bu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vaddwev.h.bu vr, vr, vr
CPU Flags: LSX
```

### Description

Add even-positioned unsigned 8-bit elements in `a` and unsigned elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (u16)(u8)a.byte[2 * i] + (u16)(u8)b.byte[2 * i];
}

// Expands to:

if (0) {
  dst.half[0] = ((u16)((u8)a.byte[0])) + ((u16)((u8)b.byte[0]));
  dst.half[1] = ((u16)((u8)a.byte[2])) + ((u16)((u8)b.byte[2]));
  dst.half[2] = ((u16)((u8)a.byte[4])) + ((u16)((u8)b.byte[4]));
  dst.half[3] = ((u16)((u8)a.byte[6])) + ((u16)((u8)b.byte[6]));
  dst.half[4] = ((u16)((u8)a.byte[8])) + ((u16)((u8)b.byte[8]));
  dst.half[5] = ((u16)((u8)a.byte[10])) + ((u16)((u8)b.byte[10]));
  dst.half[6] = ((u16)((u8)a.byte[12])) + ((u16)((u8)b.byte[12]));
  dst.half[7] = ((u16)((u8)a.byte[14])) + ((u16)((u8)b.byte[14]));
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

## \_\_m128i \_\_lsx_vaddwev_h_bu_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vaddwev_h_bu_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vaddwev.h.bu.b vr, vr, vr
CPU Flags: LSX
```

### Description

Add even-positioned unsigned 8-bit elements in `a` and signed elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (u16)(u8)a.byte[2 * i] + (s16)(s8)b.byte[2 * i];
}

// Expands to:

if (0) {
  dst.half[0] = ((u16)((u8)a.byte[0])) + ((s16)((s8)b.byte[0]));
  dst.half[1] = ((u16)((u8)a.byte[2])) + ((s16)((s8)b.byte[2]));
  dst.half[2] = ((u16)((u8)a.byte[4])) + ((s16)((s8)b.byte[4]));
  dst.half[3] = ((u16)((u8)a.byte[6])) + ((s16)((s8)b.byte[6]));
  dst.half[4] = ((u16)((u8)a.byte[8])) + ((s16)((s8)b.byte[8]));
  dst.half[5] = ((u16)((u8)a.byte[10])) + ((s16)((s8)b.byte[10]));
  dst.half[6] = ((u16)((u8)a.byte[12])) + ((s16)((s8)b.byte[12]));
  dst.half[7] = ((u16)((u8)a.byte[14])) + ((s16)((s8)b.byte[14]));
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

## \_\_m128i \_\_lsx_vaddwev_w_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vaddwev_w_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vaddwev.w.h vr, vr, vr
CPU Flags: LSX
```

### Description

Add even-positioned signed 16-bit elements in `a` and signed elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (s32)(s16)a.half[2 * i] + (s32)(s16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((s32)((s16)a.half[0])) + ((s32)((s16)b.half[0]));
  dst.word[1] = ((s32)((s16)a.half[2])) + ((s32)((s16)b.half[2]));
  dst.word[2] = ((s32)((s16)a.half[4])) + ((s32)((s16)b.half[4]));
  dst.word[3] = ((s32)((s16)a.half[6])) + ((s32)((s16)b.half[6]));
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

## \_\_m128i \_\_lsx_vaddwev_w_hu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vaddwev_w_hu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vaddwev.w.hu vr, vr, vr
CPU Flags: LSX
```

### Description

Add even-positioned unsigned 16-bit elements in `a` and unsigned elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i] + (u32)(u16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[0])) + ((u32)((u16)b.half[0]));
  dst.word[1] = ((u32)((u16)a.half[2])) + ((u32)((u16)b.half[2]));
  dst.word[2] = ((u32)((u16)a.half[4])) + ((u32)((u16)b.half[4]));
  dst.word[3] = ((u32)((u16)a.half[6])) + ((u32)((u16)b.half[6]));
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

## \_\_m128i \_\_lsx_vaddwev_w_hu_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vaddwev_w_hu_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vaddwev.w.hu.h vr, vr, vr
CPU Flags: LSX
```

### Description

Add even-positioned unsigned 16-bit elements in `a` and signed elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i] + (s32)(s16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[0])) + ((s32)((s16)b.half[0]));
  dst.word[1] = ((u32)((u16)a.half[2])) + ((s32)((s16)b.half[2]));
  dst.word[2] = ((u32)((u16)a.half[4])) + ((s32)((s16)b.half[4]));
  dst.word[3] = ((u32)((u16)a.half[6])) + ((s32)((s16)b.half[6]));
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

## \_\_m128i \_\_lsx_vaddwev_d_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vaddwev_d_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vaddwev.d.w vr, vr, vr
CPU Flags: LSX
```

### Description

Add even-positioned signed 32-bit elements in `a` and signed elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)(s32)a.word[2 * i] + (s64)(s32)b.word[2 * i];
}

// Expands to:

if (0) {
  dst.dword[0] = ((s64)((s32)a.word[0])) + ((s64)((s32)b.word[0]));
  dst.dword[1] = ((s64)((s32)a.word[2])) + ((s64)((s32)b.word[2]));
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

## \_\_m128i \_\_lsx_vaddwev_d_wu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vaddwev_d_wu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vaddwev.d.wu vr, vr, vr
CPU Flags: LSX
```

### Description

Add even-positioned unsigned 32-bit elements in `a` and unsigned elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 * i] + (u64)(u32)b.word[2 * i];
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[0])) + ((u64)((u32)b.word[0]));
  dst.dword[1] = ((u64)((u32)a.word[2])) + ((u64)((u32)b.word[2]));
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

## \_\_m128i \_\_lsx_vaddwev_d_wu_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vaddwev_d_wu_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vaddwev.d.wu.w vr, vr, vr
CPU Flags: LSX
```

### Description

Add even-positioned unsigned 32-bit elements in `a` and signed elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 * i] + (s64)(s32)b.word[2 * i];
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[0])) + ((s64)((s32)b.word[0]));
  dst.dword[1] = ((u64)((u32)a.word[2])) + ((s64)((s32)b.word[2]));
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

## \_\_m128i \_\_lsx_vaddwev_q_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vaddwev_q_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vaddwev.q.d vr, vr, vr
CPU Flags: LSX
```

### Description

Add even-positioned signed 64-bit elements in `a` and signed elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] = (s128)(s64)a.dword[2 * i] + (s128)(s64)b.dword[2 * i];
}

// Expands to:

if (0) {
  dst.qword[0] = ((s128)((s64)a.dword[0])) + ((s128)((s64)b.dword[0]));
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

## \_\_m128i \_\_lsx_vaddwev_q_du (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vaddwev_q_du (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vaddwev.q.du vr, vr, vr
CPU Flags: LSX
```

### Description

Add even-positioned unsigned 64-bit elements in `a` and unsigned elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 * i] + (u128)(u64)b.dword[2 * i];
}

// Expands to:

if (0) {
  dst.qword[0] = ((u128)((u64)a.dword[0])) + ((u128)((u64)b.dword[0]));
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

## \_\_m128i \_\_lsx_vaddwev_q_du_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vaddwev_q_du_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vaddwev.q.du.d vr, vr, vr
CPU Flags: LSX
```

### Description

Add even-positioned unsigned 64-bit elements in `a` and signed elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 * i] + (s128)(s64)b.dword[2 * i];
}

// Expands to:

if (0) {
  dst.qword[0] = ((u128)((u64)a.dword[0])) + ((s128)((s64)b.dword[0]));
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

## \_\_m128i \_\_lsx_vaddwod_h_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vaddwod_h_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vaddwod.h.b vr, vr, vr
CPU Flags: LSX
```

### Description

Add odd-positioned signed 8-bit elements in `a` and signed elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (s16)(s8)a.byte[2 * i + 1] + (s16)(s8)b.byte[2 * i + 1];
}

// Expands to:

if (0) {
  dst.half[0] = ((s16)((s8)a.byte[1])) + ((s16)((s8)b.byte[1]));
  dst.half[1] = ((s16)((s8)a.byte[3])) + ((s16)((s8)b.byte[3]));
  dst.half[2] = ((s16)((s8)a.byte[5])) + ((s16)((s8)b.byte[5]));
  dst.half[3] = ((s16)((s8)a.byte[7])) + ((s16)((s8)b.byte[7]));
  dst.half[4] = ((s16)((s8)a.byte[9])) + ((s16)((s8)b.byte[9]));
  dst.half[5] = ((s16)((s8)a.byte[11])) + ((s16)((s8)b.byte[11]));
  dst.half[6] = ((s16)((s8)a.byte[13])) + ((s16)((s8)b.byte[13]));
  dst.half[7] = ((s16)((s8)a.byte[15])) + ((s16)((s8)b.byte[15]));
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

## \_\_m128i \_\_lsx_vaddwod_h_bu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vaddwod_h_bu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vaddwod.h.bu vr, vr, vr
CPU Flags: LSX
```

### Description

Add odd-positioned unsigned 8-bit elements in `a` and unsigned elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (u16)(u8)a.byte[2 * i + 1] + (u16)(u8)b.byte[2 * i + 1];
}

// Expands to:

if (0) {
  dst.half[0] = ((u16)((u8)a.byte[1])) + ((u16)((u8)b.byte[1]));
  dst.half[1] = ((u16)((u8)a.byte[3])) + ((u16)((u8)b.byte[3]));
  dst.half[2] = ((u16)((u8)a.byte[5])) + ((u16)((u8)b.byte[5]));
  dst.half[3] = ((u16)((u8)a.byte[7])) + ((u16)((u8)b.byte[7]));
  dst.half[4] = ((u16)((u8)a.byte[9])) + ((u16)((u8)b.byte[9]));
  dst.half[5] = ((u16)((u8)a.byte[11])) + ((u16)((u8)b.byte[11]));
  dst.half[6] = ((u16)((u8)a.byte[13])) + ((u16)((u8)b.byte[13]));
  dst.half[7] = ((u16)((u8)a.byte[15])) + ((u16)((u8)b.byte[15]));
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

## \_\_m128i \_\_lsx_vaddwod_h_bu_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vaddwod_h_bu_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vaddwod.h.bu.b vr, vr, vr
CPU Flags: LSX
```

### Description

Add odd-positioned unsigned 8-bit elements in `a` and signed elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (u16)(u8)a.byte[2 * i + 1] + (s16)(s8)b.byte[2 * i + 1];
}

// Expands to:

if (0) {
  dst.half[0] = ((u16)((u8)a.byte[1])) + ((s16)((s8)b.byte[1]));
  dst.half[1] = ((u16)((u8)a.byte[3])) + ((s16)((s8)b.byte[3]));
  dst.half[2] = ((u16)((u8)a.byte[5])) + ((s16)((s8)b.byte[5]));
  dst.half[3] = ((u16)((u8)a.byte[7])) + ((s16)((s8)b.byte[7]));
  dst.half[4] = ((u16)((u8)a.byte[9])) + ((s16)((s8)b.byte[9]));
  dst.half[5] = ((u16)((u8)a.byte[11])) + ((s16)((s8)b.byte[11]));
  dst.half[6] = ((u16)((u8)a.byte[13])) + ((s16)((s8)b.byte[13]));
  dst.half[7] = ((u16)((u8)a.byte[15])) + ((s16)((s8)b.byte[15]));
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

## \_\_m128i \_\_lsx_vaddwod_w_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vaddwod_w_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vaddwod.w.h vr, vr, vr
CPU Flags: LSX
```

### Description

Add odd-positioned signed 16-bit elements in `a` and signed elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (s32)(s16)a.half[2 * i + 1] + (s32)(s16)b.half[2 * i + 1];
}

// Expands to:

if (0) {
  dst.word[0] = ((s32)((s16)a.half[1])) + ((s32)((s16)b.half[1]));
  dst.word[1] = ((s32)((s16)a.half[3])) + ((s32)((s16)b.half[3]));
  dst.word[2] = ((s32)((s16)a.half[5])) + ((s32)((s16)b.half[5]));
  dst.word[3] = ((s32)((s16)a.half[7])) + ((s32)((s16)b.half[7]));
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

## \_\_m128i \_\_lsx_vaddwod_w_hu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vaddwod_w_hu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vaddwod.w.hu vr, vr, vr
CPU Flags: LSX
```

### Description

Add odd-positioned unsigned 16-bit elements in `a` and unsigned elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i + 1] + (u32)(u16)b.half[2 * i + 1];
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[1])) + ((u32)((u16)b.half[1]));
  dst.word[1] = ((u32)((u16)a.half[3])) + ((u32)((u16)b.half[3]));
  dst.word[2] = ((u32)((u16)a.half[5])) + ((u32)((u16)b.half[5]));
  dst.word[3] = ((u32)((u16)a.half[7])) + ((u32)((u16)b.half[7]));
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

## \_\_m128i \_\_lsx_vaddwod_w_hu_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vaddwod_w_hu_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vaddwod.w.hu.h vr, vr, vr
CPU Flags: LSX
```

### Description

Add odd-positioned unsigned 16-bit elements in `a` and signed elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i + 1] + (s32)(s16)b.half[2 * i + 1];
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[1])) + ((s32)((s16)b.half[1]));
  dst.word[1] = ((u32)((u16)a.half[3])) + ((s32)((s16)b.half[3]));
  dst.word[2] = ((u32)((u16)a.half[5])) + ((s32)((s16)b.half[5]));
  dst.word[3] = ((u32)((u16)a.half[7])) + ((s32)((s16)b.half[7]));
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

## \_\_m128i \_\_lsx_vaddwod_d_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vaddwod_d_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vaddwod.d.w vr, vr, vr
CPU Flags: LSX
```

### Description

Add odd-positioned signed 32-bit elements in `a` and signed elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)(s32)a.word[2 * i + 1] + (s64)(s32)b.word[2 * i + 1];
}

// Expands to:

if (0) {
  dst.dword[0] = ((s64)((s32)a.word[1])) + ((s64)((s32)b.word[1]));
  dst.dword[1] = ((s64)((s32)a.word[3])) + ((s64)((s32)b.word[3]));
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

## \_\_m128i \_\_lsx_vaddwod_d_wu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vaddwod_d_wu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vaddwod.d.wu vr, vr, vr
CPU Flags: LSX
```

### Description

Add odd-positioned unsigned 32-bit elements in `a` and unsigned elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 * i + 1] + (u64)(u32)b.word[2 * i + 1];
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[1])) + ((u64)((u32)b.word[1]));
  dst.dword[1] = ((u64)((u32)a.word[3])) + ((u64)((u32)b.word[3]));
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

## \_\_m128i \_\_lsx_vaddwod_d_wu_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vaddwod_d_wu_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vaddwod.d.wu.w vr, vr, vr
CPU Flags: LSX
```

### Description

Add odd-positioned unsigned 32-bit elements in `a` and signed elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 * i + 1] + (s64)(s32)b.word[2 * i + 1];
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[1])) + ((s64)((s32)b.word[1]));
  dst.dword[1] = ((u64)((u32)a.word[3])) + ((s64)((s32)b.word[3]));
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

## \_\_m128i \_\_lsx_vaddwod_q_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vaddwod_q_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vaddwod.q.d vr, vr, vr
CPU Flags: LSX
```

### Description

Add odd-positioned signed 64-bit elements in `a` and signed elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] = (s128)(s64)a.dword[2 * i + 1] + (s128)(s64)b.dword[2 * i + 1];
}

// Expands to:

if (0) {
  dst.qword[0] = ((s128)((s64)a.dword[1])) + ((s128)((s64)b.dword[1]));
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

## \_\_m128i \_\_lsx_vaddwod_q_du (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vaddwod_q_du (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vaddwod.q.du vr, vr, vr
CPU Flags: LSX
```

### Description

Add odd-positioned unsigned 64-bit elements in `a` and unsigned elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 * i + 1] + (u128)(u64)b.dword[2 * i + 1];
}

// Expands to:

if (0) {
  dst.qword[0] = ((u128)((u64)a.dword[1])) + ((u128)((u64)b.dword[1]));
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

## \_\_m128i \_\_lsx_vaddwod_q_du_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vaddwod_q_du_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vaddwod.q.du.d vr, vr, vr
CPU Flags: LSX
```

### Description

Add odd-positioned unsigned 64-bit elements in `a` and signed elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 * i + 1] + (s128)(s64)b.dword[2 * i + 1];
}

// Expands to:

if (0) {
  dst.qword[0] = ((u128)((u64)a.dword[1])) + ((s128)((s64)b.dword[1]));
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

## \_\_m128i \_\_lsx_vavg_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vavg_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vavg.b vr, vr, vr
CPU Flags: LSX
```

### Description

Compute the average (rounded towards negative infinity) of signed 8-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = ((s8)a.byte[i] >> 1) + ((s8)b.byte[i] >> 1) +
                ((a.byte[i] & b.byte[i]) & 1);
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

## \_\_m128i \_\_lsx_vavg_bu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vavg_bu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vavg.bu vr, vr, vr
CPU Flags: LSX
```

### Description

Compute the average (rounded towards negative infinity) of unsigned 8-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = ((u8)a.byte[i] >> 1) + ((u8)b.byte[i] >> 1) +
                ((a.byte[i] & b.byte[i]) & 1);
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

## \_\_m128i \_\_lsx_vavg_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vavg_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vavg.h vr, vr, vr
CPU Flags: LSX
```

### Description

Compute the average (rounded towards negative infinity) of signed 16-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = ((s16)a.half[i] >> 1) + ((s16)b.half[i] >> 1) +
                ((a.half[i] & b.half[i]) & 1);
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

## \_\_m128i \_\_lsx_vavg_hu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vavg_hu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vavg.hu vr, vr, vr
CPU Flags: LSX
```

### Description

Compute the average (rounded towards negative infinity) of unsigned 16-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = ((u16)a.half[i] >> 1) + ((u16)b.half[i] >> 1) +
                ((a.half[i] & b.half[i]) & 1);
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

## \_\_m128i \_\_lsx_vavg_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vavg_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vavg.w vr, vr, vr
CPU Flags: LSX
```

### Description

Compute the average (rounded towards negative infinity) of signed 32-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = ((s32)a.word[i] >> 1) + ((s32)b.word[i] >> 1) +
                ((a.word[i] & b.word[i]) & 1);
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

## \_\_m128i \_\_lsx_vavg_wu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vavg_wu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vavg.wu vr, vr, vr
CPU Flags: LSX
```

### Description

Compute the average (rounded towards negative infinity) of unsigned 32-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = ((u32)a.word[i] >> 1) + ((u32)b.word[i] >> 1) +
                ((a.word[i] & b.word[i]) & 1);
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

## \_\_m128i \_\_lsx_vavg_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vavg_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vavg.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compute the average (rounded towards negative infinity) of signed 64-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = ((s64)a.dword[i] >> 1) + ((s64)b.dword[i] >> 1) +
                 ((a.dword[i] & b.dword[i]) & 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vavg_du (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vavg_du (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vavg.du vr, vr, vr
CPU Flags: LSX
```

### Description

Compute the average (rounded towards negative infinity) of unsigned 64-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = ((u64)a.dword[i] >> 1) + ((u64)b.dword[i] >> 1) +
                 ((a.dword[i] & b.dword[i]) & 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vavgr_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vavgr_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vavgr.b vr, vr, vr
CPU Flags: LSX
```

### Description

Compute the average (rounded towards positive infinity) of signed 8-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = ((s8)a.byte[i] >> 1) + ((s8)b.byte[i] >> 1) +
                ((a.byte[i] | b.byte[i]) & 1);
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

## \_\_m128i \_\_lsx_vavgr_bu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vavgr_bu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vavgr.bu vr, vr, vr
CPU Flags: LSX
```

### Description

Compute the average (rounded towards positive infinity) of unsigned 8-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = ((u8)a.byte[i] >> 1) + ((u8)b.byte[i] >> 1) +
                ((a.byte[i] | b.byte[i]) & 1);
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

## \_\_m128i \_\_lsx_vavgr_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vavgr_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vavgr.h vr, vr, vr
CPU Flags: LSX
```

### Description

Compute the average (rounded towards positive infinity) of signed 16-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = ((s16)a.half[i] >> 1) + ((s16)b.half[i] >> 1) +
                ((a.half[i] | b.half[i]) & 1);
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

## \_\_m128i \_\_lsx_vavgr_hu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vavgr_hu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vavgr.hu vr, vr, vr
CPU Flags: LSX
```

### Description

Compute the average (rounded towards positive infinity) of unsigned 16-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = ((u16)a.half[i] >> 1) + ((u16)b.half[i] >> 1) +
                ((a.half[i] | b.half[i]) & 1);
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

## \_\_m128i \_\_lsx_vavgr_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vavgr_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vavgr.w vr, vr, vr
CPU Flags: LSX
```

### Description

Compute the average (rounded towards positive infinity) of signed 32-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = ((s32)a.word[i] >> 1) + ((s32)b.word[i] >> 1) +
                ((a.word[i] | b.word[i]) & 1);
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

## \_\_m128i \_\_lsx_vavgr_wu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vavgr_wu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vavgr.wu vr, vr, vr
CPU Flags: LSX
```

### Description

Compute the average (rounded towards positive infinity) of unsigned 32-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = ((u32)a.word[i] >> 1) + ((u32)b.word[i] >> 1) +
                ((a.word[i] | b.word[i]) & 1);
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

## \_\_m128i \_\_lsx_vavgr_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vavgr_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vavgr.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compute the average (rounded towards positive infinity) of signed 64-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = ((s64)a.dword[i] >> 1) + ((s64)b.dword[i] >> 1) +
                 ((a.dword[i] | b.dword[i]) & 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vavgr_du (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vavgr_du (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vavgr.du vr, vr, vr
CPU Flags: LSX
```

### Description

Compute the average (rounded towards positive infinity) of unsigned 64-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = ((u64)a.dword[i] >> 1) + ((u64)b.dword[i] >> 1) +
                 ((a.dword[i] | b.dword[i]) & 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vdiv_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vdiv_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vdiv.b vr, vr, vr
CPU Flags: LSX
```

### Description

Divide signed 8-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (b.byte[i] == 0) ? 0 : ((s8)a.byte[i] / (s8)b.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 29, 32  | 0.06(1/17)       |
| 3A6000   | LA664  | 29, 32  | 0.06(1/15.5)     |
| 3C6000   | LA664  | 29, 32  | 0.07(1/13.5)     |
| 2K1000LA | LA264  | 30, 58  | 0(1/55)          |
| 2K3000   | LA364E | 30      | 0(1/55)          |

## \_\_m128i \_\_lsx_vdiv_bu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vdiv_bu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vdiv.bu vr, vr, vr
CPU Flags: LSX
```

### Description

Divide unsigned 8-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (b.byte[i] == 0) ? 0 : ((u8)a.byte[i] / (u8)b.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 29, 36  | 0.06(1/18)       |
| 3A6000   | LA664  | 29, 33  | 0.06(1/16.5)     |
| 3C6000   | LA664  | 29, 36  | 0.07(1/13.5)     |
| 2K1000LA | LA264  | 30, 44  | 0(1/55)          |
| 2K3000   | LA364E | 30      | 0(1/55)          |

## \_\_m128i \_\_lsx_vdiv_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vdiv_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vdiv.h vr, vr, vr
CPU Flags: LSX
```

### Description

Divide signed 16-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (b.half[i] == 0) ? 0 : ((s16)a.half[i] / (s16)b.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency  | Throughput (IPC) |
| -------- | ------ | -------- | ---------------- |
| 3C5000   | LA464  | 17, 21.5 | 0.09(1/11)       |
| 3A6000   | LA664  | 17       | 0.12(1/8.5)      |
| 3C6000   | LA664  | 17, 21.5 | 0.13(1/7.5)      |
| 2K1000LA | LA264  | 18, 36   | 0.03(1/31)       |
| 2K3000   | LA364E | 18, 27   | 0.03(1/31)       |

## \_\_m128i \_\_lsx_vdiv_hu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vdiv_hu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vdiv.hu vr, vr, vr
CPU Flags: LSX
```

### Description

Divide unsigned 16-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (b.half[i] == 0) ? 0 : ((u16)a.half[i] / (u16)b.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency  | Throughput (IPC) |
| -------- | ------ | -------- | ---------------- |
| 3C5000   | LA464  | 17, 21.5 | 0.07(1/14)       |
| 3A6000   | LA664  | 17, 22   | 0.11(1/9)        |
| 3C6000   | LA664  | 17, 21.5 | 0.13(1/7.5)      |
| 2K1000LA | LA264  | 18, 27   | 0.03(1/31)       |
| 2K3000   | LA364E | 18, 27   | 0.03(1/31)       |

## \_\_m128i \_\_lsx_vdiv_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vdiv_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vdiv.w vr, vr, vr
CPU Flags: LSX
```

### Description

Divide signed 32-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (b.word[i] == 0) ? 0 : ((s32)a.word[i] / (s32)b.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency  | Throughput (IPC) |
| -------- | ------ | -------- | ---------------- |
| 3C5000   | LA464  | 11, 17.5 | 0.09(1/11.5)     |
| 3A6000   | LA664  | 11       | 0.18(1/5.5)      |
| 3C6000   | LA664  | 11, 17.5 | 0.22(1/4.5)      |
| 2K1000LA | LA264  | 12, 25   | 0.06(1/18)       |
| 2K3000   | LA364E | 12, 18.5 | 0.06(1/18)       |

## \_\_m128i \_\_lsx_vdiv_wu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vdiv_wu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vdiv.wu vr, vr, vr
CPU Flags: LSX
```

### Description

Divide unsigned 32-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (b.word[i] == 0) ? 0 : ((u32)a.word[i] / (u32)b.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency  | Throughput (IPC) |
| -------- | ------ | -------- | ---------------- |
| 3C5000   | LA464  | 11, 17.5 | 0.07(1/15)       |
| 3A6000   | LA664  | 11       | 0.18(1/5.5)      |
| 3C6000   | LA664  | 11, 17.5 | 0.22(1/4.5)      |
| 2K1000LA | LA264  | 12       | 0.06(1/18)       |
| 2K3000   | LA364E | 12, 18.5 | 0.06(1/18)       |

## \_\_m128i \_\_lsx_vdiv_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vdiv_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vdiv.d vr, vr, vr
CPU Flags: LSX
```

### Description

Divide signed 64-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (b.dword[i] == 0) ? 0 : ((s64)a.dword[i] / (s64)b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 8, 18.5 | 0.11(1/9)        |
| 3A6000   | LA664  | 8       | 0.25(1/4)        |
| 3C6000   | LA664  | 8, 18.5 | 0.33(1/3)        |
| 2K1000LA | LA264  | 9, 19.5 | 0.08(1/12)       |
| 2K3000   | LA364E | 9       | 0.08(1/12)       |

## \_\_m128i \_\_lsx_vdiv_du (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vdiv_du (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vdiv.du vr, vr, vr
CPU Flags: LSX
```

### Description

Divide unsigned 64-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (b.dword[i] == 0) ? 0 : ((u64)a.dword[i] / (u64)b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 8, 18.5 | 0.11(1/9)        |
| 3A6000   | LA664  | 8       | 0.25(1/4)        |
| 3C6000   | LA664  | 8, 18.5 | 0.33(1/3)        |
| 2K1000LA | LA264  | 9       | 0.08(1/12)       |
| 2K3000   | LA364E | 9       | 0.08(1/12)       |

## \_\_m128i \_\_lsx_vhaddw_h_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vhaddw_h_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vhaddw.h.b vr, vr, vr
CPU Flags: LSX
```

### Description

Add odd-positioned signed 8-bit elements in `a` to even-positioned signed 8-bit elements in `b` to get 16-bit result.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (s16)(s8)a.byte[2 * i + 1] + (s16)(s8)b.byte[2 * i];
}

// Expands to:

if (0) {
  dst.half[0] = ((s16)((s8)a.byte[1])) + ((s16)((s8)b.byte[0]));
  dst.half[1] = ((s16)((s8)a.byte[3])) + ((s16)((s8)b.byte[2]));
  dst.half[2] = ((s16)((s8)a.byte[5])) + ((s16)((s8)b.byte[4]));
  dst.half[3] = ((s16)((s8)a.byte[7])) + ((s16)((s8)b.byte[6]));
  dst.half[4] = ((s16)((s8)a.byte[9])) + ((s16)((s8)b.byte[8]));
  dst.half[5] = ((s16)((s8)a.byte[11])) + ((s16)((s8)b.byte[10]));
  dst.half[6] = ((s16)((s8)a.byte[13])) + ((s16)((s8)b.byte[12]));
  dst.half[7] = ((s16)((s8)a.byte[15])) + ((s16)((s8)b.byte[14]));
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

## \_\_m128i \_\_lsx_vhaddw_hu_bu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vhaddw_hu_bu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vhaddw.hu.bu vr, vr, vr
CPU Flags: LSX
```

### Description

Add odd-positioned unsigned 8-bit elements in `a` to even-positioned unsigned 8-bit elements in `b` to get 16-bit result.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (u16)(u8)a.byte[2 * i + 1] + (u16)(u8)b.byte[2 * i];
}

// Expands to:

if (0) {
  dst.half[0] = ((u16)((u8)a.byte[1])) + ((u16)((u8)b.byte[0]));
  dst.half[1] = ((u16)((u8)a.byte[3])) + ((u16)((u8)b.byte[2]));
  dst.half[2] = ((u16)((u8)a.byte[5])) + ((u16)((u8)b.byte[4]));
  dst.half[3] = ((u16)((u8)a.byte[7])) + ((u16)((u8)b.byte[6]));
  dst.half[4] = ((u16)((u8)a.byte[9])) + ((u16)((u8)b.byte[8]));
  dst.half[5] = ((u16)((u8)a.byte[11])) + ((u16)((u8)b.byte[10]));
  dst.half[6] = ((u16)((u8)a.byte[13])) + ((u16)((u8)b.byte[12]));
  dst.half[7] = ((u16)((u8)a.byte[15])) + ((u16)((u8)b.byte[14]));
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

## \_\_m128i \_\_lsx_vhaddw_w_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vhaddw_w_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vhaddw.w.h vr, vr, vr
CPU Flags: LSX
```

### Description

Add odd-positioned signed 16-bit elements in `a` to even-positioned signed 16-bit elements in `b` to get 32-bit result.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (s32)(s16)a.half[2 * i + 1] + (s32)(s16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((s32)((s16)a.half[1])) + ((s32)((s16)b.half[0]));
  dst.word[1] = ((s32)((s16)a.half[3])) + ((s32)((s16)b.half[2]));
  dst.word[2] = ((s32)((s16)a.half[5])) + ((s32)((s16)b.half[4]));
  dst.word[3] = ((s32)((s16)a.half[7])) + ((s32)((s16)b.half[6]));
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

## \_\_m128i \_\_lsx_vhaddw_wu_hu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vhaddw_wu_hu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vhaddw.wu.hu vr, vr, vr
CPU Flags: LSX
```

### Description

Add odd-positioned unsigned 16-bit elements in `a` to even-positioned unsigned 16-bit elements in `b` to get 32-bit result.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i + 1] + (u32)(u16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[1])) + ((u32)((u16)b.half[0]));
  dst.word[1] = ((u32)((u16)a.half[3])) + ((u32)((u16)b.half[2]));
  dst.word[2] = ((u32)((u16)a.half[5])) + ((u32)((u16)b.half[4]));
  dst.word[3] = ((u32)((u16)a.half[7])) + ((u32)((u16)b.half[6]));
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

## \_\_m128i \_\_lsx_vhaddw_d_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vhaddw_d_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vhaddw.d.w vr, vr, vr
CPU Flags: LSX
```

### Description

Add odd-positioned signed 32-bit elements in `a` to even-positioned signed 32-bit elements in `b` to get 64-bit result.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)(s32)a.word[2 * i + 1] + (s64)(s32)b.word[2 * i];
}

// Expands to:

if (0) {
  dst.dword[0] = ((s64)((s32)a.word[1])) + ((s64)((s32)b.word[0]));
  dst.dword[1] = ((s64)((s32)a.word[3])) + ((s64)((s32)b.word[2]));
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

## \_\_m128i \_\_lsx_vhaddw_du_wu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vhaddw_du_wu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vhaddw.du.wu vr, vr, vr
CPU Flags: LSX
```

### Description

Add odd-positioned unsigned 32-bit elements in `a` to even-positioned unsigned 32-bit elements in `b` to get 64-bit result.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 * i + 1] + (u64)(u32)b.word[2 * i];
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[1])) + ((u64)((u32)b.word[0]));
  dst.dword[1] = ((u64)((u32)a.word[3])) + ((u64)((u32)b.word[2]));
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

## \_\_m128i \_\_lsx_vhaddw_q_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vhaddw_q_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vhaddw.q.d vr, vr, vr
CPU Flags: LSX
```

### Description

Add odd-positioned signed 64-bit elements in `a` to even-positioned signed 64-bit elements in `b` to get 128-bit result.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] = (s128)(s64)a.dword[2 * i + 1] + (s128)(s64)b.dword[2 * i];
}

// Expands to:

if (0) {
  dst.qword[0] = ((s128)((s64)a.dword[1])) + ((s128)((s64)b.dword[0]));
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

## \_\_m128i \_\_lsx_vhaddw_qu_du (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vhaddw_qu_du (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vhaddw.qu.du vr, vr, vr
CPU Flags: LSX
```

### Description

Add odd-positioned unsigned 64-bit elements in `a` to even-positioned unsigned 64-bit elements in `b` to get 128-bit result.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 * i + 1] + (u128)(u64)b.dword[2 * i];
}

// Expands to:

if (0) {
  dst.qword[0] = ((u128)((u64)a.dword[1])) + ((u128)((u64)b.dword[0]));
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

## \_\_m128i \_\_lsx_vhsubw_h_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vhsubw_h_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vhsubw.h.b vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract odd-positioned signed 8-bit elements in `a` by even-positioned signed 8-bit elements in `b` to get 16-bit result.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (s16)(s8)a.byte[2 * i + 1] - (s16)(s8)b.byte[2 * i];
}

// Expands to:

if (0) {
  dst.half[0] = ((s16)((s8)a.byte[1])) - ((s16)((s8)b.byte[0]));
  dst.half[1] = ((s16)((s8)a.byte[3])) - ((s16)((s8)b.byte[2]));
  dst.half[2] = ((s16)((s8)a.byte[5])) - ((s16)((s8)b.byte[4]));
  dst.half[3] = ((s16)((s8)a.byte[7])) - ((s16)((s8)b.byte[6]));
  dst.half[4] = ((s16)((s8)a.byte[9])) - ((s16)((s8)b.byte[8]));
  dst.half[5] = ((s16)((s8)a.byte[11])) - ((s16)((s8)b.byte[10]));
  dst.half[6] = ((s16)((s8)a.byte[13])) - ((s16)((s8)b.byte[12]));
  dst.half[7] = ((s16)((s8)a.byte[15])) - ((s16)((s8)b.byte[14]));
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

## \_\_m128i \_\_lsx_vhsubw_hu_bu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vhsubw_hu_bu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vhsubw.hu.bu vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract odd-positioned unsigned 8-bit elements in `a` by even-positioned unsigned 8-bit elements in `b` to get 16-bit result.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (u16)(u8)a.byte[2 * i + 1] - (u16)(u8)b.byte[2 * i];
}

// Expands to:

if (0) {
  dst.half[0] = ((u16)((u8)a.byte[1])) - ((u16)((u8)b.byte[0]));
  dst.half[1] = ((u16)((u8)a.byte[3])) - ((u16)((u8)b.byte[2]));
  dst.half[2] = ((u16)((u8)a.byte[5])) - ((u16)((u8)b.byte[4]));
  dst.half[3] = ((u16)((u8)a.byte[7])) - ((u16)((u8)b.byte[6]));
  dst.half[4] = ((u16)((u8)a.byte[9])) - ((u16)((u8)b.byte[8]));
  dst.half[5] = ((u16)((u8)a.byte[11])) - ((u16)((u8)b.byte[10]));
  dst.half[6] = ((u16)((u8)a.byte[13])) - ((u16)((u8)b.byte[12]));
  dst.half[7] = ((u16)((u8)a.byte[15])) - ((u16)((u8)b.byte[14]));
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

## \_\_m128i \_\_lsx_vhsubw_w_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vhsubw_w_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vhsubw.w.h vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract odd-positioned signed 16-bit elements in `a` by even-positioned signed 16-bit elements in `b` to get 32-bit result.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (s32)(s16)a.half[2 * i + 1] - (s32)(s16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((s32)((s16)a.half[1])) - ((s32)((s16)b.half[0]));
  dst.word[1] = ((s32)((s16)a.half[3])) - ((s32)((s16)b.half[2]));
  dst.word[2] = ((s32)((s16)a.half[5])) - ((s32)((s16)b.half[4]));
  dst.word[3] = ((s32)((s16)a.half[7])) - ((s32)((s16)b.half[6]));
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

## \_\_m128i \_\_lsx_vhsubw_wu_hu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vhsubw_wu_hu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vhsubw.wu.hu vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract odd-positioned unsigned 16-bit elements in `a` by even-positioned unsigned 16-bit elements in `b` to get 32-bit result.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i + 1] - (u32)(u16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[1])) - ((u32)((u16)b.half[0]));
  dst.word[1] = ((u32)((u16)a.half[3])) - ((u32)((u16)b.half[2]));
  dst.word[2] = ((u32)((u16)a.half[5])) - ((u32)((u16)b.half[4]));
  dst.word[3] = ((u32)((u16)a.half[7])) - ((u32)((u16)b.half[6]));
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

## \_\_m128i \_\_lsx_vhsubw_d_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vhsubw_d_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vhsubw.d.w vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract odd-positioned signed 32-bit elements in `a` by even-positioned signed 32-bit elements in `b` to get 64-bit result.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)(s32)a.word[2 * i + 1] - (s64)(s32)b.word[2 * i];
}

// Expands to:

if (0) {
  dst.dword[0] = ((s64)((s32)a.word[1])) - ((s64)((s32)b.word[0]));
  dst.dword[1] = ((s64)((s32)a.word[3])) - ((s64)((s32)b.word[2]));
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

## \_\_m128i \_\_lsx_vhsubw_du_wu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vhsubw_du_wu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vhsubw.du.wu vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract odd-positioned unsigned 32-bit elements in `a` by even-positioned unsigned 32-bit elements in `b` to get 64-bit result.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 * i + 1] - (u64)(u32)b.word[2 * i];
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[1])) - ((u64)((u32)b.word[0]));
  dst.dword[1] = ((u64)((u32)a.word[3])) - ((u64)((u32)b.word[2]));
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

## \_\_m128i \_\_lsx_vhsubw_q_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vhsubw_q_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vhsubw.q.d vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract odd-positioned signed 64-bit elements in `a` by even-positioned signed 64-bit elements in `b` to get 128-bit result.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] = (s128)(s64)a.dword[2 * i + 1] - (s128)(s64)b.dword[2 * i];
}

// Expands to:

if (0) {
  dst.qword[0] = ((s128)((s64)a.dword[1])) - ((s128)((s64)b.dword[0]));
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

## \_\_m128i \_\_lsx_vhsubw_qu_du (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vhsubw_qu_du (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vhsubw.qu.du vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract odd-positioned unsigned 64-bit elements in `a` by even-positioned unsigned 64-bit elements in `b` to get 128-bit result.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 * i + 1] - (u128)(u64)b.dword[2 * i];
}

// Expands to:

if (0) {
  dst.qword[0] = ((u128)((u64)a.dword[1])) - ((u128)((u64)b.dword[0]));
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

## \_\_m128i \_\_lsx_vmadd_b (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmadd_b (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmadd.b vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply 8-bit elements in `b` and `c`, add to elements in `a`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = b.byte[i] * c.byte[i] + a.byte[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmadd_h (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmadd_h (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmadd.h vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply 16-bit elements in `b` and `c`, add to elements in `a`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = b.half[i] * c.half[i] + a.half[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmadd_w (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmadd_w (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmadd.w vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply 32-bit elements in `b` and `c`, add to elements in `a`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = b.word[i] * c.word[i] + a.word[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmadd_d (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmadd_d (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmadd.d vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply 64-bit elements in `b` and `c`, add to elements in `a`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = b.dword[i] * c.dword[i] + a.dword[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmaddwev_h_b (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmaddwev_h_b (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmaddwev.h.b vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply even-positioned signed 8-bit elements in `b` and signed elements in `c`, add to 16-bit elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] =
      (s16)(s8)b.byte[2 * i] * (s16)(s8)c.byte[2 * i] + (s16)a.half[i];
}

// Expands to:

if (0) {
  dst.half[0] =
      (((s16)((s8)b.byte[0])) * ((s16)((s8)c.byte[0]))) + ((s16)a.half[0]);
  dst.half[1] =
      (((s16)((s8)b.byte[2])) * ((s16)((s8)c.byte[2]))) + ((s16)a.half[1]);
  dst.half[2] =
      (((s16)((s8)b.byte[4])) * ((s16)((s8)c.byte[4]))) + ((s16)a.half[2]);
  dst.half[3] =
      (((s16)((s8)b.byte[6])) * ((s16)((s8)c.byte[6]))) + ((s16)a.half[3]);
  dst.half[4] =
      (((s16)((s8)b.byte[8])) * ((s16)((s8)c.byte[8]))) + ((s16)a.half[4]);
  dst.half[5] =
      (((s16)((s8)b.byte[10])) * ((s16)((s8)c.byte[10]))) + ((s16)a.half[5]);
  dst.half[6] =
      (((s16)((s8)b.byte[12])) * ((s16)((s8)c.byte[12]))) + ((s16)a.half[6]);
  dst.half[7] =
      (((s16)((s8)b.byte[14])) * ((s16)((s8)c.byte[14]))) + ((s16)a.half[7]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmaddwev_h_bu (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmaddwev_h_bu (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmaddwev.h.bu vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply even-positioned unsigned 8-bit elements in `b` and unsigned elements in `c`, add to 16-bit elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] =
      (u16)(u8)b.byte[2 * i] * (u16)(u8)c.byte[2 * i] + (u16)a.half[i];
}

// Expands to:

if (0) {
  dst.half[0] =
      (((u16)((u8)b.byte[0])) * ((u16)((u8)c.byte[0]))) + ((u16)a.half[0]);
  dst.half[1] =
      (((u16)((u8)b.byte[2])) * ((u16)((u8)c.byte[2]))) + ((u16)a.half[1]);
  dst.half[2] =
      (((u16)((u8)b.byte[4])) * ((u16)((u8)c.byte[4]))) + ((u16)a.half[2]);
  dst.half[3] =
      (((u16)((u8)b.byte[6])) * ((u16)((u8)c.byte[6]))) + ((u16)a.half[3]);
  dst.half[4] =
      (((u16)((u8)b.byte[8])) * ((u16)((u8)c.byte[8]))) + ((u16)a.half[4]);
  dst.half[5] =
      (((u16)((u8)b.byte[10])) * ((u16)((u8)c.byte[10]))) + ((u16)a.half[5]);
  dst.half[6] =
      (((u16)((u8)b.byte[12])) * ((u16)((u8)c.byte[12]))) + ((u16)a.half[6]);
  dst.half[7] =
      (((u16)((u8)b.byte[14])) * ((u16)((u8)c.byte[14]))) + ((u16)a.half[7]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmaddwev_h_bu_b (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmaddwev_h_bu_b (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmaddwev.h.bu.b vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply even-positioned unsigned 8-bit elements in `b` and signed elements in `c`, add to 16-bit elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] =
      (u16)(u8)b.byte[2 * i] * (s16)(s8)c.byte[2 * i] + (s16)a.half[i];
}

// Expands to:

if (0) {
  dst.half[0] =
      (((u16)((u8)b.byte[0])) * ((s16)((s8)c.byte[0]))) + ((s16)a.half[0]);
  dst.half[1] =
      (((u16)((u8)b.byte[2])) * ((s16)((s8)c.byte[2]))) + ((s16)a.half[1]);
  dst.half[2] =
      (((u16)((u8)b.byte[4])) * ((s16)((s8)c.byte[4]))) + ((s16)a.half[2]);
  dst.half[3] =
      (((u16)((u8)b.byte[6])) * ((s16)((s8)c.byte[6]))) + ((s16)a.half[3]);
  dst.half[4] =
      (((u16)((u8)b.byte[8])) * ((s16)((s8)c.byte[8]))) + ((s16)a.half[4]);
  dst.half[5] =
      (((u16)((u8)b.byte[10])) * ((s16)((s8)c.byte[10]))) + ((s16)a.half[5]);
  dst.half[6] =
      (((u16)((u8)b.byte[12])) * ((s16)((s8)c.byte[12]))) + ((s16)a.half[6]);
  dst.half[7] =
      (((u16)((u8)b.byte[14])) * ((s16)((s8)c.byte[14]))) + ((s16)a.half[7]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmaddwev_w_h (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmaddwev_w_h (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmaddwev.w.h vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply even-positioned signed 16-bit elements in `b` and signed elements in `c`, add to 32-bit elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] =
      (s32)(s16)b.half[2 * i] * (s32)(s16)c.half[2 * i] + (s32)a.word[i];
}

// Expands to:

if (0) {
  dst.word[0] =
      (((s32)((s16)b.half[0])) * ((s32)((s16)c.half[0]))) + ((s32)a.word[0]);
  dst.word[1] =
      (((s32)((s16)b.half[2])) * ((s32)((s16)c.half[2]))) + ((s32)a.word[1]);
  dst.word[2] =
      (((s32)((s16)b.half[4])) * ((s32)((s16)c.half[4]))) + ((s32)a.word[2]);
  dst.word[3] =
      (((s32)((s16)b.half[6])) * ((s32)((s16)c.half[6]))) + ((s32)a.word[3]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmaddwev_w_hu (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmaddwev_w_hu (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmaddwev.w.hu vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply even-positioned unsigned 16-bit elements in `b` and unsigned elements in `c`, add to 32-bit elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] =
      (u32)(u16)b.half[2 * i] * (u32)(u16)c.half[2 * i] + (u32)a.word[i];
}

// Expands to:

if (0) {
  dst.word[0] =
      (((u32)((u16)b.half[0])) * ((u32)((u16)c.half[0]))) + ((u32)a.word[0]);
  dst.word[1] =
      (((u32)((u16)b.half[2])) * ((u32)((u16)c.half[2]))) + ((u32)a.word[1]);
  dst.word[2] =
      (((u32)((u16)b.half[4])) * ((u32)((u16)c.half[4]))) + ((u32)a.word[2]);
  dst.word[3] =
      (((u32)((u16)b.half[6])) * ((u32)((u16)c.half[6]))) + ((u32)a.word[3]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmaddwev_w_hu_h (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmaddwev_w_hu_h (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmaddwev.w.hu.h vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply even-positioned unsigned 16-bit elements in `b` and signed elements in `c`, add to 32-bit elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] =
      (u32)(u16)b.half[2 * i] * (s32)(s16)c.half[2 * i] + (s32)a.word[i];
}

// Expands to:

if (0) {
  dst.word[0] =
      (((u32)((u16)b.half[0])) * ((s32)((s16)c.half[0]))) + ((s32)a.word[0]);
  dst.word[1] =
      (((u32)((u16)b.half[2])) * ((s32)((s16)c.half[2]))) + ((s32)a.word[1]);
  dst.word[2] =
      (((u32)((u16)b.half[4])) * ((s32)((s16)c.half[4]))) + ((s32)a.word[2]);
  dst.word[3] =
      (((u32)((u16)b.half[6])) * ((s32)((s16)c.half[6]))) + ((s32)a.word[3]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmaddwev_d_w (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmaddwev_d_w (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmaddwev.d.w vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply even-positioned signed 32-bit elements in `b` and signed elements in `c`, add to 64-bit elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] =
      (s64)(s32)b.word[2 * i] * (s64)(s32)c.word[2 * i] + (s64)a.dword[i];
}

// Expands to:

if (0) {
  dst.dword[0] =
      (((s64)((s32)b.word[0])) * ((s64)((s32)c.word[0]))) + ((s64)a.dword[0]);
  dst.dword[1] =
      (((s64)((s32)b.word[2])) * ((s64)((s32)c.word[2]))) + ((s64)a.dword[1]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmaddwev_d_wu (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmaddwev_d_wu (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmaddwev.d.wu vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply even-positioned unsigned 32-bit elements in `b` and unsigned elements in `c`, add to 64-bit elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] =
      (u64)(u32)b.word[2 * i] * (u64)(u32)c.word[2 * i] + (u64)a.dword[i];
}

// Expands to:

if (0) {
  dst.dword[0] =
      (((u64)((u32)b.word[0])) * ((u64)((u32)c.word[0]))) + ((u64)a.dword[0]);
  dst.dword[1] =
      (((u64)((u32)b.word[2])) * ((u64)((u32)c.word[2]))) + ((u64)a.dword[1]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmaddwev_d_wu_w (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmaddwev_d_wu_w (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmaddwev.d.wu.w vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply even-positioned unsigned 32-bit elements in `b` and signed elements in `c`, add to 64-bit elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] =
      (u64)(u32)b.word[2 * i] * (s64)(s32)c.word[2 * i] + (s64)a.dword[i];
}

// Expands to:

if (0) {
  dst.dword[0] =
      (((u64)((u32)b.word[0])) * ((s64)((s32)c.word[0]))) + ((s64)a.dword[0]);
  dst.dword[1] =
      (((u64)((u32)b.word[2])) * ((s64)((s32)c.word[2]))) + ((s64)a.dword[1]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmaddwev_q_d (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmaddwev_q_d (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmaddwev.q.d vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply even-positioned signed 64-bit elements in `b` and signed elements in `c`, add to 128-bit elements in `a`.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] =
      (s128)(s64)b.dword[2 * i] * (s128)(s64)c.dword[2 * i] + (s128)a.qword[i];
}

// Expands to:

if (0) {
  dst.qword[0] = (((s128)((s64)b.dword[0])) * ((s128)((s64)c.dword[0]))) +
                 ((s128)a.qword[0]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 7       | 1.14             |
| 3A6000   | LA664  | 7       | 1.14             |
| 3C6000   | LA664  | 7       | 1.14             |
| 2K1000LA | LA264  | 6       | 1                |
| 2K3000   | LA364E | 6       | 1                |

## \_\_m128i \_\_lsx_vmaddwev_q_du (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmaddwev_q_du (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmaddwev.q.du vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply even-positioned unsigned 64-bit elements in `b` and unsigned elements in `c`, add to 128-bit elements in `a`.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] =
      (u128)(u64)b.dword[2 * i] * (u128)(u64)c.dword[2 * i] + (u128)a.qword[i];
}

// Expands to:

if (0) {
  dst.qword[0] = (((u128)((u64)b.dword[0])) * ((u128)((u64)c.dword[0]))) +
                 ((u128)a.qword[0]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 7       | 1.14             |
| 3A6000   | LA664  | 7       | 1.14             |
| 3C6000   | LA664  | 7       | 1.14             |
| 2K1000LA | LA264  | 6       | 1                |
| 2K3000   | LA364E | 6       | 1                |

## \_\_m128i \_\_lsx_vmaddwev_q_du_d (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmaddwev_q_du_d (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmaddwev.q.du.d vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply even-positioned unsigned 64-bit elements in `b` and signed elements in `c`, add to 128-bit elements in `a`.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] =
      (u128)(u64)b.dword[2 * i] * (s128)(s64)c.dword[2 * i] + (s128)a.qword[i];
}

// Expands to:

if (0) {
  dst.qword[0] = (((u128)((u64)b.dword[0])) * ((s128)((s64)c.dword[0]))) +
                 ((s128)a.qword[0]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 7       | 1.14             |
| 3A6000   | LA664  | 7       | 1.14             |
| 3C6000   | LA664  | 7       | 1.14             |
| 2K1000LA | LA264  | 6       | 1                |
| 2K3000   | LA364E | 6       | 1                |

## \_\_m128i \_\_lsx_vmaddwod_h_b (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmaddwod_h_b (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmaddwod.h.b vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply odd-positioned signed 8-bit elements in `b` and signed elements in `c`, add to 16-bit elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] =
      (s16)(s8)b.byte[2 * i + 1] * (s16)(s8)c.byte[2 * i + 1] + (s16)a.half[i];
}

// Expands to:

if (0) {
  dst.half[0] =
      (((s16)((s8)b.byte[1])) * ((s16)((s8)c.byte[1]))) + ((s16)a.half[0]);
  dst.half[1] =
      (((s16)((s8)b.byte[3])) * ((s16)((s8)c.byte[3]))) + ((s16)a.half[1]);
  dst.half[2] =
      (((s16)((s8)b.byte[5])) * ((s16)((s8)c.byte[5]))) + ((s16)a.half[2]);
  dst.half[3] =
      (((s16)((s8)b.byte[7])) * ((s16)((s8)c.byte[7]))) + ((s16)a.half[3]);
  dst.half[4] =
      (((s16)((s8)b.byte[9])) * ((s16)((s8)c.byte[9]))) + ((s16)a.half[4]);
  dst.half[5] =
      (((s16)((s8)b.byte[11])) * ((s16)((s8)c.byte[11]))) + ((s16)a.half[5]);
  dst.half[6] =
      (((s16)((s8)b.byte[13])) * ((s16)((s8)c.byte[13]))) + ((s16)a.half[6]);
  dst.half[7] =
      (((s16)((s8)b.byte[15])) * ((s16)((s8)c.byte[15]))) + ((s16)a.half[7]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmaddwod_h_bu (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmaddwod_h_bu (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmaddwod.h.bu vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply odd-positioned unsigned 8-bit elements in `b` and unsigned elements in `c`, add to 16-bit elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] =
      (u16)(u8)b.byte[2 * i + 1] * (u16)(u8)c.byte[2 * i + 1] + (u16)a.half[i];
}

// Expands to:

if (0) {
  dst.half[0] =
      (((u16)((u8)b.byte[1])) * ((u16)((u8)c.byte[1]))) + ((u16)a.half[0]);
  dst.half[1] =
      (((u16)((u8)b.byte[3])) * ((u16)((u8)c.byte[3]))) + ((u16)a.half[1]);
  dst.half[2] =
      (((u16)((u8)b.byte[5])) * ((u16)((u8)c.byte[5]))) + ((u16)a.half[2]);
  dst.half[3] =
      (((u16)((u8)b.byte[7])) * ((u16)((u8)c.byte[7]))) + ((u16)a.half[3]);
  dst.half[4] =
      (((u16)((u8)b.byte[9])) * ((u16)((u8)c.byte[9]))) + ((u16)a.half[4]);
  dst.half[5] =
      (((u16)((u8)b.byte[11])) * ((u16)((u8)c.byte[11]))) + ((u16)a.half[5]);
  dst.half[6] =
      (((u16)((u8)b.byte[13])) * ((u16)((u8)c.byte[13]))) + ((u16)a.half[6]);
  dst.half[7] =
      (((u16)((u8)b.byte[15])) * ((u16)((u8)c.byte[15]))) + ((u16)a.half[7]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmaddwod_h_bu_b (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmaddwod_h_bu_b (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmaddwod.h.bu.b vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply odd-positioned unsigned 8-bit elements in `b` and signed elements in `c`, add to 16-bit elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] =
      (u16)(u8)b.byte[2 * i + 1] * (s16)(s8)c.byte[2 * i + 1] + (s16)a.half[i];
}

// Expands to:

if (0) {
  dst.half[0] =
      (((u16)((u8)b.byte[1])) * ((s16)((s8)c.byte[1]))) + ((s16)a.half[0]);
  dst.half[1] =
      (((u16)((u8)b.byte[3])) * ((s16)((s8)c.byte[3]))) + ((s16)a.half[1]);
  dst.half[2] =
      (((u16)((u8)b.byte[5])) * ((s16)((s8)c.byte[5]))) + ((s16)a.half[2]);
  dst.half[3] =
      (((u16)((u8)b.byte[7])) * ((s16)((s8)c.byte[7]))) + ((s16)a.half[3]);
  dst.half[4] =
      (((u16)((u8)b.byte[9])) * ((s16)((s8)c.byte[9]))) + ((s16)a.half[4]);
  dst.half[5] =
      (((u16)((u8)b.byte[11])) * ((s16)((s8)c.byte[11]))) + ((s16)a.half[5]);
  dst.half[6] =
      (((u16)((u8)b.byte[13])) * ((s16)((s8)c.byte[13]))) + ((s16)a.half[6]);
  dst.half[7] =
      (((u16)((u8)b.byte[15])) * ((s16)((s8)c.byte[15]))) + ((s16)a.half[7]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmaddwod_w_h (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmaddwod_w_h (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmaddwod.w.h vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply odd-positioned signed 16-bit elements in `b` and signed elements in `c`, add to 32-bit elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (s32)(s16)b.half[2 * i + 1] * (s32)(s16)c.half[2 * i + 1] +
                (s32)a.word[i];
}

// Expands to:

if (0) {
  dst.word[0] =
      (((s32)((s16)b.half[1])) * ((s32)((s16)c.half[1]))) + ((s32)a.word[0]);
  dst.word[1] =
      (((s32)((s16)b.half[3])) * ((s32)((s16)c.half[3]))) + ((s32)a.word[1]);
  dst.word[2] =
      (((s32)((s16)b.half[5])) * ((s32)((s16)c.half[5]))) + ((s32)a.word[2]);
  dst.word[3] =
      (((s32)((s16)b.half[7])) * ((s32)((s16)c.half[7]))) + ((s32)a.word[3]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmaddwod_w_hu (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmaddwod_w_hu (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmaddwod.w.hu vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply odd-positioned unsigned 16-bit elements in `b` and unsigned elements in `c`, add to 32-bit elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (u32)(u16)b.half[2 * i + 1] * (u32)(u16)c.half[2 * i + 1] +
                (u32)a.word[i];
}

// Expands to:

if (0) {
  dst.word[0] =
      (((u32)((u16)b.half[1])) * ((u32)((u16)c.half[1]))) + ((u32)a.word[0]);
  dst.word[1] =
      (((u32)((u16)b.half[3])) * ((u32)((u16)c.half[3]))) + ((u32)a.word[1]);
  dst.word[2] =
      (((u32)((u16)b.half[5])) * ((u32)((u16)c.half[5]))) + ((u32)a.word[2]);
  dst.word[3] =
      (((u32)((u16)b.half[7])) * ((u32)((u16)c.half[7]))) + ((u32)a.word[3]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmaddwod_w_hu_h (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmaddwod_w_hu_h (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmaddwod.w.hu.h vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply odd-positioned unsigned 16-bit elements in `b` and signed elements in `c`, add to 32-bit elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (u32)(u16)b.half[2 * i + 1] * (s32)(s16)c.half[2 * i + 1] +
                (s32)a.word[i];
}

// Expands to:

if (0) {
  dst.word[0] =
      (((u32)((u16)b.half[1])) * ((s32)((s16)c.half[1]))) + ((s32)a.word[0]);
  dst.word[1] =
      (((u32)((u16)b.half[3])) * ((s32)((s16)c.half[3]))) + ((s32)a.word[1]);
  dst.word[2] =
      (((u32)((u16)b.half[5])) * ((s32)((s16)c.half[5]))) + ((s32)a.word[2]);
  dst.word[3] =
      (((u32)((u16)b.half[7])) * ((s32)((s16)c.half[7]))) + ((s32)a.word[3]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmaddwod_d_w (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmaddwod_d_w (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmaddwod.d.w vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply odd-positioned signed 32-bit elements in `b` and signed elements in `c`, add to 64-bit elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)(s32)b.word[2 * i + 1] * (s64)(s32)c.word[2 * i + 1] +
                 (s64)a.dword[i];
}

// Expands to:

if (0) {
  dst.dword[0] =
      (((s64)((s32)b.word[1])) * ((s64)((s32)c.word[1]))) + ((s64)a.dword[0]);
  dst.dword[1] =
      (((s64)((s32)b.word[3])) * ((s64)((s32)c.word[3]))) + ((s64)a.dword[1]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmaddwod_d_wu (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmaddwod_d_wu (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmaddwod.d.wu vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply odd-positioned unsigned 32-bit elements in `b` and unsigned elements in `c`, add to 64-bit elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (u64)(u32)b.word[2 * i + 1] * (u64)(u32)c.word[2 * i + 1] +
                 (u64)a.dword[i];
}

// Expands to:

if (0) {
  dst.dword[0] =
      (((u64)((u32)b.word[1])) * ((u64)((u32)c.word[1]))) + ((u64)a.dword[0]);
  dst.dword[1] =
      (((u64)((u32)b.word[3])) * ((u64)((u32)c.word[3]))) + ((u64)a.dword[1]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmaddwod_d_wu_w (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmaddwod_d_wu_w (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmaddwod.d.wu.w vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply odd-positioned unsigned 32-bit elements in `b` and signed elements in `c`, add to 64-bit elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (u64)(u32)b.word[2 * i + 1] * (s64)(s32)c.word[2 * i + 1] +
                 (s64)a.dword[i];
}

// Expands to:

if (0) {
  dst.dword[0] =
      (((u64)((u32)b.word[1])) * ((s64)((s32)c.word[1]))) + ((s64)a.dword[0]);
  dst.dword[1] =
      (((u64)((u32)b.word[3])) * ((s64)((s32)c.word[3]))) + ((s64)a.dword[1]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmaddwod_q_d (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmaddwod_q_d (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmaddwod.q.d vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply odd-positioned signed 64-bit elements in `b` and signed elements in `c`, add to 128-bit elements in `a`.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] = (s128)(s64)b.dword[2 * i + 1] * (s128)(s64)c.dword[2 * i + 1] +
                 (s128)a.qword[i];
}

// Expands to:

if (0) {
  dst.qword[0] = (((s128)((s64)b.dword[1])) * ((s128)((s64)c.dword[1]))) +
                 ((s128)a.qword[0]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 7       | 1.14             |
| 3A6000   | LA664  | 7       | 1.14             |
| 3C6000   | LA664  | 7       | 1.14             |
| 2K1000LA | LA264  | 6       | 1                |
| 2K3000   | LA364E | 6       | 1                |

## \_\_m128i \_\_lsx_vmaddwod_q_du (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmaddwod_q_du (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmaddwod.q.du vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply odd-positioned unsigned 64-bit elements in `b` and unsigned elements in `c`, add to 128-bit elements in `a`.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] = (u128)(u64)b.dword[2 * i + 1] * (u128)(u64)c.dword[2 * i + 1] +
                 (u128)a.qword[i];
}

// Expands to:

if (0) {
  dst.qword[0] = (((u128)((u64)b.dword[1])) * ((u128)((u64)c.dword[1]))) +
                 ((u128)a.qword[0]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 7       | 1.14             |
| 3A6000   | LA664  | 7       | 1.14             |
| 3C6000   | LA664  | 7       | 1.14             |
| 2K1000LA | LA264  | 6       | 1                |
| 2K3000   | LA364E | 6       | 1                |

## \_\_m128i \_\_lsx_vmaddwod_q_du_d (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmaddwod_q_du_d (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmaddwod.q.du.d vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply odd-positioned unsigned 64-bit elements in `b` and signed elements in `c`, add to 128-bit elements in `a`.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] = (u128)(u64)b.dword[2 * i + 1] * (s128)(s64)c.dword[2 * i + 1] +
                 (s128)a.qword[i];
}

// Expands to:

if (0) {
  dst.qword[0] = (((u128)((u64)b.dword[1])) * ((s128)((s64)c.dword[1]))) +
                 ((s128)a.qword[0]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 7       | 1.14             |
| 3A6000   | LA664  | 7       | 1.14             |
| 3C6000   | LA664  | 7       | 1.14             |
| 2K1000LA | LA264  | 6       | 1                |
| 2K3000   | LA364E | 6       | 1                |

## \_\_m128i \_\_lsx_vmax_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmax_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmax.b vr, vr, vr
CPU Flags: LSX
```

### Description

Compute elementwise maximum for signed 8-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = max((s8)a.byte[i], (s8)b.byte[i]);
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

## \_\_m128i \_\_lsx_vmax_bu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmax_bu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmax.bu vr, vr, vr
CPU Flags: LSX
```

### Description

Compute elementwise maximum for unsigned 8-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = max((u8)a.byte[i], (u8)b.byte[i]);
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

## \_\_m128i \_\_lsx_vmax_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmax_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmax.h vr, vr, vr
CPU Flags: LSX
```

### Description

Compute elementwise maximum for signed 16-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = max((s16)a.half[i], (s16)b.half[i]);
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

## \_\_m128i \_\_lsx_vmax_hu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmax_hu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmax.hu vr, vr, vr
CPU Flags: LSX
```

### Description

Compute elementwise maximum for unsigned 16-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = max((u16)a.half[i], (u16)b.half[i]);
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

## \_\_m128i \_\_lsx_vmax_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmax_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmax.w vr, vr, vr
CPU Flags: LSX
```

### Description

Compute elementwise maximum for signed 32-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = max((s32)a.word[i], (s32)b.word[i]);
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

## \_\_m128i \_\_lsx_vmax_wu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmax_wu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmax.wu vr, vr, vr
CPU Flags: LSX
```

### Description

Compute elementwise maximum for unsigned 32-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = max((u32)a.word[i], (u32)b.word[i]);
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

## \_\_m128i \_\_lsx_vmax_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmax_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmax.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compute elementwise maximum for signed 64-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = max((s64)a.dword[i], (s64)b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vmax_du (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmax_du (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmax.du vr, vr, vr
CPU Flags: LSX
```

### Description

Compute elementwise maximum for unsigned 64-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = max((u64)a.dword[i], (u64)b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vmaxi_b (\_\_m128i a, imm_n16_15 imm)

### Synopsis

```
__m128i __lsx_vmaxi_b (__m128i a, imm_n16_15 imm)
#include <lsxintrin.h>
Instruction: vmaxi.b vr, vr, imm
CPU Flags: LSX
```

### Description

Compute elementwise maximum for signed 8-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = max((s8)a.byte[i], (s8)imm);
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

## \_\_m128i \_\_lsx_vmaxi_bu (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vmaxi_bu (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vmaxi.bu vr, vr, imm
CPU Flags: LSX
```

### Description

Compute elementwise maximum for unsigned 8-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = max((u8)a.byte[i], (u8)imm);
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

## \_\_m128i \_\_lsx_vmaxi_h (\_\_m128i a, imm_n16_15 imm)

### Synopsis

```
__m128i __lsx_vmaxi_h (__m128i a, imm_n16_15 imm)
#include <lsxintrin.h>
Instruction: vmaxi.h vr, vr, imm
CPU Flags: LSX
```

### Description

Compute elementwise maximum for signed 16-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = max((s16)a.half[i], (s16)imm);
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

## \_\_m128i \_\_lsx_vmaxi_hu (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vmaxi_hu (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vmaxi.hu vr, vr, imm
CPU Flags: LSX
```

### Description

Compute elementwise maximum for unsigned 16-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = max((u16)a.half[i], (u16)imm);
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

## \_\_m128i \_\_lsx_vmaxi_w (\_\_m128i a, imm_n16_15 imm)

### Synopsis

```
__m128i __lsx_vmaxi_w (__m128i a, imm_n16_15 imm)
#include <lsxintrin.h>
Instruction: vmaxi.w vr, vr, imm
CPU Flags: LSX
```

### Description

Compute elementwise maximum for signed 32-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = max((s32)a.word[i], (s32)imm);
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

## \_\_m128i \_\_lsx_vmaxi_wu (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vmaxi_wu (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vmaxi.wu vr, vr, imm
CPU Flags: LSX
```

### Description

Compute elementwise maximum for unsigned 32-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = max((u32)a.word[i], (u32)imm);
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

## \_\_m128i \_\_lsx_vmaxi_d (\_\_m128i a, imm_n16_15 imm)

### Synopsis

```
__m128i __lsx_vmaxi_d (__m128i a, imm_n16_15 imm)
#include <lsxintrin.h>
Instruction: vmaxi.d vr, vr, imm
CPU Flags: LSX
```

### Description

Compute elementwise maximum for signed 64-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = max((s64)a.dword[i], (s64)imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vmaxi_du (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vmaxi_du (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vmaxi.du vr, vr, imm
CPU Flags: LSX
```

### Description

Compute elementwise maximum for unsigned 64-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = max((u64)a.dword[i], (u64)imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vmin_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmin_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmin.b vr, vr, vr
CPU Flags: LSX
```

### Description

Compute elementwise minimum for signed 8-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = min((s8)a.byte[i], (s8)b.byte[i]);
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

## \_\_m128i \_\_lsx_vmin_bu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmin_bu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmin.bu vr, vr, vr
CPU Flags: LSX
```

### Description

Compute elementwise minimum for unsigned 8-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = min((u8)a.byte[i], (u8)b.byte[i]);
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

## \_\_m128i \_\_lsx_vmin_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmin_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmin.h vr, vr, vr
CPU Flags: LSX
```

### Description

Compute elementwise minimum for signed 16-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = min((s16)a.half[i], (s16)b.half[i]);
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

## \_\_m128i \_\_lsx_vmin_hu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmin_hu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmin.hu vr, vr, vr
CPU Flags: LSX
```

### Description

Compute elementwise minimum for unsigned 16-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = min((u16)a.half[i], (u16)b.half[i]);
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

## \_\_m128i \_\_lsx_vmin_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmin_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmin.w vr, vr, vr
CPU Flags: LSX
```

### Description

Compute elementwise minimum for signed 32-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = min((s32)a.word[i], (s32)b.word[i]);
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

## \_\_m128i \_\_lsx_vmin_wu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmin_wu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmin.wu vr, vr, vr
CPU Flags: LSX
```

### Description

Compute elementwise minimum for unsigned 32-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = min((u32)a.word[i], (u32)b.word[i]);
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

## \_\_m128i \_\_lsx_vmin_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmin_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmin.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compute elementwise minimum for signed 64-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = min((s64)a.dword[i], (s64)b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vmin_du (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmin_du (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmin.du vr, vr, vr
CPU Flags: LSX
```

### Description

Compute elementwise minimum for unsigned 64-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = min((u64)a.dword[i], (u64)b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vmini_b (\_\_m128i a, imm_n16_15 imm)

### Synopsis

```
__m128i __lsx_vmini_b (__m128i a, imm_n16_15 imm)
#include <lsxintrin.h>
Instruction: vmini.b vr, vr, imm
CPU Flags: LSX
```

### Description

Compute elementwise minimum for signed 8-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = min((s8)a.byte[i], (s8)imm);
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

## \_\_m128i \_\_lsx_vmini_bu (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vmini_bu (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vmini.bu vr, vr, imm
CPU Flags: LSX
```

### Description

Compute elementwise minimum for unsigned 8-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = min((u8)a.byte[i], (u8)imm);
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

## \_\_m128i \_\_lsx_vmini_h (\_\_m128i a, imm_n16_15 imm)

### Synopsis

```
__m128i __lsx_vmini_h (__m128i a, imm_n16_15 imm)
#include <lsxintrin.h>
Instruction: vmini.h vr, vr, imm
CPU Flags: LSX
```

### Description

Compute elementwise minimum for signed 16-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = min((s16)a.half[i], (s16)imm);
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

## \_\_m128i \_\_lsx_vmini_hu (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vmini_hu (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vmini.hu vr, vr, imm
CPU Flags: LSX
```

### Description

Compute elementwise minimum for unsigned 16-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = min((u16)a.half[i], (u16)imm);
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

## \_\_m128i \_\_lsx_vmini_w (\_\_m128i a, imm_n16_15 imm)

### Synopsis

```
__m128i __lsx_vmini_w (__m128i a, imm_n16_15 imm)
#include <lsxintrin.h>
Instruction: vmini.w vr, vr, imm
CPU Flags: LSX
```

### Description

Compute elementwise minimum for signed 32-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = min((s32)a.word[i], (s32)imm);
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

## \_\_m128i \_\_lsx_vmini_wu (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vmini_wu (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vmini.wu vr, vr, imm
CPU Flags: LSX
```

### Description

Compute elementwise minimum for unsigned 32-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = min((u32)a.word[i], (u32)imm);
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

## \_\_m128i \_\_lsx_vmini_d (\_\_m128i a, imm_n16_15 imm)

### Synopsis

```
__m128i __lsx_vmini_d (__m128i a, imm_n16_15 imm)
#include <lsxintrin.h>
Instruction: vmini.d vr, vr, imm
CPU Flags: LSX
```

### Description

Compute elementwise minimum for signed 64-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = min((s64)a.dword[i], (s64)imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vmini_du (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vmini_du (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vmini.du vr, vr, imm
CPU Flags: LSX
```

### Description

Compute elementwise minimum for unsigned 64-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = min((u64)a.dword[i], (u64)imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vmod_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmod_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmod.b vr, vr, vr
CPU Flags: LSX
```

### Description

Modulo residual signed 8-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (b.byte[i] == 0) ? 0 : ((s8)a.byte[i] % (s8)b.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 29, 33  | 0.06(1/17)       |
| 3A6000   | LA664  | 29, 35  | 0.06(1/15.5)     |
| 3C6000   | LA664  | 29      | 0.07(1/13.5)     |
| 2K1000LA | LA264  | 30, 46  | 0(1/63)          |
| 2K3000   | LA364E | 30, 46  | 0(1/63)          |

## \_\_m128i \_\_lsx_vmod_bu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmod_bu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmod.bu vr, vr, vr
CPU Flags: LSX
```

### Description

Modulo residual unsigned 8-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (b.byte[i] == 0) ? 0 : ((u8)a.byte[i] % (u8)b.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 29, 33  | 0.05(1/19)       |
| 3A6000   | LA664  | 29, 37  | 0.06(1/17.5)     |
| 3C6000   | LA664  | 29      | 0.07(1/13.5)     |
| 2K1000LA | LA264  | 30, 46  | 0(1/62)          |
| 2K3000   | LA364E | 30, 46  | 0(1/63)          |

## \_\_m128i \_\_lsx_vmod_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmod_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmod.h vr, vr, vr
CPU Flags: LSX
```

### Description

Modulo residual signed 16-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (b.half[i] == 0) ? 0 : ((s16)a.half[i] % (s16)b.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 17, 21  | 0.09(1/11)       |
| 3A6000   | LA664  | 17, 21  | 0.12(1/8.5)      |
| 3C6000   | LA664  | 17      | 0.13(1/7.5)      |
| 2K1000LA | LA264  | 18, 26  | 0.03(1/35)       |
| 2K3000   | LA364E | 18, 26  | 0.03(1/35)       |

## \_\_m128i \_\_lsx_vmod_hu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmod_hu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmod.hu vr, vr, vr
CPU Flags: LSX
```

### Description

Modulo residual unsigned 16-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (b.half[i] == 0) ? 0 : ((u16)a.half[i] % (u16)b.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 17, 21  | 0.07(1/15)       |
| 3A6000   | LA664  | 17, 21  | 0.11(1/9.5)      |
| 3C6000   | LA664  | 17      | 0.13(1/7.5)      |
| 2K1000LA | LA264  | 18, 26  | 0.03(1/35)       |
| 2K3000   | LA364E | 18, 26  | 0.03(1/35)       |

## \_\_m128i \_\_lsx_vmod_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmod_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmod.w vr, vr, vr
CPU Flags: LSX
```

### Description

Modulo residual signed 32-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (b.word[i] == 0) ? 0 : ((s32)a.word[i] % (s32)b.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 11, 15  | 0.08(1/12)       |
| 3A6000   | LA664  | 11, 13  | 0.18(1/5.5)      |
| 3C6000   | LA664  | 11      | 0.22(1/4.5)      |
| 2K1000LA | LA264  | 12, 16  | 0.05(1/20)       |
| 2K3000   | LA364E | 12, 16  | 0.05(1/20)       |

## \_\_m128i \_\_lsx_vmod_wu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmod_wu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmod.wu vr, vr, vr
CPU Flags: LSX
```

### Description

Modulo residual unsigned 32-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (b.word[i] == 0) ? 0 : ((u32)a.word[i] % (u32)b.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 11, 15  | 0.06(1/16)       |
| 3A6000   | LA664  | 11, 13  | 0.18(1/5.5)      |
| 3C6000   | LA664  | 11      | 0.22(1/4.5)      |
| 2K1000LA | LA264  | 12, 16  | 0.05(1/20)       |
| 2K3000   | LA364E | 12, 16  | 0.05(1/20)       |

## \_\_m128i \_\_lsx_vmod_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmod_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmod.d vr, vr, vr
CPU Flags: LSX
```

### Description

Modulo residual signed 64-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (b.dword[i] == 0) ? 0 : ((s64)a.dword[i] % (s64)b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 8, 10   | 0.11(1/9.5)      |
| 3A6000   | LA664  | 8, 10   | 0.25(1/4)        |
| 3C6000   | LA664  | 8       | 0.33(1/3)        |
| 2K1000LA | LA264  | 9, 11   | 0.08(1/13)       |
| 2K3000   | LA364E | 9, 11   | 0.08(1/13)       |

## \_\_m128i \_\_lsx_vmod_du (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmod_du (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmod.du vr, vr, vr
CPU Flags: LSX
```

### Description

Modulo residual unsigned 64-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (b.dword[i] == 0) ? 0 : ((u64)a.dword[i] % (u64)b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 8, 10   | 0.11(1/9.5)      |
| 3A6000   | LA664  | 8, 10   | 0.25(1/4)        |
| 3C6000   | LA664  | 8       | 0.33(1/3)        |
| 2K1000LA | LA264  | 9, 11   | 0.08(1/13)       |
| 2K3000   | LA364E | 9, 11   | 0.08(1/13)       |

## \_\_m128i \_\_lsx_vmsub_b (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmsub_b (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmsub.b vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply 8-bit elements in `b` and `c`, negate and add elements in `a`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = -b.byte[i] * c.byte[i] + a.byte[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmsub_h (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmsub_h (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmsub.h vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply 16-bit elements in `b` and `c`, negate and add elements in `a`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = -b.half[i] * c.half[i] + a.half[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmsub_w (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmsub_w (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmsub.w vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply 32-bit elements in `b` and `c`, negate and add elements in `a`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = -b.word[i] * c.word[i] + a.word[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmsub_d (\_\_m128i a, \_\_m128i b, \_\_m128i c)

### Synopsis

```
__m128i __lsx_vmsub_d (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmsub.d vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply 64-bit elements in `b` and `c`, negate and add elements in `a`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = -b.dword[i] * c.dword[i] + a.dword[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmuh_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmuh_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmuh.b vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply signed 8-bit elements in `a` and `b`, save the high 8-bit result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (((s16)(s8)a.byte[i] * (s16)(s8)b.byte[i])) >> 8;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmuh_bu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmuh_bu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmuh.bu vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply unsigned 8-bit elements in `a` and `b`, save the high 8-bit result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (((u16)(u8)a.byte[i] * (u16)(u8)b.byte[i])) >> 8;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmuh_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmuh_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmuh.h vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply signed 16-bit elements in `a` and `b`, save the high 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (((s32)(s16)a.half[i] * (s32)(s16)b.half[i])) >> 16;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmuh_hu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmuh_hu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmuh.hu vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply unsigned 16-bit elements in `a` and `b`, save the high 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (((u32)(u16)a.half[i] * (u32)(u16)b.half[i])) >> 16;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmuh_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmuh_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmuh.w vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply signed 32-bit elements in `a` and `b`, save the high 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (((s64)(s32)a.word[i] * (s64)(s32)b.word[i])) >> 32;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmuh_wu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmuh_wu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmuh.wu vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply unsigned 32-bit elements in `a` and `b`, save the high 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (((u64)(u32)a.word[i] * (u64)(u32)b.word[i])) >> 32;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmuh_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmuh_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmuh.d vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply signed 64-bit elements in `a` and `b`, save the high 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (((s128)(s64)a.dword[i] * (s128)(s64)b.dword[i])) >> 64;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmuh_du (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmuh_du (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmuh.du vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply unsigned 64-bit elements in `a` and `b`, save the high 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (((u128)(u64)a.dword[i] * (u128)(u64)b.dword[i])) >> 64;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmul_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmul_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmul.b vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply 8-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[i] * b.byte[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmul_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmul_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmul.h vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply 16-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = a.half[i] * b.half[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmul_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmul_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmul.w vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply 32-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = a.word[i] * b.word[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmul_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmul_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmul.d vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply 64-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = a.dword[i] * b.dword[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmulwev_h_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmulwev_h_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmulwev.h.b vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply even-positioned signed 8-bit elements in `a` and signed elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (s16)(s8)a.byte[2 * i] * (s16)(s8)b.byte[2 * i];
}

// Expands to:

if (0) {
  dst.half[0] = ((s16)((s8)a.byte[0])) * ((s16)((s8)b.byte[0]));
  dst.half[1] = ((s16)((s8)a.byte[2])) * ((s16)((s8)b.byte[2]));
  dst.half[2] = ((s16)((s8)a.byte[4])) * ((s16)((s8)b.byte[4]));
  dst.half[3] = ((s16)((s8)a.byte[6])) * ((s16)((s8)b.byte[6]));
  dst.half[4] = ((s16)((s8)a.byte[8])) * ((s16)((s8)b.byte[8]));
  dst.half[5] = ((s16)((s8)a.byte[10])) * ((s16)((s8)b.byte[10]));
  dst.half[6] = ((s16)((s8)a.byte[12])) * ((s16)((s8)b.byte[12]));
  dst.half[7] = ((s16)((s8)a.byte[14])) * ((s16)((s8)b.byte[14]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmulwev_h_bu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmulwev_h_bu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmulwev.h.bu vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply even-positioned unsigned 8-bit elements in `a` and unsigned elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (u16)(u8)a.byte[2 * i] * (u16)(u8)b.byte[2 * i];
}

// Expands to:

if (0) {
  dst.half[0] = ((u16)((u8)a.byte[0])) * ((u16)((u8)b.byte[0]));
  dst.half[1] = ((u16)((u8)a.byte[2])) * ((u16)((u8)b.byte[2]));
  dst.half[2] = ((u16)((u8)a.byte[4])) * ((u16)((u8)b.byte[4]));
  dst.half[3] = ((u16)((u8)a.byte[6])) * ((u16)((u8)b.byte[6]));
  dst.half[4] = ((u16)((u8)a.byte[8])) * ((u16)((u8)b.byte[8]));
  dst.half[5] = ((u16)((u8)a.byte[10])) * ((u16)((u8)b.byte[10]));
  dst.half[6] = ((u16)((u8)a.byte[12])) * ((u16)((u8)b.byte[12]));
  dst.half[7] = ((u16)((u8)a.byte[14])) * ((u16)((u8)b.byte[14]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmulwev_h_bu_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmulwev_h_bu_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmulwev.h.bu.b vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply even-positioned unsigned 8-bit elements in `a` and signed elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (u16)(u8)a.byte[2 * i] * (s16)(s8)b.byte[2 * i];
}

// Expands to:

if (0) {
  dst.half[0] = ((u16)((u8)a.byte[0])) * ((s16)((s8)b.byte[0]));
  dst.half[1] = ((u16)((u8)a.byte[2])) * ((s16)((s8)b.byte[2]));
  dst.half[2] = ((u16)((u8)a.byte[4])) * ((s16)((s8)b.byte[4]));
  dst.half[3] = ((u16)((u8)a.byte[6])) * ((s16)((s8)b.byte[6]));
  dst.half[4] = ((u16)((u8)a.byte[8])) * ((s16)((s8)b.byte[8]));
  dst.half[5] = ((u16)((u8)a.byte[10])) * ((s16)((s8)b.byte[10]));
  dst.half[6] = ((u16)((u8)a.byte[12])) * ((s16)((s8)b.byte[12]));
  dst.half[7] = ((u16)((u8)a.byte[14])) * ((s16)((s8)b.byte[14]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmulwev_w_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmulwev_w_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmulwev.w.h vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply even-positioned signed 16-bit elements in `a` and signed elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (s32)(s16)a.half[2 * i] * (s32)(s16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((s32)((s16)a.half[0])) * ((s32)((s16)b.half[0]));
  dst.word[1] = ((s32)((s16)a.half[2])) * ((s32)((s16)b.half[2]));
  dst.word[2] = ((s32)((s16)a.half[4])) * ((s32)((s16)b.half[4]));
  dst.word[3] = ((s32)((s16)a.half[6])) * ((s32)((s16)b.half[6]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmulwev_w_hu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmulwev_w_hu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmulwev.w.hu vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply even-positioned unsigned 16-bit elements in `a` and unsigned elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i] * (u32)(u16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[0])) * ((u32)((u16)b.half[0]));
  dst.word[1] = ((u32)((u16)a.half[2])) * ((u32)((u16)b.half[2]));
  dst.word[2] = ((u32)((u16)a.half[4])) * ((u32)((u16)b.half[4]));
  dst.word[3] = ((u32)((u16)a.half[6])) * ((u32)((u16)b.half[6]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmulwev_w_hu_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmulwev_w_hu_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmulwev.w.hu.h vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply even-positioned unsigned 16-bit elements in `a` and signed elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i] * (s32)(s16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[0])) * ((s32)((s16)b.half[0]));
  dst.word[1] = ((u32)((u16)a.half[2])) * ((s32)((s16)b.half[2]));
  dst.word[2] = ((u32)((u16)a.half[4])) * ((s32)((s16)b.half[4]));
  dst.word[3] = ((u32)((u16)a.half[6])) * ((s32)((s16)b.half[6]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmulwev_d_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmulwev_d_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmulwev.d.w vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply even-positioned signed 32-bit elements in `a` and signed elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)(s32)a.word[2 * i] * (s64)(s32)b.word[2 * i];
}

// Expands to:

if (0) {
  dst.dword[0] = ((s64)((s32)a.word[0])) * ((s64)((s32)b.word[0]));
  dst.dword[1] = ((s64)((s32)a.word[2])) * ((s64)((s32)b.word[2]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmulwev_d_wu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmulwev_d_wu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmulwev.d.wu vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply even-positioned unsigned 32-bit elements in `a` and unsigned elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 * i] * (u64)(u32)b.word[2 * i];
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[0])) * ((u64)((u32)b.word[0]));
  dst.dword[1] = ((u64)((u32)a.word[2])) * ((u64)((u32)b.word[2]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmulwev_d_wu_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmulwev_d_wu_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmulwev.d.wu.w vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply even-positioned unsigned 32-bit elements in `a` and signed elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 * i] * (s64)(s32)b.word[2 * i];
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[0])) * ((s64)((s32)b.word[0]));
  dst.dword[1] = ((u64)((u32)a.word[2])) * ((s64)((s32)b.word[2]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmulwev_q_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmulwev_q_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmulwev.q.d vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply even-positioned signed 64-bit elements in `a` and signed elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] = (s128)(s64)a.dword[2 * i] * (s128)(s64)b.dword[2 * i];
}

// Expands to:

if (0) {
  dst.qword[0] = ((s128)((s64)a.dword[0])) * ((s128)((s64)b.dword[0]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 7       | 2                |
| 3A6000   | LA664  | 7       | 2                |
| 3C6000   | LA664  | 7       | 2                |
| 2K1000LA | LA264  | 6       | 1                |
| 2K3000   | LA364E | 6       | 1                |

## \_\_m128i \_\_lsx_vmulwev_q_du (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmulwev_q_du (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmulwev.q.du vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply even-positioned unsigned 64-bit elements in `a` and unsigned elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 * i] * (u128)(u64)b.dword[2 * i];
}

// Expands to:

if (0) {
  dst.qword[0] = ((u128)((u64)a.dword[0])) * ((u128)((u64)b.dword[0]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 7       | 2                |
| 3A6000   | LA664  | 7       | 2                |
| 3C6000   | LA664  | 7       | 2                |
| 2K1000LA | LA264  | 6       | 1                |
| 2K3000   | LA364E | 6       | 1                |

## \_\_m128i \_\_lsx_vmulwev_q_du_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmulwev_q_du_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmulwev.q.du.d vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply even-positioned unsigned 64-bit elements in `a` and signed elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 * i] * (s128)(s64)b.dword[2 * i];
}

// Expands to:

if (0) {
  dst.qword[0] = ((u128)((u64)a.dword[0])) * ((s128)((s64)b.dword[0]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 7       | 2                |
| 3A6000   | LA664  | 7       | 2                |
| 3C6000   | LA664  | 7       | 2                |
| 2K1000LA | LA264  | 6       | 1                |
| 2K3000   | LA364E | 6       | 1                |

## \_\_m128i \_\_lsx_vmulwod_h_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmulwod_h_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmulwod.h.b vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply odd-positioned signed 8-bit elements in `a` and signed elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (s16)(s8)a.byte[2 * i + 1] * (s16)(s8)b.byte[2 * i + 1];
}

// Expands to:

if (0) {
  dst.half[0] = ((s16)((s8)a.byte[1])) * ((s16)((s8)b.byte[1]));
  dst.half[1] = ((s16)((s8)a.byte[3])) * ((s16)((s8)b.byte[3]));
  dst.half[2] = ((s16)((s8)a.byte[5])) * ((s16)((s8)b.byte[5]));
  dst.half[3] = ((s16)((s8)a.byte[7])) * ((s16)((s8)b.byte[7]));
  dst.half[4] = ((s16)((s8)a.byte[9])) * ((s16)((s8)b.byte[9]));
  dst.half[5] = ((s16)((s8)a.byte[11])) * ((s16)((s8)b.byte[11]));
  dst.half[6] = ((s16)((s8)a.byte[13])) * ((s16)((s8)b.byte[13]));
  dst.half[7] = ((s16)((s8)a.byte[15])) * ((s16)((s8)b.byte[15]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmulwod_h_bu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmulwod_h_bu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmulwod.h.bu vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply odd-positioned unsigned 8-bit elements in `a` and unsigned elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (u16)(u8)a.byte[2 * i + 1] * (u16)(u8)b.byte[2 * i + 1];
}

// Expands to:

if (0) {
  dst.half[0] = ((u16)((u8)a.byte[1])) * ((u16)((u8)b.byte[1]));
  dst.half[1] = ((u16)((u8)a.byte[3])) * ((u16)((u8)b.byte[3]));
  dst.half[2] = ((u16)((u8)a.byte[5])) * ((u16)((u8)b.byte[5]));
  dst.half[3] = ((u16)((u8)a.byte[7])) * ((u16)((u8)b.byte[7]));
  dst.half[4] = ((u16)((u8)a.byte[9])) * ((u16)((u8)b.byte[9]));
  dst.half[5] = ((u16)((u8)a.byte[11])) * ((u16)((u8)b.byte[11]));
  dst.half[6] = ((u16)((u8)a.byte[13])) * ((u16)((u8)b.byte[13]));
  dst.half[7] = ((u16)((u8)a.byte[15])) * ((u16)((u8)b.byte[15]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmulwod_h_bu_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmulwod_h_bu_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmulwod.h.bu.b vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply odd-positioned unsigned 8-bit elements in `a` and signed elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (u16)(u8)a.byte[2 * i + 1] * (s16)(s8)b.byte[2 * i + 1];
}

// Expands to:

if (0) {
  dst.half[0] = ((u16)((u8)a.byte[1])) * ((s16)((s8)b.byte[1]));
  dst.half[1] = ((u16)((u8)a.byte[3])) * ((s16)((s8)b.byte[3]));
  dst.half[2] = ((u16)((u8)a.byte[5])) * ((s16)((s8)b.byte[5]));
  dst.half[3] = ((u16)((u8)a.byte[7])) * ((s16)((s8)b.byte[7]));
  dst.half[4] = ((u16)((u8)a.byte[9])) * ((s16)((s8)b.byte[9]));
  dst.half[5] = ((u16)((u8)a.byte[11])) * ((s16)((s8)b.byte[11]));
  dst.half[6] = ((u16)((u8)a.byte[13])) * ((s16)((s8)b.byte[13]));
  dst.half[7] = ((u16)((u8)a.byte[15])) * ((s16)((s8)b.byte[15]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmulwod_w_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmulwod_w_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmulwod.w.h vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply odd-positioned signed 16-bit elements in `a` and signed elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (s32)(s16)a.half[2 * i + 1] * (s32)(s16)b.half[2 * i + 1];
}

// Expands to:

if (0) {
  dst.word[0] = ((s32)((s16)a.half[1])) * ((s32)((s16)b.half[1]));
  dst.word[1] = ((s32)((s16)a.half[3])) * ((s32)((s16)b.half[3]));
  dst.word[2] = ((s32)((s16)a.half[5])) * ((s32)((s16)b.half[5]));
  dst.word[3] = ((s32)((s16)a.half[7])) * ((s32)((s16)b.half[7]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmulwod_w_hu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmulwod_w_hu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmulwod.w.hu vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply odd-positioned unsigned 16-bit elements in `a` and unsigned elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i + 1] * (u32)(u16)b.half[2 * i + 1];
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[1])) * ((u32)((u16)b.half[1]));
  dst.word[1] = ((u32)((u16)a.half[3])) * ((u32)((u16)b.half[3]));
  dst.word[2] = ((u32)((u16)a.half[5])) * ((u32)((u16)b.half[5]));
  dst.word[3] = ((u32)((u16)a.half[7])) * ((u32)((u16)b.half[7]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmulwod_w_hu_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmulwod_w_hu_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmulwod.w.hu.h vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply odd-positioned unsigned 16-bit elements in `a` and signed elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i + 1] * (s32)(s16)b.half[2 * i + 1];
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[1])) * ((s32)((s16)b.half[1]));
  dst.word[1] = ((u32)((u16)a.half[3])) * ((s32)((s16)b.half[3]));
  dst.word[2] = ((u32)((u16)a.half[5])) * ((s32)((s16)b.half[5]));
  dst.word[3] = ((u32)((u16)a.half[7])) * ((s32)((s16)b.half[7]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmulwod_d_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmulwod_d_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmulwod.d.w vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply odd-positioned signed 32-bit elements in `a` and signed elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)(s32)a.word[2 * i + 1] * (s64)(s32)b.word[2 * i + 1];
}

// Expands to:

if (0) {
  dst.dword[0] = ((s64)((s32)a.word[1])) * ((s64)((s32)b.word[1]));
  dst.dword[1] = ((s64)((s32)a.word[3])) * ((s64)((s32)b.word[3]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmulwod_d_wu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmulwod_d_wu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmulwod.d.wu vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply odd-positioned unsigned 32-bit elements in `a` and unsigned elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 * i + 1] * (u64)(u32)b.word[2 * i + 1];
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[1])) * ((u64)((u32)b.word[1]));
  dst.dword[1] = ((u64)((u32)a.word[3])) * ((u64)((u32)b.word[3]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmulwod_d_wu_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmulwod_d_wu_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmulwod.d.wu.w vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply odd-positioned unsigned 32-bit elements in `a` and signed elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 * i + 1] * (s64)(s32)b.word[2 * i + 1];
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[1])) * ((s64)((s32)b.word[1]));
  dst.dword[1] = ((u64)((u32)a.word[3])) * ((s64)((s32)b.word[3]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmulwod_q_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmulwod_q_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmulwod.q.d vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply odd-positioned signed 64-bit elements in `a` and signed elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] = (s128)(s64)a.dword[2 * i + 1] * (s128)(s64)b.dword[2 * i + 1];
}

// Expands to:

if (0) {
  dst.qword[0] = ((s128)((s64)a.dword[1])) * ((s128)((s64)b.dword[1]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 7       | 2                |
| 3A6000   | LA664  | 7       | 2                |
| 3C6000   | LA664  | 7       | 2                |
| 2K1000LA | LA264  | 6       | 1                |
| 2K3000   | LA364E | 6       | 1                |

## \_\_m128i \_\_lsx_vmulwod_q_du (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmulwod_q_du (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmulwod.q.du vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply odd-positioned unsigned 64-bit elements in `a` and unsigned elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 * i + 1] * (u128)(u64)b.dword[2 * i + 1];
}

// Expands to:

if (0) {
  dst.qword[0] = ((u128)((u64)a.dword[1])) * ((u128)((u64)b.dword[1]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 7       | 2                |
| 3A6000   | LA664  | 7       | 2                |
| 3C6000   | LA664  | 7       | 2                |
| 2K1000LA | LA264  | 6       | 1                |
| 2K3000   | LA364E | 6       | 1                |

## \_\_m128i \_\_lsx_vmulwod_q_du_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vmulwod_q_du_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmulwod.q.du.d vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply odd-positioned unsigned 64-bit elements in `a` and signed elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 * i + 1] * (s128)(s64)b.dword[2 * i + 1];
}

// Expands to:

if (0) {
  dst.qword[0] = ((u128)((u64)a.dword[1])) * ((s128)((s64)b.dword[1]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 7       | 2                |
| 3A6000   | LA664  | 7       | 2                |
| 3C6000   | LA664  | 7       | 2                |
| 2K1000LA | LA264  | 6       | 1                |
| 2K3000   | LA364E | 6       | 1                |

## \_\_m128i \_\_lsx_vneg_b (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vneg_b (__m128i a)
#include <lsxintrin.h>
Instruction: vneg.b vr, vr
CPU Flags: LSX
```

### Description

Negate 8-bit elements in `a` and save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = -a.byte[i];
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

## \_\_m128i \_\_lsx_vneg_h (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vneg_h (__m128i a)
#include <lsxintrin.h>
Instruction: vneg.h vr, vr
CPU Flags: LSX
```

### Description

Negate 16-bit elements in `a` and save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = -a.half[i];
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

## \_\_m128i \_\_lsx_vneg_w (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vneg_w (__m128i a)
#include <lsxintrin.h>
Instruction: vneg.w vr, vr
CPU Flags: LSX
```

### Description

Negate 32-bit elements in `a` and save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = -a.word[i];
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

## \_\_m128i \_\_lsx_vneg_d (\_\_m128i a)

### Synopsis

```
__m128i __lsx_vneg_d (__m128i a)
#include <lsxintrin.h>
Instruction: vneg.d vr, vr
CPU Flags: LSX
```

### Description

Negate 64-bit elements in `a` and save the result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = -a.dword[i];
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

## \_\_m128i \_\_lsx_vsadd_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsadd_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsadd.b vr, vr, vr
CPU Flags: LSX
```

### Description

Saturating add the signed 8-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (s8)sadd((s8)a.byte[i], (s8)b.byte[i]);
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

## \_\_m128i \_\_lsx_vsadd_bu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsadd_bu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsadd.bu vr, vr, vr
CPU Flags: LSX
```

### Description

Saturating add the unsigned 8-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (u8)sadd((u8)a.byte[i], (u8)b.byte[i]);
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

## \_\_m128i \_\_lsx_vsadd_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsadd_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsadd.h vr, vr, vr
CPU Flags: LSX
```

### Description

Saturating add the signed 16-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (s16)sadd((s16)a.half[i], (s16)b.half[i]);
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

## \_\_m128i \_\_lsx_vsadd_hu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsadd_hu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsadd.hu vr, vr, vr
CPU Flags: LSX
```

### Description

Saturating add the unsigned 16-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (u16)sadd((u16)a.half[i], (u16)b.half[i]);
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

## \_\_m128i \_\_lsx_vsadd_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsadd_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsadd.w vr, vr, vr
CPU Flags: LSX
```

### Description

Saturating add the signed 32-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (s32)sadd((s32)a.word[i], (s32)b.word[i]);
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

## \_\_m128i \_\_lsx_vsadd_wu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsadd_wu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsadd.wu vr, vr, vr
CPU Flags: LSX
```

### Description

Saturating add the unsigned 32-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (u32)sadd((u32)a.word[i], (u32)b.word[i]);
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

## \_\_m128i \_\_lsx_vsadd_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsadd_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsadd.d vr, vr, vr
CPU Flags: LSX
```

### Description

Saturating add the signed 64-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)sadd((s64)a.dword[i], (s64)b.dword[i]);
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

## \_\_m128i \_\_lsx_vsadd_du (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsadd_du (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsadd.du vr, vr, vr
CPU Flags: LSX
```

### Description

Saturating add the unsigned 64-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (u64)sadd((u64)a.dword[i], (u64)b.dword[i]);
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

## \_\_m128i \_\_lsx_vssub_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssub_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssub.b vr, vr, vr
CPU Flags: LSX
```

### Description

Saturating subtract the signed 8-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (s8)ssub((s8)a.byte[i], (s8)b.byte[i]);
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

## \_\_m128i \_\_lsx_vssub_bu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssub_bu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssub.bu vr, vr, vr
CPU Flags: LSX
```

### Description

Saturating subtract the unsigned 8-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (u8)ssub((u8)a.byte[i], (u8)b.byte[i]);
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

## \_\_m128i \_\_lsx_vssub_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssub_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssub.h vr, vr, vr
CPU Flags: LSX
```

### Description

Saturating subtract the signed 16-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (s16)ssub((s16)a.half[i], (s16)b.half[i]);
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

## \_\_m128i \_\_lsx_vssub_hu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssub_hu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssub.hu vr, vr, vr
CPU Flags: LSX
```

### Description

Saturating subtract the unsigned 16-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (u16)ssub((u16)a.half[i], (u16)b.half[i]);
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

## \_\_m128i \_\_lsx_vssub_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssub_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssub.w vr, vr, vr
CPU Flags: LSX
```

### Description

Saturating subtract the signed 32-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (s32)ssub((s32)a.word[i], (s32)b.word[i]);
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

## \_\_m128i \_\_lsx_vssub_wu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssub_wu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssub.wu vr, vr, vr
CPU Flags: LSX
```

### Description

Saturating subtract the unsigned 32-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (u32)ssub((u32)a.word[i], (u32)b.word[i]);
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

## \_\_m128i \_\_lsx_vssub_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssub_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssub.d vr, vr, vr
CPU Flags: LSX
```

### Description

Saturating subtract the signed 64-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)ssub((s64)a.dword[i], (s64)b.dword[i]);
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

## \_\_m128i \_\_lsx_vssub_du (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vssub_du (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssub.du vr, vr, vr
CPU Flags: LSX
```

### Description

Saturating subtract the unsigned 64-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (u64)ssub((u64)a.dword[i], (u64)b.dword[i]);
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

## \_\_m128i \_\_lsx_vsub_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsub_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsub.b vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract 8-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[i] - b.byte[i];
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

## \_\_m128i \_\_lsx_vsub_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsub_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsub.h vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract 16-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = a.half[i] - b.half[i];
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

## \_\_m128i \_\_lsx_vsub_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsub_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsub.w vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract 32-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = a.word[i] - b.word[i];
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

## \_\_m128i \_\_lsx_vsub_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsub_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsub.d vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract 64-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = a.dword[i] - b.dword[i];
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

## \_\_m128i \_\_lsx_vsub_q (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsub_q (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsub.q vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract 128-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
dst.qword[0] = a.qword[0] - b.qword[0];
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

## \_\_m128i \_\_lsx_vsubi_bu (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vsubi_bu (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vsubi.bu vr, vr, imm
CPU Flags: LSX
```

### Description

Subtract 8-bit elements in `a` by `imm`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[i] - imm;
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

## \_\_m128i \_\_lsx_vsubi_hu (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vsubi_hu (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vsubi.hu vr, vr, imm
CPU Flags: LSX
```

### Description

Subtract 16-bit elements in `a` by `imm`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = a.half[i] - imm;
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

## \_\_m128i \_\_lsx_vsubi_wu (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vsubi_wu (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vsubi.wu vr, vr, imm
CPU Flags: LSX
```

### Description

Subtract 32-bit elements in `a` by `imm`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = a.word[i] - imm;
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

## \_\_m128i \_\_lsx_vsubi_du (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vsubi_du (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vsubi.du vr, vr, imm
CPU Flags: LSX
```

### Description

Subtract 64-bit elements in `a` by `imm`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = a.dword[i] - imm;
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

## \_\_m128i \_\_lsx_vsubwev_h_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsubwev_h_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsubwev.h.b vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract even-positioned signed 8-bit elements in `a` and signed elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (s16)(s8)a.byte[2 * i] - (s16)(s8)b.byte[2 * i];
}

// Expands to:

if (0) {
  dst.half[0] = ((s16)((s8)a.byte[0])) - ((s16)((s8)b.byte[0]));
  dst.half[1] = ((s16)((s8)a.byte[2])) - ((s16)((s8)b.byte[2]));
  dst.half[2] = ((s16)((s8)a.byte[4])) - ((s16)((s8)b.byte[4]));
  dst.half[3] = ((s16)((s8)a.byte[6])) - ((s16)((s8)b.byte[6]));
  dst.half[4] = ((s16)((s8)a.byte[8])) - ((s16)((s8)b.byte[8]));
  dst.half[5] = ((s16)((s8)a.byte[10])) - ((s16)((s8)b.byte[10]));
  dst.half[6] = ((s16)((s8)a.byte[12])) - ((s16)((s8)b.byte[12]));
  dst.half[7] = ((s16)((s8)a.byte[14])) - ((s16)((s8)b.byte[14]));
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

## \_\_m128i \_\_lsx_vsubwev_h_bu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsubwev_h_bu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsubwev.h.bu vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract even-positioned unsigned 8-bit elements in `a` and unsigned elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (u16)(u8)a.byte[2 * i] - (u16)(u8)b.byte[2 * i];
}

// Expands to:

if (0) {
  dst.half[0] = ((u16)((u8)a.byte[0])) - ((u16)((u8)b.byte[0]));
  dst.half[1] = ((u16)((u8)a.byte[2])) - ((u16)((u8)b.byte[2]));
  dst.half[2] = ((u16)((u8)a.byte[4])) - ((u16)((u8)b.byte[4]));
  dst.half[3] = ((u16)((u8)a.byte[6])) - ((u16)((u8)b.byte[6]));
  dst.half[4] = ((u16)((u8)a.byte[8])) - ((u16)((u8)b.byte[8]));
  dst.half[5] = ((u16)((u8)a.byte[10])) - ((u16)((u8)b.byte[10]));
  dst.half[6] = ((u16)((u8)a.byte[12])) - ((u16)((u8)b.byte[12]));
  dst.half[7] = ((u16)((u8)a.byte[14])) - ((u16)((u8)b.byte[14]));
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

## \_\_m128i \_\_lsx_vsubwev_w_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsubwev_w_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsubwev.w.h vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract even-positioned signed 16-bit elements in `a` and signed elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (s32)(s16)a.half[2 * i] - (s32)(s16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((s32)((s16)a.half[0])) - ((s32)((s16)b.half[0]));
  dst.word[1] = ((s32)((s16)a.half[2])) - ((s32)((s16)b.half[2]));
  dst.word[2] = ((s32)((s16)a.half[4])) - ((s32)((s16)b.half[4]));
  dst.word[3] = ((s32)((s16)a.half[6])) - ((s32)((s16)b.half[6]));
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

## \_\_m128i \_\_lsx_vsubwev_w_hu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsubwev_w_hu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsubwev.w.hu vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract even-positioned unsigned 16-bit elements in `a` and unsigned elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i] - (u32)(u16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[0])) - ((u32)((u16)b.half[0]));
  dst.word[1] = ((u32)((u16)a.half[2])) - ((u32)((u16)b.half[2]));
  dst.word[2] = ((u32)((u16)a.half[4])) - ((u32)((u16)b.half[4]));
  dst.word[3] = ((u32)((u16)a.half[6])) - ((u32)((u16)b.half[6]));
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

## \_\_m128i \_\_lsx_vsubwev_d_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsubwev_d_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsubwev.d.w vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract even-positioned signed 32-bit elements in `a` and signed elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)(s32)a.word[2 * i] - (s64)(s32)b.word[2 * i];
}

// Expands to:

if (0) {
  dst.dword[0] = ((s64)((s32)a.word[0])) - ((s64)((s32)b.word[0]));
  dst.dword[1] = ((s64)((s32)a.word[2])) - ((s64)((s32)b.word[2]));
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

## \_\_m128i \_\_lsx_vsubwev_d_wu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsubwev_d_wu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsubwev.d.wu vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract even-positioned unsigned 32-bit elements in `a` and unsigned elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 * i] - (u64)(u32)b.word[2 * i];
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[0])) - ((u64)((u32)b.word[0]));
  dst.dword[1] = ((u64)((u32)a.word[2])) - ((u64)((u32)b.word[2]));
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

## \_\_m128i \_\_lsx_vsubwev_q_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsubwev_q_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsubwev.q.d vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract even-positioned signed 64-bit elements in `a` and signed elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] = (s128)(s64)a.dword[2 * i] - (s128)(s64)b.dword[2 * i];
}

// Expands to:

if (0) {
  dst.qword[0] = ((s128)((s64)a.dword[0])) - ((s128)((s64)b.dword[0]));
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

## \_\_m128i \_\_lsx_vsubwev_q_du (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsubwev_q_du (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsubwev.q.du vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract even-positioned unsigned 64-bit elements in `a` and unsigned elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 * i] - (u128)(u64)b.dword[2 * i];
}

// Expands to:

if (0) {
  dst.qword[0] = ((u128)((u64)a.dword[0])) - ((u128)((u64)b.dword[0]));
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

## \_\_m128i \_\_lsx_vsubwod_h_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsubwod_h_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsubwod.h.b vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract odd-positioned signed 8-bit elements in `a` and signed elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (s16)(s8)a.byte[2 * i + 1] - (s16)(s8)b.byte[2 * i + 1];
}

// Expands to:

if (0) {
  dst.half[0] = ((s16)((s8)a.byte[1])) - ((s16)((s8)b.byte[1]));
  dst.half[1] = ((s16)((s8)a.byte[3])) - ((s16)((s8)b.byte[3]));
  dst.half[2] = ((s16)((s8)a.byte[5])) - ((s16)((s8)b.byte[5]));
  dst.half[3] = ((s16)((s8)a.byte[7])) - ((s16)((s8)b.byte[7]));
  dst.half[4] = ((s16)((s8)a.byte[9])) - ((s16)((s8)b.byte[9]));
  dst.half[5] = ((s16)((s8)a.byte[11])) - ((s16)((s8)b.byte[11]));
  dst.half[6] = ((s16)((s8)a.byte[13])) - ((s16)((s8)b.byte[13]));
  dst.half[7] = ((s16)((s8)a.byte[15])) - ((s16)((s8)b.byte[15]));
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

## \_\_m128i \_\_lsx_vsubwod_h_bu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsubwod_h_bu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsubwod.h.bu vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract odd-positioned unsigned 8-bit elements in `a` and unsigned elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (u16)(u8)a.byte[2 * i + 1] - (u16)(u8)b.byte[2 * i + 1];
}

// Expands to:

if (0) {
  dst.half[0] = ((u16)((u8)a.byte[1])) - ((u16)((u8)b.byte[1]));
  dst.half[1] = ((u16)((u8)a.byte[3])) - ((u16)((u8)b.byte[3]));
  dst.half[2] = ((u16)((u8)a.byte[5])) - ((u16)((u8)b.byte[5]));
  dst.half[3] = ((u16)((u8)a.byte[7])) - ((u16)((u8)b.byte[7]));
  dst.half[4] = ((u16)((u8)a.byte[9])) - ((u16)((u8)b.byte[9]));
  dst.half[5] = ((u16)((u8)a.byte[11])) - ((u16)((u8)b.byte[11]));
  dst.half[6] = ((u16)((u8)a.byte[13])) - ((u16)((u8)b.byte[13]));
  dst.half[7] = ((u16)((u8)a.byte[15])) - ((u16)((u8)b.byte[15]));
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

## \_\_m128i \_\_lsx_vsubwod_w_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsubwod_w_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsubwod.w.h vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract odd-positioned signed 16-bit elements in `a` and signed elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (s32)(s16)a.half[2 * i + 1] - (s32)(s16)b.half[2 * i + 1];
}

// Expands to:

if (0) {
  dst.word[0] = ((s32)((s16)a.half[1])) - ((s32)((s16)b.half[1]));
  dst.word[1] = ((s32)((s16)a.half[3])) - ((s32)((s16)b.half[3]));
  dst.word[2] = ((s32)((s16)a.half[5])) - ((s32)((s16)b.half[5]));
  dst.word[3] = ((s32)((s16)a.half[7])) - ((s32)((s16)b.half[7]));
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

## \_\_m128i \_\_lsx_vsubwod_w_hu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsubwod_w_hu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsubwod.w.hu vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract odd-positioned unsigned 16-bit elements in `a` and unsigned elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i + 1] - (u32)(u16)b.half[2 * i + 1];
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[1])) - ((u32)((u16)b.half[1]));
  dst.word[1] = ((u32)((u16)a.half[3])) - ((u32)((u16)b.half[3]));
  dst.word[2] = ((u32)((u16)a.half[5])) - ((u32)((u16)b.half[5]));
  dst.word[3] = ((u32)((u16)a.half[7])) - ((u32)((u16)b.half[7]));
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

## \_\_m128i \_\_lsx_vsubwod_d_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsubwod_d_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsubwod.d.w vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract odd-positioned signed 32-bit elements in `a` and signed elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)(s32)a.word[2 * i + 1] - (s64)(s32)b.word[2 * i + 1];
}

// Expands to:

if (0) {
  dst.dword[0] = ((s64)((s32)a.word[1])) - ((s64)((s32)b.word[1]));
  dst.dword[1] = ((s64)((s32)a.word[3])) - ((s64)((s32)b.word[3]));
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

## \_\_m128i \_\_lsx_vsubwod_d_wu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsubwod_d_wu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsubwod.d.wu vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract odd-positioned unsigned 32-bit elements in `a` and unsigned elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 * i + 1] - (u64)(u32)b.word[2 * i + 1];
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[1])) - ((u64)((u32)b.word[1]));
  dst.dword[1] = ((u64)((u32)a.word[3])) - ((u64)((u32)b.word[3]));
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

## \_\_m128i \_\_lsx_vsubwod_q_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsubwod_q_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsubwod.q.d vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract odd-positioned signed 64-bit elements in `a` and signed elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] = (s128)(s64)a.dword[2 * i + 1] - (s128)(s64)b.dword[2 * i + 1];
}

// Expands to:

if (0) {
  dst.qword[0] = ((s128)((s64)a.dword[1])) - ((s128)((s64)b.dword[1]));
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

## \_\_m128i \_\_lsx_vsubwod_q_du (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsubwod_q_du (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsubwod.q.du vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract odd-positioned unsigned 64-bit elements in `a` and unsigned elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 1; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 * i + 1] - (u128)(u64)b.dword[2 * i + 1];
}

// Expands to:

if (0) {
  dst.qword[0] = ((u128)((u64)a.dword[1])) - ((u128)((u64)b.dword[1]));
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
