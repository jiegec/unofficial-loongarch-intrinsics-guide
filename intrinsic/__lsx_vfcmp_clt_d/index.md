# \_\_lsx_vfcmp_clt_d

LSX intrinsic · group: [Floating Point Comparison](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_comparison/index.md)

### Synopsis

```
__m128i __lsx_vfcmp_clt_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vfcmp.clt.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compare corresponding double precision elements in `a` and `b`, save the comparison result (all ones if LT(Less than), all zeros otherwise) into `dst`. No exception is raised for QNaN.

### Operation

```
for (int i = 0; i < 2; i++) {
  if (fp_compare_clt(a.fp64[i], b.fp64[i])) {
    dst.dword[i] = 0xFFFFFFFFFFFFFFFF;
  } else {
    dst.dword[i] = 0;
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
