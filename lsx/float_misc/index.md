# Floating Point Misc

## \_\_m128i \_\_lsx_vfclass_d (\_\_m128d a)

### Synopsis

```
__m128i __lsx_vfclass_d (__m128d a)
#include <lsxintrin.h>
Instruction: vfclass.d vr, vr
CPU Flags: LSX
```

### Description

Classifiy each double precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = fp_classify(a.fp64[i]);
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

## \_\_m128i \_\_lsx_vfclass_s (\_\_m128 a)

### Synopsis

```
__m128i __lsx_vfclass_s (__m128 a)
#include <lsxintrin.h>
Instruction: vfclass.s vr, vr
CPU Flags: LSX
```

### Description

Classifiy each single precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = fp_classify(a.fp32[i]);
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

## \_\_m128 \_\_lsx_vfrint_s (\_\_m128 a)

### Synopsis

```
__m128 __lsx_vfrint_s (__m128 a)
#include <lsxintrin.h>
Instruction: vfrint.s vr, vr
CPU Flags: LSX
```

### Description

Round single-precision floating point elements in `a` to integers, using current rounding mode specified in `fscr`, and store as floating point numbers.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = (f32)(s32)a.fp32[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128d \_\_lsx_vfrint_d (\_\_m128d a)

### Synopsis

```
__m128d __lsx_vfrint_d (__m128d a)
#include <lsxintrin.h>
Instruction: vfrint.d vr, vr
CPU Flags: LSX
```

### Description

Round single-precision floating point elements in `a` to integers, using current rounding mode specified in `fscr`, and store as floating point numbers.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = (f64)(s64)a.fp64[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128 \_\_lsx_vfrintrp_s (\_\_m128 a)

### Synopsis

```
__m128 __lsx_vfrintrp_s (__m128 a)
#include <lsxintrin.h>
Instruction: vfrintrp.s vr, vr
CPU Flags: LSX
```

### Description

Round single-precision floating point elements in `a` to integers, rounding towards positive infinity, and store as floating point numbers.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = (f32)(s32)a.fp32[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128d \_\_lsx_vfrintrp_d (\_\_m128d a)

### Synopsis

```
__m128d __lsx_vfrintrp_d (__m128d a)
#include <lsxintrin.h>
Instruction: vfrintrp.d vr, vr
CPU Flags: LSX
```

### Description

Round single-precision floating point elements in `a` to integers, rounding towards positive infinity, and store as floating point numbers.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = (f64)(s64)a.fp64[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128 \_\_lsx_vfrintrm_s (\_\_m128 a)

### Synopsis

```
__m128 __lsx_vfrintrm_s (__m128 a)
#include <lsxintrin.h>
Instruction: vfrintrm.s vr, vr
CPU Flags: LSX
```

### Description

Round single-precision floating point elements in `a` to integers, rounding towards negative infinity, and store as floating point numbers.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = (f32)(s32)a.fp32[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128d \_\_lsx_vfrintrm_d (\_\_m128d a)

### Synopsis

```
__m128d __lsx_vfrintrm_d (__m128d a)
#include <lsxintrin.h>
Instruction: vfrintrm.d vr, vr
CPU Flags: LSX
```

### Description

Round single-precision floating point elements in `a` to integers, rounding towards negative infinity, and store as floating point numbers.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = (f64)(s64)a.fp64[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128 \_\_lsx_vfrintrz_s (\_\_m128 a)

### Synopsis

```
__m128 __lsx_vfrintrz_s (__m128 a)
#include <lsxintrin.h>
Instruction: vfrintrz.s vr, vr
CPU Flags: LSX
```

### Description

Round single-precision floating point elements in `a` to integers, rounding towards zero, and store as floating point numbers.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = (f32)(s32)a.fp32[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128d \_\_lsx_vfrintrz_d (\_\_m128d a)

### Synopsis

```
__m128d __lsx_vfrintrz_d (__m128d a)
#include <lsxintrin.h>
Instruction: vfrintrz.d vr, vr
CPU Flags: LSX
```

### Description

Round single-precision floating point elements in `a` to integers, rounding towards zero, and store as floating point numbers.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = (f64)(s64)a.fp64[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128 \_\_lsx_vfrintrne_s (\_\_m128 a)

### Synopsis

```
__m128 __lsx_vfrintrne_s (__m128 a)
#include <lsxintrin.h>
Instruction: vfrintrne.s vr, vr
CPU Flags: LSX
```

### Description

Round single-precision floating point elements in `a` to integers, rounding towards nearest even, and store as floating point numbers.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = (f32)(s32)a.fp32[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128d \_\_lsx_vfrintrne_d (\_\_m128d a)

### Synopsis

```
__m128d __lsx_vfrintrne_d (__m128d a)
#include <lsxintrin.h>
Instruction: vfrintrne.d vr, vr
CPU Flags: LSX
```

### Description

Round single-precision floating point elements in `a` to integers, rounding towards nearest even, and store as floating point numbers.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = (f64)(s64)a.fp64[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |
