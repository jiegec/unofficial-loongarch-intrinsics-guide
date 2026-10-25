# \_\_lsx_vfcvth_s_h

LSX intrinsic · group: [Floating Point Conversion](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_conversion/index.md)

### Synopsis

```
__m128 __lsx_vfcvth_s_h (__m128i a)
#include <lsxintrin.h>
Instruction: vfcvth.s.h vr, vr
CPU Flags: LSX
```

### Description

Convert half precision floating point elements in higher half of every 128-bit vector of `a` to single precision.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.fp32[i] = a.fp16[i + 4];
}

// Expands to:

if (0) {
  dst.fp32[0] = a.fp16[4];
  dst.fp32[1] = a.fp16[5];
  dst.fp32[2] = a.fp16[6];
  dst.fp32[3] = a.fp16[7];
}
```

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
