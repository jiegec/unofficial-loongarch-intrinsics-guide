# \_\_lsx_vdiv_bu

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vdiv_bu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vdiv.bu vr, vr, vr
CPU Flags: LSX
```

### Description

Divide each unsigned 8-bit element in `a` by the corresponding element in `b`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = (b.byte[i] == 0) ? 0 : ((u8)a.byte[i] / (u8)b.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 29, 36  | 0.06(1/18)       |
| 3A6000   | LA664  | 29, 33  | 0.06(1/16.5)     |
| 3C6000   | LA664  | 29, 36  | 0.07(1/13.5)     |
| 2K1000LA | LA264  | 30, 44  | 0(1/55)          |
| 2K3000   | LA364E | 30      | 0(1/55)          |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
