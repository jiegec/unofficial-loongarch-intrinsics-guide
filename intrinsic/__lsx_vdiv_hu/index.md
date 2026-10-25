# \_\_lsx_vdiv_hu

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vdiv_hu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vdiv.hu vr, vr, vr
CPU Flags: LSX
```

### Description

Divide each unsigned 16-bit element in `a` by the corresponding element in `b`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (b.half[i] == 0) ? 0 : ((u16)a.half[i] / (u16)b.half[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency  | Throughput (IPC) |
| -------- | ------ | -------- | ---------------- |
| 3C5000   | LA464  | 17, 21.5 | 0.07(1/14)       |
| 3A6000   | LA664  | 17, 22   | 0.11(1/9)        |
| 3C6000   | LA664  | 17, 21.5 | 0.13(1/7.5)      |
| 2K1000LA | LA264  | 18, 27   | 0.03(1/31)       |
| 2K3000   | LA364E | 18, 27   | 0.03(1/31)       |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
