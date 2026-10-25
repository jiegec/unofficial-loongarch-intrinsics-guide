# \_\_lsx_vfdiv_s

LSX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_computation/index.md)

### Synopsis

```
__m128 __lsx_vfdiv_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfdiv.s vr, vr, vr
CPU Flags: LSX
```

### Description

Divide corresponding single precision floating point elements in `a` by elements in `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = a.fp32[i] / b.fp32[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency  | Throughput (IPC) |
| -------- | ------ | -------- | ---------------- |
| 3C5000   | LA464  | 11, 19.5 | 0.13(1/7.5)      |
| 3A6000   | LA664  | 11       | 0.18(1/5.5)      |
| 3C6000   | LA664  | 11       | 0.22(1/4.5)      |
| 2K1000LA | LA264  | 12, 28   | 0.04(1/28)       |
| 2K3000   | LA364E | 12, 28   | 0.04(1/28)       |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
