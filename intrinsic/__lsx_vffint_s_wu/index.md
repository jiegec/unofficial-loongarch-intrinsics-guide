# \_\_lsx_vffint_s_wu

LSX intrinsic · group: [Floating Point Conversion](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_conversion/index.md)

### Synopsis

```
__m128 __lsx_vffint_s_wu (__m128i a)
#include <lsxintrin.h>
Instruction: vffint.s.wu vr, vr
CPU Flags: LSX
```

### Description

Convert unsigned 32-bit integer elements in `a` to single-precision floating point numbers.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = (f32)(u32)a.word[i]; // rounding mode is not expressed in C
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
