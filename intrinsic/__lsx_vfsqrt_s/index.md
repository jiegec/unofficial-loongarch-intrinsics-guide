# \_\_lsx_vfsqrt_s

LSX intrinsic · group: [Floating Point Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_computation/index.md)

### Synopsis

```
__m128 __lsx_vfsqrt_s (__m128 a)
#include <lsxintrin.h>
Instruction: vfsqrt.s vr, vr
CPU Flags: LSX
```

### Description

Compute square root of single precision floating point elements in `a`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = sqrt(a.fp32[i]);
}
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 27      | 0.17(1/6)        |
| 3A6000   | LA664  | 11      | 0.08(1/12)       |
| 3C6000   | LA664  | 25      | 0.09(1/11.5)     |
| 2K1000LA | LA264  | 16      | 0.03(1/40)       |
| 2K3000   | LA364E | 28      | 0.03(1/40)       |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
