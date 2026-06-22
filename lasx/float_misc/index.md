# Floating Point Misc

## \_\_m256i \_\_lasx_xvfclass_d (\_\_m256d a)

### Synopsis

```
__m256i __lasx_xvfclass_d (__m256d a)
#include <lasxintrin.h>
Instruction: xvfclass.d xr, xr
CPU Flags: LASX
```

### Description

Classifiy each double precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = fp_classify(a.fp64[i]);
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256i \_\_lasx_xvfclass_s (\_\_m256 a)

### Synopsis

```
__m256i __lasx_xvfclass_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvfclass.s xr, xr
CPU Flags: LASX
```

### Description

Classifiy each single precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.word[i] = fp_classify(a.fp32[i]);
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256 \_\_lasx_xvfrint_s (\_\_m256 a)

### Synopsis

```
__m256 __lasx_xvfrint_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvfrint.s xr, xr
CPU Flags: LASX
```

### Description

Round single-precision floating point elements in `a` to integers, using current rounding mode specified in `fscr`, and store as floating point numbers.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = (f32)(s32)a.fp32[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256d \_\_lasx_xvfrint_d (\_\_m256d a)

### Synopsis

```
__m256d __lasx_xvfrint_d (__m256d a)
#include <lasxintrin.h>
Instruction: xvfrint.d xr, xr
CPU Flags: LASX
```

### Description

Round single-precision floating point elements in `a` to integers, using current rounding mode specified in `fscr`, and store as floating point numbers.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = (f64)(s64)a.fp64[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256 \_\_lasx_xvfrintrp_s (\_\_m256 a)

### Synopsis

```
__m256 __lasx_xvfrintrp_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvfrintrp.s xr, xr
CPU Flags: LASX
```

### Description

Round single-precision floating point elements in `a` to integers, rounding towards positive infinity, and store as floating point numbers.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = (f32)(s32)a.fp32[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256d \_\_lasx_xvfrintrp_d (\_\_m256d a)

### Synopsis

```
__m256d __lasx_xvfrintrp_d (__m256d a)
#include <lasxintrin.h>
Instruction: xvfrintrp.d xr, xr
CPU Flags: LASX
```

### Description

Round single-precision floating point elements in `a` to integers, rounding towards positive infinity, and store as floating point numbers.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = (f64)(s64)a.fp64[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256 \_\_lasx_xvfrintrm_s (\_\_m256 a)

### Synopsis

```
__m256 __lasx_xvfrintrm_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvfrintrm.s xr, xr
CPU Flags: LASX
```

### Description

Round single-precision floating point elements in `a` to integers, rounding towards negative infinity, and store as floating point numbers.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = (f32)(s32)a.fp32[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256d \_\_lasx_xvfrintrm_d (\_\_m256d a)

### Synopsis

```
__m256d __lasx_xvfrintrm_d (__m256d a)
#include <lasxintrin.h>
Instruction: xvfrintrm.d xr, xr
CPU Flags: LASX
```

### Description

Round single-precision floating point elements in `a` to integers, rounding towards negative infinity, and store as floating point numbers.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = (f64)(s64)a.fp64[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256 \_\_lasx_xvfrintrz_s (\_\_m256 a)

### Synopsis

```
__m256 __lasx_xvfrintrz_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvfrintrz.s xr, xr
CPU Flags: LASX
```

### Description

Round single-precision floating point elements in `a` to integers, rounding towards zero, and store as floating point numbers.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = (f32)(s32)a.fp32[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256d \_\_lasx_xvfrintrz_d (\_\_m256d a)

### Synopsis

```
__m256d __lasx_xvfrintrz_d (__m256d a)
#include <lasxintrin.h>
Instruction: xvfrintrz.d xr, xr
CPU Flags: LASX
```

### Description

Round single-precision floating point elements in `a` to integers, rounding towards zero, and store as floating point numbers.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = (f64)(s64)a.fp64[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256 \_\_lasx_xvfrintrne_s (\_\_m256 a)

### Synopsis

```
__m256 __lasx_xvfrintrne_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvfrintrne.s xr, xr
CPU Flags: LASX
```

### Description

Round single-precision floating point elements in `a` to integers, rounding towards nearest even, and store as floating point numbers.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = (f32)(s32)a.fp32[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256d \_\_lasx_xvfrintrne_d (\_\_m256d a)

### Synopsis

```
__m256d __lasx_xvfrintrne_d (__m256d a)
#include <lasxintrin.h>
Instruction: xvfrintrne.d xr, xr
CPU Flags: LASX
```

### Description

Round single-precision floating point elements in `a` to integers, rounding towards nearest even, and store as floating point numbers.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = (f64)(s64)a.fp64[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |
