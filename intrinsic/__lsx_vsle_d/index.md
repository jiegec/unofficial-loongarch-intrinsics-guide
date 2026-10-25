# \_\_lsx_vsle_d

LSX intrinsic · group: [Integer Comparison](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_comparison/index.md)

### Synopsis

```
__m128i __lsx_vsle_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsle.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compare the signed 64-bit elements in `a` and `b`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `b`, zero otherwise.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = ((s64)a.dword[i] <= (s64)b.dword[i]) ? 0xFFFFFFFFFFFFFFFF : 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
