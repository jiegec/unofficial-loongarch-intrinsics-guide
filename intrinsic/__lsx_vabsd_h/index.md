# \_\_lsx_vabsd_h

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vabsd_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vabsd.h vr, vr, vr
CPU Flags: LSX
```

### Description

Compute the absolute difference between corresponding signed 16-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = ((s16)a.half[i] > (s16)b.half[i]) ? (a.half[i] - b.half[i])
                                                  : (b.half[i] - a.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
