# \_\_lsx_vftintrz_wu_s

LSX intrinsic · group: [Floating Point Conversion](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_conversion/index.md)

### Synopsis

```
__m128i __lsx_vftintrz_wu_s (__m128 a)
#include <lsxintrin.h>
Instruction: vftintrz.wu.s vr, vr
CPU Flags: LSX
```

### Description

Convert single-precision floating point elements in `a` to unsigned 32-bit integer, rounding towards zero.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (u32)a.fp32[i]; // rounding mode is not expressed in C
}
```

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
