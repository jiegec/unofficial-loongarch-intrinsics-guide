# \_\_lsx_vffint_d_l

LSX intrinsic · group: [Floating Point Conversion](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_conversion/index.md)

### Synopsis

```
__m128d __lsx_vffint_d_l (__m128i a)
#include <lsxintrin.h>
Instruction: vffint.d.l vr, vr
CPU Flags: LSX
```

### Description

Convert signed 64-bit integer elements in `a` to double-precision floating point numbers.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = (f64)(s64)a.dword[i]; // rounding mode is not expressed in C
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 4                |
| 3C6000   | LA664  | 4       | 4                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
