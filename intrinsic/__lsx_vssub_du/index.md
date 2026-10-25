# \_\_lsx_vssub_du

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vssub_du (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vssub.du vr, vr, vr
CPU Flags: LSX
```

### Description

Saturating subtract corresponding unsigned 64-bit elements in `b` from elements in `a`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (u64)ssub((u64)a.dword[i], (u64)b.dword[i]);
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
