# \_\_lsx_vmod_hu

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vmod_hu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmod.hu vr, vr, vr
CPU Flags: LSX
```

### Description

Compute the remainder of each unsigned 16-bit element in `a` divided by the corresponding element in `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (b.half[i] == 0) ? 0 : ((u16)a.half[i] % (u16)b.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 17, 21  | 0.07(1/15)       |
| 3A6000   | LA664  | 17, 21  | 0.11(1/9.5)      |
| 3C6000   | LA664  | 17      | 0.13(1/7.5)      |
| 2K1000LA | LA264  | 18, 26  | 0.03(1/35)       |
| 2K3000   | LA364E | 18, 26  | 0.03(1/35)       |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
