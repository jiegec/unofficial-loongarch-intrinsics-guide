# \_\_lsx_vmod_d

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vmod_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmod.d vr, vr, vr
CPU Flags: LSX
```

### Description

Compute the remainder of each signed 64-bit element in `a` divided by the corresponding element in `b`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (b.dword[i] == 0) ? 0 : ((s64)a.dword[i] % (s64)b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 8, 10   | 0.11(1/9.5)      |
| 3A6000   | LA664  | 8, 10   | 0.25(1/4)        |
| 3C6000   | LA664  | 8       | 0.33(1/3)        |
| 2K1000LA | LA264  | 9, 11   | 0.08(1/13)       |
| 2K3000   | LA364E | 9, 11   | 0.08(1/13)       |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
