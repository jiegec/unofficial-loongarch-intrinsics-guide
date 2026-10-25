# \_\_lsx_vdiv_d

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vdiv_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vdiv.d vr, vr, vr
CPU Flags: LSX
```

### Description

Divide each signed 64-bit element in `a` by the corresponding element in `b`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = (b.dword[i] == 0) ? 0 : ((s64)a.dword[i] / (s64)b.dword[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 8, 18.5 | 0.11(1/9)        |
| 3A6000   | LA664  | 8       | 0.25(1/4)        |
| 3C6000   | LA664  | 8, 18.5 | 0.33(1/3)        |
| 2K1000LA | LA264  | 9, 19.5 | 0.08(1/12)       |
| 2K3000   | LA364E | 9       | 0.08(1/12)       |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
