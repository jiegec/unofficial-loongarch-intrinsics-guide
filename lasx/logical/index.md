# Logical

## \_\_m256i \_\_lasx_xvand_v (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvand_v (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvand.v xr, xr, xr
CPU Flags: LASX
```

### Description

Compute bitwise AND between elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = a.dword[i] & b.dword[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvandi_b (\_\_m256i a, imm0_255 imm)

### Synopsis

```
__m256i __lasx_xvandi_b (__m256i a, imm0_255 imm)
#include <lasxintrin.h>
Instruction: xvandi.b xr, xr, imm
CPU Flags: LASX
```

### Description

Compute bitwise AND between elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[i] & imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvandn_v (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvandn_v (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvandn.v xr, xr, xr
CPU Flags: LASX
```

### Description

Compute bitwise ANDN between elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = b.dword[i] & (~a.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvnor_v (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvnor_v (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvnor.v xr, xr, xr
CPU Flags: LASX
```

### Description

Compute bitwise NOR between elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = ~(a.dword[i] | b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvnori_b (\_\_m256i a, imm0_255 imm)

### Synopsis

```
__m256i __lasx_xvnori_b (__m256i a, imm0_255 imm)
#include <lasxintrin.h>
Instruction: xvnori.b xr, xr, imm
CPU Flags: LASX
```

### Description

Compute bitwise NOR between elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = ~(a.byte[i] | imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvor_v (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvor_v (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvor.v xr, xr, xr
CPU Flags: LASX
```

### Description

Compute bitwise OR between elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = a.dword[i] | b.dword[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvori_b (\_\_m256i a, imm0_255 imm)

### Synopsis

```
__m256i __lasx_xvori_b (__m256i a, imm0_255 imm)
#include <lasxintrin.h>
Instruction: xvori.b xr, xr, imm
CPU Flags: LASX
```

### Description

Compute bitwise OR between elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[i] | imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvorn_v (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvorn_v (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvorn.v xr, xr, xr
CPU Flags: LASX
```

### Description

Compute bitwise ORN between elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = a.dword[i] | (~b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvxor_v (\_\_m256i a, \_\_m256i b)

### Synopsis

```
__m256i __lasx_xvxor_v (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvxor.v xr, xr, xr
CPU Flags: LASX
```

### Description

Compute bitwise XOR between elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = a.dword[i] ^ b.dword[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

## \_\_m256i \_\_lasx_xvxori_b (\_\_m256i a, imm0_255 imm)

### Synopsis

```
__m256i __lasx_xvxori_b (__m256i a, imm0_255 imm)
#include <lasxintrin.h>
Instruction: xvxori.b xr, xr, imm
CPU Flags: LASX
```

### Description

Compute bitwise XOR between elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[i] ^ imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |
