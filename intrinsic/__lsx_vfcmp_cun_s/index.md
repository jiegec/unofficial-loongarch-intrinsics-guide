# \_\_lsx_vfcmp_cun_s

LSX intrinsic · group: [Floating Point Comparison](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_comparison/index.md)

### Synopsis

```
__m128i __lsx_vfcmp_cun_s (__m128 a, __m128 b)
#include <lsxintrin.h>
Instruction: vfcmp.cun.s vr, vr, vr
CPU Flags: LSX
```

### Description

Compare corresponding single precision elements in `a` and `b`, save the comparison result (all ones if UN(Unordered), all zeros otherwise) into `dst`. No exception is raised for QNaN.

### Operation

```
for (int i = 0; i < 4; i++) {
  if (fp_compare_cun(a.fp32[i], b.fp32[i])) {
    dst.word[i] = 0xFFFFFFFF;
  } else {
    dst.word[i] = 0;
  }
```

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
