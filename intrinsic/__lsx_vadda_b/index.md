# \_\_lsx_vadda_b

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vadda_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vadda.b vr, vr, vr
CPU Flags: LSX
```

### Description

Add the absolute values of the 8-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = abs((s8)a.byte[i]) + abs((s8)b.byte[i]);
}
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
