# \_\_lsx_vandn_v

LSX intrinsic · group: [Logical](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/logical/index.md)

### Synopsis

```
__m128i __lsx_vandn_v (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vandn.v vr, vr, vr
CPU Flags: LSX
```

### Description

Compute bitwise ANDN (~a & b) of elements in `a` and `b`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = b.dword[i] & (~a.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
