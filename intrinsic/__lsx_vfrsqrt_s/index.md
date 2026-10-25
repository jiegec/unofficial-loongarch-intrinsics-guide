# \_\_lsx_vfrsqrt_s

LSX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_computation/index.md)

### Synopsis

```
__m128 __lsx_vfrsqrt_s (__m128 a)
#include <lsxintrin.h>
Instruction: vfrsqrt.s vr, vr
CPU Flags: LSX
```

### Description

Compute reciprocal of square root of single precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = 1.0 / sqrt(a.fp32[i]);
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 21      | 0.11(1/9)        |
| 3A6000   | LA664  | 17      | 0.05(1/19)       |
| 3C6000   | LA664  | 21      | 0.11(1/9.5)      |
| 2K1000LA | LA264  | 26      | 0(1/62)          |
| 2K3000   | LA364E | 26      | 0(1/62)          |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
