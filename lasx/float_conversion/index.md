# Floating Point Conversion

## \_\_m256d \_\_lasx_xvfcvth_d_s (\_\_m256 a)

### Synopsis

```
__m256d __lasx_xvfcvth_d_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvfcvth.d.s xr, xr
CPU Flags: LASX
```

### Description

Convert single precision floating point elements in higher half of `a` to double precision.

### Operation

```
int i;
for (i = 0; i < 2; i++) {
  dst.fp64[i] = a.fp32[i + 2];
}
for (; i < 4; i++) {
  dst.fp64[i] = a.fp32[i + 4];
}

// Expands to:

if (0) {
  dst.fp64[0] = a.fp32[2];
  dst.fp64[1] = a.fp32[3];
  dst.fp64[2] = a.fp32[6];
  dst.fp64[3] = a.fp32[7];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 1                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256d \_\_lasx_xvfcvtl_d_s (\_\_m256 a)

### Synopsis

```
__m256d __lasx_xvfcvtl_d_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvfcvtl.d.s xr, xr
CPU Flags: LASX
```

### Description

Convert single precision floating point elements in lower half of `a` to double precision.

### Operation

```
int i;
for (i = 0; i < 2; i++) {
  dst.fp64[i] = a.fp32[i];
}
for (; i < 4; i++) {
  dst.fp64[i] = a.fp32[i + 2];
}

// Expands to:

if (0) {
  dst.fp64[0] = a.fp32[0];
  dst.fp64[1] = a.fp32[1];
  dst.fp64[2] = a.fp32[4];
  dst.fp64[3] = a.fp32[5];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 1                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256 \_\_lasx_xvfcvt_s_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256 __lasx_xvfcvt_s_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfcvt.s.d xr, xr, xr
CPU Flags: LASX
```

### Description

Convert double precision floating point elements in `a` and `b` to single precision.

### Operation

```
int i;
for (i = 0; i < 2; i++) {
  dst.fp32[i] = b.fp64[i];
}
for (; i < 4; i++) {
  dst.fp32[i] = a.fp64[i - 2];
}
for (; i < 6; i++) {
  dst.fp32[i] = b.fp64[i - 2];
}
for (; i < 8; i++) {
  dst.fp32[i] = a.fp64[i - 4];
}

// Expands to:

if (0) {
  dst.fp32[0] = b.fp64[0];
  dst.fp32[1] = b.fp64[1];
  dst.fp32[2] = a.fp64[0];
  dst.fp32[3] = a.fp64[1];
  dst.fp32[4] = b.fp64[2];
  dst.fp32[5] = b.fp64[3];
  dst.fp32[6] = a.fp64[2];
  dst.fp32[7] = a.fp64[3];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 1                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256 \_\_lasx_xvfcvth_s_h (\_\_m256i a)

### Synopsis

```
__m256 __lasx_xvfcvth_s_h (__m256i a)
#include <lasxintrin.h>
Instruction: xvfcvth.s.h xr, xr
CPU Flags: LASX
```

### Description

Convert half precision floating point elements in higher half of `a` to single precision.

### Operation

```
int i;
for (i = 0; i < 4; i++) {
  dst.fp32[i] = a.fp16[i + 4];
}
for (; i < 8; i++) {
  dst.fp32[i] = a.fp16[i + 8];
}

// Expands to:

if (0) {
  dst.fp32[0] = a.fp16[4];
  dst.fp32[1] = a.fp16[5];
  dst.fp32[2] = a.fp16[6];
  dst.fp32[3] = a.fp16[7];
  dst.fp32[4] = a.fp16[12];
  dst.fp32[5] = a.fp16[13];
  dst.fp32[6] = a.fp16[14];
  dst.fp32[7] = a.fp16[15];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 1                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256 \_\_lasx_xvfcvtl_s_h (\_\_m256i a)

### Synopsis

```
__m256 __lasx_xvfcvtl_s_h (__m256i a)
#include <lasxintrin.h>
Instruction: xvfcvtl.s.h xr, xr
CPU Flags: LASX
```

### Description

Convert half precision floating point elements in lower half of `a` to single precision.

### Operation

```
int i;
for (i = 0; i < 4; i++) {
  dst.fp32[i] = a.fp16[i];
}
for (; i < 8; i++) {
  dst.fp32[i] = a.fp16[i + 4];
}

// Expands to:

if (0) {
  dst.fp32[0] = a.fp16[0];
  dst.fp32[1] = a.fp16[1];
  dst.fp32[2] = a.fp16[2];
  dst.fp32[3] = a.fp16[3];
  dst.fp32[4] = a.fp16[8];
  dst.fp32[5] = a.fp16[9];
  dst.fp32[6] = a.fp16[10];
  dst.fp32[7] = a.fp16[11];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 1                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256i \_\_lasx_xvfcvt_h_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256i __lasx_xvfcvt_h_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfcvt.h.s xr, xr, xr
CPU Flags: LASX
```

### Description

Convert single precision floating point elements in `a` and `b` to half precision.

### Operation

```
int i;
for (i = 0; i < 4; i++) {
  dst.fp16[i] = b.fp32[i];
}
for (; i < 8; i++) {
  dst.fp16[i] = a.fp32[i - 4];
}
for (; i < 12; i++) {
  dst.fp16[i] = b.fp32[i - 4];
}
for (; i < 16; i++) {
  dst.fp16[i] = a.fp32[i - 8];
}

// Expands to:

if (0) {
  dst.fp16[0] = b.fp32[0];
  dst.fp16[1] = b.fp32[1];
  dst.fp16[2] = b.fp32[2];
  dst.fp16[3] = b.fp32[3];
  dst.fp16[4] = a.fp32[0];
  dst.fp16[5] = a.fp32[1];
  dst.fp16[6] = a.fp32[2];
  dst.fp16[7] = a.fp32[3];
  dst.fp16[8] = b.fp32[4];
  dst.fp16[9] = b.fp32[5];
  dst.fp16[10] = b.fp32[6];
  dst.fp16[11] = b.fp32[7];
  dst.fp16[12] = a.fp32[4];
  dst.fp16[13] = a.fp32[5];
  dst.fp16[14] = a.fp32[6];
  dst.fp16[15] = a.fp32[7];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 1                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

## \_\_m256d \_\_lasx_xvffinth_d_w (\_\_m256i a)

### Synopsis

```
__m256d __lasx_xvffinth_d_w (__m256i a)
#include <lasxintrin.h>
Instruction: xvffinth.d.w xr, xr
CPU Flags: LASX
```

### Description

Convert 32-bit integer elements in higher part of `a` to double precision floating point numbers.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = (f64)(s32)a.word[i + 2]; // rounding mode is not expressed in C
}
for (int i = 2; i < 4; i++) {
  dst.fp64[i] = (f64)(s32)a.word[i + 4]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.fp64[0] = (f64)((s32)a.word[2]);
  dst.fp64[1] = (f64)((s32)a.word[3]);
  dst.fp64[2] = (f64)((s32)a.word[6]);
  dst.fp64[3] = (f64)((s32)a.word[7]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 1                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |

## \_\_m256d \_\_lasx_xvffintl_d_w (\_\_m256i a)

### Synopsis

```
__m256d __lasx_xvffintl_d_w (__m256i a)
#include <lasxintrin.h>
Instruction: xvffintl.d.w xr, xr
CPU Flags: LASX
```

### Description

Convert 32-bit integer elements in lower part of `a` to double precision floating point numbers.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = (f64)(s32)a.word[i]; // rounding mode is not expressed in C
}
for (int i = 2; i < 4; i++) {
  dst.fp64[i] = (f64)(s32)a.word[i + 2]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.fp64[0] = (f64)((s32)a.word[0]);
  dst.fp64[1] = (f64)((s32)a.word[1]);
  dst.fp64[2] = (f64)((s32)a.word[4]);
  dst.fp64[3] = (f64)((s32)a.word[5]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 1                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |

## \_\_m256d \_\_lasx_xvffint_d_l (\_\_m256i a)

### Synopsis

```
__m256d __lasx_xvffint_d_l (__m256i a)
#include <lasxintrin.h>
Instruction: xvffint.d.l xr, xr
CPU Flags: LASX
```

### Description

Convert signed 64-bit integer elements in `a` to double-precision floating point numbers.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = (f64)(s64)a.dword[i]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.fp64[0] = (f64)((s64)a.dword[0]);
  dst.fp64[1] = (f64)((s64)a.dword[1]);
  dst.fp64[2] = (f64)((s64)a.dword[2]);
  dst.fp64[3] = (f64)((s64)a.dword[3]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 4                |
| 3C6000 | LA664 | 4       | 4                |

## \_\_m256d \_\_lasx_xvffint_d_lu (\_\_m256i a)

### Synopsis

```
__m256d __lasx_xvffint_d_lu (__m256i a)
#include <lasxintrin.h>
Instruction: xvffint.d.lu xr, xr
CPU Flags: LASX
```

### Description

Convert unsigned 64-bit integer elements in `a` to double-precision floating point numbers.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = (f64)(u64)a.dword[i]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.fp64[0] = (f64)((u64)a.dword[0]);
  dst.fp64[1] = (f64)((u64)a.dword[1]);
  dst.fp64[2] = (f64)((u64)a.dword[2]);
  dst.fp64[3] = (f64)((u64)a.dword[3]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 4                |
| 3C6000 | LA664 | 4       | 4                |

## \_\_m256 \_\_lasx_xvffint_s_w (\_\_m256i a)

### Synopsis

```
__m256 __lasx_xvffint_s_w (__m256i a)
#include <lasxintrin.h>
Instruction: xvffint.s.w xr, xr
CPU Flags: LASX
```

### Description

Convert signed 32-bit integer elements in `a` to single-precision floating point numbers.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = (f32)(s32)a.word[i]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.fp32[0] = (f32)((s32)a.word[0]);
  dst.fp32[1] = (f32)((s32)a.word[1]);
  dst.fp32[2] = (f32)((s32)a.word[2]);
  dst.fp32[3] = (f32)((s32)a.word[3]);
  dst.fp32[4] = (f32)((s32)a.word[4]);
  dst.fp32[5] = (f32)((s32)a.word[5]);
  dst.fp32[6] = (f32)((s32)a.word[6]);
  dst.fp32[7] = (f32)((s32)a.word[7]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 4                |
| 3C6000 | LA664 | 4       | 4                |

## \_\_m256 \_\_lasx_xvffint_s_wu (\_\_m256i a)

### Synopsis

```
__m256 __lasx_xvffint_s_wu (__m256i a)
#include <lasxintrin.h>
Instruction: xvffint.s.wu xr, xr
CPU Flags: LASX
```

### Description

Convert unsigned 32-bit integer elements in `a` to single-precision floating point numbers.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = (f32)(u32)a.word[i]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.fp32[0] = (f32)((u32)a.word[0]);
  dst.fp32[1] = (f32)((u32)a.word[1]);
  dst.fp32[2] = (f32)((u32)a.word[2]);
  dst.fp32[3] = (f32)((u32)a.word[3]);
  dst.fp32[4] = (f32)((u32)a.word[4]);
  dst.fp32[5] = (f32)((u32)a.word[5]);
  dst.fp32[6] = (f32)((u32)a.word[6]);
  dst.fp32[7] = (f32)((u32)a.word[7]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 4                |
| 3C6000 | LA664 | 4       | 4                |

## \_\_m256 \_\_lasx_xvffint_s_l (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256 __lasx_xvffint_s_l (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvffint.s.l xr, xr, xr
CPU Flags: LASX
```

### Description

Convert 64-bit integer elements in `a` and `b` to single-precision floating point numbers.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] =
      (i < 2) ? (f32)(s64)b.dword[i]
              : (f32)(s64)a.dword[i - 2]; // rounding mode is not expressed in C
}
for (int i = 4; i < 8; i++) {
  dst.fp32[i] =
      (i < 6) ? (f32)(s64)b.dword[i - 2]
              : (f32)(s64)a.dword[i - 4]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.fp32[0] = (f32)((s64)b.dword[0]);
  dst.fp32[1] = (f32)((s64)b.dword[1]);
  dst.fp32[2] = (f32)((s64)a.dword[0]);
  dst.fp32[3] = (f32)((s64)a.dword[1]);
  dst.fp32[4] = (f32)((s64)b.dword[2]);
  dst.fp32[5] = (f32)((s64)b.dword[3]);
  dst.fp32[6] = (f32)((s64)a.dword[2]);
  dst.fp32[7] = (f32)((s64)a.dword[3]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 1                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |

## \_\_m256i \_\_lasx_xvftintl_l_s (\_\_m256 a)

### Synopsis

```
__m256i __lasx_xvftintl_l_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvftintl.l.s xr, xr
CPU Flags: LASX
```

### Description

Convert single-precision floating point elements in lower part of `a` to 64-bit integer, using current rounding mode specified in `fscr`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)a.fp32[i]; // rounding mode is not expressed in C
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = (s64)a.fp32[i + 2]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.dword[0] = (s64)a.fp32[0];
  dst.dword[1] = (s64)a.fp32[1];
  dst.dword[2] = (s64)a.fp32[4];
  dst.dword[3] = (s64)a.fp32[5];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 1                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |

## \_\_m256i \_\_lasx_xvftinth_l_s (\_\_m256 a)

### Synopsis

```
__m256i __lasx_xvftinth_l_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvftinth.l.s xr, xr
CPU Flags: LASX
```

### Description

Convert single-precision floating point elements in higher part of `a` to 64-bit integer, using current rounding mode specified in `fscr`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)a.fp32[i + 2]; // rounding mode is not expressed in C
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = (s64)a.fp32[i + 4]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.dword[0] = (s64)a.fp32[2];
  dst.dword[1] = (s64)a.fp32[3];
  dst.dword[2] = (s64)a.fp32[6];
  dst.dword[3] = (s64)a.fp32[7];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 1                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |

## \_\_m256i \_\_lasx_xvftintrml_l_s (\_\_m256 a)

### Synopsis

```
__m256i __lasx_xvftintrml_l_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvftintrml.l.s xr, xr
CPU Flags: LASX
```

### Description

Convert single-precision floating point elements in lower part of `a` to 64-bit integer, rounding towards negative infinity.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)a.fp32[i]; // rounding mode is not expressed in C
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = (s64)a.fp32[i + 2]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.dword[0] = (s64)a.fp32[0];
  dst.dword[1] = (s64)a.fp32[1];
  dst.dword[2] = (s64)a.fp32[4];
  dst.dword[3] = (s64)a.fp32[5];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 1                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |

## \_\_m256i \_\_lasx_xvftintrmh_l_s (\_\_m256 a)

### Synopsis

```
__m256i __lasx_xvftintrmh_l_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvftintrmh.l.s xr, xr
CPU Flags: LASX
```

### Description

Convert single-precision floating point elements in higher part of `a` to 64-bit integer, rounding towards negative infinity.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)a.fp32[i + 2]; // rounding mode is not expressed in C
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = (s64)a.fp32[i + 4]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.dword[0] = (s64)a.fp32[2];
  dst.dword[1] = (s64)a.fp32[3];
  dst.dword[2] = (s64)a.fp32[6];
  dst.dword[3] = (s64)a.fp32[7];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 1                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |

## \_\_m256i \_\_lasx_xvftintrpl_l_s (\_\_m256 a)

### Synopsis

```
__m256i __lasx_xvftintrpl_l_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvftintrpl.l.s xr, xr
CPU Flags: LASX
```

### Description

Convert single-precision floating point elements in lower part of `a` to 64-bit integer, rounding towards positive infinity.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)a.fp32[i]; // rounding mode is not expressed in C
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = (s64)a.fp32[i + 2]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.dword[0] = (s64)a.fp32[0];
  dst.dword[1] = (s64)a.fp32[1];
  dst.dword[2] = (s64)a.fp32[4];
  dst.dword[3] = (s64)a.fp32[5];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 1                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |

## \_\_m256i \_\_lasx_xvftintrph_l_s (\_\_m256 a)

### Synopsis

```
__m256i __lasx_xvftintrph_l_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvftintrph.l.s xr, xr
CPU Flags: LASX
```

### Description

Convert single-precision floating point elements in higher part of `a` to 64-bit integer, rounding towards positive infinity.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)a.fp32[i + 2]; // rounding mode is not expressed in C
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = (s64)a.fp32[i + 4]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.dword[0] = (s64)a.fp32[2];
  dst.dword[1] = (s64)a.fp32[3];
  dst.dword[2] = (s64)a.fp32[6];
  dst.dword[3] = (s64)a.fp32[7];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 1                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |

## \_\_m256i \_\_lasx_xvftintrzl_l_s (\_\_m256 a)

### Synopsis

```
__m256i __lasx_xvftintrzl_l_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvftintrzl.l.s xr, xr
CPU Flags: LASX
```

### Description

Convert single-precision floating point elements in lower part of `a` to 64-bit integer, rounding towards zero.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)a.fp32[i]; // rounding mode is not expressed in C
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = (s64)a.fp32[i + 2]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.dword[0] = (s64)a.fp32[0];
  dst.dword[1] = (s64)a.fp32[1];
  dst.dword[2] = (s64)a.fp32[4];
  dst.dword[3] = (s64)a.fp32[5];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 1                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |

## \_\_m256i \_\_lasx_xvftintrzh_l_s (\_\_m256 a)

### Synopsis

```
__m256i __lasx_xvftintrzh_l_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvftintrzh.l.s xr, xr
CPU Flags: LASX
```

### Description

Convert single-precision floating point elements in higher part of `a` to 64-bit integer, rounding towards zero.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)a.fp32[i + 2]; // rounding mode is not expressed in C
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = (s64)a.fp32[i + 4]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.dword[0] = (s64)a.fp32[2];
  dst.dword[1] = (s64)a.fp32[3];
  dst.dword[2] = (s64)a.fp32[6];
  dst.dword[3] = (s64)a.fp32[7];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 1                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |

## \_\_m256i \_\_lasx_xvftintrnel_l_s (\_\_m256 a)

### Synopsis

```
__m256i __lasx_xvftintrnel_l_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvftintrnel.l.s xr, xr
CPU Flags: LASX
```

### Description

Convert single-precision floating point elements in lower part of `a` to 64-bit integer, rounding towards nearest even.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)a.fp32[i]; // rounding mode is not expressed in C
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = (s64)a.fp32[i + 2]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.dword[0] = (s64)a.fp32[0];
  dst.dword[1] = (s64)a.fp32[1];
  dst.dword[2] = (s64)a.fp32[4];
  dst.dword[3] = (s64)a.fp32[5];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 1                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |

## \_\_m256i \_\_lasx_xvftintrneh_l_s (\_\_m256 a)

### Synopsis

```
__m256i __lasx_xvftintrneh_l_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvftintrneh.l.s xr, xr
CPU Flags: LASX
```

### Description

Convert single-precision floating point elements in higher part of `a` to 64-bit integer, rounding towards nearest even.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)a.fp32[i + 2]; // rounding mode is not expressed in C
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = (s64)a.fp32[i + 4]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.dword[0] = (s64)a.fp32[2];
  dst.dword[1] = (s64)a.fp32[3];
  dst.dword[2] = (s64)a.fp32[6];
  dst.dword[3] = (s64)a.fp32[7];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 1                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |

## \_\_m256i \_\_lasx_xvftint_l_d (\_\_m256d a)

### Synopsis

```
__m256i __lasx_xvftint_l_d (__m256d a)
#include <lasxintrin.h>
Instruction: xvftint.l.d xr, xr
CPU Flags: LASX
```

### Description

Convert double-precision floating point elements in `a` to signed 64-bit integer, using current rounding mode specified in `fscr`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (s64)a.fp64[i]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.dword[0] = (s64)a.fp64[0];
  dst.dword[1] = (s64)a.fp64[1];
  dst.dword[2] = (s64)a.fp64[2];
  dst.dword[3] = (s64)a.fp64[3];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 4                |
| 3C6000 | LA664 | 4       | 4                |

## \_\_m256i \_\_lasx_xvftint_w_s (\_\_m256 a)

### Synopsis

```
__m256i __lasx_xvftint_w_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvftint.w.s xr, xr
CPU Flags: LASX
```

### Description

Convert single-precision floating point elements in `a` to signed 32-bit integer, using current rounding mode specified in `fscr`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (s32)a.fp32[i]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.word[0] = (s32)a.fp32[0];
  dst.word[1] = (s32)a.fp32[1];
  dst.word[2] = (s32)a.fp32[2];
  dst.word[3] = (s32)a.fp32[3];
  dst.word[4] = (s32)a.fp32[4];
  dst.word[5] = (s32)a.fp32[5];
  dst.word[6] = (s32)a.fp32[6];
  dst.word[7] = (s32)a.fp32[7];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 4                |
| 3C6000 | LA664 | 4       | 4                |

## \_\_m256i \_\_lasx_xvftintrm_l_d (\_\_m256d a)

### Synopsis

```
__m256i __lasx_xvftintrm_l_d (__m256d a)
#include <lasxintrin.h>
Instruction: xvftintrm.l.d xr, xr
CPU Flags: LASX
```

### Description

Convert double-precision floating point elements in `a` to signed 64-bit integer, rounding towards negative infinity.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (s64)a.fp64[i]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.dword[0] = (s64)a.fp64[0];
  dst.dword[1] = (s64)a.fp64[1];
  dst.dword[2] = (s64)a.fp64[2];
  dst.dword[3] = (s64)a.fp64[3];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 4                |
| 3C6000 | LA664 | 4       | 4                |

## \_\_m256i \_\_lasx_xvftintrm_w_s (\_\_m256 a)

### Synopsis

```
__m256i __lasx_xvftintrm_w_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvftintrm.w.s xr, xr
CPU Flags: LASX
```

### Description

Convert single-precision floating point elements in `a` to signed 32-bit integer, rounding towards negative infinity.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (s32)a.fp32[i]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.word[0] = (s32)a.fp32[0];
  dst.word[1] = (s32)a.fp32[1];
  dst.word[2] = (s32)a.fp32[2];
  dst.word[3] = (s32)a.fp32[3];
  dst.word[4] = (s32)a.fp32[4];
  dst.word[5] = (s32)a.fp32[5];
  dst.word[6] = (s32)a.fp32[6];
  dst.word[7] = (s32)a.fp32[7];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 4                |
| 3C6000 | LA664 | 4       | 4                |

## \_\_m256i \_\_lasx_xvftintrp_l_d (\_\_m256d a)

### Synopsis

```
__m256i __lasx_xvftintrp_l_d (__m256d a)
#include <lasxintrin.h>
Instruction: xvftintrp.l.d xr, xr
CPU Flags: LASX
```

### Description

Convert double-precision floating point elements in `a` to signed 64-bit integer, rounding towards positive infinity.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (s64)a.fp64[i]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.dword[0] = (s64)a.fp64[0];
  dst.dword[1] = (s64)a.fp64[1];
  dst.dword[2] = (s64)a.fp64[2];
  dst.dword[3] = (s64)a.fp64[3];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 4                |
| 3C6000 | LA664 | 4       | 4                |

## \_\_m256i \_\_lasx_xvftintrp_w_s (\_\_m256 a)

### Synopsis

```
__m256i __lasx_xvftintrp_w_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvftintrp.w.s xr, xr
CPU Flags: LASX
```

### Description

Convert single-precision floating point elements in `a` to signed 32-bit integer, rounding towards positive infinity.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (s32)a.fp32[i]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.word[0] = (s32)a.fp32[0];
  dst.word[1] = (s32)a.fp32[1];
  dst.word[2] = (s32)a.fp32[2];
  dst.word[3] = (s32)a.fp32[3];
  dst.word[4] = (s32)a.fp32[4];
  dst.word[5] = (s32)a.fp32[5];
  dst.word[6] = (s32)a.fp32[6];
  dst.word[7] = (s32)a.fp32[7];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 4                |
| 3C6000 | LA664 | 4       | 4                |

## \_\_m256i \_\_lasx_xvftintrz_l_d (\_\_m256d a)

### Synopsis

```
__m256i __lasx_xvftintrz_l_d (__m256d a)
#include <lasxintrin.h>
Instruction: xvftintrz.l.d xr, xr
CPU Flags: LASX
```

### Description

Convert double-precision floating point elements in `a` to signed 64-bit integer, rounding towards zero.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (s64)a.fp64[i]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.dword[0] = (s64)a.fp64[0];
  dst.dword[1] = (s64)a.fp64[1];
  dst.dword[2] = (s64)a.fp64[2];
  dst.dword[3] = (s64)a.fp64[3];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 4                |
| 3C6000 | LA664 | 4       | 4                |

## \_\_m256i \_\_lasx_xvftintrz_w_s (\_\_m256 a)

### Synopsis

```
__m256i __lasx_xvftintrz_w_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvftintrz.w.s xr, xr
CPU Flags: LASX
```

### Description

Convert single-precision floating point elements in `a` to signed 32-bit integer, rounding towards zero.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (s32)a.fp32[i]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.word[0] = (s32)a.fp32[0];
  dst.word[1] = (s32)a.fp32[1];
  dst.word[2] = (s32)a.fp32[2];
  dst.word[3] = (s32)a.fp32[3];
  dst.word[4] = (s32)a.fp32[4];
  dst.word[5] = (s32)a.fp32[5];
  dst.word[6] = (s32)a.fp32[6];
  dst.word[7] = (s32)a.fp32[7];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 4                |
| 3C6000 | LA664 | 4       | 4                |

## \_\_m256i \_\_lasx_xvftintrne_l_d (\_\_m256d a)

### Synopsis

```
__m256i __lasx_xvftintrne_l_d (__m256d a)
#include <lasxintrin.h>
Instruction: xvftintrne.l.d xr, xr
CPU Flags: LASX
```

### Description

Convert double-precision floating point elements in `a` to signed 64-bit integer, rounding towards nearest even.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (s64)a.fp64[i]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.dword[0] = (s64)a.fp64[0];
  dst.dword[1] = (s64)a.fp64[1];
  dst.dword[2] = (s64)a.fp64[2];
  dst.dword[3] = (s64)a.fp64[3];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 4                |
| 3C6000 | LA664 | 4       | 4                |

## \_\_m256i \_\_lasx_xvftintrne_w_s (\_\_m256 a)

### Synopsis

```
__m256i __lasx_xvftintrne_w_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvftintrne.w.s xr, xr
CPU Flags: LASX
```

### Description

Convert single-precision floating point elements in `a` to signed 32-bit integer, rounding towards nearest even.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (s32)a.fp32[i]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.word[0] = (s32)a.fp32[0];
  dst.word[1] = (s32)a.fp32[1];
  dst.word[2] = (s32)a.fp32[2];
  dst.word[3] = (s32)a.fp32[3];
  dst.word[4] = (s32)a.fp32[4];
  dst.word[5] = (s32)a.fp32[5];
  dst.word[6] = (s32)a.fp32[6];
  dst.word[7] = (s32)a.fp32[7];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 4                |
| 3C6000 | LA664 | 4       | 4                |

## \_\_m256i \_\_lasx_xvftint_lu_d (\_\_m256d a)

### Synopsis

```
__m256i __lasx_xvftint_lu_d (__m256d a)
#include <lasxintrin.h>
Instruction: xvftint.lu.d xr, xr
CPU Flags: LASX
```

### Description

Convert double-precision floating point elements in `a` to unsigned 64-bit integer, using current rounding mode specified in `fscr`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (u64)a.fp64[i]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.dword[0] = (u64)a.fp64[0];
  dst.dword[1] = (u64)a.fp64[1];
  dst.dword[2] = (u64)a.fp64[2];
  dst.dword[3] = (u64)a.fp64[3];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 4                |
| 3C6000 | LA664 | 4       | 4                |

## \_\_m256i \_\_lasx_xvftint_wu_s (\_\_m256 a)

### Synopsis

```
__m256i __lasx_xvftint_wu_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvftint.wu.s xr, xr
CPU Flags: LASX
```

### Description

Convert single-precision floating point elements in `a` to unsigned 32-bit integer, using current rounding mode specified in `fscr`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (u32)a.fp32[i]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.word[0] = (u32)a.fp32[0];
  dst.word[1] = (u32)a.fp32[1];
  dst.word[2] = (u32)a.fp32[2];
  dst.word[3] = (u32)a.fp32[3];
  dst.word[4] = (u32)a.fp32[4];
  dst.word[5] = (u32)a.fp32[5];
  dst.word[6] = (u32)a.fp32[6];
  dst.word[7] = (u32)a.fp32[7];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 4                |
| 3C6000 | LA664 | 4       | 4                |

## \_\_m256i \_\_lasx_xvftintrz_lu_d (\_\_m256d a)

### Synopsis

```
__m256i __lasx_xvftintrz_lu_d (__m256d a)
#include <lasxintrin.h>
Instruction: xvftintrz.lu.d xr, xr
CPU Flags: LASX
```

### Description

Convert double-precision floating point elements in `a` to unsigned 64-bit integer, rounding towards zero.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (u64)a.fp64[i]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.dword[0] = (u64)a.fp64[0];
  dst.dword[1] = (u64)a.fp64[1];
  dst.dword[2] = (u64)a.fp64[2];
  dst.dword[3] = (u64)a.fp64[3];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 4                |
| 3C6000 | LA664 | 4       | 4                |

## \_\_m256i \_\_lasx_xvftintrz_wu_s (\_\_m256 a)

### Synopsis

```
__m256i __lasx_xvftintrz_wu_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvftintrz.wu.s xr, xr
CPU Flags: LASX
```

### Description

Convert single-precision floating point elements in `a` to unsigned 32-bit integer, rounding towards zero.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (u32)a.fp32[i]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.word[0] = (u32)a.fp32[0];
  dst.word[1] = (u32)a.fp32[1];
  dst.word[2] = (u32)a.fp32[2];
  dst.word[3] = (u32)a.fp32[3];
  dst.word[4] = (u32)a.fp32[4];
  dst.word[5] = (u32)a.fp32[5];
  dst.word[6] = (u32)a.fp32[6];
  dst.word[7] = (u32)a.fp32[7];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 4                |
| 3C6000 | LA664 | 4       | 4                |

## \_\_m256i \_\_lasx_xvftint_w_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256i __lasx_xvftint_w_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvftint.w.d xr, xr, xr
CPU Flags: LASX
```

### Description

Convert double-precision floating point elements in `a` and `b` to 32-bit integer, using current rounding mode specified in `fscr`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2)
                    ? (s32)b.fp64[i]
                    : (s32)a.fp64[i - 2]; // rounding mode is not expressed in C
}
for (int i = 4; i < 8; i++) {
  dst.word[i] = (i < 6)
                    ? (s32)b.fp64[i - 2]
                    : (s32)a.fp64[i - 4]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.word[0] = (s32)b.fp64[0];
  dst.word[1] = (s32)b.fp64[1];
  dst.word[2] = (s32)a.fp64[0];
  dst.word[3] = (s32)a.fp64[1];
  dst.word[4] = (s32)b.fp64[2];
  dst.word[5] = (s32)b.fp64[3];
  dst.word[6] = (s32)a.fp64[2];
  dst.word[7] = (s32)a.fp64[3];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 1                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |

## \_\_m256i \_\_lasx_xvftintrm_w_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256i __lasx_xvftintrm_w_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvftintrm.w.d xr, xr, xr
CPU Flags: LASX
```

### Description

Convert double-precision floating point elements in `a` and `b` to 32-bit integer, rounding towards negative infinity.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2)
                    ? (s32)b.fp64[i]
                    : (s32)a.fp64[i - 2]; // rounding mode is not expressed in C
}
for (int i = 4; i < 8; i++) {
  dst.word[i] = (i < 6)
                    ? (s32)b.fp64[i - 2]
                    : (s32)a.fp64[i - 4]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.word[0] = (s32)b.fp64[0];
  dst.word[1] = (s32)b.fp64[1];
  dst.word[2] = (s32)a.fp64[0];
  dst.word[3] = (s32)a.fp64[1];
  dst.word[4] = (s32)b.fp64[2];
  dst.word[5] = (s32)b.fp64[3];
  dst.word[6] = (s32)a.fp64[2];
  dst.word[7] = (s32)a.fp64[3];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 1                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |

## \_\_m256i \_\_lasx_xvftintrp_w_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256i __lasx_xvftintrp_w_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvftintrp.w.d xr, xr, xr
CPU Flags: LASX
```

### Description

Convert double-precision floating point elements in `a` and `b` to 32-bit integer, rounding towards positive infinity.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2)
                    ? (s32)b.fp64[i]
                    : (s32)a.fp64[i - 2]; // rounding mode is not expressed in C
}
for (int i = 4; i < 8; i++) {
  dst.word[i] = (i < 6)
                    ? (s32)b.fp64[i - 2]
                    : (s32)a.fp64[i - 4]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.word[0] = (s32)b.fp64[0];
  dst.word[1] = (s32)b.fp64[1];
  dst.word[2] = (s32)a.fp64[0];
  dst.word[3] = (s32)a.fp64[1];
  dst.word[4] = (s32)b.fp64[2];
  dst.word[5] = (s32)b.fp64[3];
  dst.word[6] = (s32)a.fp64[2];
  dst.word[7] = (s32)a.fp64[3];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 1                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |

## \_\_m256i \_\_lasx_xvftintrz_w_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256i __lasx_xvftintrz_w_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvftintrz.w.d xr, xr, xr
CPU Flags: LASX
```

### Description

Convert double-precision floating point elements in `a` and `b` to 32-bit integer, rounding towards zero.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2)
                    ? (s32)b.fp64[i]
                    : (s32)a.fp64[i - 2]; // rounding mode is not expressed in C
}
for (int i = 4; i < 8; i++) {
  dst.word[i] = (i < 6)
                    ? (s32)b.fp64[i - 2]
                    : (s32)a.fp64[i - 4]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.word[0] = (s32)b.fp64[0];
  dst.word[1] = (s32)b.fp64[1];
  dst.word[2] = (s32)a.fp64[0];
  dst.word[3] = (s32)a.fp64[1];
  dst.word[4] = (s32)b.fp64[2];
  dst.word[5] = (s32)b.fp64[3];
  dst.word[6] = (s32)a.fp64[2];
  dst.word[7] = (s32)a.fp64[3];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 1                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |

## \_\_m256i \_\_lasx_xvftintrne_w_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256i __lasx_xvftintrne_w_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvftintrne.w.d xr, xr, xr
CPU Flags: LASX
```

### Description

Convert double-precision floating point elements in `a` and `b` to 32-bit integer, rounding towards nearest even.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2)
                    ? (s32)b.fp64[i]
                    : (s32)a.fp64[i - 2]; // rounding mode is not expressed in C
}
for (int i = 4; i < 8; i++) {
  dst.word[i] = (i < 6)
                    ? (s32)b.fp64[i - 2]
                    : (s32)a.fp64[i - 4]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.word[0] = (s32)b.fp64[0];
  dst.word[1] = (s32)b.fp64[1];
  dst.word[2] = (s32)a.fp64[0];
  dst.word[3] = (s32)a.fp64[1];
  dst.word[4] = (s32)b.fp64[2];
  dst.word[5] = (s32)b.fp64[3];
  dst.word[6] = (s32)a.fp64[2];
  dst.word[7] = (s32)a.fp64[3];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 1                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |
