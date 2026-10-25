# \_\_lsx_vssub_d

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vssub_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssub.d vr, vr, vr
CPU Flags: LSX
```

### Description

Saturating subtract corresponding signed 64-bit elements in `b` from elements in `a`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)ssub((s64)a.dword[i], (s64)b.dword[i]);
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
