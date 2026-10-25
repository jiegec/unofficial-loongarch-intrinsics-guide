# \_\_lasx_xvfclass_s

LASX intrinsic · group: [Floating Point Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_misc/index.md)

### Synopsis

```
__m256i __lasx_xvfclass_s (__m256 a)
#include <lasxintrin.h>
Instruction: xvfclass.s xr, xr
CPU Flags: LASX
```

### Description

Classify each single precision floating point element in `a` according to IEEE 754 `class(x)`, writing the 10-bit classification result into the corresponding element of `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.word[i] = fp_classify(a.fp32[i]);
}
```

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 4                |
| 3C6000 | LA664 | 2       | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
