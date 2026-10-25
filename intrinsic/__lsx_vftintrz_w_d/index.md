# \_\_lsx_vftintrz_w_d

LSX intrinsic · group: [Floating Point Conversion](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_conversion/index.md)

### Synopsis

```
__m128i __lsx_vftintrz_w_d (__m128d a, __m128d b)
#include <lsxintrin.h>
Instruction: vftintrz.w.d vr, vr, vr
CPU Flags: LSX
```

### Description

For each 128-bit lane, convert double-precision floating point elements in the corresponding lane of `a` into the upper half and elements in `b` into the lower half, as signed 32-bit integers, rounding towards zero.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2)
                    ? (s32)b.fp64[i]
                    : (s32)a.fp64[i - 2]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.word[0] = (s32)b.fp64[0];
  dst.word[1] = (s32)b.fp64[1];
  dst.word[2] = (s32)a.fp64[0];
  dst.word[3] = (s32)a.fp64[1];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 5       | 1                |
| 3A6000   | LA664  | 5       | 2                |
| 3C6000   | LA664  | 5       | 2                |
| 2K1000LA | LA264  | 5       | 0.5(1/2)         |
| 2K3000   | LA364E | 5       | 0.5(1/2)         |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
