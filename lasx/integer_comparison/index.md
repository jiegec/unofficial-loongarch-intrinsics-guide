# Integer Comparison

## \_\_m256i \_\_lasx_xvseq_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvseq_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvseq.b xr, xr, xr
CPU Flags: LASX
```

### Description

Compare the 8-bit elements in `a` and `b`, store all-ones to `dst` if equal, zero otherwise.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = (a.byte[i] == b.byte[i]) ? 0xFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvseq_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvseq_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvseq.h xr, xr, xr
CPU Flags: LASX
```

### Description

Compare the 16-bit elements in `a` and `b`, store all-ones to `dst` if equal, zero otherwise.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = (a.half[i] == b.half[i]) ? 0xFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvseq_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvseq_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvseq.w xr, xr, xr
CPU Flags: LASX
```

### Description

Compare the 32-bit elements in `a` and `b`, store all-ones to `dst` if equal, zero otherwise.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (a.word[i] == b.word[i]) ? 0xFFFFFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvseq_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvseq_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvseq.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compare the 64-bit elements in `a` and `b`, store all-ones to `dst` if equal, zero otherwise.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (a.dword[i] == b.dword[i]) ? 0xFFFFFFFFFFFFFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvseqi_b (\_\_m256i a, imm_n16_15 imm)

### Synopsis

```
__m256i __lasx_xvseqi_b (__m256i a, imm_n16_15 imm)
#include <lasxintrin.h>
Instruction: xvseqi.b xr, xr, imm
CPU Flags: LASX
```

### Description

Compare the 8-bit elements in `a` and `imm`, store all-ones to `dst` if equal, zero otherwise.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = ((s8)a.byte[i] == imm) ? 0xFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvseqi_h (\_\_m256i a, imm_n16_15 imm)

### Synopsis

```
__m256i __lasx_xvseqi_h (__m256i a, imm_n16_15 imm)
#include <lasxintrin.h>
Instruction: xvseqi.h xr, xr, imm
CPU Flags: LASX
```

### Description

Compare the 16-bit elements in `a` and `imm`, store all-ones to `dst` if equal, zero otherwise.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = ((s16)a.half[i] == imm) ? 0xFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvseqi_w (\_\_m256i a, imm_n16_15 imm)

### Synopsis

```
__m256i __lasx_xvseqi_w (__m256i a, imm_n16_15 imm)
#include <lasxintrin.h>
Instruction: xvseqi.w xr, xr, imm
CPU Flags: LASX
```

### Description

Compare the 32-bit elements in `a` and `imm`, store all-ones to `dst` if equal, zero otherwise.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = ((s32)a.word[i] == imm) ? 0xFFFFFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvseqi_d (\_\_m256i a, imm_n16_15 imm)

### Synopsis

```
__m256i __lasx_xvseqi_d (__m256i a, imm_n16_15 imm)
#include <lasxintrin.h>
Instruction: xvseqi.d xr, xr, imm
CPU Flags: LASX
```

### Description

Compare the 64-bit elements in `a` and `imm`, store all-ones to `dst` if equal, zero otherwise.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = ((s64)a.dword[i] == imm) ? 0xFFFFFFFFFFFFFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvslt_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvslt_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvslt.b xr, xr, xr
CPU Flags: LASX
```

### Description

Compare the signed 8-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = ((s8)a.byte[i] < (s8)b.byte[i]) ? 0xFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvslt_bu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvslt_bu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvslt.bu xr, xr, xr
CPU Flags: LASX
```

### Description

Compare the unsigned 8-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = ((u8)a.byte[i] < (u8)b.byte[i]) ? 0xFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvslt_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvslt_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvslt.h xr, xr, xr
CPU Flags: LASX
```

### Description

Compare the signed 16-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = ((s16)a.half[i] < (s16)b.half[i]) ? 0xFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvslt_hu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvslt_hu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvslt.hu xr, xr, xr
CPU Flags: LASX
```

### Description

Compare the unsigned 16-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = ((u16)a.half[i] < (u16)b.half[i]) ? 0xFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvslt_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvslt_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvslt.w xr, xr, xr
CPU Flags: LASX
```

### Description

Compare the signed 32-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = ((s32)a.word[i] < (s32)b.word[i]) ? 0xFFFFFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvslt_wu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvslt_wu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvslt.wu xr, xr, xr
CPU Flags: LASX
```

### Description

Compare the unsigned 32-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = ((u32)a.word[i] < (u32)b.word[i]) ? 0xFFFFFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvslt_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvslt_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvslt.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compare the signed 64-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = ((s64)a.dword[i] < (s64)b.dword[i]) ? 0xFFFFFFFFFFFFFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvslt_du (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvslt_du (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvslt.du xr, xr, xr
CPU Flags: LASX
```

### Description

Compare the unsigned 64-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = ((u64)a.dword[i] < (u64)b.dword[i]) ? 0xFFFFFFFFFFFFFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvslti_b (\_\_m256i a, imm_n16_15 imm)

### Synopsis

```
__m256i __lasx_xvslti_b (__m256i a, imm_n16_15 imm)
#include <lasxintrin.h>
Instruction: xvslti.b xr, xr, imm
CPU Flags: LASX
```

### Description

Compare the signed 8-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = ((s8)a.byte[i] < imm) ? 0xFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvslti_bu (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvslti_bu (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvslti.bu xr, xr, imm
CPU Flags: LASX
```

### Description

Compare the unsigned 8-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = ((u8)a.byte[i] < imm) ? 0xFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvslti_h (\_\_m256i a, imm_n16_15 imm)

### Synopsis

```
__m256i __lasx_xvslti_h (__m256i a, imm_n16_15 imm)
#include <lasxintrin.h>
Instruction: xvslti.h xr, xr, imm
CPU Flags: LASX
```

### Description

Compare the signed 16-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = ((s16)a.half[i] < imm) ? 0xFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvslti_hu (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvslti_hu (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvslti.hu xr, xr, imm
CPU Flags: LASX
```

### Description

Compare the unsigned 16-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = ((u16)a.half[i] < imm) ? 0xFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvslti_w (\_\_m256i a, imm_n16_15 imm)

### Synopsis

```
__m256i __lasx_xvslti_w (__m256i a, imm_n16_15 imm)
#include <lasxintrin.h>
Instruction: xvslti.w xr, xr, imm
CPU Flags: LASX
```

### Description

Compare the signed 32-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = ((s32)a.word[i] < imm) ? 0xFFFFFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvslti_wu (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvslti_wu (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvslti.wu xr, xr, imm
CPU Flags: LASX
```

### Description

Compare the unsigned 32-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = ((u32)a.word[i] < imm) ? 0xFFFFFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvslti_d (\_\_m256i a, imm_n16_15 imm)

### Synopsis

```
__m256i __lasx_xvslti_d (__m256i a, imm_n16_15 imm)
#include <lasxintrin.h>
Instruction: xvslti.d xr, xr, imm
CPU Flags: LASX
```

### Description

Compare the signed 64-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = ((s64)a.dword[i] < imm) ? 0xFFFFFFFFFFFFFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvslti_du (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvslti_du (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvslti.du xr, xr, imm
CPU Flags: LASX
```

### Description

Compare the unsigned 64-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = ((u64)a.dword[i] < imm) ? 0xFFFFFFFFFFFFFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvsle_b (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsle_b (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsle.b xr, xr, xr
CPU Flags: LASX
```

### Description

Compare the signed 8-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = ((s8)a.byte[i] <= (s8)b.byte[i]) ? 0xFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsle_bu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsle_bu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsle.bu xr, xr, xr
CPU Flags: LASX
```

### Description

Compare the unsigned 8-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = ((u8)a.byte[i] <= (u8)b.byte[i]) ? 0xFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsle_h (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsle_h (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsle.h xr, xr, xr
CPU Flags: LASX
```

### Description

Compare the signed 16-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = ((s16)a.half[i] <= (s16)b.half[i]) ? 0xFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsle_hu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsle_hu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsle.hu xr, xr, xr
CPU Flags: LASX
```

### Description

Compare the unsigned 16-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = ((u16)a.half[i] <= (u16)b.half[i]) ? 0xFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsle_w (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsle_w (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsle.w xr, xr, xr
CPU Flags: LASX
```

### Description

Compare the signed 32-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = ((s32)a.word[i] <= (s32)b.word[i]) ? 0xFFFFFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsle_wu (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsle_wu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsle.wu xr, xr, xr
CPU Flags: LASX
```

### Description

Compare the unsigned 32-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = ((u32)a.word[i] <= (u32)b.word[i]) ? 0xFFFFFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvsle_d (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsle_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsle.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compare the signed 64-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = ((s64)a.dword[i] <= (s64)b.dword[i]) ? 0xFFFFFFFFFFFFFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvsle_du (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvsle_du (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsle.du xr, xr, xr
CPU Flags: LASX
```

### Description

Compare the unsigned 64-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = ((u64)a.dword[i] <= (u64)b.dword[i]) ? 0xFFFFFFFFFFFFFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvslei_b (\_\_m256i a, imm_n16_15 imm)

### Synopsis

```
__m256i __lasx_xvslei_b (__m256i a, imm_n16_15 imm)
#include <lasxintrin.h>
Instruction: xvslei.b xr, xr, imm
CPU Flags: LASX
```

### Description

Compare the signed 8-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = ((s8)a.byte[i] <= imm) ? 0xFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvslei_bu (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvslei_bu (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvslei.bu xr, xr, imm
CPU Flags: LASX
```

### Description

Compare the unsigned 8-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = ((u8)a.byte[i] <= imm) ? 0xFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvslei_h (\_\_m256i a, imm_n16_15 imm)

### Synopsis

```
__m256i __lasx_xvslei_h (__m256i a, imm_n16_15 imm)
#include <lasxintrin.h>
Instruction: xvslei.h xr, xr, imm
CPU Flags: LASX
```

### Description

Compare the signed 16-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = ((s16)a.half[i] <= imm) ? 0xFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvslei_hu (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvslei_hu (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvslei.hu xr, xr, imm
CPU Flags: LASX
```

### Description

Compare the unsigned 16-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = ((u16)a.half[i] <= imm) ? 0xFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvslei_w (\_\_m256i a, imm_n16_15 imm)

### Synopsis

```
__m256i __lasx_xvslei_w (__m256i a, imm_n16_15 imm)
#include <lasxintrin.h>
Instruction: xvslei.w xr, xr, imm
CPU Flags: LASX
```

### Description

Compare the signed 32-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = ((s32)a.word[i] <= imm) ? 0xFFFFFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvslei_wu (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvslei_wu (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvslei.wu xr, xr, imm
CPU Flags: LASX
```

### Description

Compare the unsigned 32-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = ((u32)a.word[i] <= imm) ? 0xFFFFFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvslei_d (\_\_m256i a, imm_n16_15 imm)

### Synopsis

```
__m256i __lasx_xvslei_d (__m256i a, imm_n16_15 imm)
#include <lasxintrin.h>
Instruction: xvslei.d xr, xr, imm
CPU Flags: LASX
```

### Description

Compare the signed 64-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = ((s64)a.dword[i] <= imm) ? 0xFFFFFFFFFFFFFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvslei_du (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvslei_du (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvslei.du xr, xr, imm
CPU Flags: LASX
```

### Description

Compare the unsigned 64-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = ((u64)a.dword[i] <= imm) ? 0xFFFFFFFFFFFFFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |
