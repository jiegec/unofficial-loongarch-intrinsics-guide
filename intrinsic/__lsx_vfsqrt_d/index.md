# \_\_lsx_vfsqrt_d

LSX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_computation/index.md)

### Synopsis

```
__m128d __lsx_vfsqrt_d (__m128d a)
#include <lsxintrin.h>
Instruction: vfsqrt.d vr, vr
CPU Flags: LSX
```

### Description

Compute square root of double precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = sqrt(a.fp64[i]);
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 36      | 0.05(1/18.5)     |
| 3A6000   | LA664  | 36      | 0.06(1/17.5)     |
| 3C6000   | LA664  | 36      | 0.06(1/17)       |
| 2K1000LA | LA264  | 11      | 0.03(1/37)       |
| 2K3000   | LA364E | 11      | 0.03(1/37)       |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
