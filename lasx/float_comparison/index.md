# Floating Point Comparison

## \_\_m256i \_\_lasx_xvfcmp_caf_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256i __lasx_xvfcmp_caf_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfcmp.caf.s xr, xr, xr
CPU Flags: LASX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if AF(Always False), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (fp_compare_caf(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_caf_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256i __lasx_xvfcmp_caf_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfcmp.caf.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if AF(Always False), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_caf(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_ceq_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256i __lasx_xvfcmp_ceq_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfcmp.ceq.s xr, xr, xr
CPU Flags: LASX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if EQ(Equal), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (fp_compare_ceq(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_ceq_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256i __lasx_xvfcmp_ceq_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfcmp.ceq.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if EQ(Equal), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_ceq(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_cle_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256i __lasx_xvfcmp_cle_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfcmp.cle.s xr, xr, xr
CPU Flags: LASX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if LE(Less than or Equal), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (fp_compare_cle(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_cle_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256i __lasx_xvfcmp_cle_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfcmp.cle.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if LE(Less than or Equal), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_cle(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_clt_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256i __lasx_xvfcmp_clt_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfcmp.clt.s xr, xr, xr
CPU Flags: LASX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if LT(Less than), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (fp_compare_clt(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_clt_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256i __lasx_xvfcmp_clt_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfcmp.clt.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if LT(Less than), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_clt(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_cne_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256i __lasx_xvfcmp_cne_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfcmp.cne.s xr, xr, xr
CPU Flags: LASX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if NE(Not Equal), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (fp_compare_cne(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_cne_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256i __lasx_xvfcmp_cne_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfcmp.cne.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if NE(Not Equal), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_cne(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_cor_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256i __lasx_xvfcmp_cor_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfcmp.cor.s xr, xr, xr
CPU Flags: LASX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if OR(Ordered), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (fp_compare_cor(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_cor_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256i __lasx_xvfcmp_cor_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfcmp.cor.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if OR(Ordered), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_cor(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_cueq_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256i __lasx_xvfcmp_cueq_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfcmp.cueq.s xr, xr, xr
CPU Flags: LASX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if UEQ(Unordered or Equal), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (fp_compare_cueq(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_cueq_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256i __lasx_xvfcmp_cueq_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfcmp.cueq.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if UEQ(Unordered or Equal), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_cueq(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_cule_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256i __lasx_xvfcmp_cule_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfcmp.cule.s xr, xr, xr
CPU Flags: LASX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if ULE(Unordered, Less than or Equal), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (fp_compare_cule(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_cule_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256i __lasx_xvfcmp_cule_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfcmp.cule.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if ULE(Unordered, Less than or Equal), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_cule(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_cult_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256i __lasx_xvfcmp_cult_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfcmp.cult.s xr, xr, xr
CPU Flags: LASX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if ULT(Unordered or Less than), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (fp_compare_cult(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_cult_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256i __lasx_xvfcmp_cult_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfcmp.cult.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if ULT(Unordered or Less than), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_cult(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_cun_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256i __lasx_xvfcmp_cun_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfcmp.cun.s xr, xr, xr
CPU Flags: LASX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if UN(Unordered), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (fp_compare_cun(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_cun_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256i __lasx_xvfcmp_cun_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfcmp.cun.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if UN(Unordered), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_cun(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_cune_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256i __lasx_xvfcmp_cune_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfcmp.cune.s xr, xr, xr
CPU Flags: LASX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if UNE(Unordered or Not Equal), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (fp_compare_cune(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_cune_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256i __lasx_xvfcmp_cune_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfcmp.cune.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if UNE(Unordered or Not Equal), all zeros otherwise) into `dst`. Do not trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_cune(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_saf_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256i __lasx_xvfcmp_saf_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfcmp.saf.s xr, xr, xr
CPU Flags: LASX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if AF(Always False), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (fp_compare_saf(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_saf_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256i __lasx_xvfcmp_saf_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfcmp.saf.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if AF(Always False), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_saf(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_seq_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256i __lasx_xvfcmp_seq_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfcmp.seq.s xr, xr, xr
CPU Flags: LASX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if EQ(Equal), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (fp_compare_seq(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_seq_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256i __lasx_xvfcmp_seq_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfcmp.seq.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if EQ(Equal), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_seq(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_sle_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256i __lasx_xvfcmp_sle_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfcmp.sle.s xr, xr, xr
CPU Flags: LASX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if LE(Less than or Equal), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (fp_compare_sle(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_sle_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256i __lasx_xvfcmp_sle_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfcmp.sle.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if LE(Less than or Equal), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_sle(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_slt_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256i __lasx_xvfcmp_slt_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfcmp.slt.s xr, xr, xr
CPU Flags: LASX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if LT(Less than), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (fp_compare_slt(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_slt_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256i __lasx_xvfcmp_slt_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfcmp.slt.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if LT(Less than), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_slt(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_sne_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256i __lasx_xvfcmp_sne_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfcmp.sne.s xr, xr, xr
CPU Flags: LASX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if NE(Not Equal), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (fp_compare_sne(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_sne_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256i __lasx_xvfcmp_sne_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfcmp.sne.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if NE(Not Equal), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_sne(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_sor_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256i __lasx_xvfcmp_sor_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfcmp.sor.s xr, xr, xr
CPU Flags: LASX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if OR(Ordered), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (fp_compare_sor(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_sor_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256i __lasx_xvfcmp_sor_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfcmp.sor.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if OR(Ordered), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_sor(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_sueq_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256i __lasx_xvfcmp_sueq_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfcmp.sueq.s xr, xr, xr
CPU Flags: LASX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if UEQ(Unordered or Equal), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (fp_compare_sueq(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_sueq_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256i __lasx_xvfcmp_sueq_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfcmp.sueq.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if UEQ(Unordered or Equal), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_sueq(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_sule_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256i __lasx_xvfcmp_sule_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfcmp.sule.s xr, xr, xr
CPU Flags: LASX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if ULE(Unordered, Less than or Equal), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (fp_compare_sule(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_sule_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256i __lasx_xvfcmp_sule_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfcmp.sule.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if ULE(Unordered, Less than or Equal), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_sule(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_sult_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256i __lasx_xvfcmp_sult_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfcmp.sult.s xr, xr, xr
CPU Flags: LASX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if ULT(Unordered or Less than), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (fp_compare_sult(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_sult_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256i __lasx_xvfcmp_sult_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfcmp.sult.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if ULT(Unordered or Less than), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_sult(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_sun_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256i __lasx_xvfcmp_sun_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfcmp.sun.s xr, xr, xr
CPU Flags: LASX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if UN(Unordered), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (fp_compare_sun(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_sun_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256i __lasx_xvfcmp_sun_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfcmp.sun.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if UN(Unordered), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_sun(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_sune_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256i __lasx_xvfcmp_sune_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfcmp.sune.s xr, xr, xr
CPU Flags: LASX
```

### Description

Compare single precision elements in `a` and `b`, save the comparison result (all ones if UNE(Unordered or Not Equal), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 8; i++) {
  if (fp_compare_sune(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfcmp_sune_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256i __lasx_xvfcmp_sune_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfcmp.sune.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compare double precision elements in `a` and `b`, save the comparison result (all ones if UNE(Unordered or Not Equal), all zeros otherwise) into `dst`. Trap for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_sune(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
  }
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |
