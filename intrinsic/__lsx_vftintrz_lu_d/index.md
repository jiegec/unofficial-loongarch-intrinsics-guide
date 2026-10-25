# \_\_lsx_vftintrz_lu_d

LSX intrinsic · group: [Floating Point Conversion](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_conversion/index.md)

### Synopsis

```
__m128i __lsx_vftintrz_lu_d (__m128d a)
#include <lsxintrin.h>
Instruction: vftintrz.lu.d vr, vr
CPU Flags: LSX
```

### Description

Convert double-precision floating point elements in `a` to unsigned 64-bit integer, rounding towards zero.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (u64)a.fp64[i]; // rounding mode is not expressed in C
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
