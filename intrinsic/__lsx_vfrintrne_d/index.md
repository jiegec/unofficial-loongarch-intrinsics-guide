# \_\_lsx_vfrintrne_d

LSX intrinsic · group: [Floating Point Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_misc/index.md)

### Synopsis

```
__m128d __lsx_vfrintrne_d (__m128d a)
#include <lsxintrin.h>
Instruction: vfrintrne.d vr, vr
CPU Flags: LSX
```

### Description

Round double-precision floating point elements in `a` to integers, rounding towards nearest even, and store as floating point numbers.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = (f64)(s64)a.fp64[i]; // rounding mode is not expressed in C
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
