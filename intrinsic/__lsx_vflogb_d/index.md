# \_\_lsx_vflogb_d

LSX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_computation/index.md)

### Synopsis

```
__m128d __lsx_vflogb_d (__m128d a)
#include <lsxintrin.h>
Instruction: vflogb.d vr, vr
CPU Flags: LSX
```

### Description

Compute base-2 logarithm of double precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = log2(a.fp64[i]);
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
