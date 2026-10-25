# \_\_lsx_vfrecip_d

LSX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_computation/index.md)

### Synopsis

```
__m128d __lsx_vfrecip_d (__m128d a)
#include <lsxintrin.h>
Instruction: vfrecip.d vr, vr
CPU Flags: LSX
```

### Description

Compute reciprocal of double precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = 1 / a.fp64[i];
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 23      | 0.08(1/12)       |
| 3A6000   | LA664  | 8       | 0.25(1/4)        |
| 3C6000   | LA664  | 23      | 0.1(1/10.5)      |
| 2K1000LA | LA264  | 24      | 0.04(1/24)       |
| 2K3000   | LA364E | 24      | 0.04(1/24)       |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
