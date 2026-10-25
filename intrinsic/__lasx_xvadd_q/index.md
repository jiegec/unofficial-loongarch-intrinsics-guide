# \_\_lasx_xvadd_q

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvadd_q (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvadd.q xr, xr, xr
CPU Flags: LASX
```

### Description

Add corresponding 128-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.qword[i] = a.qword[i] + b.qword[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
