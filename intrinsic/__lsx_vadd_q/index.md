# \_\_lsx_vadd_q

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vadd_q (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vadd.q vr, vr, vr
CPU Flags: LSX
```

### Description

Add corresponding 128-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
dst.qword[0] = a.qword[0] + b.qword[0];
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
