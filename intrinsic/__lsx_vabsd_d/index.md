# \_\_lsx_vabsd_d

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vabsd_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vabsd.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compute the absolute difference between corresponding signed 64-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = ((s64)a.dword[i] > (s64)b.dword[i])
                     ? (a.dword[i] - b.dword[i])
                     : (b.dword[i] - a.dword[i]);
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
