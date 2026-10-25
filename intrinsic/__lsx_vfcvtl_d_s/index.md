# \_\_lsx_vfcvtl_d_s

LSX intrinsic · group: [Floating Point Conversion](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_conversion/index.md)

### Synopsis

```
__m128d __lsx_vfcvtl_d_s (__m128 a)
#include <lsxintrin.h>
Instruction: vfcvtl.d.s vr, vr
CPU Flags: LSX
```

### Description

Convert single precision floating point elements in lower half of every 128-bit vector of `a` to double precision.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.fp64[i] = a.fp32[i];
}

// Expands to:

if (0) {
  dst.fp64[0] = a.fp32[0];
  dst.fp64[1] = a.fp32[1];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 1                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 3       | 0.5(1/2)         |
| 2K3000   | LA364E | 3       | 0.5(1/2)         |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
