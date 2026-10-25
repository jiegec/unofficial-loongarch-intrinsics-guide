# \_\_lasx_xvfclass_d

LASX intrinsic · group: [Floating Point Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/float_misc/index.md)

### Synopsis

```
__m256i __lasx_xvfclass_d (__m256d a)
#include <lasxintrin.h>
Instruction: xvfclass.d xr, xr
CPU Flags: LASX
```

### Description

Classify each double precision floating point element in `a` according to IEEE 754 `class(x)`, writing the 10-bit classification result into the corresponding element of `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = fp_classify(a.fp64[i]);
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
