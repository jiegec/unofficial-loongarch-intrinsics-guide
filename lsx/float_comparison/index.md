# Floating Point Comparison

## \_\_m128i \_\_lsx_vfcmp_caf_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128i __lsx_vfcmp_caf_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfcmp.caf.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if AF(Always False), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_caf(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_caf_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128i __lsx_vfcmp_caf_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfcmp.caf.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if AF(Always False), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (fp_compare_caf(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_ceq_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128i __lsx_vfcmp_ceq_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfcmp.ceq.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if EQ(Equal), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_ceq(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_ceq_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128i __lsx_vfcmp_ceq_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfcmp.ceq.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if EQ(Equal), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (fp_compare_ceq(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_cle_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128i __lsx_vfcmp_cle_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfcmp.cle.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if LE(Less than or Equal), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_cle(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_cle_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128i __lsx_vfcmp_cle_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfcmp.cle.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if LE(Less than or Equal), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (fp_compare_cle(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_clt_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128i __lsx_vfcmp_clt_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfcmp.clt.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if LT(Less than), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_clt(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_clt_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128i __lsx_vfcmp_clt_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfcmp.clt.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if LT(Less than), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (fp_compare_clt(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_cne_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128i __lsx_vfcmp_cne_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfcmp.cne.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if NE(Not Equal), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_cne(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_cne_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128i __lsx_vfcmp_cne_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfcmp.cne.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if NE(Not Equal), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (fp_compare_cne(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_cor_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128i __lsx_vfcmp_cor_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfcmp.cor.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if OR(Ordered), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_cor(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_cor_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128i __lsx_vfcmp_cor_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfcmp.cor.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if OR(Ordered), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (fp_compare_cor(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_cueq_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128i __lsx_vfcmp_cueq_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfcmp.cueq.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if UEQ(Unordered or Equal), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_cueq(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_cueq_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128i __lsx_vfcmp_cueq_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfcmp.cueq.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if UEQ(Unordered or Equal), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (fp_compare_cueq(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_cule_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128i __lsx_vfcmp_cule_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfcmp.cule.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if ULE(Unordered, Less than or Equal), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_cule(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_cule_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128i __lsx_vfcmp_cule_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfcmp.cule.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if ULE(Unordered, Less than or Equal), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (fp_compare_cule(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_cult_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128i __lsx_vfcmp_cult_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfcmp.cult.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if ULT(Unordered or Less than), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_cult(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_cult_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128i __lsx_vfcmp_cult_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfcmp.cult.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if ULT(Unordered or Less than), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (fp_compare_cult(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_cun_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128i __lsx_vfcmp_cun_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfcmp.cun.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if UN(Unordered), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_cun(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_cun_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128i __lsx_vfcmp_cun_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfcmp.cun.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if UN(Unordered), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (fp_compare_cun(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_cune_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128i __lsx_vfcmp_cune_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfcmp.cune.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if UNE(Unordered or Not Equal), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_cune(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_cune_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128i __lsx_vfcmp_cune_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfcmp.cune.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if UNE(Unordered or Not Equal), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (fp_compare_cune(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_saf_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128i __lsx_vfcmp_saf_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfcmp.saf.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if AF(Always False), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_saf(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_saf_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128i __lsx_vfcmp_saf_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfcmp.saf.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if AF(Always False), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (fp_compare_saf(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_seq_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128i __lsx_vfcmp_seq_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfcmp.seq.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if EQ(Equal), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_seq(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_seq_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128i __lsx_vfcmp_seq_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfcmp.seq.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if EQ(Equal), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (fp_compare_seq(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_sle_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128i __lsx_vfcmp_sle_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfcmp.sle.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if LE(Less than or Equal), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_sle(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_sle_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128i __lsx_vfcmp_sle_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfcmp.sle.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if LE(Less than or Equal), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (fp_compare_sle(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_slt_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128i __lsx_vfcmp_slt_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfcmp.slt.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if LT(Less than), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_slt(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_slt_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128i __lsx_vfcmp_slt_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfcmp.slt.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if LT(Less than), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (fp_compare_slt(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_sne_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128i __lsx_vfcmp_sne_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfcmp.sne.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if NE(Not Equal), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_sne(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_sne_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128i __lsx_vfcmp_sne_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfcmp.sne.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if NE(Not Equal), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (fp_compare_sne(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_sor_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128i __lsx_vfcmp_sor_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfcmp.sor.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if OR(Ordered), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_sor(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_sor_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128i __lsx_vfcmp_sor_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfcmp.sor.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if OR(Ordered), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (fp_compare_sor(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_sueq_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128i __lsx_vfcmp_sueq_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfcmp.sueq.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if UEQ(Unordered or Equal), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_sueq(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_sueq_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128i __lsx_vfcmp_sueq_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfcmp.sueq.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if UEQ(Unordered or Equal), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (fp_compare_sueq(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_sule_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128i __lsx_vfcmp_sule_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfcmp.sule.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if ULE(Unordered, Less than or Equal), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_sule(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_sule_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128i __lsx_vfcmp_sule_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfcmp.sule.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if ULE(Unordered, Less than or Equal), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (fp_compare_sule(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_sult_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128i __lsx_vfcmp_sult_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfcmp.sult.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if ULT(Unordered or Less than), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_sult(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_sult_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128i __lsx_vfcmp_sult_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfcmp.sult.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if ULT(Unordered or Less than), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (fp_compare_sult(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_sun_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128i __lsx_vfcmp_sun_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfcmp.sun.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if UN(Unordered), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_sun(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_sun_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128i __lsx_vfcmp_sun_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfcmp.sun.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if UN(Unordered), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (fp_compare_sun(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_sune_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128i __lsx_vfcmp_sune_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfcmp.sune.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if UNE(Unordered or Not Equal), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_sune(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

## \_\_m128i \_\_lsx_vfcmp_sune_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128i __lsx_vfcmp_sune_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfcmp.sune.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if UNE(Unordered or Not Equal), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (fp_compare_sune(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |
