# Fused Multiply-Add

## \_\_m256d \_\_lasx_xvfmadd_d (\_\_m256d a, \_\_m256d b, \_\_m256d c)

### Synopsis

```
__m256d __lasx_xvfmadd_d (__m256d a, __m256d b, __m256d c)
#include <lasxintrin.h>
Instruction: xvfmadd.d xr, xr, xr, xr
CPU Flags: LASX
```

### Description

Compute packed double precision floating point FMA(Fused Multiply-Add): multiply elements in `a` and `b`, accumulate to elements in `c` and store the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = a.fp64[i] * b.fp64[i] + c.fp64[i];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 2                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |

## \_\_m256 \_\_lasx_xvfmadd_s (\_\_m256 a, \_\_m256 b, \_\_m256 c)

### Synopsis

```
__m256 __lasx_xvfmadd_s (__m256 a, __m256 b, __m256 c)
#include <lasxintrin.h>
Instruction: xvfmadd.s xr, xr, xr, xr
CPU Flags: LASX
```

### Description

Compute packed single precision floating point FMA(Fused Multiply-Add): multiply elements in `a` and `b`, accumulate to elements in `c` and store the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = a.fp32[i] * b.fp32[i] + c.fp32[i];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 2                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |

## \_\_m256d \_\_lasx_xvfmsub_d (\_\_m256d a, \_\_m256d b, \_\_m256d c)

### Synopsis

```
__m256d __lasx_xvfmsub_d (__m256d a, __m256d b, __m256d c)
#include <lasxintrin.h>
Instruction: xvfmsub.d xr, xr, xr, xr
CPU Flags: LASX
```

### Description

Compute packed double precision floating point FMA(Fused Multiply-Add): multiply elements in `a` and `b`, subtract elements in `c` and store the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = a.fp64[i] * b.fp64[i] - c.fp64[i];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 2                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |

## \_\_m256 \_\_lasx_xvfmsub_s (\_\_m256 a, \_\_m256 b, \_\_m256 c)

### Synopsis

```
__m256 __lasx_xvfmsub_s (__m256 a, __m256 b, __m256 c)
#include <lasxintrin.h>
Instruction: xvfmsub.s xr, xr, xr, xr
CPU Flags: LASX
```

### Description

Compute packed single precision floating point FMA(Fused Multiply-Add): multiply elements in `a` and `b`, subtract elements in `c` and store the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = a.fp32[i] * b.fp32[i] - c.fp32[i];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 2                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |

## \_\_m256d \_\_lasx_xvfnmadd_d (\_\_m256d a, \_\_m256d b, \_\_m256d c)

### Synopsis

```
__m256d __lasx_xvfnmadd_d (__m256d a, __m256d b, __m256d c)
#include <lasxintrin.h>
Instruction: xvfnmadd.d xr, xr, xr, xr
CPU Flags: LASX
```

### Description

Compute packed double precision floating point FMA(Fused Multiply-Add): multiply elements in `a` and `b`, accumulate to elements in `c` and store the negated result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = (a.fp64[i] * b.fp64[i] + c.fp64[i]);
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 2                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |

## \_\_m256 \_\_lasx_xvfnmadd_s (\_\_m256 a, \_\_m256 b, \_\_m256 c)

### Synopsis

```
__m256 __lasx_xvfnmadd_s (__m256 a, __m256 b, __m256 c)
#include <lasxintrin.h>
Instruction: xvfnmadd.s xr, xr, xr, xr
CPU Flags: LASX
```

### Description

Compute packed single precision floating point FMA(Fused Multiply-Add): multiply elements in `a` and `b`, accumulate to elements in `c` and store the negated result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = -(a.fp32[i] * b.fp32[i] + c.fp32[i]);
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 2                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |

## \_\_m256d \_\_lasx_xvfnmsub_d (\_\_m256d a, \_\_m256d b, \_\_m256d c)

### Synopsis

```
__m256d __lasx_xvfnmsub_d (__m256d a, __m256d b, __m256d c)
#include <lasxintrin.h>
Instruction: xvfnmsub.d xr, xr, xr, xr
CPU Flags: LASX
```

### Description

Compute packed double precision floating point FMA(Fused Multiply-Add): multiply elements in `a` and `b`, subtract elements in `c` and store the negated result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = -(a.fp64[i] * b.fp64[i] - c.fp64[i]);
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 2                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |

## \_\_m256 \_\_lasx_xvfnmsub_s (\_\_m256 a, \_\_m256 b, \_\_m256 c)

### Synopsis

```
__m256 __lasx_xvfnmsub_s (__m256 a, __m256 b, __m256 c)
#include <lasxintrin.h>
Instruction: xvfnmsub.s xr, xr, xr, xr
CPU Flags: LASX
```

### Description

Compute packed single precision floating point FMA(Fused Multiply-Add): multiply elements in `a` and `b`, subtract elements in `c` and store the negated result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = -(a.fp32[i] * b.fp32[i] - c.fp32[i]);
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 5       | 2                |
| 3A6000 | LA664 | 5       | 2                |
| 3C6000 | LA664 | 5       | 2                |
