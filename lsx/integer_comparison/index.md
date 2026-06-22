# Integer Comparison

## \_\_m128i \_\_lsx_vseq_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vseq_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vseq.b vr, vr, vr
CPU Flags: LSX
```

### Description

Compare the 8-bit elements in `a` and `b`, store all-ones to `dst` if equal, zero otherwise.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (a.byte[i] == b.byte[i]) ? 0xFF : 0;
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

## \_\_m128i \_\_lsx_vseq_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vseq_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vseq.h vr, vr, vr
CPU Flags: LSX
```

### Description

Compare the 16-bit elements in `a` and `b`, store all-ones to `dst` if equal, zero otherwise.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (a.half[i] == b.half[i]) ? 0xFFFF : 0;
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

## \_\_m128i \_\_lsx_vseq_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vseq_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vseq.w vr, vr, vr
CPU Flags: LSX
```

### Description

Compare the 32-bit elements in `a` and `b`, store all-ones to `dst` if equal, zero otherwise.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (a.word[i] == b.word[i]) ? 0xFFFFFFFF : 0;
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

## \_\_m128i \_\_lsx_vseq_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vseq_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vseq.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compare the 64-bit elements in `a` and `b`, store all-ones to `dst` if equal, zero otherwise.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (a.dword[i] == b.dword[i]) ? 0xFFFFFFFFFFFFFFFF : 0;
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

## \_\_m128i \_\_lsx_vseqi_b (\_\_m128i a, imm_n16_15 imm)

### Synopsis

```
__m128i __lsx_vseqi_b (__m128i a, imm_n16_15 imm)
#include <lsxintrin.h>
Instruction: vseqi.b vr, vr, imm
CPU Flags: LSX
```

### Description

Compare the 8-bit elements in `a` and `imm`, store all-ones to `dst` if equal, zero otherwise.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = ((s8)a.byte[i] == imm) ? 0xFF : 0;
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

## \_\_m128i \_\_lsx_vseqi_h (\_\_m128i a, imm_n16_15 imm)

### Synopsis

```
__m128i __lsx_vseqi_h (__m128i a, imm_n16_15 imm)
#include <lsxintrin.h>
Instruction: vseqi.h vr, vr, imm
CPU Flags: LSX
```

### Description

Compare the 16-bit elements in `a` and `imm`, store all-ones to `dst` if equal, zero otherwise.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = ((s16)a.half[i] == imm) ? 0xFFFF : 0;
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

## \_\_m128i \_\_lsx_vseqi_w (\_\_m128i a, imm_n16_15 imm)

### Synopsis

```
__m128i __lsx_vseqi_w (__m128i a, imm_n16_15 imm)
#include <lsxintrin.h>
Instruction: vseqi.w vr, vr, imm
CPU Flags: LSX
```

### Description

Compare the 32-bit elements in `a` and `imm`, store all-ones to `dst` if equal, zero otherwise.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = ((s32)a.word[i] == imm) ? 0xFFFFFFFF : 0;
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

## \_\_m128i \_\_lsx_vseqi_d (\_\_m128i a, imm_n16_15 imm)

### Synopsis

```
__m128i __lsx_vseqi_d (__m128i a, imm_n16_15 imm)
#include <lsxintrin.h>
Instruction: vseqi.d vr, vr, imm
CPU Flags: LSX
```

### Description

Compare the 64-bit elements in `a` and `imm`, store all-ones to `dst` if equal, zero otherwise.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = ((s64)a.dword[i] == imm) ? 0xFFFFFFFFFFFFFFFF : 0;
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

## \_\_m128i \_\_lsx_vslt_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vslt_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vslt.b vr, vr, vr
CPU Flags: LSX
```

### Description

Compare the signed 8-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = ((s8)a.byte[i] < (s8)b.byte[i]) ? 0xFF : 0;
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

## \_\_m128i \_\_lsx_vslt_bu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vslt_bu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vslt.bu vr, vr, vr
CPU Flags: LSX
```

### Description

Compare the unsigned 8-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = ((u8)a.byte[i] < (u8)b.byte[i]) ? 0xFF : 0;
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

## \_\_m128i \_\_lsx_vslt_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vslt_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vslt.h vr, vr, vr
CPU Flags: LSX
```

### Description

Compare the signed 16-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = ((s16)a.half[i] < (s16)b.half[i]) ? 0xFFFF : 0;
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

## \_\_m128i \_\_lsx_vslt_hu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vslt_hu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vslt.hu vr, vr, vr
CPU Flags: LSX
```

### Description

Compare the unsigned 16-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = ((u16)a.half[i] < (u16)b.half[i]) ? 0xFFFF : 0;
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

## \_\_m128i \_\_lsx_vslt_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vslt_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vslt.w vr, vr, vr
CPU Flags: LSX
```

### Description

Compare the signed 32-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = ((s32)a.word[i] < (s32)b.word[i]) ? 0xFFFFFFFF : 0;
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

## \_\_m128i \_\_lsx_vslt_wu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vslt_wu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vslt.wu vr, vr, vr
CPU Flags: LSX
```

### Description

Compare the unsigned 32-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = ((u32)a.word[i] < (u32)b.word[i]) ? 0xFFFFFFFF : 0;
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

## \_\_m128i \_\_lsx_vslt_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vslt_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vslt.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compare the signed 64-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = ((s64)a.dword[i] < (s64)b.dword[i]) ? 0xFFFFFFFFFFFFFFFF : 0;
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

## \_\_m128i \_\_lsx_vslt_du (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vslt_du (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vslt.du vr, vr, vr
CPU Flags: LSX
```

### Description

Compare the unsigned 64-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = ((u64)a.dword[i] < (u64)b.dword[i]) ? 0xFFFFFFFFFFFFFFFF : 0;
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

## \_\_m128i \_\_lsx_vslti_b (\_\_m128i a, imm_n16_15 imm)

### Synopsis

```
__m128i __lsx_vslti_b (__m128i a, imm_n16_15 imm)
#include <lsxintrin.h>
Instruction: vslti.b vr, vr, imm
CPU Flags: LSX
```

### Description

Compare the signed 8-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = ((s8)a.byte[i] < imm) ? 0xFF : 0;
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

## \_\_m128i \_\_lsx_vslti_bu (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vslti_bu (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vslti.bu vr, vr, imm
CPU Flags: LSX
```

### Description

Compare the unsigned 8-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = ((u8)a.byte[i] < imm) ? 0xFF : 0;
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

## \_\_m128i \_\_lsx_vslti_h (\_\_m128i a, imm_n16_15 imm)

### Synopsis

```
__m128i __lsx_vslti_h (__m128i a, imm_n16_15 imm)
#include <lsxintrin.h>
Instruction: vslti.h vr, vr, imm
CPU Flags: LSX
```

### Description

Compare the signed 16-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = ((s16)a.half[i] < imm) ? 0xFFFF : 0;
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

## \_\_m128i \_\_lsx_vslti_hu (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vslti_hu (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vslti.hu vr, vr, imm
CPU Flags: LSX
```

### Description

Compare the unsigned 16-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = ((u16)a.half[i] < imm) ? 0xFFFF : 0;
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

## \_\_m128i \_\_lsx_vslti_w (\_\_m128i a, imm_n16_15 imm)

### Synopsis

```
__m128i __lsx_vslti_w (__m128i a, imm_n16_15 imm)
#include <lsxintrin.h>
Instruction: vslti.w vr, vr, imm
CPU Flags: LSX
```

### Description

Compare the signed 32-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = ((s32)a.word[i] < imm) ? 0xFFFFFFFF : 0;
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

## \_\_m128i \_\_lsx_vslti_wu (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vslti_wu (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vslti.wu vr, vr, imm
CPU Flags: LSX
```

### Description

Compare the unsigned 32-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = ((u32)a.word[i] < imm) ? 0xFFFFFFFF : 0;
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

## \_\_m128i \_\_lsx_vslti_d (\_\_m128i a, imm_n16_15 imm)

### Synopsis

```
__m128i __lsx_vslti_d (__m128i a, imm_n16_15 imm)
#include <lsxintrin.h>
Instruction: vslti.d vr, vr, imm
CPU Flags: LSX
```

### Description

Compare the signed 64-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = ((s64)a.dword[i] < imm) ? 0xFFFFFFFFFFFFFFFF : 0;
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

## \_\_m128i \_\_lsx_vslti_du (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vslti_du (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vslti.du vr, vr, imm
CPU Flags: LSX
```

### Description

Compare the unsigned 64-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = ((u64)a.dword[i] < imm) ? 0xFFFFFFFFFFFFFFFF : 0;
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

## \_\_m128i \_\_lsx_vsle_b (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsle_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsle.b vr, vr, vr
CPU Flags: LSX
```

### Description

Compare the signed 8-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = ((s8)a.byte[i] <= (s8)b.byte[i]) ? 0xFF : 0;
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

## \_\_m128i \_\_lsx_vsle_bu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsle_bu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsle.bu vr, vr, vr
CPU Flags: LSX
```

### Description

Compare the unsigned 8-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = ((u8)a.byte[i] <= (u8)b.byte[i]) ? 0xFF : 0;
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

## \_\_m128i \_\_lsx_vsle_h (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsle_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsle.h vr, vr, vr
CPU Flags: LSX
```

### Description

Compare the signed 16-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = ((s16)a.half[i] <= (s16)b.half[i]) ? 0xFFFF : 0;
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

## \_\_m128i \_\_lsx_vsle_hu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsle_hu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsle.hu vr, vr, vr
CPU Flags: LSX
```

### Description

Compare the unsigned 16-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = ((u16)a.half[i] <= (u16)b.half[i]) ? 0xFFFF : 0;
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

## \_\_m128i \_\_lsx_vsle_w (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsle_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsle.w vr, vr, vr
CPU Flags: LSX
```

### Description

Compare the signed 32-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = ((s32)a.word[i] <= (s32)b.word[i]) ? 0xFFFFFFFF : 0;
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

## \_\_m128i \_\_lsx_vsle_wu (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsle_wu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsle.wu vr, vr, vr
CPU Flags: LSX
```

### Description

Compare the unsigned 32-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = ((u32)a.word[i] <= (u32)b.word[i]) ? 0xFFFFFFFF : 0;
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

## \_\_m128i \_\_lsx_vsle_d (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsle_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsle.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compare the signed 64-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = ((s64)a.dword[i] <= (s64)b.dword[i]) ? 0xFFFFFFFFFFFFFFFF : 0;
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

## \_\_m128i \_\_lsx_vsle_du (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vsle_du (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsle.du vr, vr, vr
CPU Flags: LSX
```

### Description

Compare the unsigned 64-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = ((u64)a.dword[i] <= (u64)b.dword[i]) ? 0xFFFFFFFFFFFFFFFF : 0;
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

## \_\_m128i \_\_lsx_vslei_b (\_\_m128i a, imm_n16_15 imm)

### Synopsis

```
__m128i __lsx_vslei_b (__m128i a, imm_n16_15 imm)
#include <lsxintrin.h>
Instruction: vslei.b vr, vr, imm
CPU Flags: LSX
```

### Description

Compare the signed 8-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = ((s8)a.byte[i] <= imm) ? 0xFF : 0;
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

## \_\_m128i \_\_lsx_vslei_bu (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vslei_bu (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vslei.bu vr, vr, imm
CPU Flags: LSX
```

### Description

Compare the unsigned 8-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = ((u8)a.byte[i] <= imm) ? 0xFF : 0;
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

## \_\_m128i \_\_lsx_vslei_h (\_\_m128i a, imm_n16_15 imm)

### Synopsis

```
__m128i __lsx_vslei_h (__m128i a, imm_n16_15 imm)
#include <lsxintrin.h>
Instruction: vslei.h vr, vr, imm
CPU Flags: LSX
```

### Description

Compare the signed 16-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = ((s16)a.half[i] <= imm) ? 0xFFFF : 0;
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

## \_\_m128i \_\_lsx_vslei_hu (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vslei_hu (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vslei.hu vr, vr, imm
CPU Flags: LSX
```

### Description

Compare the unsigned 16-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = ((u16)a.half[i] <= imm) ? 0xFFFF : 0;
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

## \_\_m128i \_\_lsx_vslei_w (\_\_m128i a, imm_n16_15 imm)

### Synopsis

```
__m128i __lsx_vslei_w (__m128i a, imm_n16_15 imm)
#include <lsxintrin.h>
Instruction: vslei.w vr, vr, imm
CPU Flags: LSX
```

### Description

Compare the signed 32-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = ((s32)a.word[i] <= imm) ? 0xFFFFFFFF : 0;
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

## \_\_m128i \_\_lsx_vslei_wu (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vslei_wu (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vslei.wu vr, vr, imm
CPU Flags: LSX
```

### Description

Compare the unsigned 32-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = ((u32)a.word[i] <= imm) ? 0xFFFFFFFF : 0;
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

## \_\_m128i \_\_lsx_vslei_d (\_\_m128i a, imm_n16_15 imm)

### Synopsis

```
__m128i __lsx_vslei_d (__m128i a, imm_n16_15 imm)
#include <lsxintrin.h>
Instruction: vslei.d vr, vr, imm
CPU Flags: LSX
```

### Description

Compare the signed 64-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = ((s64)a.dword[i] <= imm) ? 0xFFFFFFFFFFFFFFFF : 0;
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

## \_\_m128i \_\_lsx_vslei_du (\_\_m128i a, imm0_31 imm)

### Synopsis

```
__m128i __lsx_vslei_du (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vslei.du vr, vr, imm
CPU Flags: LSX
```

### Description

Compare the unsigned 64-bit elements in `a` and `imm`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = ((u64)a.dword[i] <= imm) ? 0xFFFFFFFFFFFFFFFF : 0;
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
