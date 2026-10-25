# \_\_lsx_vmadd_w

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vmadd_w (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmadd.w vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply 32-bit elements in `b` and `c`, add the low 32 bits of the product to elements in `a`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = b.word[i] * c.word[i] + a.word[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
