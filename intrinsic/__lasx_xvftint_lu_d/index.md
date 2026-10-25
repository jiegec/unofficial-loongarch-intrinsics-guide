# \_\_lasx_xvftint_lu_d

LASX intrinsic · group: [Floating Point Conversion](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_conversion/index.md)

### Synopsis

```
__m256i __lasx_xvftint_lu_d (__m256d a)
#include <lasxintrin.h>
Instruction: xvftint.lu.d xr, xr
CPU Flags: LASX
```

### Description

Convert double-precision floating point elements in `a` to unsigned 64-bit integer, using current rounding mode specified in `fscr`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (u64)a.fp64[i]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.dword[0] = (u64)a.fp64[0];
  dst.dword[1] = (u64)a.fp64[1];
  dst.dword[2] = (u64)a.fp64[2];
  dst.dword[3] = (u64)a.fp64[3];
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 4                |
| 3C6000 | LA664 | 4       | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
