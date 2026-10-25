# \_\_lsx_vmod_bu

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vmod_bu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmod.bu vr, vr, vr
CPU Flags: LSX
```

### Description

Compute the remainder of each unsigned 8-bit element in `a` divided by the corresponding element in `b`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (b.byte[i] == 0) ? 0 : ((u8)a.byte[i] % (u8)b.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 29, 33  | 0.05(1/19)       |
| 3A6000   | LA664  | 29, 37  | 0.06(1/17.5)     |
| 3C6000   | LA664  | 29      | 0.07(1/13.5)     |
| 2K1000LA | LA264  | 30, 46  | 0(1/62)          |
| 2K3000   | LA364E | 30, 46  | 0(1/63)          |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
