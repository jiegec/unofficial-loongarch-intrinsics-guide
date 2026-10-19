# Undocumented Intrinsics

The following intrinsics are undocumented: they are not supported by either compiler or assembler. You have to use `.word` directive in assembly to use them.

## \_\_m128 \_\_lsx_vfscaleb_s (\_\_m128 a, \_\_m128i b)

### Synopsis

```
__m128 __lsx_vfscaleb_s (__m128 a, __m128i b)
#include <lsxintrin.h>
Instruction: vfscaleb.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compute IEEE 754 scaleB (multiply by 2^`b`) of single precision floating point elements in `a` using integer exponents in `b`. Currently undocumented.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = __builtin_scalbn(a.fp32[i], b.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128d \_\_lsx_vfscaleb_d (\_\_m128d a, \_\_m128i b)

### Synopsis

```
__m128d __lsx_vfscaleb_d (__m128d a, __m128i b)
#include <lsxintrin.h>
Instruction: vfscaleb.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compute IEEE 754 scaleB (multiply by 2^`b`) of double precision floating point elements in `a` using integer exponents in `b`. Currently undocumented.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = __builtin_scalbn(a.fp64[i], b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

## \_\_m128i \_\_lsx_vmepatmsk_v (int mode, int uimm5)

### Synopsis

```
__m128i __lsx_vmepatmsk_v (int mode, int uimm5)
#include <lsxintrin.h>
Instruction: vmepatmsk.v vr, mode, uimm5
CPU Flags: LSX
```

### Description

Compute an 8-bit pattern selected by `mode` (0: repeating 0..3; 1: 0..3 shifted by 1 for each group; 2: 4..7 shifted by 1 for each group; 3: 0..15), then add `uimm5` to each byte element.

### Examples

```
__m128i __lsx_vmepatmsk_v(3, 1)
= 0x0807060504030201 0x100f0e0d0c0b0a09
```

### Operation

```
if (mode == 0b00) {
  for (int i = 0; i < 16; i++) {
    dst.byte[i] = uimm5 + (i % 4); // [0 1 2 3 0 1 2 3 0 1 2 3 0 1 2 3]
  }
} else if (mode == 0b01) {
  for (int i = 0; i < 16; i++) {
    dst.byte[i] =
        uimm5 + (i / 4) + (i % 4); // [0 1 2 3 1 2 3 4 2 3 4 5 3 4 5 6]
  }
} else if (mode == 0b10) {
  for (int i = 0; i < 16; i++) {
    dst.byte[i] =
        uimm5 + (i / 4) + (i % 4) + 4; // [4 5 6 7 5 6 7 8 6 7 8 9 7 8 9 10]
  }
} else if (mode == 0b11) {
  for (int i = 0; i < 16; i++) {
    dst.byte[i] = uimm5 + i; // [0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15]
  }
} else {
  // illegal instruction
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | N/A     | 2                |
| 3C6000   | LA664  | N/A     | 4                |
| 2K1000LA | LA264  | N/A     | 1                |
| 2K3000   | LA364E | N/A     | 1                |
