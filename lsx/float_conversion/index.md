# Floating Point Conversion

## \_\_m128d \_\_lsx_vfcvth_d_s (\_\_m128 a)

### Synopsis

```
__m128d __lsx_vfcvth_d_s (__m128 a)
#include <lsxintrin.h>
Instruction: vfcvth.d.s vr, vr
CPU Flags: LSX
```

### Description

Convert single precision floating point elements in higher half of `a` to double precision.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = a.fp32[i + 2];
}

// Expands to:

if (0) {
  dst.fp64[0] = a.fp32[2];
  dst.fp64[1] = a.fp32[3];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 1                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128d \_\_lsx_vfcvtl_d_s (\_\_m128 a)

### Synopsis

```
__m128d __lsx_vfcvtl_d_s (__m128 a)
#include <lsxintrin.h>
Instruction: vfcvtl.d.s vr, vr
CPU Flags: LSX
```

### Description

Convert single precision floating point elements in lower half of `a` to double precision.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = a.fp32[i];
}

// Expands to:

if (0) {
  dst.fp64[0] = a.fp32[0];
  dst.fp64[1] = a.fp32[1];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 1                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128 \_\_lsx_vfcvt_s_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128 __lsx_vfcvt_s_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfcvt.s.d vr, vr, vr
CPU Flags: LSX
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

// Expands to:

if (0) {
  dst.fp32[0] = b.fp64[0];
  dst.fp32[1] = b.fp64[1];
  dst.fp32[2] = a.fp64[0];
  dst.fp32[3] = a.fp64[1];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 1                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128 \_\_lsx_vfcvth_s_h (\_\_m128i a)

### Synopsis

```
__m128 __lsx_vfcvth_s_h (__m128i a)
#include <lsxintrin.h>
Instruction: vfcvth.s.h vr, vr
CPU Flags: LSX
```

### Description

Convert half precision floating point elements in higher half of `a` to single precision.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = a.fp16[i + 4];
}

// Expands to:

if (0) {
  dst.fp32[0] = a.fp16[4];
  dst.fp32[1] = a.fp16[5];
  dst.fp32[2] = a.fp16[6];
  dst.fp32[3] = a.fp16[7];
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 1                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128 \_\_lsx_vfcvtl_s_h (\_\_m128i a)

### Synopsis

```
__m128 __lsx_vfcvtl_s_h (__m128i a)
#include <lsxintrin.h>
Instruction: vfcvtl.s.h vr, vr
CPU Flags: LSX
```

### Description

Convert half precision floating point elements in lower half of `a` to single precision.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = a.fp16[i];
}

// Expands to:

if (0) {
  dst.fp32[0] = a.fp16[0];
  dst.fp32[1] = a.fp16[1];
  dst.fp32[2] = a.fp16[2];
  dst.fp32[3] = a.fp16[3];
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 1                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vfcvt_h_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128i __lsx_vfcvt_h_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfcvt.h.s vr, vr, vr
CPU Flags: LSX
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
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 1                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

## \_\_m128d \_\_lsx_vffinth_d_w (\_\_m128i a)

### Synopsis

```
__m128d __lsx_vffinth_d_w (__m128i a)
#include <lsxintrin.h>
Instruction: vffinth.d.w vr, vr
CPU Flags: LSX
```

### Description

Convert 32-bit integer elements in higher part of `a` to double precision floating point numbers.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = (f64)(s32)a.word[i + 2]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 5       | 1                |
| 3A6000   | LA664  | 5       | 2                |
| 3C6000   | LA664  | 5       | 2                |
| 2K1000LA | LA264  | 5       | 0.5(1/2)         |
| 2K3000   | LA364E | 5       | 0.5(1/2)         |

## \_\_m128d \_\_lsx_vffintl_d_w (\_\_m128i a)

### Synopsis

```
__m128d __lsx_vffintl_d_w (__m128i a)
#include <lsxintrin.h>
Instruction: vffintl.d.w vr, vr
CPU Flags: LSX
```

### Description

Convert 32-bit integer elements in lower part of `a` to double precision floating point numbers.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = (f64)(s32)a.word[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 5       | 1                |
| 3A6000   | LA664  | 5       | 2                |
| 3C6000   | LA664  | 5       | 2                |
| 2K1000LA | LA264  | 5       | 0.5(1/2)         |
| 2K3000   | LA364E | 5       | 0.5(1/2)         |

## \_\_m128d \_\_lsx_vffint_d_l (\_\_m128i a)

### Synopsis

```
__m128d __lsx_vffint_d_l (__m128i a)
#include <lsxintrin.h>
Instruction: vffint.d.l vr, vr
CPU Flags: LSX
```

### Description

Convert signed 64-bit integer elements in `a` to double-precision floating point numbers.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = (f64)(s64)a.dword[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 4                |
| 3C6000   | LA664  | 4       | 4                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128d \_\_lsx_vffint_d_lu (\_\_m128i a)

### Synopsis

```
__m128d __lsx_vffint_d_lu (__m128i a)
#include <lsxintrin.h>
Instruction: vffint.d.lu vr, vr
CPU Flags: LSX
```

### Description

Convert unsigned 64-bit integer elements in `a` to double-precision floating point numbers.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = (f64)(u64)a.dword[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 4                |
| 3C6000   | LA664  | 4       | 4                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128 \_\_lsx_vffint_s_w (\_\_m128i a)

### Synopsis

```
__m128 __lsx_vffint_s_w (__m128i a)
#include <lsxintrin.h>
Instruction: vffint.s.w vr, vr
CPU Flags: LSX
```

### Description

Convert signed 32-bit integer elements in `a` to single-precision floating point numbers.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = (f32)(s32)a.word[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 4                |
| 3C6000   | LA664  | 4       | 4                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128 \_\_lsx_vffint_s_wu (\_\_m128i a)

### Synopsis

```
__m128 __lsx_vffint_s_wu (__m128i a)
#include <lsxintrin.h>
Instruction: vffint.s.wu vr, vr
CPU Flags: LSX
```

### Description

Convert unsigned 32-bit integer elements in `a` to single-precision floating point numbers.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = (f32)(u32)a.word[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 4                |
| 3C6000   | LA664  | 4       | 4                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128 \_\_lsx_vffint_s_l (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128 __lsx_vffint_s_l (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vffint.s.l vr, vr, vr
CPU Flags: LSX
```

### Description

Convert 64-bit integer elements in `a` and `b` to single-precision floating point numbers.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] =
      (i < 2) ? (f32)(s32)a.dword[i]
              : (f32)(s32)b.dword[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 5       | 1                |
| 3A6000   | LA664  | 5       | 2                |
| 3C6000   | LA664  | 5       | 2                |
| 2K1000LA | LA264  | 5       | 0.5(1/2)         |
| 2K3000   | LA364E | 5       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vftintl_l_s (\_\_m128 a)

### Synopsis

```
__m128i __lsx_vftintl_l_s (__m128 a)
#include <lsxintrin.h>
Instruction: vftintl.l.s vr, vr
CPU Flags: LSX
```

### Description

Convert single-precision floating point elements in lower part of `a` to 64-bit integer, using current rounding mode specified in `fscr`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)a.fp32[i]; // rounding mode is not expressed in C
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 5       | 1                |
| 3A6000   | LA664  | 5       | 2                |
| 3C6000   | LA664  | 5       | 2                |
| 2K1000LA | LA264  | 5       | 0.5(1/2)         |
| 2K3000   | LA364E | 5       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vftinth_l_s (\_\_m128 a)

### Synopsis

```
__m128i __lsx_vftinth_l_s (__m128 a)
#include <lsxintrin.h>
Instruction: vftinth.l.s vr, vr
CPU Flags: LSX
```

### Description

Convert single-precision floating point elements in higher part of `a` to 64-bit integer, using current rounding mode specified in `fscr`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)a.fp32[i + 2]; // rounding mode is not expressed in C
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 5       | 1                |
| 3A6000   | LA664  | 5       | 2                |
| 3C6000   | LA664  | 5       | 2                |
| 2K1000LA | LA264  | 5       | 0.5(1/2)         |
| 2K3000   | LA364E | 5       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vftintrml_l_s (\_\_m128 a)

### Synopsis

```
__m128i __lsx_vftintrml_l_s (__m128 a)
#include <lsxintrin.h>
Instruction: vftintrml.l.s vr, vr
CPU Flags: LSX
```

### Description

Convert single-precision floating point elements in lower part of `a` to 64-bit integer, rounding towards negative infinity.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)a.fp32[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 5       | 1                |
| 3A6000   | LA664  | 5       | 2                |
| 3C6000   | LA664  | 5       | 2                |
| 2K1000LA | LA264  | 5       | 0.5(1/2)         |
| 2K3000   | LA364E | 5       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vftintrmh_l_s (\_\_m128 a)

### Synopsis

```
__m128i __lsx_vftintrmh_l_s (__m128 a)
#include <lsxintrin.h>
Instruction: vftintrmh.l.s vr, vr
CPU Flags: LSX
```

### Description

Convert single-precision floating point elements in higher part of `a` to 64-bit integer, rounding towards negative infinity.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)a.fp32[i + 2]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 5       | 1                |
| 3A6000   | LA664  | 5       | 2                |
| 3C6000   | LA664  | 5       | 2                |
| 2K1000LA | LA264  | 5       | 0.5(1/2)         |
| 2K3000   | LA364E | 5       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vftintrpl_l_s (\_\_m128 a)

### Synopsis

```
__m128i __lsx_vftintrpl_l_s (__m128 a)
#include <lsxintrin.h>
Instruction: vftintrpl.l.s vr, vr
CPU Flags: LSX
```

### Description

Convert single-precision floating point elements in lower part of `a` to 64-bit integer, rounding towards positive infinity.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)a.fp32[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 5       | 1                |
| 3A6000   | LA664  | 5       | 2                |
| 3C6000   | LA664  | 5       | 2                |
| 2K1000LA | LA264  | 5       | 0.5(1/2)         |
| 2K3000   | LA364E | 5       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vftintrph_l_s (\_\_m128 a)

### Synopsis

```
__m128i __lsx_vftintrph_l_s (__m128 a)
#include <lsxintrin.h>
Instruction: vftintrph.l.s vr, vr
CPU Flags: LSX
```

### Description

Convert single-precision floating point elements in higher part of `a` to 64-bit integer, rounding towards positive infinity.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)a.fp32[i + 2]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 5       | 1                |
| 3A6000   | LA664  | 5       | 2                |
| 3C6000   | LA664  | 5       | 2                |
| 2K1000LA | LA264  | 5       | 0.5(1/2)         |
| 2K3000   | LA364E | 5       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vftintrzl_l_s (\_\_m128 a)

### Synopsis

```
__m128i __lsx_vftintrzl_l_s (__m128 a)
#include <lsxintrin.h>
Instruction: vftintrzl.l.s vr, vr
CPU Flags: LSX
```

### Description

Convert single-precision floating point elements in lower part of `a` to 64-bit integer, rounding towards zero.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)a.fp32[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 5       | 1                |
| 3A6000   | LA664  | 5       | 2                |
| 3C6000   | LA664  | 5       | 2                |
| 2K1000LA | LA264  | 5       | 0.5(1/2)         |
| 2K3000   | LA364E | 5       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vftintrzh_l_s (\_\_m128 a)

### Synopsis

```
__m128i __lsx_vftintrzh_l_s (__m128 a)
#include <lsxintrin.h>
Instruction: vftintrzh.l.s vr, vr
CPU Flags: LSX
```

### Description

Convert single-precision floating point elements in higher part of `a` to 64-bit integer, rounding towards zero.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)a.fp32[i + 2]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 5       | 1                |
| 3A6000   | LA664  | 5       | 2                |
| 3C6000   | LA664  | 5       | 2                |
| 2K1000LA | LA264  | 5       | 0.5(1/2)         |
| 2K3000   | LA364E | 5       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vftintrnel_l_s (\_\_m128 a)

### Synopsis

```
__m128i __lsx_vftintrnel_l_s (__m128 a)
#include <lsxintrin.h>
Instruction: vftintrnel.l.s vr, vr
CPU Flags: LSX
```

### Description

Convert single-precision floating point elements in lower part of `a` to 64-bit integer, rounding towards nearest even.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)a.fp32[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 5       | 1                |
| 3A6000   | LA664  | 5       | 2                |
| 3C6000   | LA664  | 5       | 2                |
| 2K1000LA | LA264  | 5       | 0.5(1/2)         |
| 2K3000   | LA364E | 5       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vftintrneh_l_s (\_\_m128 a)

### Synopsis

```
__m128i __lsx_vftintrneh_l_s (__m128 a)
#include <lsxintrin.h>
Instruction: vftintrneh.l.s vr, vr
CPU Flags: LSX
```

### Description

Convert single-precision floating point elements in higher part of `a` to 64-bit integer, rounding towards nearest even.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)a.fp32[i + 2]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 5       | 1                |
| 3A6000   | LA664  | 5       | 2                |
| 3C6000   | LA664  | 5       | 2                |
| 2K1000LA | LA264  | 5       | 0.5(1/2)         |
| 2K3000   | LA364E | 5       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vftint_l_d (\_\_m128d a)

### Synopsis

```
__m128i __lsx_vftint_l_d (__m128d a)
#include <lsxintrin.h>
Instruction: vftint.l.d vr, vr
CPU Flags: LSX
```

### Description

Convert double-precision floating point elements in `a` to signed 64-bit integer, using current rounding mode specified in `fscr`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)a.fp64[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 4                |
| 3C6000   | LA664  | 4       | 4                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vftint_w_s (\_\_m128 a)

### Synopsis

```
__m128i __lsx_vftint_w_s (__m128 a)
#include <lsxintrin.h>
Instruction: vftint.w.s vr, vr
CPU Flags: LSX
```

### Description

Convert single-precision floating point elements in `a` to signed 32-bit integer, using current rounding mode specified in `fscr`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (s32)a.fp32[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 4                |
| 3C6000   | LA664  | 4       | 4                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vftintrm_l_d (\_\_m128d a)

### Synopsis

```
__m128i __lsx_vftintrm_l_d (__m128d a)
#include <lsxintrin.h>
Instruction: vftintrm.l.d vr, vr
CPU Flags: LSX
```

### Description

Convert double-precision floating point elements in `a` to signed 64-bit integer, rounding towards negative infinity.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)a.fp64[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 4                |
| 3C6000   | LA664  | 4       | 4                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vftintrm_w_s (\_\_m128 a)

### Synopsis

```
__m128i __lsx_vftintrm_w_s (__m128 a)
#include <lsxintrin.h>
Instruction: vftintrm.w.s vr, vr
CPU Flags: LSX
```

### Description

Convert single-precision floating point elements in `a` to signed 32-bit integer, rounding towards negative infinity.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (s32)a.fp32[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 4                |
| 3C6000   | LA664  | 4       | 4                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vftintrp_l_d (\_\_m128d a)

### Synopsis

```
__m128i __lsx_vftintrp_l_d (__m128d a)
#include <lsxintrin.h>
Instruction: vftintrp.l.d vr, vr
CPU Flags: LSX
```

### Description

Convert double-precision floating point elements in `a` to signed 64-bit integer, rounding towards positive infinity.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)a.fp64[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 4                |
| 3C6000   | LA664  | 4       | 4                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vftintrp_w_s (\_\_m128 a)

### Synopsis

```
__m128i __lsx_vftintrp_w_s (__m128 a)
#include <lsxintrin.h>
Instruction: vftintrp.w.s vr, vr
CPU Flags: LSX
```

### Description

Convert single-precision floating point elements in `a` to signed 32-bit integer, rounding towards positive infinity.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (s32)a.fp32[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 4                |
| 3C6000   | LA664  | 4       | 4                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vftintrz_l_d (\_\_m128d a)

### Synopsis

```
__m128i __lsx_vftintrz_l_d (__m128d a)
#include <lsxintrin.h>
Instruction: vftintrz.l.d vr, vr
CPU Flags: LSX
```

### Description

Convert double-precision floating point elements in `a` to signed 64-bit integer, rounding towards zero.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)a.fp64[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 4                |
| 3C6000   | LA664  | 4       | 4                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vftintrz_w_s (\_\_m128 a)

### Synopsis

```
__m128i __lsx_vftintrz_w_s (__m128 a)
#include <lsxintrin.h>
Instruction: vftintrz.w.s vr, vr
CPU Flags: LSX
```

### Description

Convert single-precision floating point elements in `a` to signed 32-bit integer, rounding towards zero.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (s32)a.fp32[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 4                |
| 3C6000   | LA664  | 4       | 4                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vftintrne_l_d (\_\_m128d a)

### Synopsis

```
__m128i __lsx_vftintrne_l_d (__m128d a)
#include <lsxintrin.h>
Instruction: vftintrne.l.d vr, vr
CPU Flags: LSX
```

### Description

Convert double-precision floating point elements in `a` to signed 64-bit integer, rounding towards nearest even.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)a.fp64[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 4                |
| 3C6000   | LA664  | 4       | 4                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vftintrne_w_s (\_\_m128 a)

### Synopsis

```
__m128i __lsx_vftintrne_w_s (__m128 a)
#include <lsxintrin.h>
Instruction: vftintrne.w.s vr, vr
CPU Flags: LSX
```

### Description

Convert single-precision floating point elements in `a` to signed 32-bit integer, rounding towards nearest even.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (s32)a.fp32[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 4                |
| 3C6000   | LA664  | 4       | 4                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vftint_lu_d (\_\_m128d a)

### Synopsis

```
__m128i __lsx_vftint_lu_d (__m128d a)
#include <lsxintrin.h>
Instruction: vftint.lu.d vr, vr
CPU Flags: LSX
```

### Description

Convert double-precision floating point elements in `a` to unsigned 64-bit integer, using current rounding mode specified in `fscr`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (u64)a.fp64[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 4                |
| 3C6000   | LA664  | 4       | 4                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vftint_wu_s (\_\_m128 a)

### Synopsis

```
__m128i __lsx_vftint_wu_s (__m128 a)
#include <lsxintrin.h>
Instruction: vftint.wu.s vr, vr
CPU Flags: LSX
```

### Description

Convert single-precision floating point elements in `a` to unsigned 32-bit integer, using current rounding mode specified in `fscr`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (u32)a.fp32[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 4                |
| 3C6000   | LA664  | 4       | 4                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vftintrz_lu_d (\_\_m128d a)

### Synopsis

```
__m128i __lsx_vftintrz_lu_d (__m128d a)
#include <lsxintrin.h>
Instruction: vftintrz.lu.d vr, vr
CPU Flags: LSX
```

### Description

Convert double-precision floating point elements in `a` to unsigned 64-bit integer, rounding towards zero.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (u64)a.fp64[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 4                |
| 3C6000   | LA664  | 4       | 4                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vftintrz_wu_s (\_\_m128 a)

### Synopsis

```
__m128i __lsx_vftintrz_wu_s (__m128 a)
#include <lsxintrin.h>
Instruction: vftintrz.wu.s vr, vr
CPU Flags: LSX
```

### Description

Convert single-precision floating point elements in `a` to unsigned 32-bit integer, rounding towards zero.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (u32)a.fp32[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 4                |
| 3C6000   | LA664  | 4       | 4                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vftint_w_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128i __lsx_vftint_w_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vftint.w.d vr, vr, vr
CPU Flags: LSX
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

// Expands to:

if (0) {
  dst.word[0] = (s32)b.fp64[0];
  dst.word[1] = (s32)b.fp64[1];
  dst.word[2] = (s32)a.fp64[0];
  dst.word[3] = (s32)a.fp64[1];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 5       | 1                |
| 3A6000   | LA664  | 5       | 2                |
| 3C6000   | LA664  | 5       | 2                |
| 2K1000LA | LA264  | 5       | 0.5(1/2)         |
| 2K3000   | LA364E | 5       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vftintrm_w_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128i __lsx_vftintrm_w_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vftintrm.w.d vr, vr, vr
CPU Flags: LSX
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

// Expands to:

if (0) {
  dst.word[0] = (s32)b.fp64[0];
  dst.word[1] = (s32)b.fp64[1];
  dst.word[2] = (s32)a.fp64[0];
  dst.word[3] = (s32)a.fp64[1];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 5       | 1                |
| 3A6000   | LA664  | 5       | 2                |
| 3C6000   | LA664  | 5       | 2                |
| 2K1000LA | LA264  | 5       | 0.5(1/2)         |
| 2K3000   | LA364E | 5       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vftintrp_w_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128i __lsx_vftintrp_w_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vftintrp.w.d vr, vr, vr
CPU Flags: LSX
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

// Expands to:

if (0) {
  dst.word[0] = (s32)b.fp64[0];
  dst.word[1] = (s32)b.fp64[1];
  dst.word[2] = (s32)a.fp64[0];
  dst.word[3] = (s32)a.fp64[1];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 5       | 1                |
| 3A6000   | LA664  | 5       | 2                |
| 3C6000   | LA664  | 5       | 2                |
| 2K1000LA | LA264  | 5       | 0.5(1/2)         |
| 2K3000   | LA364E | 5       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vftintrz_w_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128i __lsx_vftintrz_w_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vftintrz.w.d vr, vr, vr
CPU Flags: LSX
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

// Expands to:

if (0) {
  dst.word[0] = (s32)b.fp64[0];
  dst.word[1] = (s32)b.fp64[1];
  dst.word[2] = (s32)a.fp64[0];
  dst.word[3] = (s32)a.fp64[1];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 5       | 1                |
| 3A6000   | LA664  | 5       | 2                |
| 3C6000   | LA664  | 5       | 2                |
| 2K1000LA | LA264  | 5       | 0.5(1/2)         |
| 2K3000   | LA364E | 5       | 0.5(1/2)         |

## \_\_m128i \_\_lsx_vftintrne_w_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128i __lsx_vftintrne_w_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vftintrne.w.d vr, vr, vr
CPU Flags: LSX
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

// Expands to:

if (0) {
  dst.word[0] = (s32)b.fp64[0];
  dst.word[1] = (s32)b.fp64[1];
  dst.word[2] = (s32)a.fp64[0];
  dst.word[3] = (s32)a.fp64[1];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 5       | 1                |
| 3A6000   | LA664  | 5       | 2                |
| 3C6000   | LA664  | 5       | 2                |
| 2K1000LA | LA264  | 5       | 0.5(1/2)         |
| 2K3000   | LA364E | 5       | 0.5(1/2)         |
