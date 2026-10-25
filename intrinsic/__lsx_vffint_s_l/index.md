# \_\_lsx_vffint_s_l

LSX intrinsic · group: [Floating Point Conversion](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_conversion/index.md)

### Synopsis

```
__m128 __lsx_vffint_s_l (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vffint.s.l vr, vr, vr
CPU Flags: LSX
```

### Description

For each 128-bit lane, convert signed 64-bit integer elements in the corresponding lane of `a` into the upper half and elements in `b` into the lower half, as single-precision floating point numbers.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] =
      (i < 2) ? (f32)(s64)b.dword[i]
              : (f32)(s64)a.dword[i - 2]; // rounding mode is not expressed in C
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 5       | 1                |
| 3A6000   | LA664  | 5       | 2                |
| 3C6000   | LA664  | 5       | 2                |
| 2K1000LA | LA264  | 5       | 0.5(1/2)         |
| 2K3000   | LA364E | 5       | 0.5(1/2)         |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
