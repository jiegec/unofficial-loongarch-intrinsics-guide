# Floating Point Computation

## \_\_m128 \_\_lsx_vfadd_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128 __lsx_vfadd_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfadd.s vr, vr, vr
CPU Flags: LSX
```

### Description

Add single precision floating point elements in `a` to elements in `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = a.fp32[i] + b.fp32[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 5       | 2                |
| 3A6000   | LA664  | 3       | 4                |
| 3C6000   | LA664  | 3       | 4                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128d \_\_lsx_vfadd_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128d __lsx_vfadd_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfadd.d vr, vr, vr
CPU Flags: LSX
```

### Description

Add double precision floating point elements in `a` to elements in `b`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = a.fp64[i] + b.fp64[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 5       | 2                |
| 3A6000   | LA664  | 3       | 4                |
| 3C6000   | LA664  | 3       | 4                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128 \_\_lsx_vfdiv_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128 __lsx_vfdiv_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfdiv.s vr, vr, vr
CPU Flags: LSX
```

### Description

Divide single precision floating point elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = a.fp32[i] / b.fp32[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency  | Throughput (IPC) |
| -------- | ------ | -------- | ---------------- |
| 3C5000   | LA464  | 11, 19.5 | 0.13(1/7.5)      |
| 3A6000   | LA664  | 11       | 0.18(1/5.5)      |
| 3C6000   | LA664  | 11       | 0.22(1/4.5)      |
| 2K1000LA | LA264  | 12, 28   | 0.04(1/28)       |
| 2K3000   | LA364E | 12, 28   | 0.04(1/28)       |

## \_\_m128d \_\_lsx_vfdiv_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128d __lsx_vfdiv_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfdiv.d vr, vr, vr
CPU Flags: LSX
```

### Description

Divide double precision floating point elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = a.fp64[i] / b.fp64[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 8, 16.5 | 0.08(1/12.5)     |
| 3A6000   | LA664  | 8, 21.5 | 0.25(1/4)        |
| 3C6000   | LA664  | 8, 16.5 | 0.33(1/3)        |
| 2K1000LA | LA264  | 9, 24   | 0.04(1/24)       |
| 2K3000   | LA364E | 9, 24   | 0.04(1/24)       |

## \_\_m128 \_\_lsx_vfmax_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128 __lsx_vfmax_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfmax.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compute maximum of single precision floating point elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = fmax(a.fp32[i], b.fp32[i]);
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

## \_\_m128d \_\_lsx_vfmax_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128d __lsx_vfmax_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfmax.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compute maximum of double precision floating point elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = fmax(a.fp64[i], b.fp64[i]);
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

## \_\_m128 \_\_lsx_vfmaxa_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128 __lsx_vfmaxa_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfmaxa.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compute maximum of single precision floating point elements in `a` and `b` by magnitude.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (abs(a.fp32[i]) > abs(b.fp32[i])) {
    dst.fp32[i] = a.fp32[i];
  } else if (abs(b.fp32[i]) > abs(a.fp32[i])) {
    dst.fp32[i] = b.fp32[i];
  } else {
    dst.fp32[i] = fmax(a.fp32[i], b.fp32[i]);
  }
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

## \_\_m128d \_\_lsx_vfmaxa_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128d __lsx_vfmaxa_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfmaxa.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compute maximum of double precision floating point elements in `a` and `b` by magnitude.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (abs(a.fp64[i]) > abs(b.fp64[i])) {
    dst.fp64[i] = a.fp64[i];
  } else if (abs(b.fp64[i]) > abs(a.fp64[i])) {
    dst.fp64[i] = b.fp64[i];
  } else {
    dst.fp64[i] = fmax(a.fp64[i], b.fp64[i]);
  }
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

## \_\_m128 \_\_lsx_vfmin_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128 __lsx_vfmin_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfmin.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compute minimum of single precision floating point elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = fmin(a.fp32[i], b.fp32[i]);
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

## \_\_m128d \_\_lsx_vfmin_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128d __lsx_vfmin_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfmin.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compute minimum of double precision floating point elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = fmin(a.fp64[i], b.fp64[i]);
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

## \_\_m128 \_\_lsx_vfmina_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128 __lsx_vfmina_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfmina.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compute minimum of single precision floating point elements in `a` and `b` by magnitude.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (abs(a.fp32[i]) < abs(b.fp32[i])) {
    dst.fp32[i] = a.fp32[i];
  } else if (abs(b.fp32[i]) < abs(a.fp32[i])) {
    dst.fp32[i] = b.fp32[i];
  } else {
    dst.fp32[i] = fmin(a.fp32[i], b.fp32[i]);
  }
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

## \_\_m128d \_\_lsx_vfmina_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128d __lsx_vfmina_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfmina.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compute minimum of double precision floating point elements in `a` and `b` by magnitude.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (abs(a.fp64[i]) < abs(b.fp64[i])) {
    dst.fp64[i] = a.fp64[i];
  } else if (abs(b.fp64[i]) < abs(a.fp64[i])) {
    dst.fp64[i] = b.fp64[i];
  } else {
    dst.fp64[i] = fmin(a.fp64[i], b.fp64[i]);
  }
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

## \_\_m128 \_\_lsx_vfmul_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128 __lsx_vfmul_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfmul.s vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply single precision floating point elements in `a` and elements in `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = a.fp32[i] * b.fp32[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 5       | 2                |
| 3A6000   | LA664  | 5       | 2                |
| 3C6000   | LA664  | 5       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128d \_\_lsx_vfmul_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128d __lsx_vfmul_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfmul.d vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply double precision floating point elements in `a` and elements in `b`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = a.fp64[i] * b.fp64[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 5       | 2                |
| 3A6000   | LA664  | 5       | 2                |
| 3C6000   | LA664  | 5       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128 \_\_lsx_vfsub_s (\_\_m128 a, \_\_m128 b)

### Synopsis

```
__m128 __lsx_vfsub_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfsub.s vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract single precision floating point elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = a.fp32[i] - b.fp32[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 5       | 2                |
| 3A6000   | LA664  | 3       | 4                |
| 3C6000   | LA664  | 3       | 4                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128d \_\_lsx_vfsub_d (\_\_m128d a, \_\_m128d b)

### Synopsis

```
__m128d __lsx_vfsub_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfsub.d vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract double precision floating point elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = a.fp64[i] - b.fp64[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 5       | 2                |
| 3A6000   | LA664  | 3       | 4                |
| 3C6000   | LA664  | 3       | 4                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128 \_\_lsx_vflogb_s (\_\_m128 a)

### Synopsis

```
__m128 __lsx_vflogb_s (__m128 a)
#include <lsxintrin.h>
Instruction: vflogb.s vr, vr
CPU Flags: LSX
```

### Description

Compute 2-based logarithm of single precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = log2(a.fp32[i]);
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

## \_\_m128d \_\_lsx_vflogb_d (\_\_m128d a)

### Synopsis

```
__m128d __lsx_vflogb_d (__m128d a)
#include <lsxintrin.h>
Instruction: vflogb.d vr, vr
CPU Flags: LSX
```

### Description

Compute 2-based logarithm of double precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = log2(a.fp64[i]);
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

## \_\_m128 \_\_lsx_vfsqrt_s (\_\_m128 a)

### Synopsis

```
__m128 __lsx_vfsqrt_s (__m128 a)
#include <lsxintrin.h>
Instruction: vfsqrt.s vr, vr
CPU Flags: LSX
```

### Description

Compute square root of single precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = sqrt(a.fp32[i]);
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 27      | 0.17(1/6)        |
| 3A6000   | LA664  | 11      | 0.08(1/12)       |
| 3C6000   | LA664  | 25      | 0.09(1/11.5)     |
| 2K1000LA | LA264  | 16      | 0.03(1/40)       |
| 2K3000   | LA364E | 28      | 0.03(1/40)       |

## \_\_m128d \_\_lsx_vfsqrt_d (\_\_m128d a)

### Synopsis

```
__m128d __lsx_vfsqrt_d (__m128d a)
#include <lsxintrin.h>
Instruction: vfsqrt.d vr, vr
CPU Flags: LSX
```

### Description

Compute square root of double precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = sqrt(a.fp64[i]);
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 36      | 0.05(1/18.5)     |
| 3A6000   | LA664  | 36      | 0.06(1/17.5)     |
| 3C6000   | LA664  | 36      | 0.06(1/17)       |
| 2K1000LA | LA264  | 11      | 0.03(1/37)       |
| 2K3000   | LA364E | 11      | 0.03(1/37)       |

## \_\_m128 \_\_lsx_vfrsqrt_s (\_\_m128 a)

### Synopsis

```
__m128 __lsx_vfrsqrt_s (__m128 a)
#include <lsxintrin.h>
Instruction: vfrsqrt.s vr, vr
CPU Flags: LSX
```

### Description

Compute reciprocal of square root of single precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = 1.0 / sqrt(a.fp32[i]);
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 21      | 0.11(1/9)        |
| 3A6000   | LA664  | 17      | 0.05(1/19)       |
| 3C6000   | LA664  | 21      | 0.11(1/9.5)      |
| 2K1000LA | LA264  | 26      | 0(1/62)          |
| 2K3000   | LA364E | 26      | 0(1/62)          |

## \_\_m128d \_\_lsx_vfrsqrt_d (\_\_m128d a)

### Synopsis

```
__m128d __lsx_vfrsqrt_d (__m128d a)
#include <lsxintrin.h>
Instruction: vfrsqrt.d vr, vr
CPU Flags: LSX
```

### Description

Compute reciprocal of square root of double precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = 1.0 / sqrt(a.fp64[i]);
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 15      | 0.04(1/27.5)     |
| 3A6000   | LA664  | 15      | 0.04(1/26.5)     |
| 3C6000   | LA664  | 15      | 0.04(1/26)       |
| 2K1000LA | LA264  | 16      | 0(1/55)          |
| 2K3000   | LA364E | 16      | 0(1/55)          |

## \_\_m128 \_\_lsx_vfrecip_s (\_\_m128 a)

### Synopsis

```
__m128 __lsx_vfrecip_s (__m128 a)
#include <lsxintrin.h>
Instruction: vfrecip.s vr, vr
CPU Flags: LSX
```

### Description

Compute reciprocal of single precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = 1 / a.fp32[i];
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 27      | 0.14(1/7)        |
| 3A6000   | LA664  | 11      | 0.18(1/5.5)      |
| 3C6000   | LA664  | 27      | 0.12(1/8.5)      |
| 2K1000LA | LA264  | 28      | 0.04(1/28)       |
| 2K3000   | LA364E | 28      | 0.04(1/28)       |

## \_\_m128d \_\_lsx_vfrecip_d (\_\_m128d a)

### Synopsis

```
__m128d __lsx_vfrecip_d (__m128d a)
#include <lsxintrin.h>
Instruction: vfrecip.d vr, vr
CPU Flags: LSX
```

### Description

Compute reciprocal of double precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = 1 / a.fp64[i];
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 23      | 0.08(1/12)       |
| 3A6000   | LA664  | 8       | 0.25(1/4)        |
| 3C6000   | LA664  | 23      | 0.1(1/10.5)      |
| 2K1000LA | LA264  | 24      | 0.04(1/24)       |
| 2K3000   | LA364E | 24      | 0.04(1/24)       |

## \_\_m128 \_\_lsx_vfrsqrte_s (\_\_m128 a)

### Synopsis

```
__m128 __lsx_vfrsqrte_s (__m128 a)
#include <lsxintrin.h>
Instruction: vfrsqrte.s vr, vr
CPU Flags: LSX
```

### Description

Compute estimated reciprocal of square root of single precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = 1.0 / sqrt(a.fp32[i]); // estimated
}
```

## \_\_m128d \_\_lsx_vfrsqrte_d (\_\_m128d a)

### Synopsis

```
__m128d __lsx_vfrsqrte_d (__m128d a)
#include <lsxintrin.h>
Instruction: vfrsqrte.d vr, vr
CPU Flags: LSX
```

### Description

Compute estimated reciprocal of square root of double precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = 1.0 / sqrt(a.fp64[i]); // estimated
}
```

## \_\_m128 \_\_lsx_vfrecipe_s (\_\_m128 a)

### Synopsis

```
__m128 __lsx_vfrecipe_s (__m128 a)
#include <lsxintrin.h>
Instruction: vfrecipe.s vr, vr
CPU Flags: LSX
```

### Description

Compute estimated reciprocal of single precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = 1 / a.fp32[i]; // estimated
}
```

## \_\_m128d \_\_lsx_vfrecipe_d (\_\_m128d a)

### Synopsis

```
__m128d __lsx_vfrecipe_d (__m128d a)
#include <lsxintrin.h>
Instruction: vfrecipe.d vr, vr
CPU Flags: LSX
```

### Description

Compute estimated reciprocal of double precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = 1 / a.fp64[i]; // estimated
}
```
