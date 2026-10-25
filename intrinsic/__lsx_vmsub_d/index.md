# \_\_lsx_vmsub_d

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vmsub_d (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmsub.d vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply 64-bit elements in `b` and `c`, subtract the low 64 bits of the product from elements in `a`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = -b.dword[i] * c.dword[i] + a.dword[i];
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
