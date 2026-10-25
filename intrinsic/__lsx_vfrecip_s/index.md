# \_\_lsx_vfrecip_s

LSX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_computation/index.md)

### Synopsis

```
__m128 __lsx_vfrecip_s (__m128 a)
#include <lsxintrin.h>
Instruction: vfrecip.s vr, vr
CPU Flags: LSX
```

### Description

Compute reciprocal of single precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = 1 / a.fp32[i];
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 27      | 0.14(1/7)        |
| 3A6000   | LA664  | 11      | 0.18(1/5.5)      |
| 3C6000   | LA664  | 27      | 0.12(1/8.5)      |
| 2K1000LA | LA264  | 28      | 0.04(1/28)       |
| 2K3000   | LA364E | 28      | 0.04(1/28)       |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
