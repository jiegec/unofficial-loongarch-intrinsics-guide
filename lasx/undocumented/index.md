# Undocumented Intrinsics

The following intrinsics are undocumented: they are not supported by either compiler or assembler. You have to use `.word` directive in assembly to use them.

## \_\_m256 \_\_lasx_xvfscaleb_s (\_\_m256 a, \_\_m256i b)

### Synopsis

```
__m256 __lasx_xvfscaleb_s (__m256 a, __m256i b)
#include <lasxintrin.h>
Instruction: xvfscaleb.s xr, xr, xr
CPU Flags: LASX
```

### Description

Compute IEEE754 scaleB of single precision floating point elements in `a` by integer elements in `b`. Currently undocumented.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.fp32[i] = __builtin_scalbn(a.fp32[i], b.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256d \_\_lasx_xvfscaleb_d (\_\_m256d a, \_\_m256i b)

### Synopsis

```
__m256d __lasx_xvfscaleb_d (__m256d a, __m256i b)
#include <lasxintrin.h>
Instruction: xvfscaleb.d xr, xr, xr
CPU Flags: LASX
```

### Description

Compute IEEE754 scaleB of double precision floating point elements in `a` by integer elements in `b`. Currently undocumented.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = __builtin_scalbn(a.fp64[i], b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C6000 | LA664 | 4       | 2                |

## \_\_m256i \_\_lasx_xvmepatmsk_v (int mode, int uimm5)

### Synopsis

```
__m256i __lasx_xvmepatmsk_v (int mode, int uimm5)
#include <lasxintrin.h>
Instruction: xvmepatmsk.v xr, mode, uimm5
CPU Flags: LASX
```

### Description

Compute pattern according to `mode`, then add `uimm5` to each element.

### Examples

```
__m256i __lasx_xvmepatmsk_v(3, 1)
= 0x0807060504030201 0x100f0e0d0c0b0a09 0x0807060504030201 0x100f0e0d0c0b0a09
```

### Operation

```
if (mode == 0b00) {
  for (int i = 0; i < 16; i++) {
    dst.byte[i + 16] = dst.byte[i] =
        uimm5 + (i % 4); // [0 1 2 3 0 1 2 3 0 1 2 3 0 1 2 3]
  }
} else if (mode == 0b01) {
  for (int i = 0; i < 16; i++) {
    dst.byte[i + 16] = dst.byte[i] =
        uimm5 + (i / 4) + (i % 4); // [0 1 2 3 1 2 3 4 2 3 4 5 3 4 5 6]
  }
} else if (mode == 0b10) {
  for (int i = 0; i < 16; i++) {
    dst.byte[i + 16] = dst.byte[i] =
        uimm5 + (i / 4) + (i % 4) + 4; // [4 5 6 7 5 6 7 8 6 7 8 9 7 8 9 10]
  }
} else if (mode == 0b11) {
  for (int i = 0; i < 16; i++) {
    dst.byte[i + 16] = dst.byte[i] =
        uimm5 + i; // [0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15]
  }
} else {
  // illegal instruction
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C6000 | LA664 | N/A     | 4                |

The following intrinsics are undocumented in another way: they are not supported by compiler but supported by assembler. You have to use inline assembly to use them in C/C++.

## \_\_m256i \_\_lasx_xvhseli_d (\_\_m256i a, imm0_31 imm)

### Synopsis

```
__m256i __lasx_xvhseli_d (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvhseli.d xr, xr, imm
CPU Flags: LASX
```

### Description

Select double words from `a` with indices recorded in `imm` and store into `dst`.

### Operation

```
dst.dword[0] = (imm & 1) ? a.dword[1] : a.dword[0];
dst.dword[1] = (imm & 2) ? a.dword[1] : a.dword[0];
dst.dword[2] = (imm & 4) ? a.dword[3] : a.dword[2];
dst.dword[3] = (imm & 8) ? a.dword[3] : a.dword[2];
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 1                |
| 3A6000 | LA664 | 1       | 1                |
| 3C6000 | LA664 | 1       | 1                |
