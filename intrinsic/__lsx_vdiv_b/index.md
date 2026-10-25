# \_\_lsx_vdiv_b

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vdiv_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vdiv.b vr, vr, vr
CPU Flags: LSX
```

### Description

Divide each signed 8-bit element in `a` by the corresponding element in `b`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (b.byte[i] == 0) ? 0 : ((s8)a.byte[i] / (s8)b.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 29, 32  | 0.06(1/17)       |
| 3A6000   | LA664  | 29, 32  | 0.06(1/15.5)     |
| 3C6000   | LA664  | 29, 32  | 0.07(1/13.5)     |
| 2K1000LA | LA264  | 30, 58  | 0(1/55)          |
| 2K3000   | LA364E | 30      | 0(1/55)          |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
