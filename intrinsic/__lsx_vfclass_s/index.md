# \_\_lsx_vfclass_s

LSX intrinsic · group: [Floating Point Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/float_misc/index.md)

### Synopsis

```
__m128i __lsx_vfclass_s (__m128 a)
#include <lsxintrin.h>
Instruction: vfclass.s vr, vr
CPU Flags: LSX
```

### Description

Classify each single precision floating point element in `a` according to IEEE 754 `class(x)`, writing the 10-bit classification result into the corresponding element of `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = fp_classify(a.fp32[i]);
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
