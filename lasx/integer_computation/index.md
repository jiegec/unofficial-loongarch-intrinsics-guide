# Integer Computation

## \_\_m256i \_\_lasx_xvadd_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvadd_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvadd.b xr, xr, xr
CPU Flags: LASX
```

### Description

Add 8-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[i] + b.byte[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvadd_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvadd_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvadd.h xr, xr, xr
CPU Flags: LASX
```

### Description

Add 16-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = a.half[i] + b.half[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvadd_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvadd_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvadd.w xr, xr, xr
CPU Flags: LASX
```

### Description

Add 32-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = a.word[i] + b.word[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvadd_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvadd_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvadd.d xr, xr, xr
CPU Flags: LASX
```

### Description

Add 64-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = a.dword[i] + b.dword[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvadd_q (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvadd_q (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvadd.q xr, xr, xr
CPU Flags: LASX
```

### Description

Add 128-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] = a.qword[i] + b.qword[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvabsd_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvabsd_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvabsd.b xr, xr, xr
CPU Flags: LASX
```

### Description

Compute absolute difference of signed 8-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = ((s8)a.byte[i] > (s8)b.byte[i]) ? (a.byte[i] - b.byte[i])
                                                : (b.byte[i] - a.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvabsd_bu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvabsd_bu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvabsd.bu xr, xr, xr
CPU Flags: LASX
```

### Description

Compute absolute difference of unsigned 8-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = ((u8)a.byte[i] > (u8)b.byte[i]) ? (a.byte[i] - b.byte[i])
                                                : (b.byte[i] - a.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvabsd_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvabsd_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvabsd.h xr, xr, xr
CPU Flags: LASX
```

### Description

Compute absolute difference of signed 16-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = ((s16)a.half[i] > (s16)b.half[i]) ? (a.half[i] - b.half[i])
                                                  : (b.half[i] - a.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvabsd_hu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvabsd_hu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvabsd.hu xr, xr, xr
CPU Flags: LASX
```

### Description

Compute absolute difference of unsigned 16-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = ((u16)a.half[i] > (u16)b.half[i]) ? (a.half[i] - b.half[i])
                                                  : (b.half[i] - a.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvabsd_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvabsd_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvabsd.w xr, xr, xr
CPU Flags: LASX
```

### Description

Compute absolute difference of signed 32-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = ((s32)a.word[i] > (s32)b.word[i]) ? (a.word[i] - b.word[i])
                                                  : (b.word[i] - a.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvabsd_wu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvabsd_wu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvabsd.wu xr, xr, xr
CPU Flags: LASX
```

### Description

Compute absolute difference of unsigned 32-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = ((u32)a.word[i] > (u32)b.word[i]) ? (a.word[i] - b.word[i])
                                                  : (b.word[i] - a.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvabsd_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvabsd_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvabsd.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compute absolute difference of signed 64-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = ((s64)a.dword[i] > (s64)b.dword[i])
                     ? (a.dword[i] - b.dword[i])
                     : (b.dword[i] - a.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvabsd_du (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvabsd_du (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvabsd.du xr, xr, xr
CPU Flags: LASX
```

### Description

Compute absolute difference of unsigned 64-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = ((u64)a.dword[i] > (u64)b.dword[i])
                     ? (a.dword[i] - b.dword[i])
                     : (b.dword[i] - a.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvadda_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvadda_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvadda.b xr, xr, xr
CPU Flags: LASX
```

### Description

Add absolute of 8-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = abs((s8)a.byte[i]) + abs((s8)b.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvadda_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvadda_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvadda.h xr, xr, xr
CPU Flags: LASX
```

### Description

Add absolute of 16-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = abs((s16)a.half[i]) + abs((s16)b.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvadda_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvadda_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvadda.w xr, xr, xr
CPU Flags: LASX
```

### Description

Add absolute of 32-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = abs((s32)a.word[i]) + abs((s32)b.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvadda_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvadda_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvadda.d xr, xr, xr
CPU Flags: LASX
```

### Description

Add absolute of 64-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = abs((s64)a.dword[i]) + abs((s64)b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvaddi_bu (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvaddi_bu (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvaddi.bu xr, xr, imm
CPU Flags: LASX
```

### Description

Add 8-bit elements in `a` and `imm`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[i] + imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvaddi_hu (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvaddi_hu (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvaddi.hu xr, xr, imm
CPU Flags: LASX
```

### Description

Add 16-bit elements in `a` and `imm`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = a.half[i] + imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvaddi_wu (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvaddi_wu (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvaddi.wu xr, xr, imm
CPU Flags: LASX
```

### Description

Add 32-bit elements in `a` and `imm`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = a.word[i] + imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvaddi_du (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvaddi_du (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvaddi.du xr, xr, imm
CPU Flags: LASX
```

### Description

Add 64-bit elements in `a` and `imm`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = a.dword[i] + imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvaddwev_h_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvaddwev_h_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvaddwev.h.b xr, xr, xr
CPU Flags: LASX
```

### Description

Add even-positioned signed 8-bit elements in `a` and signed elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
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
  dst.half[8] = ((s16)((s8)a.byte[16])) + ((s16)((s8)b.byte[16]));
  dst.half[9] = ((s16)((s8)a.byte[18])) + ((s16)((s8)b.byte[18]));
  dst.half[10] = ((s16)((s8)a.byte[20])) + ((s16)((s8)b.byte[20]));
  dst.half[11] = ((s16)((s8)a.byte[22])) + ((s16)((s8)b.byte[22]));
  dst.half[12] = ((s16)((s8)a.byte[24])) + ((s16)((s8)b.byte[24]));
  dst.half[13] = ((s16)((s8)a.byte[26])) + ((s16)((s8)b.byte[26]));
  dst.half[14] = ((s16)((s8)a.byte[28])) + ((s16)((s8)b.byte[28]));
  dst.half[15] = ((s16)((s8)a.byte[30])) + ((s16)((s8)b.byte[30]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvaddwev_h_bu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvaddwev_h_bu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvaddwev.h.bu xr, xr, xr
CPU Flags: LASX
```

### Description

Add even-positioned unsigned 8-bit elements in `a` and unsigned elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
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
  dst.half[8] = ((u16)((u8)a.byte[16])) + ((u16)((u8)b.byte[16]));
  dst.half[9] = ((u16)((u8)a.byte[18])) + ((u16)((u8)b.byte[18]));
  dst.half[10] = ((u16)((u8)a.byte[20])) + ((u16)((u8)b.byte[20]));
  dst.half[11] = ((u16)((u8)a.byte[22])) + ((u16)((u8)b.byte[22]));
  dst.half[12] = ((u16)((u8)a.byte[24])) + ((u16)((u8)b.byte[24]));
  dst.half[13] = ((u16)((u8)a.byte[26])) + ((u16)((u8)b.byte[26]));
  dst.half[14] = ((u16)((u8)a.byte[28])) + ((u16)((u8)b.byte[28]));
  dst.half[15] = ((u16)((u8)a.byte[30])) + ((u16)((u8)b.byte[30]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvaddwev_h_bu_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvaddwev_h_bu_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvaddwev.h.bu.b xr, xr, xr
CPU Flags: LASX
```

### Description

Add even-positioned unsigned 8-bit elements in `a` and signed elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
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
  dst.half[8] = ((u16)((u8)a.byte[16])) + ((s16)((s8)b.byte[16]));
  dst.half[9] = ((u16)((u8)a.byte[18])) + ((s16)((s8)b.byte[18]));
  dst.half[10] = ((u16)((u8)a.byte[20])) + ((s16)((s8)b.byte[20]));
  dst.half[11] = ((u16)((u8)a.byte[22])) + ((s16)((s8)b.byte[22]));
  dst.half[12] = ((u16)((u8)a.byte[24])) + ((s16)((s8)b.byte[24]));
  dst.half[13] = ((u16)((u8)a.byte[26])) + ((s16)((s8)b.byte[26]));
  dst.half[14] = ((u16)((u8)a.byte[28])) + ((s16)((s8)b.byte[28]));
  dst.half[15] = ((u16)((u8)a.byte[30])) + ((s16)((s8)b.byte[30]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvaddwev_w_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvaddwev_w_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvaddwev.w.h xr, xr, xr
CPU Flags: LASX
```

### Description

Add even-positioned signed 16-bit elements in `a` and signed elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (s32)(s16)a.half[2 * i] + (s32)(s16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((s32)((s16)a.half[0])) + ((s32)((s16)b.half[0]));
  dst.word[1] = ((s32)((s16)a.half[2])) + ((s32)((s16)b.half[2]));
  dst.word[2] = ((s32)((s16)a.half[4])) + ((s32)((s16)b.half[4]));
  dst.word[3] = ((s32)((s16)a.half[6])) + ((s32)((s16)b.half[6]));
  dst.word[4] = ((s32)((s16)a.half[8])) + ((s32)((s16)b.half[8]));
  dst.word[5] = ((s32)((s16)a.half[10])) + ((s32)((s16)b.half[10]));
  dst.word[6] = ((s32)((s16)a.half[12])) + ((s32)((s16)b.half[12]));
  dst.word[7] = ((s32)((s16)a.half[14])) + ((s32)((s16)b.half[14]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvaddwev_w_hu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvaddwev_w_hu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvaddwev.w.hu xr, xr, xr
CPU Flags: LASX
```

### Description

Add even-positioned unsigned 16-bit elements in `a` and unsigned elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i] + (u32)(u16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[0])) + ((u32)((u16)b.half[0]));
  dst.word[1] = ((u32)((u16)a.half[2])) + ((u32)((u16)b.half[2]));
  dst.word[2] = ((u32)((u16)a.half[4])) + ((u32)((u16)b.half[4]));
  dst.word[3] = ((u32)((u16)a.half[6])) + ((u32)((u16)b.half[6]));
  dst.word[4] = ((u32)((u16)a.half[8])) + ((u32)((u16)b.half[8]));
  dst.word[5] = ((u32)((u16)a.half[10])) + ((u32)((u16)b.half[10]));
  dst.word[6] = ((u32)((u16)a.half[12])) + ((u32)((u16)b.half[12]));
  dst.word[7] = ((u32)((u16)a.half[14])) + ((u32)((u16)b.half[14]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvaddwev_w_hu_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvaddwev_w_hu_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvaddwev.w.hu.h xr, xr, xr
CPU Flags: LASX
```

### Description

Add even-positioned unsigned 16-bit elements in `a` and signed elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i] + (s32)(s16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[0])) + ((s32)((s16)b.half[0]));
  dst.word[1] = ((u32)((u16)a.half[2])) + ((s32)((s16)b.half[2]));
  dst.word[2] = ((u32)((u16)a.half[4])) + ((s32)((s16)b.half[4]));
  dst.word[3] = ((u32)((u16)a.half[6])) + ((s32)((s16)b.half[6]));
  dst.word[4] = ((u32)((u16)a.half[8])) + ((s32)((s16)b.half[8]));
  dst.word[5] = ((u32)((u16)a.half[10])) + ((s32)((s16)b.half[10]));
  dst.word[6] = ((u32)((u16)a.half[12])) + ((s32)((s16)b.half[12]));
  dst.word[7] = ((u32)((u16)a.half[14])) + ((s32)((s16)b.half[14]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvaddwev_d_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvaddwev_d_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvaddwev.d.w xr, xr, xr
CPU Flags: LASX
```

### Description

Add even-positioned signed 32-bit elements in `a` and signed elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (s64)(s32)a.word[2 * i] + (s64)(s32)b.word[2 * i];
}

// Expands to:

if (0) {
  dst.dword[0] = ((s64)((s32)a.word[0])) + ((s64)((s32)b.word[0]));
  dst.dword[1] = ((s64)((s32)a.word[2])) + ((s64)((s32)b.word[2]));
  dst.dword[2] = ((s64)((s32)a.word[4])) + ((s64)((s32)b.word[4]));
  dst.dword[3] = ((s64)((s32)a.word[6])) + ((s64)((s32)b.word[6]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvaddwev_d_wu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvaddwev_d_wu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvaddwev.d.wu xr, xr, xr
CPU Flags: LASX
```

### Description

Add even-positioned unsigned 32-bit elements in `a` and unsigned elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 * i] + (u64)(u32)b.word[2 * i];
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[0])) + ((u64)((u32)b.word[0]));
  dst.dword[1] = ((u64)((u32)a.word[2])) + ((u64)((u32)b.word[2]));
  dst.dword[2] = ((u64)((u32)a.word[4])) + ((u64)((u32)b.word[4]));
  dst.dword[3] = ((u64)((u32)a.word[6])) + ((u64)((u32)b.word[6]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvaddwev_d_wu_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvaddwev_d_wu_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvaddwev.d.wu.w xr, xr, xr
CPU Flags: LASX
```

### Description

Add even-positioned unsigned 32-bit elements in `a` and signed elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 * i] + (s64)(s32)b.word[2 * i];
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[0])) + ((s64)((s32)b.word[0]));
  dst.dword[1] = ((u64)((u32)a.word[2])) + ((s64)((s32)b.word[2]));
  dst.dword[2] = ((u64)((u32)a.word[4])) + ((s64)((s32)b.word[4]));
  dst.dword[3] = ((u64)((u32)a.word[6])) + ((s64)((s32)b.word[6]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvaddwev_q_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvaddwev_q_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvaddwev.q.d xr, xr, xr
CPU Flags: LASX
```

### Description

Add even-positioned signed 64-bit elements in `a` and signed elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] = (s128)(s64)a.dword[2 * i] + (s128)(s64)b.dword[2 * i];
}

// Expands to:

if (0) {
  dst.qword[0] = ((s128)((s64)a.dword[0])) + ((s128)((s64)b.dword[0]));
  dst.qword[1] = ((s128)((s64)a.dword[2])) + ((s128)((s64)b.dword[2]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvaddwev_q_du (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvaddwev_q_du (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvaddwev.q.du xr, xr, xr
CPU Flags: LASX
```

### Description

Add even-positioned unsigned 64-bit elements in `a` and unsigned elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 * i] + (u128)(u64)b.dword[2 * i];
}

// Expands to:

if (0) {
  dst.qword[0] = ((u128)((u64)a.dword[0])) + ((u128)((u64)b.dword[0]));
  dst.qword[1] = ((u128)((u64)a.dword[2])) + ((u128)((u64)b.dword[2]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvaddwev_q_du_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvaddwev_q_du_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvaddwev.q.du.d xr, xr, xr
CPU Flags: LASX
```

### Description

Add even-positioned unsigned 64-bit elements in `a` and signed elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 * i] + (s128)(s64)b.dword[2 * i];
}

// Expands to:

if (0) {
  dst.qword[0] = ((u128)((u64)a.dword[0])) + ((s128)((s64)b.dword[0]));
  dst.qword[1] = ((u128)((u64)a.dword[2])) + ((s128)((s64)b.dword[2]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvaddwod_h_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvaddwod_h_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvaddwod.h.b xr, xr, xr
CPU Flags: LASX
```

### Description

Add odd-positioned signed 8-bit elements in `a` and signed elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
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
  dst.half[8] = ((s16)((s8)a.byte[17])) + ((s16)((s8)b.byte[17]));
  dst.half[9] = ((s16)((s8)a.byte[19])) + ((s16)((s8)b.byte[19]));
  dst.half[10] = ((s16)((s8)a.byte[21])) + ((s16)((s8)b.byte[21]));
  dst.half[11] = ((s16)((s8)a.byte[23])) + ((s16)((s8)b.byte[23]));
  dst.half[12] = ((s16)((s8)a.byte[25])) + ((s16)((s8)b.byte[25]));
  dst.half[13] = ((s16)((s8)a.byte[27])) + ((s16)((s8)b.byte[27]));
  dst.half[14] = ((s16)((s8)a.byte[29])) + ((s16)((s8)b.byte[29]));
  dst.half[15] = ((s16)((s8)a.byte[31])) + ((s16)((s8)b.byte[31]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvaddwod_h_bu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvaddwod_h_bu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvaddwod.h.bu xr, xr, xr
CPU Flags: LASX
```

### Description

Add odd-positioned unsigned 8-bit elements in `a` and unsigned elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
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
  dst.half[8] = ((u16)((u8)a.byte[17])) + ((u16)((u8)b.byte[17]));
  dst.half[9] = ((u16)((u8)a.byte[19])) + ((u16)((u8)b.byte[19]));
  dst.half[10] = ((u16)((u8)a.byte[21])) + ((u16)((u8)b.byte[21]));
  dst.half[11] = ((u16)((u8)a.byte[23])) + ((u16)((u8)b.byte[23]));
  dst.half[12] = ((u16)((u8)a.byte[25])) + ((u16)((u8)b.byte[25]));
  dst.half[13] = ((u16)((u8)a.byte[27])) + ((u16)((u8)b.byte[27]));
  dst.half[14] = ((u16)((u8)a.byte[29])) + ((u16)((u8)b.byte[29]));
  dst.half[15] = ((u16)((u8)a.byte[31])) + ((u16)((u8)b.byte[31]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvaddwod_h_bu_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvaddwod_h_bu_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvaddwod.h.bu.b xr, xr, xr
CPU Flags: LASX
```

### Description

Add odd-positioned unsigned 8-bit elements in `a` and signed elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
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
  dst.half[8] = ((u16)((u8)a.byte[17])) + ((s16)((s8)b.byte[17]));
  dst.half[9] = ((u16)((u8)a.byte[19])) + ((s16)((s8)b.byte[19]));
  dst.half[10] = ((u16)((u8)a.byte[21])) + ((s16)((s8)b.byte[21]));
  dst.half[11] = ((u16)((u8)a.byte[23])) + ((s16)((s8)b.byte[23]));
  dst.half[12] = ((u16)((u8)a.byte[25])) + ((s16)((s8)b.byte[25]));
  dst.half[13] = ((u16)((u8)a.byte[27])) + ((s16)((s8)b.byte[27]));
  dst.half[14] = ((u16)((u8)a.byte[29])) + ((s16)((s8)b.byte[29]));
  dst.half[15] = ((u16)((u8)a.byte[31])) + ((s16)((s8)b.byte[31]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvaddwod_w_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvaddwod_w_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvaddwod.w.h xr, xr, xr
CPU Flags: LASX
```

### Description

Add odd-positioned signed 16-bit elements in `a` and signed elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (s32)(s16)a.half[2 * i + 1] + (s32)(s16)b.half[2 * i + 1];
}

// Expands to:

if (0) {
  dst.word[0] = ((s32)((s16)a.half[1])) + ((s32)((s16)b.half[1]));
  dst.word[1] = ((s32)((s16)a.half[3])) + ((s32)((s16)b.half[3]));
  dst.word[2] = ((s32)((s16)a.half[5])) + ((s32)((s16)b.half[5]));
  dst.word[3] = ((s32)((s16)a.half[7])) + ((s32)((s16)b.half[7]));
  dst.word[4] = ((s32)((s16)a.half[9])) + ((s32)((s16)b.half[9]));
  dst.word[5] = ((s32)((s16)a.half[11])) + ((s32)((s16)b.half[11]));
  dst.word[6] = ((s32)((s16)a.half[13])) + ((s32)((s16)b.half[13]));
  dst.word[7] = ((s32)((s16)a.half[15])) + ((s32)((s16)b.half[15]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvaddwod_w_hu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvaddwod_w_hu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvaddwod.w.hu xr, xr, xr
CPU Flags: LASX
```

### Description

Add odd-positioned unsigned 16-bit elements in `a` and unsigned elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i + 1] + (u32)(u16)b.half[2 * i + 1];
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[1])) + ((u32)((u16)b.half[1]));
  dst.word[1] = ((u32)((u16)a.half[3])) + ((u32)((u16)b.half[3]));
  dst.word[2] = ((u32)((u16)a.half[5])) + ((u32)((u16)b.half[5]));
  dst.word[3] = ((u32)((u16)a.half[7])) + ((u32)((u16)b.half[7]));
  dst.word[4] = ((u32)((u16)a.half[9])) + ((u32)((u16)b.half[9]));
  dst.word[5] = ((u32)((u16)a.half[11])) + ((u32)((u16)b.half[11]));
  dst.word[6] = ((u32)((u16)a.half[13])) + ((u32)((u16)b.half[13]));
  dst.word[7] = ((u32)((u16)a.half[15])) + ((u32)((u16)b.half[15]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvaddwod_w_hu_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvaddwod_w_hu_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvaddwod.w.hu.h xr, xr, xr
CPU Flags: LASX
```

### Description

Add odd-positioned unsigned 16-bit elements in `a` and signed elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i + 1] + (s32)(s16)b.half[2 * i + 1];
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[1])) + ((s32)((s16)b.half[1]));
  dst.word[1] = ((u32)((u16)a.half[3])) + ((s32)((s16)b.half[3]));
  dst.word[2] = ((u32)((u16)a.half[5])) + ((s32)((s16)b.half[5]));
  dst.word[3] = ((u32)((u16)a.half[7])) + ((s32)((s16)b.half[7]));
  dst.word[4] = ((u32)((u16)a.half[9])) + ((s32)((s16)b.half[9]));
  dst.word[5] = ((u32)((u16)a.half[11])) + ((s32)((s16)b.half[11]));
  dst.word[6] = ((u32)((u16)a.half[13])) + ((s32)((s16)b.half[13]));
  dst.word[7] = ((u32)((u16)a.half[15])) + ((s32)((s16)b.half[15]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvaddwod_d_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvaddwod_d_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvaddwod.d.w xr, xr, xr
CPU Flags: LASX
```

### Description

Add odd-positioned signed 32-bit elements in `a` and signed elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (s64)(s32)a.word[2 * i + 1] + (s64)(s32)b.word[2 * i + 1];
}

// Expands to:

if (0) {
  dst.dword[0] = ((s64)((s32)a.word[1])) + ((s64)((s32)b.word[1]));
  dst.dword[1] = ((s64)((s32)a.word[3])) + ((s64)((s32)b.word[3]));
  dst.dword[2] = ((s64)((s32)a.word[5])) + ((s64)((s32)b.word[5]));
  dst.dword[3] = ((s64)((s32)a.word[7])) + ((s64)((s32)b.word[7]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvaddwod_d_wu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvaddwod_d_wu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvaddwod.d.wu xr, xr, xr
CPU Flags: LASX
```

### Description

Add odd-positioned unsigned 32-bit elements in `a` and unsigned elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 * i + 1] + (u64)(u32)b.word[2 * i + 1];
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[1])) + ((u64)((u32)b.word[1]));
  dst.dword[1] = ((u64)((u32)a.word[3])) + ((u64)((u32)b.word[3]));
  dst.dword[2] = ((u64)((u32)a.word[5])) + ((u64)((u32)b.word[5]));
  dst.dword[3] = ((u64)((u32)a.word[7])) + ((u64)((u32)b.word[7]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvaddwod_d_wu_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvaddwod_d_wu_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvaddwod.d.wu.w xr, xr, xr
CPU Flags: LASX
```

### Description

Add odd-positioned unsigned 32-bit elements in `a` and signed elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 * i + 1] + (s64)(s32)b.word[2 * i + 1];
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[1])) + ((s64)((s32)b.word[1]));
  dst.dword[1] = ((u64)((u32)a.word[3])) + ((s64)((s32)b.word[3]));
  dst.dword[2] = ((u64)((u32)a.word[5])) + ((s64)((s32)b.word[5]));
  dst.dword[3] = ((u64)((u32)a.word[7])) + ((s64)((s32)b.word[7]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvaddwod_q_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvaddwod_q_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvaddwod.q.d xr, xr, xr
CPU Flags: LASX
```

### Description

Add odd-positioned signed 64-bit elements in `a` and signed elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] = (s128)(s64)a.dword[2 * i + 1] + (s128)(s64)b.dword[2 * i + 1];
}

// Expands to:

if (0) {
  dst.qword[0] = ((s128)((s64)a.dword[1])) + ((s128)((s64)b.dword[1]));
  dst.qword[1] = ((s128)((s64)a.dword[3])) + ((s128)((s64)b.dword[3]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvaddwod_q_du (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvaddwod_q_du (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvaddwod.q.du xr, xr, xr
CPU Flags: LASX
```

### Description

Add odd-positioned unsigned 64-bit elements in `a` and unsigned elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 * i + 1] + (u128)(u64)b.dword[2 * i + 1];
}

// Expands to:

if (0) {
  dst.qword[0] = ((u128)((u64)a.dword[1])) + ((u128)((u64)b.dword[1]));
  dst.qword[1] = ((u128)((u64)a.dword[3])) + ((u128)((u64)b.dword[3]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvaddwod_q_du_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvaddwod_q_du_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvaddwod.q.du.d xr, xr, xr
CPU Flags: LASX
```

### Description

Add odd-positioned unsigned 64-bit elements in `a` and signed elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 * i + 1] + (s128)(s64)b.dword[2 * i + 1];
}

// Expands to:

if (0) {
  dst.qword[0] = ((u128)((u64)a.dword[1])) + ((s128)((s64)b.dword[1]));
  dst.qword[1] = ((u128)((u64)a.dword[3])) + ((s128)((s64)b.dword[3]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvavg_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvavg_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvavg.b xr, xr, xr
CPU Flags: LASX
```

### Description

Compute the average (rounded towards negative infinity) of signed 8-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = ((s8)a.byte[i] >> 1) + ((s8)b.byte[i] >> 1) +
                ((a.byte[i] & b.byte[i]) & 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvavg_bu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvavg_bu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvavg.bu xr, xr, xr
CPU Flags: LASX
```

### Description

Compute the average (rounded towards negative infinity) of unsigned 8-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = ((u8)a.byte[i] >> 1) + ((u8)b.byte[i] >> 1) +
                ((a.byte[i] & b.byte[i]) & 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvavg_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvavg_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvavg.h xr, xr, xr
CPU Flags: LASX
```

### Description

Compute the average (rounded towards negative infinity) of signed 16-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = ((s16)a.half[i] >> 1) + ((s16)b.half[i] >> 1) +
                ((a.half[i] & b.half[i]) & 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvavg_hu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvavg_hu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvavg.hu xr, xr, xr
CPU Flags: LASX
```

### Description

Compute the average (rounded towards negative infinity) of unsigned 16-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = ((u16)a.half[i] >> 1) + ((u16)b.half[i] >> 1) +
                ((a.half[i] & b.half[i]) & 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvavg_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvavg_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvavg.w xr, xr, xr
CPU Flags: LASX
```

### Description

Compute the average (rounded towards negative infinity) of signed 32-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = ((s32)a.word[i] >> 1) + ((s32)b.word[i] >> 1) +
                ((a.word[i] & b.word[i]) & 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvavg_wu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvavg_wu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvavg.wu xr, xr, xr
CPU Flags: LASX
```

### Description

Compute the average (rounded towards negative infinity) of unsigned 32-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = ((u32)a.word[i] >> 1) + ((u32)b.word[i] >> 1) +
                ((a.word[i] & b.word[i]) & 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvavg_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvavg_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvavg.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compute the average (rounded towards negative infinity) of signed 64-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = ((s64)a.dword[i] >> 1) + ((s64)b.dword[i] >> 1) +
                 ((a.dword[i] & b.dword[i]) & 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvavg_du (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvavg_du (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvavg.du xr, xr, xr
CPU Flags: LASX
```

### Description

Compute the average (rounded towards negative infinity) of unsigned 64-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = ((u64)a.dword[i] >> 1) + ((u64)b.dword[i] >> 1) +
                 ((a.dword[i] & b.dword[i]) & 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvavgr_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvavgr_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvavgr.b xr, xr, xr
CPU Flags: LASX
```

### Description

Compute the average (rounded towards positive infinity) of signed 8-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = ((s8)a.byte[i] >> 1) + ((s8)b.byte[i] >> 1) +
                ((a.byte[i] | b.byte[i]) & 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvavgr_bu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvavgr_bu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvavgr.bu xr, xr, xr
CPU Flags: LASX
```

### Description

Compute the average (rounded towards positive infinity) of unsigned 8-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = ((u8)a.byte[i] >> 1) + ((u8)b.byte[i] >> 1) +
                ((a.byte[i] | b.byte[i]) & 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvavgr_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvavgr_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvavgr.h xr, xr, xr
CPU Flags: LASX
```

### Description

Compute the average (rounded towards positive infinity) of signed 16-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = ((s16)a.half[i] >> 1) + ((s16)b.half[i] >> 1) +
                ((a.half[i] | b.half[i]) & 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvavgr_hu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvavgr_hu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvavgr.hu xr, xr, xr
CPU Flags: LASX
```

### Description

Compute the average (rounded towards positive infinity) of unsigned 16-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = ((u16)a.half[i] >> 1) + ((u16)b.half[i] >> 1) +
                ((a.half[i] | b.half[i]) & 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvavgr_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvavgr_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvavgr.w xr, xr, xr
CPU Flags: LASX
```

### Description

Compute the average (rounded towards positive infinity) of signed 32-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = ((s32)a.word[i] >> 1) + ((s32)b.word[i] >> 1) +
                ((a.word[i] | b.word[i]) & 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvavgr_wu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvavgr_wu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvavgr.wu xr, xr, xr
CPU Flags: LASX
```

### Description

Compute the average (rounded towards positive infinity) of unsigned 32-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = ((u32)a.word[i] >> 1) + ((u32)b.word[i] >> 1) +
                ((a.word[i] | b.word[i]) & 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvavgr_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvavgr_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvavgr.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compute the average (rounded towards positive infinity) of signed 64-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = ((s64)a.dword[i] >> 1) + ((s64)b.dword[i] >> 1) +
                 ((a.dword[i] | b.dword[i]) & 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvavgr_du (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvavgr_du (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvavgr.du xr, xr, xr
CPU Flags: LASX
```

### Description

Compute the average (rounded towards positive infinity) of unsigned 64-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = ((u64)a.dword[i] >> 1) + ((u64)b.dword[i] >> 1) +
                 ((a.dword[i] | b.dword[i]) & 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvdiv_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvdiv_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvdiv.b xr, xr, xr
CPU Flags: LASX
```

### Description

Divide signed 8-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = (b.byte[i] == 0) ? 0 : ((s8)a.byte[i] / (s8)b.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 32, 36  | 0.05(1/20.5)     |
| 3A6000 | LA664 | 29, 32  | 0.06(1/15.5)     |
| 3C6000 | LA664 | 55, 57  | 0.04(1/27.5)     |

## \_\_m256i \_\_lasx_xvdiv_bu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvdiv_bu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvdiv.bu xr, xr, xr
CPU Flags: LASX
```

### Description

Divide unsigned 8-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = (b.byte[i] == 0) ? 0 : ((u8)a.byte[i] / (u8)b.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 29, 36  | 0.05(1/20.5)     |
| 3A6000 | LA664 | 29, 33  | 0.06(1/16.5)     |
| 3C6000 | LA664 | 29, 36  | 0.07(1/13.5)     |

## \_\_m256i \_\_lasx_xvdiv_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvdiv_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvdiv.h xr, xr, xr
CPU Flags: LASX
```

### Description

Divide signed 16-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = (b.half[i] == 0) ? 0 : ((s16)a.half[i] / (s16)b.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency  | Throughput (IPC) |
| ------ | ----- | -------- | ---------------- |
| 3C5000 | LA464 | 21.5, 22 | 0.08(1/13)       |
| 3A6000 | LA664 | 17       | 0.12(1/8.5)      |
| 3C6000 | LA664 | 34, 40   | 0.05(1/19)       |

## \_\_m256i \_\_lasx_xvdiv_hu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvdiv_hu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvdiv.hu xr, xr, xr
CPU Flags: LASX
```

### Description

Divide unsigned 16-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = (b.half[i] == 0) ? 0 : ((u16)a.half[i] / (u16)b.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency  | Throughput (IPC) |
| ------ | ----- | -------- | ---------------- |
| 3C5000 | LA464 | 17, 21.5 | 0.07(1/15)       |
| 3A6000 | LA664 | 17, 22   | 0.11(1/9)        |
| 3C6000 | LA664 | 17, 21.5 | 0.13(1/7.5)      |

## \_\_m256i \_\_lasx_xvdiv_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvdiv_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvdiv.w xr, xr, xr
CPU Flags: LASX
```

### Description

Divide signed 32-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (b.word[i] == 0) ? 0 : ((s32)a.word[i] / (s32)b.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency  | Throughput (IPC) |
| ------ | ----- | -------- | ---------------- |
| 3C5000 | LA464 | 11, 17.5 | 0.09(1/11.5)     |
| 3A6000 | LA664 | 11       | 0.18(1/5.5)      |
| 3C6000 | LA664 | 23.5, 30 | 0.13(1/7.5)      |

## \_\_m256i \_\_lasx_xvdiv_wu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvdiv_wu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvdiv.wu xr, xr, xr
CPU Flags: LASX
```

### Description

Divide unsigned 32-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (b.word[i] == 0) ? 0 : ((u32)a.word[i] / (u32)b.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency  | Throughput (IPC) |
| ------ | ----- | -------- | ---------------- |
| 3C5000 | LA464 | 11, 17.5 | 0.07(1/15)       |
| 3A6000 | LA664 | 11       | 0.18(1/5.5)      |
| 3C6000 | LA664 | 11, 17.5 | 0.22(1/4.5)      |

## \_\_m256i \_\_lasx_xvdiv_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvdiv_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvdiv.d xr, xr, xr
CPU Flags: LASX
```

### Description

Divide signed 64-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (b.dword[i] == 0) ? 0 : ((s64)a.dword[i] / (s64)b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency  | Throughput (IPC) |
| ------ | ----- | -------- | ---------------- |
| 3C5000 | LA464 | 8, 18.5  | 0.11(1/9)        |
| 3A6000 | LA664 | 8        | 0.25(1/4)        |
| 3C6000 | LA664 | 18.5, 19 | 0.12(1/8.5)      |

## \_\_m256i \_\_lasx_xvdiv_du (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvdiv_du (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvdiv.du xr, xr, xr
CPU Flags: LASX
```

### Description

Divide unsigned 64-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (b.dword[i] == 0) ? 0 : ((u64)a.dword[i] / (u64)b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 8, 18.5 | 0.11(1/9)        |
| 3A6000 | LA664 | 8       | 0.25(1/4)        |
| 3C6000 | LA664 | 8, 18.5 | 0.33(1/3)        |

## \_\_m256i \_\_lasx_xvhaddw_h_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvhaddw_h_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvhaddw.h.b xr, xr, xr
CPU Flags: LASX
```

### Description

Add odd-positioned signed 8-bit elements in `a` to even-positioned signed 8-bit elements in `b` to get 16-bit result.

### Operation

```
for (int i = 0; i < 16; i++) {
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
  dst.half[8] = ((s16)((s8)a.byte[17])) + ((s16)((s8)b.byte[16]));
  dst.half[9] = ((s16)((s8)a.byte[19])) + ((s16)((s8)b.byte[18]));
  dst.half[10] = ((s16)((s8)a.byte[21])) + ((s16)((s8)b.byte[20]));
  dst.half[11] = ((s16)((s8)a.byte[23])) + ((s16)((s8)b.byte[22]));
  dst.half[12] = ((s16)((s8)a.byte[25])) + ((s16)((s8)b.byte[24]));
  dst.half[13] = ((s16)((s8)a.byte[27])) + ((s16)((s8)b.byte[26]));
  dst.half[14] = ((s16)((s8)a.byte[29])) + ((s16)((s8)b.byte[28]));
  dst.half[15] = ((s16)((s8)a.byte[31])) + ((s16)((s8)b.byte[30]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvhaddw_hu_bu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvhaddw_hu_bu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvhaddw.hu.bu xr, xr, xr
CPU Flags: LASX
```

### Description

Add odd-positioned unsigned 8-bit elements in `a` to even-positioned unsigned 8-bit elements in `b` to get 16-bit result.

### Operation

```
for (int i = 0; i < 16; i++) {
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
  dst.half[8] = ((u16)((u8)a.byte[17])) + ((u16)((u8)b.byte[16]));
  dst.half[9] = ((u16)((u8)a.byte[19])) + ((u16)((u8)b.byte[18]));
  dst.half[10] = ((u16)((u8)a.byte[21])) + ((u16)((u8)b.byte[20]));
  dst.half[11] = ((u16)((u8)a.byte[23])) + ((u16)((u8)b.byte[22]));
  dst.half[12] = ((u16)((u8)a.byte[25])) + ((u16)((u8)b.byte[24]));
  dst.half[13] = ((u16)((u8)a.byte[27])) + ((u16)((u8)b.byte[26]));
  dst.half[14] = ((u16)((u8)a.byte[29])) + ((u16)((u8)b.byte[28]));
  dst.half[15] = ((u16)((u8)a.byte[31])) + ((u16)((u8)b.byte[30]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvhaddw_w_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvhaddw_w_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvhaddw.w.h xr, xr, xr
CPU Flags: LASX
```

### Description

Add odd-positioned signed 16-bit elements in `a` to even-positioned signed 16-bit elements in `b` to get 32-bit result.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (s32)(s16)a.half[2 * i + 1] + (s32)(s16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((s32)((s16)a.half[1])) + ((s32)((s16)b.half[0]));
  dst.word[1] = ((s32)((s16)a.half[3])) + ((s32)((s16)b.half[2]));
  dst.word[2] = ((s32)((s16)a.half[5])) + ((s32)((s16)b.half[4]));
  dst.word[3] = ((s32)((s16)a.half[7])) + ((s32)((s16)b.half[6]));
  dst.word[4] = ((s32)((s16)a.half[9])) + ((s32)((s16)b.half[8]));
  dst.word[5] = ((s32)((s16)a.half[11])) + ((s32)((s16)b.half[10]));
  dst.word[6] = ((s32)((s16)a.half[13])) + ((s32)((s16)b.half[12]));
  dst.word[7] = ((s32)((s16)a.half[15])) + ((s32)((s16)b.half[14]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvhaddw_wu_hu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvhaddw_wu_hu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvhaddw.wu.hu xr, xr, xr
CPU Flags: LASX
```

### Description

Add odd-positioned unsigned 16-bit elements in `a` to even-positioned unsigned 16-bit elements in `b` to get 32-bit result.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i + 1] + (u32)(u16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[1])) + ((u32)((u16)b.half[0]));
  dst.word[1] = ((u32)((u16)a.half[3])) + ((u32)((u16)b.half[2]));
  dst.word[2] = ((u32)((u16)a.half[5])) + ((u32)((u16)b.half[4]));
  dst.word[3] = ((u32)((u16)a.half[7])) + ((u32)((u16)b.half[6]));
  dst.word[4] = ((u32)((u16)a.half[9])) + ((u32)((u16)b.half[8]));
  dst.word[5] = ((u32)((u16)a.half[11])) + ((u32)((u16)b.half[10]));
  dst.word[6] = ((u32)((u16)a.half[13])) + ((u32)((u16)b.half[12]));
  dst.word[7] = ((u32)((u16)a.half[15])) + ((u32)((u16)b.half[14]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvhaddw_d_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvhaddw_d_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvhaddw.d.w xr, xr, xr
CPU Flags: LASX
```

### Description

Add odd-positioned signed 32-bit elements in `a` to even-positioned signed 32-bit elements in `b` to get 64-bit result.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (s64)(s32)a.word[2 * i + 1] + (s64)(s32)b.word[2 * i];
}

// Expands to:

if (0) {
  dst.dword[0] = ((s64)((s32)a.word[1])) + ((s64)((s32)b.word[0]));
  dst.dword[1] = ((s64)((s32)a.word[3])) + ((s64)((s32)b.word[2]));
  dst.dword[2] = ((s64)((s32)a.word[5])) + ((s64)((s32)b.word[4]));
  dst.dword[3] = ((s64)((s32)a.word[7])) + ((s64)((s32)b.word[6]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvhaddw_du_wu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvhaddw_du_wu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvhaddw.du.wu xr, xr, xr
CPU Flags: LASX
```

### Description

Add odd-positioned unsigned 32-bit elements in `a` to even-positioned unsigned 32-bit elements in `b` to get 64-bit result.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 * i + 1] + (u64)(u32)b.word[2 * i];
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[1])) + ((u64)((u32)b.word[0]));
  dst.dword[1] = ((u64)((u32)a.word[3])) + ((u64)((u32)b.word[2]));
  dst.dword[2] = ((u64)((u32)a.word[5])) + ((u64)((u32)b.word[4]));
  dst.dword[3] = ((u64)((u32)a.word[7])) + ((u64)((u32)b.word[6]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvhaddw_q_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvhaddw_q_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvhaddw.q.d xr, xr, xr
CPU Flags: LASX
```

### Description

Add odd-positioned signed 64-bit elements in `a` to even-positioned signed 64-bit elements in `b` to get 128-bit result.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] = (s128)(s64)a.dword[2 * i + 1] + (s128)(s64)b.dword[2 * i];
}

// Expands to:

if (0) {
  dst.qword[0] = ((s128)((s64)a.dword[1])) + ((s128)((s64)b.dword[0]));
  dst.qword[1] = ((s128)((s64)a.dword[3])) + ((s128)((s64)b.dword[2]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvhaddw_qu_du (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvhaddw_qu_du (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvhaddw.qu.du xr, xr, xr
CPU Flags: LASX
```

### Description

Add odd-positioned unsigned 64-bit elements in `a` to even-positioned unsigned 64-bit elements in `b` to get 128-bit result.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 * i + 1] + (u128)(u64)b.dword[2 * i];
}

// Expands to:

if (0) {
  dst.qword[0] = ((u128)((u64)a.dword[1])) + ((u128)((u64)b.dword[0]));
  dst.qword[1] = ((u128)((u64)a.dword[3])) + ((u128)((u64)b.dword[2]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvhsubw_h_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvhsubw_h_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvhsubw.h.b xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract odd-positioned signed 8-bit elements in `a` by even-positioned signed 8-bit elements in `b` to get 16-bit result.

### Operation

```
for (int i = 0; i < 16; i++) {
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
  dst.half[8] = ((s16)((s8)a.byte[17])) - ((s16)((s8)b.byte[16]));
  dst.half[9] = ((s16)((s8)a.byte[19])) - ((s16)((s8)b.byte[18]));
  dst.half[10] = ((s16)((s8)a.byte[21])) - ((s16)((s8)b.byte[20]));
  dst.half[11] = ((s16)((s8)a.byte[23])) - ((s16)((s8)b.byte[22]));
  dst.half[12] = ((s16)((s8)a.byte[25])) - ((s16)((s8)b.byte[24]));
  dst.half[13] = ((s16)((s8)a.byte[27])) - ((s16)((s8)b.byte[26]));
  dst.half[14] = ((s16)((s8)a.byte[29])) - ((s16)((s8)b.byte[28]));
  dst.half[15] = ((s16)((s8)a.byte[31])) - ((s16)((s8)b.byte[30]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvhsubw_hu_bu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvhsubw_hu_bu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvhsubw.hu.bu xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract odd-positioned unsigned 8-bit elements in `a` by even-positioned unsigned 8-bit elements in `b` to get 16-bit result.

### Operation

```
for (int i = 0; i < 16; i++) {
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
  dst.half[8] = ((u16)((u8)a.byte[17])) - ((u16)((u8)b.byte[16]));
  dst.half[9] = ((u16)((u8)a.byte[19])) - ((u16)((u8)b.byte[18]));
  dst.half[10] = ((u16)((u8)a.byte[21])) - ((u16)((u8)b.byte[20]));
  dst.half[11] = ((u16)((u8)a.byte[23])) - ((u16)((u8)b.byte[22]));
  dst.half[12] = ((u16)((u8)a.byte[25])) - ((u16)((u8)b.byte[24]));
  dst.half[13] = ((u16)((u8)a.byte[27])) - ((u16)((u8)b.byte[26]));
  dst.half[14] = ((u16)((u8)a.byte[29])) - ((u16)((u8)b.byte[28]));
  dst.half[15] = ((u16)((u8)a.byte[31])) - ((u16)((u8)b.byte[30]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvhsubw_w_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvhsubw_w_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvhsubw.w.h xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract odd-positioned signed 16-bit elements in `a` by even-positioned signed 16-bit elements in `b` to get 32-bit result.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (s32)(s16)a.half[2 * i + 1] - (s32)(s16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((s32)((s16)a.half[1])) - ((s32)((s16)b.half[0]));
  dst.word[1] = ((s32)((s16)a.half[3])) - ((s32)((s16)b.half[2]));
  dst.word[2] = ((s32)((s16)a.half[5])) - ((s32)((s16)b.half[4]));
  dst.word[3] = ((s32)((s16)a.half[7])) - ((s32)((s16)b.half[6]));
  dst.word[4] = ((s32)((s16)a.half[9])) - ((s32)((s16)b.half[8]));
  dst.word[5] = ((s32)((s16)a.half[11])) - ((s32)((s16)b.half[10]));
  dst.word[6] = ((s32)((s16)a.half[13])) - ((s32)((s16)b.half[12]));
  dst.word[7] = ((s32)((s16)a.half[15])) - ((s32)((s16)b.half[14]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvhsubw_wu_hu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvhsubw_wu_hu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvhsubw.wu.hu xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract odd-positioned unsigned 16-bit elements in `a` by even-positioned unsigned 16-bit elements in `b` to get 32-bit result.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i + 1] - (u32)(u16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[1])) - ((u32)((u16)b.half[0]));
  dst.word[1] = ((u32)((u16)a.half[3])) - ((u32)((u16)b.half[2]));
  dst.word[2] = ((u32)((u16)a.half[5])) - ((u32)((u16)b.half[4]));
  dst.word[3] = ((u32)((u16)a.half[7])) - ((u32)((u16)b.half[6]));
  dst.word[4] = ((u32)((u16)a.half[9])) - ((u32)((u16)b.half[8]));
  dst.word[5] = ((u32)((u16)a.half[11])) - ((u32)((u16)b.half[10]));
  dst.word[6] = ((u32)((u16)a.half[13])) - ((u32)((u16)b.half[12]));
  dst.word[7] = ((u32)((u16)a.half[15])) - ((u32)((u16)b.half[14]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvhsubw_d_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvhsubw_d_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvhsubw.d.w xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract odd-positioned signed 32-bit elements in `a` by even-positioned signed 32-bit elements in `b` to get 64-bit result.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (s64)(s32)a.word[2 * i + 1] - (s64)(s32)b.word[2 * i];
}

// Expands to:

if (0) {
  dst.dword[0] = ((s64)((s32)a.word[1])) - ((s64)((s32)b.word[0]));
  dst.dword[1] = ((s64)((s32)a.word[3])) - ((s64)((s32)b.word[2]));
  dst.dword[2] = ((s64)((s32)a.word[5])) - ((s64)((s32)b.word[4]));
  dst.dword[3] = ((s64)((s32)a.word[7])) - ((s64)((s32)b.word[6]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvhsubw_du_wu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvhsubw_du_wu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvhsubw.du.wu xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract odd-positioned unsigned 32-bit elements in `a` by even-positioned unsigned 32-bit elements in `b` to get 64-bit result.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 * i + 1] - (u64)(u32)b.word[2 * i];
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[1])) - ((u64)((u32)b.word[0]));
  dst.dword[1] = ((u64)((u32)a.word[3])) - ((u64)((u32)b.word[2]));
  dst.dword[2] = ((u64)((u32)a.word[5])) - ((u64)((u32)b.word[4]));
  dst.dword[3] = ((u64)((u32)a.word[7])) - ((u64)((u32)b.word[6]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvhsubw_q_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvhsubw_q_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvhsubw.q.d xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract odd-positioned signed 64-bit elements in `a` by even-positioned signed 64-bit elements in `b` to get 128-bit result.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] = (s128)(s64)a.dword[2 * i + 1] - (s128)(s64)b.dword[2 * i];
}

// Expands to:

if (0) {
  dst.qword[0] = ((s128)((s64)a.dword[1])) - ((s128)((s64)b.dword[0]));
  dst.qword[1] = ((s128)((s64)a.dword[3])) - ((s128)((s64)b.dword[2]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvhsubw_qu_du (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvhsubw_qu_du (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvhsubw.qu.du xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract odd-positioned unsigned 64-bit elements in `a` by even-positioned unsigned 64-bit elements in `b` to get 128-bit result.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 * i + 1] - (u128)(u64)b.dword[2 * i];
}

// Expands to:

if (0) {
  dst.qword[0] = ((u128)((u64)a.dword[1])) - ((u128)((u64)b.dword[0]));
  dst.qword[1] = ((u128)((u64)a.dword[3])) - ((u128)((u64)b.dword[2]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvmadd_b (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmadd_b (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmadd.b xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply 8-bit elements in `b` and `c`, add to elements in `a`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = b.byte[i] * c.byte[i] + a.byte[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmadd_h (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmadd_h (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmadd.h xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply 16-bit elements in `b` and `c`, add to elements in `a`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = b.half[i] * c.half[i] + a.half[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmadd_w (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmadd_w (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmadd.w xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply 32-bit elements in `b` and `c`, add to elements in `a`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = b.word[i] * c.word[i] + a.word[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmadd_d (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmadd_d (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmadd.d xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply 64-bit elements in `b` and `c`, add to elements in `a`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = b.dword[i] * c.dword[i] + a.dword[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmaddwev_h_b (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmaddwev_h_b (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmaddwev.h.b xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply even-positioned signed 8-bit elements in `b` and signed elements in `c`, add to 16-bit elements in `a`.

### Operation

```
for (int i = 0; i < 16; i++) {
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
  dst.half[8] =
      (((s16)((s8)b.byte[16])) * ((s16)((s8)c.byte[16]))) + ((s16)a.half[8]);
  dst.half[9] =
      (((s16)((s8)b.byte[18])) * ((s16)((s8)c.byte[18]))) + ((s16)a.half[9]);
  dst.half[10] =
      (((s16)((s8)b.byte[20])) * ((s16)((s8)c.byte[20]))) + ((s16)a.half[10]);
  dst.half[11] =
      (((s16)((s8)b.byte[22])) * ((s16)((s8)c.byte[22]))) + ((s16)a.half[11]);
  dst.half[12] =
      (((s16)((s8)b.byte[24])) * ((s16)((s8)c.byte[24]))) + ((s16)a.half[12]);
  dst.half[13] =
      (((s16)((s8)b.byte[26])) * ((s16)((s8)c.byte[26]))) + ((s16)a.half[13]);
  dst.half[14] =
      (((s16)((s8)b.byte[28])) * ((s16)((s8)c.byte[28]))) + ((s16)a.half[14]);
  dst.half[15] =
      (((s16)((s8)b.byte[30])) * ((s16)((s8)c.byte[30]))) + ((s16)a.half[15]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmaddwev_h_bu (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmaddwev_h_bu (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmaddwev.h.bu xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply even-positioned unsigned 8-bit elements in `b` and unsigned elements in `c`, add to 16-bit elements in `a`.

### Operation

```
for (int i = 0; i < 16; i++) {
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
  dst.half[8] =
      (((u16)((u8)b.byte[16])) * ((u16)((u8)c.byte[16]))) + ((u16)a.half[8]);
  dst.half[9] =
      (((u16)((u8)b.byte[18])) * ((u16)((u8)c.byte[18]))) + ((u16)a.half[9]);
  dst.half[10] =
      (((u16)((u8)b.byte[20])) * ((u16)((u8)c.byte[20]))) + ((u16)a.half[10]);
  dst.half[11] =
      (((u16)((u8)b.byte[22])) * ((u16)((u8)c.byte[22]))) + ((u16)a.half[11]);
  dst.half[12] =
      (((u16)((u8)b.byte[24])) * ((u16)((u8)c.byte[24]))) + ((u16)a.half[12]);
  dst.half[13] =
      (((u16)((u8)b.byte[26])) * ((u16)((u8)c.byte[26]))) + ((u16)a.half[13]);
  dst.half[14] =
      (((u16)((u8)b.byte[28])) * ((u16)((u8)c.byte[28]))) + ((u16)a.half[14]);
  dst.half[15] =
      (((u16)((u8)b.byte[30])) * ((u16)((u8)c.byte[30]))) + ((u16)a.half[15]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmaddwev_h_bu_b (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmaddwev_h_bu_b (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmaddwev.h.bu.b xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply even-positioned unsigned 8-bit elements in `b` and signed elements in `c`, add to 16-bit elements in `a`.

### Operation

```
for (int i = 0; i < 16; i++) {
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
  dst.half[8] =
      (((u16)((u8)b.byte[16])) * ((s16)((s8)c.byte[16]))) + ((s16)a.half[8]);
  dst.half[9] =
      (((u16)((u8)b.byte[18])) * ((s16)((s8)c.byte[18]))) + ((s16)a.half[9]);
  dst.half[10] =
      (((u16)((u8)b.byte[20])) * ((s16)((s8)c.byte[20]))) + ((s16)a.half[10]);
  dst.half[11] =
      (((u16)((u8)b.byte[22])) * ((s16)((s8)c.byte[22]))) + ((s16)a.half[11]);
  dst.half[12] =
      (((u16)((u8)b.byte[24])) * ((s16)((s8)c.byte[24]))) + ((s16)a.half[12]);
  dst.half[13] =
      (((u16)((u8)b.byte[26])) * ((s16)((s8)c.byte[26]))) + ((s16)a.half[13]);
  dst.half[14] =
      (((u16)((u8)b.byte[28])) * ((s16)((s8)c.byte[28]))) + ((s16)a.half[14]);
  dst.half[15] =
      (((u16)((u8)b.byte[30])) * ((s16)((s8)c.byte[30]))) + ((s16)a.half[15]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmaddwev_w_h (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmaddwev_w_h (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmaddwev.w.h xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply even-positioned signed 16-bit elements in `b` and signed elements in `c`, add to 32-bit elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
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
  dst.word[4] =
      (((s32)((s16)b.half[8])) * ((s32)((s16)c.half[8]))) + ((s32)a.word[4]);
  dst.word[5] =
      (((s32)((s16)b.half[10])) * ((s32)((s16)c.half[10]))) + ((s32)a.word[5]);
  dst.word[6] =
      (((s32)((s16)b.half[12])) * ((s32)((s16)c.half[12]))) + ((s32)a.word[6]);
  dst.word[7] =
      (((s32)((s16)b.half[14])) * ((s32)((s16)c.half[14]))) + ((s32)a.word[7]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmaddwev_w_hu (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmaddwev_w_hu (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmaddwev.w.hu xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply even-positioned unsigned 16-bit elements in `b` and unsigned elements in `c`, add to 32-bit elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
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
  dst.word[4] =
      (((u32)((u16)b.half[8])) * ((u32)((u16)c.half[8]))) + ((u32)a.word[4]);
  dst.word[5] =
      (((u32)((u16)b.half[10])) * ((u32)((u16)c.half[10]))) + ((u32)a.word[5]);
  dst.word[6] =
      (((u32)((u16)b.half[12])) * ((u32)((u16)c.half[12]))) + ((u32)a.word[6]);
  dst.word[7] =
      (((u32)((u16)b.half[14])) * ((u32)((u16)c.half[14]))) + ((u32)a.word[7]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmaddwev_w_hu_h (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmaddwev_w_hu_h (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmaddwev.w.hu.h xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply even-positioned unsigned 16-bit elements in `b` and signed elements in `c`, add to 32-bit elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
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
  dst.word[4] =
      (((u32)((u16)b.half[8])) * ((s32)((s16)c.half[8]))) + ((s32)a.word[4]);
  dst.word[5] =
      (((u32)((u16)b.half[10])) * ((s32)((s16)c.half[10]))) + ((s32)a.word[5]);
  dst.word[6] =
      (((u32)((u16)b.half[12])) * ((s32)((s16)c.half[12]))) + ((s32)a.word[6]);
  dst.word[7] =
      (((u32)((u16)b.half[14])) * ((s32)((s16)c.half[14]))) + ((s32)a.word[7]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmaddwev_d_w (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmaddwev_d_w (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmaddwev.d.w xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply even-positioned signed 32-bit elements in `b` and signed elements in `c`, add to 64-bit elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] =
      (s64)(s32)b.word[2 * i] * (s64)(s32)c.word[2 * i] + (s64)a.dword[i];
}

// Expands to:

if (0) {
  dst.dword[0] =
      (((s64)((s32)b.word[0])) * ((s64)((s32)c.word[0]))) + ((s64)a.dword[0]);
  dst.dword[1] =
      (((s64)((s32)b.word[2])) * ((s64)((s32)c.word[2]))) + ((s64)a.dword[1]);
  dst.dword[2] =
      (((s64)((s32)b.word[4])) * ((s64)((s32)c.word[4]))) + ((s64)a.dword[2]);
  dst.dword[3] =
      (((s64)((s32)b.word[6])) * ((s64)((s32)c.word[6]))) + ((s64)a.dword[3]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmaddwev_d_wu (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmaddwev_d_wu (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmaddwev.d.wu xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply even-positioned unsigned 32-bit elements in `b` and unsigned elements in `c`, add to 64-bit elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] =
      (u64)(u32)b.word[2 * i] * (u64)(u32)c.word[2 * i] + (u64)a.dword[i];
}

// Expands to:

if (0) {
  dst.dword[0] =
      (((u64)((u32)b.word[0])) * ((u64)((u32)c.word[0]))) + ((u64)a.dword[0]);
  dst.dword[1] =
      (((u64)((u32)b.word[2])) * ((u64)((u32)c.word[2]))) + ((u64)a.dword[1]);
  dst.dword[2] =
      (((u64)((u32)b.word[4])) * ((u64)((u32)c.word[4]))) + ((u64)a.dword[2]);
  dst.dword[3] =
      (((u64)((u32)b.word[6])) * ((u64)((u32)c.word[6]))) + ((u64)a.dword[3]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmaddwev_d_wu_w (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmaddwev_d_wu_w (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmaddwev.d.wu.w xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply even-positioned unsigned 32-bit elements in `b` and signed elements in `c`, add to 64-bit elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] =
      (u64)(u32)b.word[2 * i] * (s64)(s32)c.word[2 * i] + (s64)a.dword[i];
}

// Expands to:

if (0) {
  dst.dword[0] =
      (((u64)((u32)b.word[0])) * ((s64)((s32)c.word[0]))) + ((s64)a.dword[0]);
  dst.dword[1] =
      (((u64)((u32)b.word[2])) * ((s64)((s32)c.word[2]))) + ((s64)a.dword[1]);
  dst.dword[2] =
      (((u64)((u32)b.word[4])) * ((s64)((s32)c.word[4]))) + ((s64)a.dword[2]);
  dst.dword[3] =
      (((u64)((u32)b.word[6])) * ((s64)((s32)c.word[6]))) + ((s64)a.dword[3]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmaddwev_q_d (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmaddwev_q_d (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmaddwev.q.d xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply even-positioned signed 64-bit elements in `b` and signed elements in `c`, add to 128-bit elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] =
      (s128)(s64)b.dword[2 * i] * (s128)(s64)c.dword[2 * i] + (s128)a.qword[i];
}

// Expands to:

if (0) {
  dst.qword[0] = (((s128)((s64)b.dword[0])) * ((s128)((s64)c.dword[0]))) +
                 ((s128)a.qword[0]);
  dst.qword[1] = (((s128)((s64)b.dword[2])) * ((s128)((s64)c.dword[2]))) +
                 ((s128)a.qword[1]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 7       | 1.14             |
| 3A6000 | LA664 | 7       | 1.14             |
| 3C6000 | LA664 | 7       | 1.14             |

## \_\_m256i \_\_lasx_xvmaddwev_q_du (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmaddwev_q_du (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmaddwev.q.du xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply even-positioned unsigned 64-bit elements in `b` and unsigned elements in `c`, add to 128-bit elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] =
      (u128)(u64)b.dword[2 * i] * (u128)(u64)c.dword[2 * i] + (u128)a.qword[i];
}

// Expands to:

if (0) {
  dst.qword[0] = (((u128)((u64)b.dword[0])) * ((u128)((u64)c.dword[0]))) +
                 ((u128)a.qword[0]);
  dst.qword[1] = (((u128)((u64)b.dword[2])) * ((u128)((u64)c.dword[2]))) +
                 ((u128)a.qword[1]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 7       | 1.14             |
| 3A6000 | LA664 | 7       | 1.14             |
| 3C6000 | LA664 | 7       | 1.14             |

## \_\_m256i \_\_lasx_xvmaddwev_q_du_d (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmaddwev_q_du_d (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmaddwev.q.du.d xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply even-positioned unsigned 64-bit elements in `b` and signed elements in `c`, add to 128-bit elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] =
      (u128)(u64)b.dword[2 * i] * (s128)(s64)c.dword[2 * i] + (s128)a.qword[i];
}

// Expands to:

if (0) {
  dst.qword[0] = (((u128)((u64)b.dword[0])) * ((s128)((s64)c.dword[0]))) +
                 ((s128)a.qword[0]);
  dst.qword[1] = (((u128)((u64)b.dword[2])) * ((s128)((s64)c.dword[2]))) +
                 ((s128)a.qword[1]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 7       | 1.14             |
| 3A6000 | LA664 | 7       | 1.14             |
| 3C6000 | LA664 | 7       | 1.14             |

## \_\_m256i \_\_lasx_xvmaddwod_h_b (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmaddwod_h_b (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmaddwod.h.b xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply odd-positioned signed 8-bit elements in `b` and signed elements in `c`, add to 16-bit elements in `a`.

### Operation

```
for (int i = 0; i < 16; i++) {
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
  dst.half[8] =
      (((s16)((s8)b.byte[17])) * ((s16)((s8)c.byte[17]))) + ((s16)a.half[8]);
  dst.half[9] =
      (((s16)((s8)b.byte[19])) * ((s16)((s8)c.byte[19]))) + ((s16)a.half[9]);
  dst.half[10] =
      (((s16)((s8)b.byte[21])) * ((s16)((s8)c.byte[21]))) + ((s16)a.half[10]);
  dst.half[11] =
      (((s16)((s8)b.byte[23])) * ((s16)((s8)c.byte[23]))) + ((s16)a.half[11]);
  dst.half[12] =
      (((s16)((s8)b.byte[25])) * ((s16)((s8)c.byte[25]))) + ((s16)a.half[12]);
  dst.half[13] =
      (((s16)((s8)b.byte[27])) * ((s16)((s8)c.byte[27]))) + ((s16)a.half[13]);
  dst.half[14] =
      (((s16)((s8)b.byte[29])) * ((s16)((s8)c.byte[29]))) + ((s16)a.half[14]);
  dst.half[15] =
      (((s16)((s8)b.byte[31])) * ((s16)((s8)c.byte[31]))) + ((s16)a.half[15]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmaddwod_h_bu (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmaddwod_h_bu (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmaddwod.h.bu xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply odd-positioned unsigned 8-bit elements in `b` and unsigned elements in `c`, add to 16-bit elements in `a`.

### Operation

```
for (int i = 0; i < 16; i++) {
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
  dst.half[8] =
      (((u16)((u8)b.byte[17])) * ((u16)((u8)c.byte[17]))) + ((u16)a.half[8]);
  dst.half[9] =
      (((u16)((u8)b.byte[19])) * ((u16)((u8)c.byte[19]))) + ((u16)a.half[9]);
  dst.half[10] =
      (((u16)((u8)b.byte[21])) * ((u16)((u8)c.byte[21]))) + ((u16)a.half[10]);
  dst.half[11] =
      (((u16)((u8)b.byte[23])) * ((u16)((u8)c.byte[23]))) + ((u16)a.half[11]);
  dst.half[12] =
      (((u16)((u8)b.byte[25])) * ((u16)((u8)c.byte[25]))) + ((u16)a.half[12]);
  dst.half[13] =
      (((u16)((u8)b.byte[27])) * ((u16)((u8)c.byte[27]))) + ((u16)a.half[13]);
  dst.half[14] =
      (((u16)((u8)b.byte[29])) * ((u16)((u8)c.byte[29]))) + ((u16)a.half[14]);
  dst.half[15] =
      (((u16)((u8)b.byte[31])) * ((u16)((u8)c.byte[31]))) + ((u16)a.half[15]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmaddwod_h_bu_b (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmaddwod_h_bu_b (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmaddwod.h.bu.b xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply odd-positioned unsigned 8-bit elements in `b` and signed elements in `c`, add to 16-bit elements in `a`.

### Operation

```
for (int i = 0; i < 16; i++) {
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
  dst.half[8] =
      (((u16)((u8)b.byte[17])) * ((s16)((s8)c.byte[17]))) + ((s16)a.half[8]);
  dst.half[9] =
      (((u16)((u8)b.byte[19])) * ((s16)((s8)c.byte[19]))) + ((s16)a.half[9]);
  dst.half[10] =
      (((u16)((u8)b.byte[21])) * ((s16)((s8)c.byte[21]))) + ((s16)a.half[10]);
  dst.half[11] =
      (((u16)((u8)b.byte[23])) * ((s16)((s8)c.byte[23]))) + ((s16)a.half[11]);
  dst.half[12] =
      (((u16)((u8)b.byte[25])) * ((s16)((s8)c.byte[25]))) + ((s16)a.half[12]);
  dst.half[13] =
      (((u16)((u8)b.byte[27])) * ((s16)((s8)c.byte[27]))) + ((s16)a.half[13]);
  dst.half[14] =
      (((u16)((u8)b.byte[29])) * ((s16)((s8)c.byte[29]))) + ((s16)a.half[14]);
  dst.half[15] =
      (((u16)((u8)b.byte[31])) * ((s16)((s8)c.byte[31]))) + ((s16)a.half[15]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmaddwod_w_h (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmaddwod_w_h (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmaddwod.w.h xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply odd-positioned signed 16-bit elements in `b` and signed elements in `c`, add to 32-bit elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
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
  dst.word[4] =
      (((s32)((s16)b.half[9])) * ((s32)((s16)c.half[9]))) + ((s32)a.word[4]);
  dst.word[5] =
      (((s32)((s16)b.half[11])) * ((s32)((s16)c.half[11]))) + ((s32)a.word[5]);
  dst.word[6] =
      (((s32)((s16)b.half[13])) * ((s32)((s16)c.half[13]))) + ((s32)a.word[6]);
  dst.word[7] =
      (((s32)((s16)b.half[15])) * ((s32)((s16)c.half[15]))) + ((s32)a.word[7]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmaddwod_w_hu (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmaddwod_w_hu (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmaddwod.w.hu xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply odd-positioned unsigned 16-bit elements in `b` and unsigned elements in `c`, add to 32-bit elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
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
  dst.word[4] =
      (((u32)((u16)b.half[9])) * ((u32)((u16)c.half[9]))) + ((u32)a.word[4]);
  dst.word[5] =
      (((u32)((u16)b.half[11])) * ((u32)((u16)c.half[11]))) + ((u32)a.word[5]);
  dst.word[6] =
      (((u32)((u16)b.half[13])) * ((u32)((u16)c.half[13]))) + ((u32)a.word[6]);
  dst.word[7] =
      (((u32)((u16)b.half[15])) * ((u32)((u16)c.half[15]))) + ((u32)a.word[7]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmaddwod_w_hu_h (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmaddwod_w_hu_h (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmaddwod.w.hu.h xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply odd-positioned unsigned 16-bit elements in `b` and signed elements in `c`, add to 32-bit elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
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
  dst.word[4] =
      (((u32)((u16)b.half[9])) * ((s32)((s16)c.half[9]))) + ((s32)a.word[4]);
  dst.word[5] =
      (((u32)((u16)b.half[11])) * ((s32)((s16)c.half[11]))) + ((s32)a.word[5]);
  dst.word[6] =
      (((u32)((u16)b.half[13])) * ((s32)((s16)c.half[13]))) + ((s32)a.word[6]);
  dst.word[7] =
      (((u32)((u16)b.half[15])) * ((s32)((s16)c.half[15]))) + ((s32)a.word[7]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmaddwod_d_w (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmaddwod_d_w (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmaddwod.d.w xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply odd-positioned signed 32-bit elements in `b` and signed elements in `c`, add to 64-bit elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (s64)(s32)b.word[2 * i + 1] * (s64)(s32)c.word[2 * i + 1] +
                 (s64)a.dword[i];
}

// Expands to:

if (0) {
  dst.dword[0] =
      (((s64)((s32)b.word[1])) * ((s64)((s32)c.word[1]))) + ((s64)a.dword[0]);
  dst.dword[1] =
      (((s64)((s32)b.word[3])) * ((s64)((s32)c.word[3]))) + ((s64)a.dword[1]);
  dst.dword[2] =
      (((s64)((s32)b.word[5])) * ((s64)((s32)c.word[5]))) + ((s64)a.dword[2]);
  dst.dword[3] =
      (((s64)((s32)b.word[7])) * ((s64)((s32)c.word[7]))) + ((s64)a.dword[3]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmaddwod_d_wu (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmaddwod_d_wu (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmaddwod.d.wu xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply odd-positioned unsigned 32-bit elements in `b` and unsigned elements in `c`, add to 64-bit elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (u64)(u32)b.word[2 * i + 1] * (u64)(u32)c.word[2 * i + 1] +
                 (u64)a.dword[i];
}

// Expands to:

if (0) {
  dst.dword[0] =
      (((u64)((u32)b.word[1])) * ((u64)((u32)c.word[1]))) + ((u64)a.dword[0]);
  dst.dword[1] =
      (((u64)((u32)b.word[3])) * ((u64)((u32)c.word[3]))) + ((u64)a.dword[1]);
  dst.dword[2] =
      (((u64)((u32)b.word[5])) * ((u64)((u32)c.word[5]))) + ((u64)a.dword[2]);
  dst.dword[3] =
      (((u64)((u32)b.word[7])) * ((u64)((u32)c.word[7]))) + ((u64)a.dword[3]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmaddwod_d_wu_w (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmaddwod_d_wu_w (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmaddwod.d.wu.w xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply odd-positioned unsigned 32-bit elements in `b` and signed elements in `c`, add to 64-bit elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (u64)(u32)b.word[2 * i + 1] * (s64)(s32)c.word[2 * i + 1] +
                 (s64)a.dword[i];
}

// Expands to:

if (0) {
  dst.dword[0] =
      (((u64)((u32)b.word[1])) * ((s64)((s32)c.word[1]))) + ((s64)a.dword[0]);
  dst.dword[1] =
      (((u64)((u32)b.word[3])) * ((s64)((s32)c.word[3]))) + ((s64)a.dword[1]);
  dst.dword[2] =
      (((u64)((u32)b.word[5])) * ((s64)((s32)c.word[5]))) + ((s64)a.dword[2]);
  dst.dword[3] =
      (((u64)((u32)b.word[7])) * ((s64)((s32)c.word[7]))) + ((s64)a.dword[3]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmaddwod_q_d (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmaddwod_q_d (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmaddwod.q.d xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply odd-positioned signed 64-bit elements in `b` and signed elements in `c`, add to 128-bit elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] = (s128)(s64)b.dword[2 * i + 1] * (s128)(s64)c.dword[2 * i + 1] +
                 (s128)a.qword[i];
}

// Expands to:

if (0) {
  dst.qword[0] = (((s128)((s64)b.dword[1])) * ((s128)((s64)c.dword[1]))) +
                 ((s128)a.qword[0]);
  dst.qword[1] = (((s128)((s64)b.dword[3])) * ((s128)((s64)c.dword[3]))) +
                 ((s128)a.qword[1]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 7       | 1.14             |
| 3A6000 | LA664 | 7       | 1.14             |
| 3C6000 | LA664 | 7       | 1.14             |

## \_\_m256i \_\_lasx_xvmaddwod_q_du (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmaddwod_q_du (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmaddwod.q.du xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply odd-positioned unsigned 64-bit elements in `b` and unsigned elements in `c`, add to 128-bit elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] = (u128)(u64)b.dword[2 * i + 1] * (u128)(u64)c.dword[2 * i + 1] +
                 (u128)a.qword[i];
}

// Expands to:

if (0) {
  dst.qword[0] = (((u128)((u64)b.dword[1])) * ((u128)((u64)c.dword[1]))) +
                 ((u128)a.qword[0]);
  dst.qword[1] = (((u128)((u64)b.dword[3])) * ((u128)((u64)c.dword[3]))) +
                 ((u128)a.qword[1]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 7       | 1.14             |
| 3A6000 | LA664 | 7       | 1.14             |
| 3C6000 | LA664 | 7       | 1.14             |

## \_\_m256i \_\_lasx_xvmaddwod_q_du_d (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmaddwod_q_du_d (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmaddwod.q.du.d xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply odd-positioned unsigned 64-bit elements in `b` and signed elements in `c`, add to 128-bit elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] = (u128)(u64)b.dword[2 * i + 1] * (s128)(s64)c.dword[2 * i + 1] +
                 (s128)a.qword[i];
}

// Expands to:

if (0) {
  dst.qword[0] = (((u128)((u64)b.dword[1])) * ((s128)((s64)c.dword[1]))) +
                 ((s128)a.qword[0]);
  dst.qword[1] = (((u128)((u64)b.dword[3])) * ((s128)((s64)c.dword[3]))) +
                 ((s128)a.qword[1]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 7       | 1.14             |
| 3A6000 | LA664 | 7       | 1.14             |
| 3C6000 | LA664 | 7       | 1.14             |

## \_\_m256i \_\_lasx_xvmax_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmax_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmax.b xr, xr, xr
CPU Flags: LASX
```

### Description

Compute elementwise maximum for signed 8-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = max((s8)a.byte[i], (s8)b.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvmax_bu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmax_bu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmax.bu xr, xr, xr
CPU Flags: LASX
```

### Description

Compute elementwise maximum for unsigned 8-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = max((u8)a.byte[i], (u8)b.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvmax_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmax_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmax.h xr, xr, xr
CPU Flags: LASX
```

### Description

Compute elementwise maximum for signed 16-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = max((s16)a.half[i], (s16)b.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvmax_hu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmax_hu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmax.hu xr, xr, xr
CPU Flags: LASX
```

### Description

Compute elementwise maximum for unsigned 16-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = max((u16)a.half[i], (u16)b.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvmax_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmax_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmax.w xr, xr, xr
CPU Flags: LASX
```

### Description

Compute elementwise maximum for signed 32-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = max((s32)a.word[i], (s32)b.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvmax_wu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmax_wu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmax.wu xr, xr, xr
CPU Flags: LASX
```

### Description

Compute elementwise maximum for unsigned 32-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = max((u32)a.word[i], (u32)b.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvmax_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmax_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmax.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compute elementwise maximum for signed 64-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = max((s64)a.dword[i], (s64)b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvmax_du (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmax_du (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmax.du xr, xr, xr
CPU Flags: LASX
```

### Description

Compute elementwise maximum for unsigned 64-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = max((u64)a.dword[i], (u64)b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvmaxi_b (\_\_m256i a, imm_n16_15 imm)

### Synopsis

```
__m256i __lasx_xvmaxi_b (__m256i a, imm_n16_15 imm)
#include <lasxintrin.h>
Instruction: xvmaxi.b xr, xr, imm
CPU Flags: LASX
```

### Description

Compute elementwise maximum for signed 8-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = max((s8)a.byte[i], (s8)imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvmaxi_bu (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvmaxi_bu (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvmaxi.bu xr, xr, imm
CPU Flags: LASX
```

### Description

Compute elementwise maximum for unsigned 8-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = max((u8)a.byte[i], (u8)imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvmaxi_h (\_\_m256i a, imm_n16_15 imm)

### Synopsis

```
__m256i __lasx_xvmaxi_h (__m256i a, imm_n16_15 imm)
#include <lasxintrin.h>
Instruction: xvmaxi.h xr, xr, imm
CPU Flags: LASX
```

### Description

Compute elementwise maximum for signed 16-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = max((s16)a.half[i], (s16)imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvmaxi_hu (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvmaxi_hu (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvmaxi.hu xr, xr, imm
CPU Flags: LASX
```

### Description

Compute elementwise maximum for unsigned 16-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = max((u16)a.half[i], (u16)imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvmaxi_w (\_\_m256i a, imm_n16_15 imm)

### Synopsis

```
__m256i __lasx_xvmaxi_w (__m256i a, imm_n16_15 imm)
#include <lasxintrin.h>
Instruction: xvmaxi.w xr, xr, imm
CPU Flags: LASX
```

### Description

Compute elementwise maximum for signed 32-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = max((s32)a.word[i], (s32)imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvmaxi_wu (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvmaxi_wu (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvmaxi.wu xr, xr, imm
CPU Flags: LASX
```

### Description

Compute elementwise maximum for unsigned 32-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = max((u32)a.word[i], (u32)imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvmaxi_d (\_\_m256i a, imm_n16_15 imm)

### Synopsis

```
__m256i __lasx_xvmaxi_d (__m256i a, imm_n16_15 imm)
#include <lasxintrin.h>
Instruction: xvmaxi.d xr, xr, imm
CPU Flags: LASX
```

### Description

Compute elementwise maximum for signed 64-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = max((s64)a.dword[i], (s64)imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvmaxi_du (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvmaxi_du (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvmaxi.du xr, xr, imm
CPU Flags: LASX
```

### Description

Compute elementwise maximum for unsigned 64-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = max((u64)a.dword[i], (u64)imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvmin_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmin_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmin.b xr, xr, xr
CPU Flags: LASX
```

### Description

Compute elementwise minimum for signed 8-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = min((s8)a.byte[i], (s8)b.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvmin_bu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmin_bu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmin.bu xr, xr, xr
CPU Flags: LASX
```

### Description

Compute elementwise minimum for unsigned 8-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = min((u8)a.byte[i], (u8)b.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvmin_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmin_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmin.h xr, xr, xr
CPU Flags: LASX
```

### Description

Compute elementwise minimum for signed 16-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = min((s16)a.half[i], (s16)b.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvmin_hu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmin_hu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmin.hu xr, xr, xr
CPU Flags: LASX
```

### Description

Compute elementwise minimum for unsigned 16-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = min((u16)a.half[i], (u16)b.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvmin_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmin_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmin.w xr, xr, xr
CPU Flags: LASX
```

### Description

Compute elementwise minimum for signed 32-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = min((s32)a.word[i], (s32)b.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvmin_wu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmin_wu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmin.wu xr, xr, xr
CPU Flags: LASX
```

### Description

Compute elementwise minimum for unsigned 32-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = min((u32)a.word[i], (u32)b.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvmin_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmin_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmin.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compute elementwise minimum for signed 64-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = min((s64)a.dword[i], (s64)b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvmin_du (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmin_du (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmin.du xr, xr, xr
CPU Flags: LASX
```

### Description

Compute elementwise minimum for unsigned 64-bit elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = min((u64)a.dword[i], (u64)b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvmini_b (\_\_m256i a, imm_n16_15 imm)

### Synopsis

```
__m256i __lasx_xvmini_b (__m256i a, imm_n16_15 imm)
#include <lasxintrin.h>
Instruction: xvmini.b xr, xr, imm
CPU Flags: LASX
```

### Description

Compute elementwise minimum for signed 8-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = min((s8)a.byte[i], (s8)imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvmini_bu (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvmini_bu (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvmini.bu xr, xr, imm
CPU Flags: LASX
```

### Description

Compute elementwise minimum for unsigned 8-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = min((u8)a.byte[i], (u8)imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvmini_h (\_\_m256i a, imm_n16_15 imm)

### Synopsis

```
__m256i __lasx_xvmini_h (__m256i a, imm_n16_15 imm)
#include <lasxintrin.h>
Instruction: xvmini.h xr, xr, imm
CPU Flags: LASX
```

### Description

Compute elementwise minimum for signed 16-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = min((s16)a.half[i], (s16)imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvmini_hu (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvmini_hu (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvmini.hu xr, xr, imm
CPU Flags: LASX
```

### Description

Compute elementwise minimum for unsigned 16-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = min((u16)a.half[i], (u16)imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvmini_w (\_\_m256i a, imm_n16_15 imm)

### Synopsis

```
__m256i __lasx_xvmini_w (__m256i a, imm_n16_15 imm)
#include <lasxintrin.h>
Instruction: xvmini.w xr, xr, imm
CPU Flags: LASX
```

### Description

Compute elementwise minimum for signed 32-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = min((s32)a.word[i], (s32)imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvmini_wu (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvmini_wu (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvmini.wu xr, xr, imm
CPU Flags: LASX
```

### Description

Compute elementwise minimum for unsigned 32-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = min((u32)a.word[i], (u32)imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvmini_d (\_\_m256i a, imm_n16_15 imm)

### Synopsis

```
__m256i __lasx_xvmini_d (__m256i a, imm_n16_15 imm)
#include <lasxintrin.h>
Instruction: xvmini.d xr, xr, imm
CPU Flags: LASX
```

### Description

Compute elementwise minimum for signed 64-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = min((s64)a.dword[i], (s64)imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvmini_du (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvmini_du (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvmini.du xr, xr, imm
CPU Flags: LASX
```

### Description

Compute elementwise minimum for unsigned 64-bit elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = min((u64)a.dword[i], (u64)imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvmod_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmod_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmod.b xr, xr, xr
CPU Flags: LASX
```

### Description

Modulo residual signed 8-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = (b.byte[i] == 0) ? 0 : ((s8)a.byte[i] % (s8)b.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 29, 33  | 0.05(1/21.5)     |
| 3A6000 | LA664 | 29, 41  | 0.06(1/15.5)     |
| 3C6000 | LA664 | 29      | 0.07(1/13.5)     |

## \_\_m256i \_\_lasx_xvmod_bu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmod_bu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmod.bu xr, xr, xr
CPU Flags: LASX
```

### Description

Modulo residual unsigned 8-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = (b.byte[i] == 0) ? 0 : ((u8)a.byte[i] % (u8)b.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 29, 37  | 0.05(1/22)       |
| 3A6000 | LA664 | 29, 37  | 0.06(1/17.5)     |
| 3C6000 | LA664 | 29      | 0.07(1/13.5)     |

## \_\_m256i \_\_lasx_xvmod_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmod_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmod.h xr, xr, xr
CPU Flags: LASX
```

### Description

Modulo residual signed 16-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = (b.half[i] == 0) ? 0 : ((s16)a.half[i] % (s16)b.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 17, 21  | 0.07(1/13.5)     |
| 3A6000 | LA664 | 17, 21  | 0.12(1/8.5)      |
| 3C6000 | LA664 | 17      | 0.13(1/7.5)      |

## \_\_m256i \_\_lasx_xvmod_hu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmod_hu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmod.hu xr, xr, xr
CPU Flags: LASX
```

### Description

Modulo residual unsigned 16-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = (b.half[i] == 0) ? 0 : ((u16)a.half[i] % (u16)b.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 17, 23  | 0.06(1/16)       |
| 3A6000 | LA664 | 17, 25  | 0.11(1/9.5)      |
| 3C6000 | LA664 | 17      | 0.13(1/7.5)      |

## \_\_m256i \_\_lasx_xvmod_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmod_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmod.w xr, xr, xr
CPU Flags: LASX
```

### Description

Modulo residual signed 32-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (b.word[i] == 0) ? 0 : ((s32)a.word[i] % (s32)b.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 11, 15  | 0.07(1/13.5)     |
| 3A6000 | LA664 | 11, 13  | 0.18(1/5.5)      |
| 3C6000 | LA664 | 11      | 0.22(1/4.5)      |

## \_\_m256i \_\_lasx_xvmod_wu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmod_wu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmod.wu xr, xr, xr
CPU Flags: LASX
```

### Description

Modulo residual unsigned 32-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (b.word[i] == 0) ? 0 : ((u32)a.word[i] % (u32)b.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 11, 15  | 0.06(1/16)       |
| 3A6000 | LA664 | 11, 13  | 0.18(1/5.5)      |
| 3C6000 | LA664 | 11      | 0.22(1/4.5)      |

## \_\_m256i \_\_lasx_xvmod_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmod_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmod.d xr, xr, xr
CPU Flags: LASX
```

### Description

Modulo residual signed 64-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (b.dword[i] == 0) ? 0 : ((s64)a.dword[i] % (s64)b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 8, 10   | 0.11(1/9.5)      |
| 3A6000 | LA664 | 8, 10   | 0.25(1/4)        |
| 3C6000 | LA664 | 8       | 0.33(1/3)        |

## \_\_m256i \_\_lasx_xvmod_du (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmod_du (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmod.du xr, xr, xr
CPU Flags: LASX
```

### Description

Modulo residual unsigned 64-bit elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (b.dword[i] == 0) ? 0 : ((u64)a.dword[i] % (u64)b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 8, 10   | 0.11(1/9.5)      |
| 3A6000 | LA664 | 8, 10   | 0.25(1/4)        |
| 3C6000 | LA664 | 8       | 0.33(1/3)        |

## \_\_m256i \_\_lasx_xvmsub_b (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmsub_b (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmsub.b xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply 8-bit elements in `b` and `c`, negate and add elements in `a`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = -b.byte[i] * c.byte[i] + a.byte[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmsub_h (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmsub_h (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmsub.h xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply 16-bit elements in `b` and `c`, negate and add elements in `a`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = -b.half[i] * c.half[i] + a.half[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmsub_w (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmsub_w (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmsub.w xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply 32-bit elements in `b` and `c`, negate and add elements in `a`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = -b.word[i] * c.word[i] + a.word[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmsub_d (\_\_m256i a, \_\_m256i b, \_\_m256i c)

### Synopsis

```
__m256i __lasx_xvmsub_d (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmsub.d xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply 64-bit elements in `b` and `c`, negate and add elements in `a`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = -b.dword[i] * c.dword[i] + a.dword[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmuh_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmuh_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmuh.b xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply signed 8-bit elements in `a` and `b`, save the high 8-bit result in `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = (((s16)(s8)a.byte[i] * (s16)(s8)b.byte[i])) >> 8;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmuh_bu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmuh_bu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmuh.bu xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply unsigned 8-bit elements in `a` and `b`, save the high 8-bit result in `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = (((u16)(u8)a.byte[i] * (u16)(u8)b.byte[i])) >> 8;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmuh_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmuh_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmuh.h xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply signed 16-bit elements in `a` and `b`, save the high 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = (((s32)(s16)a.half[i] * (s32)(s16)b.half[i])) >> 16;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmuh_hu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmuh_hu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmuh.hu xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply unsigned 16-bit elements in `a` and `b`, save the high 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = (((u32)(u16)a.half[i] * (u32)(u16)b.half[i])) >> 16;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmuh_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmuh_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmuh.w xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply signed 32-bit elements in `a` and `b`, save the high 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (((s64)(s32)a.word[i] * (s64)(s32)b.word[i])) >> 32;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmuh_wu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmuh_wu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmuh.wu xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply unsigned 32-bit elements in `a` and `b`, save the high 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (((u64)(u32)a.word[i] * (u64)(u32)b.word[i])) >> 32;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmuh_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmuh_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmuh.d xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply signed 64-bit elements in `a` and `b`, save the high 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (((s128)(s64)a.dword[i] * (s128)(s64)b.dword[i])) >> 64;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmuh_du (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmuh_du (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmuh.du xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply unsigned 64-bit elements in `a` and `b`, save the high 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (((u128)(u64)a.dword[i] * (u128)(u64)b.dword[i])) >> 64;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmul_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmul_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmul.b xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply 8-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[i] * b.byte[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmul_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmul_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmul.h xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply 16-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = a.half[i] * b.half[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmul_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmul_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmul.w xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply 32-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = a.word[i] * b.word[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmul_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmul_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmul.d xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply 64-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = a.dword[i] * b.dword[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmulwev_h_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmulwev_h_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmulwev.h.b xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply even-positioned signed 8-bit elements in `a` and signed elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
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
  dst.half[8] = ((s16)((s8)a.byte[16])) * ((s16)((s8)b.byte[16]));
  dst.half[9] = ((s16)((s8)a.byte[18])) * ((s16)((s8)b.byte[18]));
  dst.half[10] = ((s16)((s8)a.byte[20])) * ((s16)((s8)b.byte[20]));
  dst.half[11] = ((s16)((s8)a.byte[22])) * ((s16)((s8)b.byte[22]));
  dst.half[12] = ((s16)((s8)a.byte[24])) * ((s16)((s8)b.byte[24]));
  dst.half[13] = ((s16)((s8)a.byte[26])) * ((s16)((s8)b.byte[26]));
  dst.half[14] = ((s16)((s8)a.byte[28])) * ((s16)((s8)b.byte[28]));
  dst.half[15] = ((s16)((s8)a.byte[30])) * ((s16)((s8)b.byte[30]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmulwev_h_bu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmulwev_h_bu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmulwev.h.bu xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply even-positioned unsigned 8-bit elements in `a` and unsigned elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
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
  dst.half[8] = ((u16)((u8)a.byte[16])) * ((u16)((u8)b.byte[16]));
  dst.half[9] = ((u16)((u8)a.byte[18])) * ((u16)((u8)b.byte[18]));
  dst.half[10] = ((u16)((u8)a.byte[20])) * ((u16)((u8)b.byte[20]));
  dst.half[11] = ((u16)((u8)a.byte[22])) * ((u16)((u8)b.byte[22]));
  dst.half[12] = ((u16)((u8)a.byte[24])) * ((u16)((u8)b.byte[24]));
  dst.half[13] = ((u16)((u8)a.byte[26])) * ((u16)((u8)b.byte[26]));
  dst.half[14] = ((u16)((u8)a.byte[28])) * ((u16)((u8)b.byte[28]));
  dst.half[15] = ((u16)((u8)a.byte[30])) * ((u16)((u8)b.byte[30]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmulwev_h_bu_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmulwev_h_bu_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmulwev.h.bu.b xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply even-positioned unsigned 8-bit elements in `a` and signed elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
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
  dst.half[8] = ((u16)((u8)a.byte[16])) * ((s16)((s8)b.byte[16]));
  dst.half[9] = ((u16)((u8)a.byte[18])) * ((s16)((s8)b.byte[18]));
  dst.half[10] = ((u16)((u8)a.byte[20])) * ((s16)((s8)b.byte[20]));
  dst.half[11] = ((u16)((u8)a.byte[22])) * ((s16)((s8)b.byte[22]));
  dst.half[12] = ((u16)((u8)a.byte[24])) * ((s16)((s8)b.byte[24]));
  dst.half[13] = ((u16)((u8)a.byte[26])) * ((s16)((s8)b.byte[26]));
  dst.half[14] = ((u16)((u8)a.byte[28])) * ((s16)((s8)b.byte[28]));
  dst.half[15] = ((u16)((u8)a.byte[30])) * ((s16)((s8)b.byte[30]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmulwev_w_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmulwev_w_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmulwev.w.h xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply even-positioned signed 16-bit elements in `a` and signed elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (s32)(s16)a.half[2 * i] * (s32)(s16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((s32)((s16)a.half[0])) * ((s32)((s16)b.half[0]));
  dst.word[1] = ((s32)((s16)a.half[2])) * ((s32)((s16)b.half[2]));
  dst.word[2] = ((s32)((s16)a.half[4])) * ((s32)((s16)b.half[4]));
  dst.word[3] = ((s32)((s16)a.half[6])) * ((s32)((s16)b.half[6]));
  dst.word[4] = ((s32)((s16)a.half[8])) * ((s32)((s16)b.half[8]));
  dst.word[5] = ((s32)((s16)a.half[10])) * ((s32)((s16)b.half[10]));
  dst.word[6] = ((s32)((s16)a.half[12])) * ((s32)((s16)b.half[12]));
  dst.word[7] = ((s32)((s16)a.half[14])) * ((s32)((s16)b.half[14]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmulwev_w_hu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmulwev_w_hu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmulwev.w.hu xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply even-positioned unsigned 16-bit elements in `a` and unsigned elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i] * (u32)(u16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[0])) * ((u32)((u16)b.half[0]));
  dst.word[1] = ((u32)((u16)a.half[2])) * ((u32)((u16)b.half[2]));
  dst.word[2] = ((u32)((u16)a.half[4])) * ((u32)((u16)b.half[4]));
  dst.word[3] = ((u32)((u16)a.half[6])) * ((u32)((u16)b.half[6]));
  dst.word[4] = ((u32)((u16)a.half[8])) * ((u32)((u16)b.half[8]));
  dst.word[5] = ((u32)((u16)a.half[10])) * ((u32)((u16)b.half[10]));
  dst.word[6] = ((u32)((u16)a.half[12])) * ((u32)((u16)b.half[12]));
  dst.word[7] = ((u32)((u16)a.half[14])) * ((u32)((u16)b.half[14]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmulwev_w_hu_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmulwev_w_hu_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmulwev.w.hu.h xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply even-positioned unsigned 16-bit elements in `a` and signed elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i] * (s32)(s16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[0])) * ((s32)((s16)b.half[0]));
  dst.word[1] = ((u32)((u16)a.half[2])) * ((s32)((s16)b.half[2]));
  dst.word[2] = ((u32)((u16)a.half[4])) * ((s32)((s16)b.half[4]));
  dst.word[3] = ((u32)((u16)a.half[6])) * ((s32)((s16)b.half[6]));
  dst.word[4] = ((u32)((u16)a.half[8])) * ((s32)((s16)b.half[8]));
  dst.word[5] = ((u32)((u16)a.half[10])) * ((s32)((s16)b.half[10]));
  dst.word[6] = ((u32)((u16)a.half[12])) * ((s32)((s16)b.half[12]));
  dst.word[7] = ((u32)((u16)a.half[14])) * ((s32)((s16)b.half[14]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmulwev_d_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmulwev_d_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmulwev.d.w xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply even-positioned signed 32-bit elements in `a` and signed elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (s64)(s32)a.word[2 * i] * (s64)(s32)b.word[2 * i];
}

// Expands to:

if (0) {
  dst.dword[0] = ((s64)((s32)a.word[0])) * ((s64)((s32)b.word[0]));
  dst.dword[1] = ((s64)((s32)a.word[2])) * ((s64)((s32)b.word[2]));
  dst.dword[2] = ((s64)((s32)a.word[4])) * ((s64)((s32)b.word[4]));
  dst.dword[3] = ((s64)((s32)a.word[6])) * ((s64)((s32)b.word[6]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmulwev_d_wu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmulwev_d_wu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmulwev.d.wu xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply even-positioned unsigned 32-bit elements in `a` and unsigned elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 * i] * (u64)(u32)b.word[2 * i];
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[0])) * ((u64)((u32)b.word[0]));
  dst.dword[1] = ((u64)((u32)a.word[2])) * ((u64)((u32)b.word[2]));
  dst.dword[2] = ((u64)((u32)a.word[4])) * ((u64)((u32)b.word[4]));
  dst.dword[3] = ((u64)((u32)a.word[6])) * ((u64)((u32)b.word[6]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmulwev_d_wu_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmulwev_d_wu_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmulwev.d.wu.w xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply even-positioned unsigned 32-bit elements in `a` and signed elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 * i] * (s64)(s32)b.word[2 * i];
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[0])) * ((s64)((s32)b.word[0]));
  dst.dword[1] = ((u64)((u32)a.word[2])) * ((s64)((s32)b.word[2]));
  dst.dword[2] = ((u64)((u32)a.word[4])) * ((s64)((s32)b.word[4]));
  dst.dword[3] = ((u64)((u32)a.word[6])) * ((s64)((s32)b.word[6]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmulwev_q_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmulwev_q_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmulwev.q.d xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply even-positioned signed 64-bit elements in `a` and signed elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] = (s128)(s64)a.dword[2 * i] * (s128)(s64)b.dword[2 * i];
}

// Expands to:

if (0) {
  dst.qword[0] = ((s128)((s64)a.dword[0])) * ((s128)((s64)b.dword[0]));
  dst.qword[1] = ((s128)((s64)a.dword[2])) * ((s128)((s64)b.dword[2]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 7       | 2                |
| 3A6000 | LA664 | 7       | 2                |
| 3C6000 | LA664 | 7       | 2                |

## \_\_m256i \_\_lasx_xvmulwev_q_du (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmulwev_q_du (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmulwev.q.du xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply even-positioned unsigned 64-bit elements in `a` and unsigned elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 * i] * (u128)(u64)b.dword[2 * i];
}

// Expands to:

if (0) {
  dst.qword[0] = ((u128)((u64)a.dword[0])) * ((u128)((u64)b.dword[0]));
  dst.qword[1] = ((u128)((u64)a.dword[2])) * ((u128)((u64)b.dword[2]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 7       | 2                |
| 3A6000 | LA664 | 7       | 2                |
| 3C6000 | LA664 | 7       | 2                |

## \_\_m256i \_\_lasx_xvmulwev_q_du_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmulwev_q_du_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmulwev.q.du.d xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply even-positioned unsigned 64-bit elements in `a` and signed elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 * i] * (s128)(s64)b.dword[2 * i];
}

// Expands to:

if (0) {
  dst.qword[0] = ((u128)((u64)a.dword[0])) * ((s128)((s64)b.dword[0]));
  dst.qword[1] = ((u128)((u64)a.dword[2])) * ((s128)((s64)b.dword[2]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 7       | 2                |
| 3A6000 | LA664 | 7       | 2                |
| 3C6000 | LA664 | 7       | 2                |

## \_\_m256i \_\_lasx_xvmulwod_h_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmulwod_h_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmulwod.h.b xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply odd-positioned signed 8-bit elements in `a` and signed elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
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
  dst.half[8] = ((s16)((s8)a.byte[17])) * ((s16)((s8)b.byte[17]));
  dst.half[9] = ((s16)((s8)a.byte[19])) * ((s16)((s8)b.byte[19]));
  dst.half[10] = ((s16)((s8)a.byte[21])) * ((s16)((s8)b.byte[21]));
  dst.half[11] = ((s16)((s8)a.byte[23])) * ((s16)((s8)b.byte[23]));
  dst.half[12] = ((s16)((s8)a.byte[25])) * ((s16)((s8)b.byte[25]));
  dst.half[13] = ((s16)((s8)a.byte[27])) * ((s16)((s8)b.byte[27]));
  dst.half[14] = ((s16)((s8)a.byte[29])) * ((s16)((s8)b.byte[29]));
  dst.half[15] = ((s16)((s8)a.byte[31])) * ((s16)((s8)b.byte[31]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmulwod_h_bu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmulwod_h_bu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmulwod.h.bu xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply odd-positioned unsigned 8-bit elements in `a` and unsigned elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
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
  dst.half[8] = ((u16)((u8)a.byte[17])) * ((u16)((u8)b.byte[17]));
  dst.half[9] = ((u16)((u8)a.byte[19])) * ((u16)((u8)b.byte[19]));
  dst.half[10] = ((u16)((u8)a.byte[21])) * ((u16)((u8)b.byte[21]));
  dst.half[11] = ((u16)((u8)a.byte[23])) * ((u16)((u8)b.byte[23]));
  dst.half[12] = ((u16)((u8)a.byte[25])) * ((u16)((u8)b.byte[25]));
  dst.half[13] = ((u16)((u8)a.byte[27])) * ((u16)((u8)b.byte[27]));
  dst.half[14] = ((u16)((u8)a.byte[29])) * ((u16)((u8)b.byte[29]));
  dst.half[15] = ((u16)((u8)a.byte[31])) * ((u16)((u8)b.byte[31]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmulwod_h_bu_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmulwod_h_bu_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmulwod.h.bu.b xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply odd-positioned unsigned 8-bit elements in `a` and signed elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
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
  dst.half[8] = ((u16)((u8)a.byte[17])) * ((s16)((s8)b.byte[17]));
  dst.half[9] = ((u16)((u8)a.byte[19])) * ((s16)((s8)b.byte[19]));
  dst.half[10] = ((u16)((u8)a.byte[21])) * ((s16)((s8)b.byte[21]));
  dst.half[11] = ((u16)((u8)a.byte[23])) * ((s16)((s8)b.byte[23]));
  dst.half[12] = ((u16)((u8)a.byte[25])) * ((s16)((s8)b.byte[25]));
  dst.half[13] = ((u16)((u8)a.byte[27])) * ((s16)((s8)b.byte[27]));
  dst.half[14] = ((u16)((u8)a.byte[29])) * ((s16)((s8)b.byte[29]));
  dst.half[15] = ((u16)((u8)a.byte[31])) * ((s16)((s8)b.byte[31]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmulwod_w_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmulwod_w_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmulwod.w.h xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply odd-positioned signed 16-bit elements in `a` and signed elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (s32)(s16)a.half[2 * i + 1] * (s32)(s16)b.half[2 * i + 1];
}

// Expands to:

if (0) {
  dst.word[0] = ((s32)((s16)a.half[1])) * ((s32)((s16)b.half[1]));
  dst.word[1] = ((s32)((s16)a.half[3])) * ((s32)((s16)b.half[3]));
  dst.word[2] = ((s32)((s16)a.half[5])) * ((s32)((s16)b.half[5]));
  dst.word[3] = ((s32)((s16)a.half[7])) * ((s32)((s16)b.half[7]));
  dst.word[4] = ((s32)((s16)a.half[9])) * ((s32)((s16)b.half[9]));
  dst.word[5] = ((s32)((s16)a.half[11])) * ((s32)((s16)b.half[11]));
  dst.word[6] = ((s32)((s16)a.half[13])) * ((s32)((s16)b.half[13]));
  dst.word[7] = ((s32)((s16)a.half[15])) * ((s32)((s16)b.half[15]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmulwod_w_hu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmulwod_w_hu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmulwod.w.hu xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply odd-positioned unsigned 16-bit elements in `a` and unsigned elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i + 1] * (u32)(u16)b.half[2 * i + 1];
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[1])) * ((u32)((u16)b.half[1]));
  dst.word[1] = ((u32)((u16)a.half[3])) * ((u32)((u16)b.half[3]));
  dst.word[2] = ((u32)((u16)a.half[5])) * ((u32)((u16)b.half[5]));
  dst.word[3] = ((u32)((u16)a.half[7])) * ((u32)((u16)b.half[7]));
  dst.word[4] = ((u32)((u16)a.half[9])) * ((u32)((u16)b.half[9]));
  dst.word[5] = ((u32)((u16)a.half[11])) * ((u32)((u16)b.half[11]));
  dst.word[6] = ((u32)((u16)a.half[13])) * ((u32)((u16)b.half[13]));
  dst.word[7] = ((u32)((u16)a.half[15])) * ((u32)((u16)b.half[15]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmulwod_w_hu_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmulwod_w_hu_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmulwod.w.hu.h xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply odd-positioned unsigned 16-bit elements in `a` and signed elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i + 1] * (s32)(s16)b.half[2 * i + 1];
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[1])) * ((s32)((s16)b.half[1]));
  dst.word[1] = ((u32)((u16)a.half[3])) * ((s32)((s16)b.half[3]));
  dst.word[2] = ((u32)((u16)a.half[5])) * ((s32)((s16)b.half[5]));
  dst.word[3] = ((u32)((u16)a.half[7])) * ((s32)((s16)b.half[7]));
  dst.word[4] = ((u32)((u16)a.half[9])) * ((s32)((s16)b.half[9]));
  dst.word[5] = ((u32)((u16)a.half[11])) * ((s32)((s16)b.half[11]));
  dst.word[6] = ((u32)((u16)a.half[13])) * ((s32)((s16)b.half[13]));
  dst.word[7] = ((u32)((u16)a.half[15])) * ((s32)((s16)b.half[15]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmulwod_d_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmulwod_d_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmulwod.d.w xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply odd-positioned signed 32-bit elements in `a` and signed elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (s64)(s32)a.word[2 * i + 1] * (s64)(s32)b.word[2 * i + 1];
}

// Expands to:

if (0) {
  dst.dword[0] = ((s64)((s32)a.word[1])) * ((s64)((s32)b.word[1]));
  dst.dword[1] = ((s64)((s32)a.word[3])) * ((s64)((s32)b.word[3]));
  dst.dword[2] = ((s64)((s32)a.word[5])) * ((s64)((s32)b.word[5]));
  dst.dword[3] = ((s64)((s32)a.word[7])) * ((s64)((s32)b.word[7]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmulwod_d_wu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmulwod_d_wu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmulwod.d.wu xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply odd-positioned unsigned 32-bit elements in `a` and unsigned elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 * i + 1] * (u64)(u32)b.word[2 * i + 1];
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[1])) * ((u64)((u32)b.word[1]));
  dst.dword[1] = ((u64)((u32)a.word[3])) * ((u64)((u32)b.word[3]));
  dst.dword[2] = ((u64)((u32)a.word[5])) * ((u64)((u32)b.word[5]));
  dst.dword[3] = ((u64)((u32)a.word[7])) * ((u64)((u32)b.word[7]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmulwod_d_wu_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmulwod_d_wu_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmulwod.d.wu.w xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply odd-positioned unsigned 32-bit elements in `a` and signed elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 * i + 1] * (s64)(s32)b.word[2 * i + 1];
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[1])) * ((s64)((s32)b.word[1]));
  dst.dword[1] = ((u64)((u32)a.word[3])) * ((s64)((s32)b.word[3]));
  dst.dword[2] = ((u64)((u32)a.word[5])) * ((s64)((s32)b.word[5]));
  dst.dword[3] = ((u64)((u32)a.word[7])) * ((s64)((s32)b.word[7]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmulwod_q_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmulwod_q_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmulwod.q.d xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply odd-positioned signed 64-bit elements in `a` and signed elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] = (s128)(s64)a.dword[2 * i + 1] * (s128)(s64)b.dword[2 * i + 1];
}

// Expands to:

if (0) {
  dst.qword[0] = ((s128)((s64)a.dword[1])) * ((s128)((s64)b.dword[1]));
  dst.qword[1] = ((s128)((s64)a.dword[3])) * ((s128)((s64)b.dword[3]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 7       | 2                |
| 3A6000 | LA664 | 7       | 2                |
| 3C6000 | LA664 | 7       | 2                |

## \_\_m256i \_\_lasx_xvmulwod_q_du (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmulwod_q_du (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmulwod.q.du xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply odd-positioned unsigned 64-bit elements in `a` and unsigned elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 * i + 1] * (u128)(u64)b.dword[2 * i + 1];
}

// Expands to:

if (0) {
  dst.qword[0] = ((u128)((u64)a.dword[1])) * ((u128)((u64)b.dword[1]));
  dst.qword[1] = ((u128)((u64)a.dword[3])) * ((u128)((u64)b.dword[3]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 7       | 2                |
| 3A6000 | LA664 | 7       | 2                |
| 3C6000 | LA664 | 7       | 2                |

## \_\_m256i \_\_lasx_xvmulwod_q_du_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvmulwod_q_du_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmulwod.q.du.d xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply odd-positioned unsigned 64-bit elements in `a` and signed elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 * i + 1] * (s128)(s64)b.dword[2 * i + 1];
}

// Expands to:

if (0) {
  dst.qword[0] = ((u128)((u64)a.dword[1])) * ((s128)((s64)b.dword[1]));
  dst.qword[1] = ((u128)((u64)a.dword[3])) * ((s128)((s64)b.dword[3]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 7       | 2                |
| 3A6000 | LA664 | 7       | 2                |
| 3C6000 | LA664 | 7       | 2                |

## \_\_m256i \_\_lasx_xvneg_b (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvneg_b (__m256i a)
#include <lasxintrin.h>
Instruction: xvneg.b xr, xr
CPU Flags: LASX
```

### Description

Negate 8-bit elements in `a` and save the result in `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = -a.byte[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvneg_h (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvneg_h (__m256i a)
#include <lasxintrin.h>
Instruction: xvneg.h xr, xr
CPU Flags: LASX
```

### Description

Negate 16-bit elements in `a` and save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = -a.half[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvneg_w (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvneg_w (__m256i a)
#include <lasxintrin.h>
Instruction: xvneg.w xr, xr
CPU Flags: LASX
```

### Description

Negate 32-bit elements in `a` and save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = -a.word[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvneg_d (\_\_m256i a)

### Synopsis

```
__m256i __lasx_xvneg_d (__m256i a)
#include <lasxintrin.h>
Instruction: xvneg.d xr, xr
CPU Flags: LASX
```

### Description

Negate 64-bit elements in `a` and save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = -a.dword[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsadd_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsadd_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsadd.b xr, xr, xr
CPU Flags: LASX
```

### Description

Saturating add the signed 8-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = (s8)sadd((s8)a.byte[i], (s8)b.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsadd_bu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsadd_bu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsadd.bu xr, xr, xr
CPU Flags: LASX
```

### Description

Saturating add the unsigned 8-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = (u8)sadd((u8)a.byte[i], (u8)b.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsadd_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsadd_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsadd.h xr, xr, xr
CPU Flags: LASX
```

### Description

Saturating add the signed 16-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = (s16)sadd((s16)a.half[i], (s16)b.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsadd_hu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsadd_hu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsadd.hu xr, xr, xr
CPU Flags: LASX
```

### Description

Saturating add the unsigned 16-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = (u16)sadd((u16)a.half[i], (u16)b.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsadd_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsadd_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsadd.w xr, xr, xr
CPU Flags: LASX
```

### Description

Saturating add the signed 32-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (s32)sadd((s32)a.word[i], (s32)b.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsadd_wu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsadd_wu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsadd.wu xr, xr, xr
CPU Flags: LASX
```

### Description

Saturating add the unsigned 32-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (u32)sadd((u32)a.word[i], (u32)b.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsadd_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsadd_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsadd.d xr, xr, xr
CPU Flags: LASX
```

### Description

Saturating add the signed 64-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (s64)sadd((s64)a.dword[i], (s64)b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsadd_du (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsadd_du (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsadd.du xr, xr, xr
CPU Flags: LASX
```

### Description

Saturating add the unsigned 64-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (u64)sadd((u64)a.dword[i], (u64)b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvssub_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssub_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssub.b xr, xr, xr
CPU Flags: LASX
```

### Description

Saturating subtract the signed 8-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = (s8)ssub((s8)a.byte[i], (s8)b.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvssub_bu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssub_bu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssub.bu xr, xr, xr
CPU Flags: LASX
```

### Description

Saturating subtract the unsigned 8-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = (u8)ssub((u8)a.byte[i], (u8)b.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvssub_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssub_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssub.h xr, xr, xr
CPU Flags: LASX
```

### Description

Saturating subtract the signed 16-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = (s16)ssub((s16)a.half[i], (s16)b.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvssub_hu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssub_hu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssub.hu xr, xr, xr
CPU Flags: LASX
```

### Description

Saturating subtract the unsigned 16-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = (u16)ssub((u16)a.half[i], (u16)b.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvssub_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssub_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssub.w xr, xr, xr
CPU Flags: LASX
```

### Description

Saturating subtract the signed 32-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (s32)ssub((s32)a.word[i], (s32)b.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvssub_wu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssub_wu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssub.wu xr, xr, xr
CPU Flags: LASX
```

### Description

Saturating subtract the unsigned 32-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (u32)ssub((u32)a.word[i], (u32)b.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvssub_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssub_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssub.d xr, xr, xr
CPU Flags: LASX
```

### Description

Saturating subtract the signed 64-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (s64)ssub((s64)a.dword[i], (s64)b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvssub_du (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvssub_du (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvssub.du xr, xr, xr
CPU Flags: LASX
```

### Description

Saturating subtract the unsigned 64-bit elements in `a` and `b`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (u64)ssub((u64)a.dword[i], (u64)b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsub_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsub_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsub.b xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract 8-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[i] - b.byte[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsub_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsub_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsub.h xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract 16-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = a.half[i] - b.half[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsub_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsub_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsub.w xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract 32-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = a.word[i] - b.word[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsub_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsub_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsub.d xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract 64-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = a.dword[i] - b.dword[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsub_q (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsub_q (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsub.q xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract 128-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] = a.qword[i] - b.qword[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvsubi_bu (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvsubi_bu (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvsubi.bu xr, xr, imm
CPU Flags: LASX
```

### Description

Subtract 8-bit elements in `a` by `imm`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[i] - imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsubi_hu (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvsubi_hu (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvsubi.hu xr, xr, imm
CPU Flags: LASX
```

### Description

Subtract 16-bit elements in `a` by `imm`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = a.half[i] - imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsubi_wu (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvsubi_wu (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvsubi.wu xr, xr, imm
CPU Flags: LASX
```

### Description

Subtract 32-bit elements in `a` by `imm`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = a.word[i] - imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsubi_du (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvsubi_du (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvsubi.du xr, xr, imm
CPU Flags: LASX
```

### Description

Subtract 64-bit elements in `a` by `imm`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = a.dword[i] - imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsubwev_h_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsubwev_h_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsubwev.h.b xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract even-positioned signed 8-bit elements in `a` and signed elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
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
  dst.half[8] = ((s16)((s8)a.byte[16])) - ((s16)((s8)b.byte[16]));
  dst.half[9] = ((s16)((s8)a.byte[18])) - ((s16)((s8)b.byte[18]));
  dst.half[10] = ((s16)((s8)a.byte[20])) - ((s16)((s8)b.byte[20]));
  dst.half[11] = ((s16)((s8)a.byte[22])) - ((s16)((s8)b.byte[22]));
  dst.half[12] = ((s16)((s8)a.byte[24])) - ((s16)((s8)b.byte[24]));
  dst.half[13] = ((s16)((s8)a.byte[26])) - ((s16)((s8)b.byte[26]));
  dst.half[14] = ((s16)((s8)a.byte[28])) - ((s16)((s8)b.byte[28]));
  dst.half[15] = ((s16)((s8)a.byte[30])) - ((s16)((s8)b.byte[30]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsubwev_h_bu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsubwev_h_bu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsubwev.h.bu xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract even-positioned unsigned 8-bit elements in `a` and unsigned elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
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
  dst.half[8] = ((u16)((u8)a.byte[16])) - ((u16)((u8)b.byte[16]));
  dst.half[9] = ((u16)((u8)a.byte[18])) - ((u16)((u8)b.byte[18]));
  dst.half[10] = ((u16)((u8)a.byte[20])) - ((u16)((u8)b.byte[20]));
  dst.half[11] = ((u16)((u8)a.byte[22])) - ((u16)((u8)b.byte[22]));
  dst.half[12] = ((u16)((u8)a.byte[24])) - ((u16)((u8)b.byte[24]));
  dst.half[13] = ((u16)((u8)a.byte[26])) - ((u16)((u8)b.byte[26]));
  dst.half[14] = ((u16)((u8)a.byte[28])) - ((u16)((u8)b.byte[28]));
  dst.half[15] = ((u16)((u8)a.byte[30])) - ((u16)((u8)b.byte[30]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsubwev_w_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsubwev_w_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsubwev.w.h xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract even-positioned signed 16-bit elements in `a` and signed elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (s32)(s16)a.half[2 * i] - (s32)(s16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((s32)((s16)a.half[0])) - ((s32)((s16)b.half[0]));
  dst.word[1] = ((s32)((s16)a.half[2])) - ((s32)((s16)b.half[2]));
  dst.word[2] = ((s32)((s16)a.half[4])) - ((s32)((s16)b.half[4]));
  dst.word[3] = ((s32)((s16)a.half[6])) - ((s32)((s16)b.half[6]));
  dst.word[4] = ((s32)((s16)a.half[8])) - ((s32)((s16)b.half[8]));
  dst.word[5] = ((s32)((s16)a.half[10])) - ((s32)((s16)b.half[10]));
  dst.word[6] = ((s32)((s16)a.half[12])) - ((s32)((s16)b.half[12]));
  dst.word[7] = ((s32)((s16)a.half[14])) - ((s32)((s16)b.half[14]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsubwev_w_hu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsubwev_w_hu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsubwev.w.hu xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract even-positioned unsigned 16-bit elements in `a` and unsigned elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i] - (u32)(u16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[0])) - ((u32)((u16)b.half[0]));
  dst.word[1] = ((u32)((u16)a.half[2])) - ((u32)((u16)b.half[2]));
  dst.word[2] = ((u32)((u16)a.half[4])) - ((u32)((u16)b.half[4]));
  dst.word[3] = ((u32)((u16)a.half[6])) - ((u32)((u16)b.half[6]));
  dst.word[4] = ((u32)((u16)a.half[8])) - ((u32)((u16)b.half[8]));
  dst.word[5] = ((u32)((u16)a.half[10])) - ((u32)((u16)b.half[10]));
  dst.word[6] = ((u32)((u16)a.half[12])) - ((u32)((u16)b.half[12]));
  dst.word[7] = ((u32)((u16)a.half[14])) - ((u32)((u16)b.half[14]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsubwev_d_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsubwev_d_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsubwev.d.w xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract even-positioned signed 32-bit elements in `a` and signed elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (s64)(s32)a.word[2 * i] - (s64)(s32)b.word[2 * i];
}

// Expands to:

if (0) {
  dst.dword[0] = ((s64)((s32)a.word[0])) - ((s64)((s32)b.word[0]));
  dst.dword[1] = ((s64)((s32)a.word[2])) - ((s64)((s32)b.word[2]));
  dst.dword[2] = ((s64)((s32)a.word[4])) - ((s64)((s32)b.word[4]));
  dst.dword[3] = ((s64)((s32)a.word[6])) - ((s64)((s32)b.word[6]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsubwev_d_wu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsubwev_d_wu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsubwev.d.wu xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract even-positioned unsigned 32-bit elements in `a` and unsigned elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 * i] - (u64)(u32)b.word[2 * i];
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[0])) - ((u64)((u32)b.word[0]));
  dst.dword[1] = ((u64)((u32)a.word[2])) - ((u64)((u32)b.word[2]));
  dst.dword[2] = ((u64)((u32)a.word[4])) - ((u64)((u32)b.word[4]));
  dst.dword[3] = ((u64)((u32)a.word[6])) - ((u64)((u32)b.word[6]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsubwev_q_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsubwev_q_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsubwev.q.d xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract even-positioned signed 64-bit elements in `a` and signed elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] = (s128)(s64)a.dword[2 * i] - (s128)(s64)b.dword[2 * i];
}

// Expands to:

if (0) {
  dst.qword[0] = ((s128)((s64)a.dword[0])) - ((s128)((s64)b.dword[0]));
  dst.qword[1] = ((s128)((s64)a.dword[2])) - ((s128)((s64)b.dword[2]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvsubwev_q_du (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsubwev_q_du (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsubwev.q.du xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract even-positioned unsigned 64-bit elements in `a` and unsigned elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 * i] - (u128)(u64)b.dword[2 * i];
}

// Expands to:

if (0) {
  dst.qword[0] = ((u128)((u64)a.dword[0])) - ((u128)((u64)b.dword[0]));
  dst.qword[1] = ((u128)((u64)a.dword[2])) - ((u128)((u64)b.dword[2]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvsubwod_h_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsubwod_h_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsubwod.h.b xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract odd-positioned signed 8-bit elements in `a` and signed elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
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
  dst.half[8] = ((s16)((s8)a.byte[17])) - ((s16)((s8)b.byte[17]));
  dst.half[9] = ((s16)((s8)a.byte[19])) - ((s16)((s8)b.byte[19]));
  dst.half[10] = ((s16)((s8)a.byte[21])) - ((s16)((s8)b.byte[21]));
  dst.half[11] = ((s16)((s8)a.byte[23])) - ((s16)((s8)b.byte[23]));
  dst.half[12] = ((s16)((s8)a.byte[25])) - ((s16)((s8)b.byte[25]));
  dst.half[13] = ((s16)((s8)a.byte[27])) - ((s16)((s8)b.byte[27]));
  dst.half[14] = ((s16)((s8)a.byte[29])) - ((s16)((s8)b.byte[29]));
  dst.half[15] = ((s16)((s8)a.byte[31])) - ((s16)((s8)b.byte[31]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsubwod_h_bu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsubwod_h_bu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsubwod.h.bu xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract odd-positioned unsigned 8-bit elements in `a` and unsigned elements in `b`, save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
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
  dst.half[8] = ((u16)((u8)a.byte[17])) - ((u16)((u8)b.byte[17]));
  dst.half[9] = ((u16)((u8)a.byte[19])) - ((u16)((u8)b.byte[19]));
  dst.half[10] = ((u16)((u8)a.byte[21])) - ((u16)((u8)b.byte[21]));
  dst.half[11] = ((u16)((u8)a.byte[23])) - ((u16)((u8)b.byte[23]));
  dst.half[12] = ((u16)((u8)a.byte[25])) - ((u16)((u8)b.byte[25]));
  dst.half[13] = ((u16)((u8)a.byte[27])) - ((u16)((u8)b.byte[27]));
  dst.half[14] = ((u16)((u8)a.byte[29])) - ((u16)((u8)b.byte[29]));
  dst.half[15] = ((u16)((u8)a.byte[31])) - ((u16)((u8)b.byte[31]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsubwod_w_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsubwod_w_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsubwod.w.h xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract odd-positioned signed 16-bit elements in `a` and signed elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (s32)(s16)a.half[2 * i + 1] - (s32)(s16)b.half[2 * i + 1];
}

// Expands to:

if (0) {
  dst.word[0] = ((s32)((s16)a.half[1])) - ((s32)((s16)b.half[1]));
  dst.word[1] = ((s32)((s16)a.half[3])) - ((s32)((s16)b.half[3]));
  dst.word[2] = ((s32)((s16)a.half[5])) - ((s32)((s16)b.half[5]));
  dst.word[3] = ((s32)((s16)a.half[7])) - ((s32)((s16)b.half[7]));
  dst.word[4] = ((s32)((s16)a.half[9])) - ((s32)((s16)b.half[9]));
  dst.word[5] = ((s32)((s16)a.half[11])) - ((s32)((s16)b.half[11]));
  dst.word[6] = ((s32)((s16)a.half[13])) - ((s32)((s16)b.half[13]));
  dst.word[7] = ((s32)((s16)a.half[15])) - ((s32)((s16)b.half[15]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsubwod_w_hu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsubwod_w_hu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsubwod.w.hu xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract odd-positioned unsigned 16-bit elements in `a` and unsigned elements in `b`, save the 32-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (u32)(u16)a.half[2 * i + 1] - (u32)(u16)b.half[2 * i + 1];
}

// Expands to:

if (0) {
  dst.word[0] = ((u32)((u16)a.half[1])) - ((u32)((u16)b.half[1]));
  dst.word[1] = ((u32)((u16)a.half[3])) - ((u32)((u16)b.half[3]));
  dst.word[2] = ((u32)((u16)a.half[5])) - ((u32)((u16)b.half[5]));
  dst.word[3] = ((u32)((u16)a.half[7])) - ((u32)((u16)b.half[7]));
  dst.word[4] = ((u32)((u16)a.half[9])) - ((u32)((u16)b.half[9]));
  dst.word[5] = ((u32)((u16)a.half[11])) - ((u32)((u16)b.half[11]));
  dst.word[6] = ((u32)((u16)a.half[13])) - ((u32)((u16)b.half[13]));
  dst.word[7] = ((u32)((u16)a.half[15])) - ((u32)((u16)b.half[15]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsubwod_d_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsubwod_d_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsubwod.d.w xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract odd-positioned signed 32-bit elements in `a` and signed elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (s64)(s32)a.word[2 * i + 1] - (s64)(s32)b.word[2 * i + 1];
}

// Expands to:

if (0) {
  dst.dword[0] = ((s64)((s32)a.word[1])) - ((s64)((s32)b.word[1]));
  dst.dword[1] = ((s64)((s32)a.word[3])) - ((s64)((s32)b.word[3]));
  dst.dword[2] = ((s64)((s32)a.word[5])) - ((s64)((s32)b.word[5]));
  dst.dword[3] = ((s64)((s32)a.word[7])) - ((s64)((s32)b.word[7]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsubwod_d_wu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsubwod_d_wu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsubwod.d.wu xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract odd-positioned unsigned 32-bit elements in `a` and unsigned elements in `b`, save the 64-bit result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (u64)(u32)a.word[2 * i + 1] - (u64)(u32)b.word[2 * i + 1];
}

// Expands to:

if (0) {
  dst.dword[0] = ((u64)((u32)a.word[1])) - ((u64)((u32)b.word[1]));
  dst.dword[1] = ((u64)((u32)a.word[3])) - ((u64)((u32)b.word[3]));
  dst.dword[2] = ((u64)((u32)a.word[5])) - ((u64)((u32)b.word[5]));
  dst.dword[3] = ((u64)((u32)a.word[7])) - ((u64)((u32)b.word[7]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

## \_\_m256i \_\_lasx_xvsubwod_q_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsubwod_q_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsubwod.q.d xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract odd-positioned signed 64-bit elements in `a` and signed elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] = (s128)(s64)a.dword[2 * i + 1] - (s128)(s64)b.dword[2 * i + 1];
}

// Expands to:

if (0) {
  dst.qword[0] = ((s128)((s64)a.dword[1])) - ((s128)((s64)b.dword[1]));
  dst.qword[1] = ((s128)((s64)a.dword[3])) - ((s128)((s64)b.dword[3]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvsubwod_q_du (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsubwod_q_du (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsubwod.q.du xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract odd-positioned unsigned 64-bit elements in `a` and unsigned elements in `b`, save the 128-bit result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] = (u128)(u64)a.dword[2 * i + 1] - (u128)(u64)b.dword[2 * i + 1];
}

// Expands to:

if (0) {
  dst.qword[0] = ((u128)((u64)a.dword[1])) - ((u128)((u64)b.dword[1]));
  dst.qword[1] = ((u128)((u64)a.dword[3])) - ((u128)((u64)b.dword[3]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |
