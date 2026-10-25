# \_\_lasx_xvffint_d_lu

LASX intrinsic · group: [Floating Point Conversion](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_conversion/index.md)

### Synopsis

```
__m256d __lasx_xvffint_d_lu (__m256i a)
#include <lasxintrin.h>
Instruction: xvffint.d.lu xr, xr
CPU Flags: LASX
```

### Description

Convert unsigned 64-bit integer elements in `a` to double-precision floating point numbers.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp64[i] = (f64)(u64)a.dword[i]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.fp64[0] = (f64)((u64)a.dword[0]);
  dst.fp64[1] = (f64)((u64)a.dword[1]);
  dst.fp64[2] = (f64)((u64)a.dword[2]);
  dst.fp64[3] = (f64)((u64)a.dword[3]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 4                |
| 3C6000 | LA664 | 4       | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
