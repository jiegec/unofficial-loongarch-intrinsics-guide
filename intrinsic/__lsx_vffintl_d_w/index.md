# \_\_lsx_vffintl_d_w

LSX intrinsic · group: [Floating Point Conversion](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_conversion/index.md)

### Synopsis

```
__m128d __lsx_vffintl_d_w (__m128i a)
#include <lsxintrin.h>
Instruction: vffintl.d.w vr, vr
CPU Flags: LSX
```

### Description

Convert signed 32-bit integer elements in lower part of every 128-bit vector of `a` to double precision floating point numbers.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = (f64)(s32)a.word[i]; // rounding mode is not expressed in C
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
