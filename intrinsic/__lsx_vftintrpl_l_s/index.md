# \_\_lsx_vftintrpl_l_s

LSX intrinsic · group: [Floating Point Conversion](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_conversion/index.md)

### Synopsis

```
__m128i __lsx_vftintrpl_l_s (__m128 a)
#include <lsxintrin.h>
Instruction: vftintrpl.l.s vr, vr
CPU Flags: LSX
```

### Description

Convert single-precision floating point elements in lower part of every 128-bit vector of `a` to signed 64-bit integer, rounding towards positive infinity.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)a.fp32[i]; // rounding mode is not expressed in C
}
```

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
