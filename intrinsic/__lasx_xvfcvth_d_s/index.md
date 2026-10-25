# \_\_lasx_xvfcvth_d_s

LASX intrinsic · group: [Floating Point Conversion](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_conversion/index.md)

### Synopsis

```
__m256d __lasx_xvfcvth_d_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvfcvth.d.s xr, xr
CPU Flags: LASX
```

### Description

Convert single precision floating point elements in higher half of every 128-bit vector of `a` to double precision.

### Operation

```
int i;
for (i = 0; i < 2; i++) {
  dst.fp64[i] = a.fp32[i + 2];
}
for (; i < 4; i++) {
  dst.fp64[i] = a.fp32[i + 4];
}

// Expands to:

if (0) {
  dst.fp64[0] = a.fp32[2];
  dst.fp64[1] = a.fp32[3];
  dst.fp64[2] = a.fp32[6];
  dst.fp64[3] = a.fp32[7];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 1                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
