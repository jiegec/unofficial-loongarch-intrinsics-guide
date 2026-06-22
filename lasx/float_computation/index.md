# Floating Point Computation

## \_\_m256 \_\_lasx_xvfadd_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256 __lasx_xvfadd_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfadd.s xr, xr, xr
CPU Flags: LASX
```

### Description

Add single precision floating point elements in `a` to elements in `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = a.fp32[i] + b.fp32[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

## \_\_m256d \_\_lasx_xvfadd_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256d __lasx_xvfadd_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfadd.d xr, xr, xr
CPU Flags: LASX
```

### Description

Add double precision floating point elements in `a` to elements in `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = a.fp64[i] + b.fp64[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

## \_\_m256 \_\_lasx_xvfdiv_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256 __lasx_xvfdiv_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfdiv.s xr, xr, xr
CPU Flags: LASX
```

### Description

Divide single precision floating point elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = a.fp32[i] / b.fp32[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency  | Throughput (IPC) |
| ------ | ----- | -------- | ---------------- |
| 3C5000 | LA464 | 11, 19.5 | 0.1(1/10.5)      |
| 3A6000 | LA664 | 11       | 0.18(1/5.5)      |
| 3C6000 | LA664 | 11       | 0.22(1/4.5)      |

## \_\_m256d \_\_lasx_xvfdiv_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256d __lasx_xvfdiv_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfdiv.d xr, xr, xr
CPU Flags: LASX
```

### Description

Divide double precision floating point elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = a.fp64[i] / b.fp64[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 8, 17   | 0.08(1/12.5)     |
| 3A6000 | LA664 | 8, 21.5 | 0.25(1/4)        |
| 3C6000 | LA664 | 8, 16.5 | 0.33(1/3)        |

## \_\_m256 \_\_lasx_xvfmax_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256 __lasx_xvfmax_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfmax.s xr, xr, xr
CPU Flags: LASX
```

### Description

Compute maximum of single precision floating point elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = fmax(a.fp32[i], b.fp32[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256d \_\_lasx_xvfmax_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256d __lasx_xvfmax_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfmax.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compute maximum of double precision floating point elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = fmax(a.fp64[i], b.fp64[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256 \_\_lasx_xvfmaxa_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256 __lasx_xvfmaxa_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfmaxa.s xr, xr, xr
CPU Flags: LASX
```

### Description

Compute maximum of single precision floating point elements in `a` and `b` by magnitude.

### Operation

```
for (int i = 0; i < 8; i++) {
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

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256d \_\_lasx_xvfmaxa_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256d __lasx_xvfmaxa_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfmaxa.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compute maximum of double precision floating point elements in `a` and `b` by magnitude.

### Operation

```
for (int i = 0; i < 4; i++) {
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

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256 \_\_lasx_xvfmin_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256 __lasx_xvfmin_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfmin.s xr, xr, xr
CPU Flags: LASX
```

### Description

Compute minimum of single precision floating point elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = fmin(a.fp32[i], b.fp32[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256d \_\_lasx_xvfmin_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256d __lasx_xvfmin_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfmin.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compute minimum of double precision floating point elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = fmin(a.fp64[i], b.fp64[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256 \_\_lasx_xvfmina_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256 __lasx_xvfmina_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfmina.s xr, xr, xr
CPU Flags: LASX
```

### Description

Compute minimum of single precision floating point elements in `a` and `b` by magnitude.

### Operation

```
for (int i = 0; i < 8; i++) {
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

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256d \_\_lasx_xvfmina_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256d __lasx_xvfmina_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfmina.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compute minimum of double precision floating point elements in `a` and `b` by magnitude.

### Operation

```
for (int i = 0; i < 4; i++) {
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

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

## \_\_m256 \_\_lasx_xvfmul_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256 __lasx_xvfmul_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfmul.s xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply single precision floating point elements in `a` and elements in `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = a.fp32[i] * b.fp32[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 2                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |

## \_\_m256d \_\_lasx_xvfmul_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256d __lasx_xvfmul_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfmul.d xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply double precision floating point elements in `a` and elements in `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = a.fp64[i] * b.fp64[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 2                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |

## \_\_m256 \_\_lasx_xvfsub_s (\_\_m256 a, \_\_m256 b)

### Synopsis

```
__m256 __lasx_xvfsub_s (__m256 a, __m256 b)
#include <lasxintrin.h>
Instruction: xvfsub.s xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract single precision floating point elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = a.fp32[i] - b.fp32[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

## \_\_m256d \_\_lasx_xvfsub_d (\_\_m256d a, \_\_m256d b)

### Synopsis

```
__m256d __lasx_xvfsub_d (__m256d a, __m256d b)
#include <lasxintrin.h>
Instruction: xvfsub.d xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract double precision floating point elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = a.fp64[i] - b.fp64[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

## \_\_m256 \_\_lasx_xvflogb_s (\_\_m256 a)

### Synopsis

```
__m256 __lasx_xvflogb_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvflogb.s xr, xr
CPU Flags: LASX
```

### Description

Compute 2-based logarithm of single precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = log2(a.fp32[i]);
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 4                |
| 3C6000 | LA664 | 4       | 4                |

## \_\_m256d \_\_lasx_xvflogb_d (\_\_m256d a)

### Synopsis

```
__m256d __lasx_xvflogb_d (__m256d a)
#include <lasxintrin.h>
Instruction: xvflogb.d xr, xr
CPU Flags: LASX
```

### Description

Compute 2-based logarithm of double precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = log2(a.fp64[i]);
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 4                |
| 3C6000 | LA664 | 4       | 4                |

## \_\_m256 \_\_lasx_xvfsqrt_s (\_\_m256 a)

### Synopsis

```
__m256 __lasx_xvfsqrt_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvfsqrt.s xr, xr
CPU Flags: LASX
```

### Description

Compute square root of single precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = sqrt(a.fp32[i]);
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 15      | 0.07(1/13.5)     |
| 3A6000 | LA664 | 15      | 0.08(1/12)       |
| 3C6000 | LA664 | 25      | 0.09(1/11.5)     |

## \_\_m256d \_\_lasx_xvfsqrt_d (\_\_m256d a)

### Synopsis

```
__m256d __lasx_xvfsqrt_d (__m256d a)
#include <lasxintrin.h>
Instruction: xvfsqrt.d xr, xr
CPU Flags: LASX
```

### Description

Compute square root of double precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = sqrt(a.fp64[i]);
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 36      | 0.05(1/18.5)     |
| 3A6000 | LA664 | 36      | 0.06(1/17.5)     |
| 3C6000 | LA664 | 36      | 0.06(1/17)       |

## \_\_m256 \_\_lasx_xvfrsqrt_s (\_\_m256 a)

### Synopsis

```
__m256 __lasx_xvfrsqrt_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvfrsqrt.s xr, xr
CPU Flags: LASX
```

### Description

Compute reciprocal of square root of single precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = 1.0 / sqrt(a.fp32[i]);
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 25      | 0.03(1/32)       |
| 3A6000 | LA664 | 25      | 0.05(1/19)       |
| 3C6000 | LA664 | 21      | 0.11(1/9.5)      |

## \_\_m256d \_\_lasx_xvfrsqrt_d (\_\_m256d a)

### Synopsis

```
__m256d __lasx_xvfrsqrt_d (__m256d a)
#include <lasxintrin.h>
Instruction: xvfrsqrt.d xr, xr
CPU Flags: LASX
```

### Description

Compute reciprocal of square root of double precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = 1.0 / sqrt(a.fp64[i]);
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 15      | 0.04(1/27.5)     |
| 3A6000 | LA664 | 15      | 0.04(1/26.5)     |
| 3C6000 | LA664 | 15      | 0.04(1/26)       |

## \_\_m256 \_\_lasx_xvfrecip_s (\_\_m256 a)

### Synopsis

```
__m256 __lasx_xvfrecip_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvfrecip.s xr, xr
CPU Flags: LASX
```

### Description

Compute reciprocal of single precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = 1 / a.fp32[i];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 27      | 0.14(1/7)        |
| 3A6000 | LA664 | 27      | 0.18(1/5.5)      |
| 3C6000 | LA664 | 27      | 0.12(1/8.5)      |

## \_\_m256d \_\_lasx_xvfrecip_d (\_\_m256d a)

### Synopsis

```
__m256d __lasx_xvfrecip_d (__m256d a)
#include <lasxintrin.h>
Instruction: xvfrecip.d xr, xr
CPU Flags: LASX
```

### Description

Compute reciprocal of double precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = 1 / a.fp64[i];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 23      | 0.08(1/12)       |
| 3A6000 | LA664 | 23      | 0.25(1/4)        |
| 3C6000 | LA664 | 23      | 0.1(1/10.5)      |

## \_\_m256 \_\_lasx_xvfrsqrte_s (\_\_m256 a)

### Synopsis

```
__m256 __lasx_xvfrsqrte_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvfrsqrte.s xr, xr
CPU Flags: LASX
```

### Description

Compute estimated reciprocal of square root of single precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = 1.0 / sqrt(a.fp32[i]); // estimated
}
```

## \_\_m256d \_\_lasx_xvfrsqrte_d (\_\_m256d a)

### Synopsis

```
__m256d __lasx_xvfrsqrte_d (__m256d a)
#include <lasxintrin.h>
Instruction: xvfrsqrte.d xr, xr
CPU Flags: LASX
```

### Description

Compute estimated reciprocal of square root of double precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = 1.0 / sqrt(a.fp64[i]); // estimated
}
```

## \_\_m256 \_\_lasx_xvfrecipe_s (\_\_m256 a)

### Synopsis

```
__m256 __lasx_xvfrecipe_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvfrecipe.s xr, xr
CPU Flags: LASX
```

### Description

Compute estimated reciprocal of single precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = 1 / a.fp32[i]; // estimated
}
```

## \_\_m256d \_\_lasx_xvfrecipe_d (\_\_m256d a)

### Synopsis

```
__m256d __lasx_xvfrecipe_d (__m256d a)
#include <lasxintrin.h>
Instruction: xvfrecipe.d xr, xr
CPU Flags: LASX
```

### Description

Compute estimated reciprocal of double precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = 1 / a.fp64[i]; // estimated
}
```
