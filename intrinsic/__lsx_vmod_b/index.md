# \_\_lsx_vmod_b

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vmod_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmod.b vr, vr, vr
CPU Flags: LSX
```

### Description

Compute the remainder of each signed 8-bit element in `a` divided by the corresponding element in `b`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (b.byte[i] == 0) ? 0 : ((s8)a.byte[i] % (s8)b.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 29, 33  | 0.06(1/17)       |
| 3A6000   | LA664  | 29, 35  | 0.06(1/15.5)     |
| 3C6000   | LA664  | 29      | 0.07(1/13.5)     |
| 2K1000LA | LA264  | 30, 46  | 0(1/63)          |
| 2K3000   | LA364E | 30, 46  | 0(1/63)          |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
