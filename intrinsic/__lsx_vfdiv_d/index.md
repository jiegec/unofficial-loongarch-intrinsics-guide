# \_\_lsx_vfdiv_d

LSX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_computation/index.md)

### Synopsis

```
__m128d __lsx_vfdiv_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfdiv.d vr, vr, vr
CPU Flags: LSX
```

### Description

Divide corresponding double precision floating point elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = a.fp64[i] / b.fp64[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 8, 16.5 | 0.08(1/12.5)     |
| 3A6000   | LA664  | 8, 21.5 | 0.25(1/4)        |
| 3C6000   | LA664  | 8, 16.5 | 0.33(1/3)        |
| 2K1000LA | LA264  | 9, 24   | 0.04(1/24)       |
| 2K3000   | LA364E | 9, 24   | 0.04(1/24)       |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
