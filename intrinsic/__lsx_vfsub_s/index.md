# \_\_lsx_vfsub_s

LSX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_computation/index.md)

### Synopsis

```
__m128 __lsx_vfsub_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfsub.s vr, vr, vr
CPU Flags: LSX
```

### Description

Subtract corresponding single precision floating point elements in `b` from elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = a.fp32[i] - b.fp32[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 5       | 2                |
| 3A6000   | LA664  | 3       | 4                |
| 3C6000   | LA664  | 3       | 4                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
