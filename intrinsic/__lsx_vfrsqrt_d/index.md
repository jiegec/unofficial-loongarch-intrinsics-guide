# \_\_lsx_vfrsqrt_d

LSX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_computation/index.md)

### Synopsis

```
__m128d __lsx_vfrsqrt_d (__m128d a)
#include <lsxintrin.h>
Instruction: vfrsqrt.d vr, vr
CPU Flags: LSX
```

### Description

Compute reciprocal of square root of double precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = 1.0 / sqrt(a.fp64[i]);
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 15      | 0.04(1/27.5)     |
| 3A6000   | LA664  | 15      | 0.04(1/26.5)     |
| 3C6000   | LA664  | 15      | 0.04(1/26)       |
| 2K1000LA | LA264  | 16      | 0(1/55)          |
| 2K3000   | LA364E | 16      | 0(1/55)          |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
