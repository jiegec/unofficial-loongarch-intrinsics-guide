# \_\_lsx_vsigncov_b

LSX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/misc/index.md)

### Synopsis

```
__m128i __lsx_vsigncov_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsigncov.b vr, vr, vr
CPU Flags: LSX
```

### Description

If the 8-bit element in `a` equals to zero, set the result to zero. If the signed 8-bit element in `a` is positive, copy element in `b` to result. Otherwise, copy negated element in `b` to result. If `a` and `b` are the same vectors, it is equivalent to computing absolute value.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] =
      (a.byte[i] == 0) ? 0 : ((s8)a.byte[i] > 0 ? b.byte[i] : -b.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 2                |
| 3C6000   | LA664  | 1       | 2                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
