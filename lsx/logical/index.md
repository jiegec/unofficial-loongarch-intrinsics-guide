# Logical

## \_\_m128i \_\_lsx_vand_v (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vand_v (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vand.v vr, vr, vr
CPU Flags: LSX
```

### Description

Compute bitwise AND between elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = a.dword[i] & b.dword[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vandi_b (\_\_m128i a, imm0_255 imm)

### Synopsis

```
__m128i __lsx_vandi_b (__m128i a, imm0_255 imm)
#include <lsxintrin.h>
Instruction: vandi.b vr, vr, imm
CPU Flags: LSX
```

### Description

Compute bitwise AND between elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[i] & imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vandn_v (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vandn_v (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vandn.v vr, vr, vr
CPU Flags: LSX
```

### Description

Compute bitwise ANDN between elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = b.dword[i] & (~a.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vnor_v (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vnor_v (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vnor.v vr, vr, vr
CPU Flags: LSX
```

### Description

Compute bitwise NOR between elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = ~(a.dword[i] | b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vnori_b (\_\_m128i a, imm0_255 imm)

### Synopsis

```
__m128i __lsx_vnori_b (__m128i a, imm0_255 imm)
#include <lsxintrin.h>
Instruction: vnori.b vr, vr, imm
CPU Flags: LSX
```

### Description

Compute bitwise NOR between elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = ~(a.byte[i] | imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vor_v (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vor_v (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vor.v vr, vr, vr
CPU Flags: LSX
```

### Description

Compute bitwise OR between elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = a.dword[i] | b.dword[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vori_b (\_\_m128i a, imm0_255 imm)

### Synopsis

```
__m128i __lsx_vori_b (__m128i a, imm0_255 imm)
#include <lsxintrin.h>
Instruction: vori.b vr, vr, imm
CPU Flags: LSX
```

### Description

Compute bitwise OR between elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[i] | imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 3                |

## \_\_m128i \_\_lsx_vorn_v (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vorn_v (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vorn.v vr, vr, vr
CPU Flags: LSX
```

### Description

Compute bitwise ORN between elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = a.dword[i] | (~b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vxor_v (\_\_m128i a, \_\_m128i b)

### Synopsis

```
__m128i __lsx_vxor_v (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vxor.v vr, vr, vr
CPU Flags: LSX
```

### Description

Compute bitwise XOR between elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = a.dword[i] ^ b.dword[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

## \_\_m128i \_\_lsx_vxori_b (\_\_m128i a, imm0_255 imm)

### Synopsis

```
__m128i __lsx_vxori_b (__m128i a, imm0_255 imm)
#include <lsxintrin.h>
Instruction: vxori.b vr, vr, imm
CPU Flags: LSX
```

### Description

Compute bitwise XOR between elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[i] ^ imm;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |
