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

Compute IEEE 754 scaleB (multiply by 2^`b`) of single precision floating point elements in `a` using integer exponents in `b`. Currently undocumented.

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

Compute IEEE 754 scaleB (multiply by 2^`b`) of double precision floating point elements in `a` using integer exponents in `b`. Currently undocumented.

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

Compute an 8-bit pattern selected by `mode` (0: repeating 0..3; 1: 0..3 shifted by 1 for each group; 2: 4..7 shifted by 1 for each group; 3: 0..15), then add `uimm5` to each byte element.

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
| 3C5000 | LA464 | N/A     | 2                |
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

Select each double word of `dst` from an adjacent pair in `a` according to `imm`: bits 0 and 1 choose the sources for D[0] and D[1] from {a.D[0], a.D[1]}, and bits 2 and 3 choose the sources for D[2] and D[3] from {a.D[2], a.D[3]}.

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
