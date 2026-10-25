# \_\_lsx_vmod_w

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vmod_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmod.w vr, vr, vr
CPU Flags: LSX
```

### Description

Compute the remainder of each signed 32-bit element in `a` divided by the corresponding element in `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (b.word[i] == 0) ? 0 : ((s32)a.word[i] % (s32)b.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 11, 15  | 0.08(1/12)       |
| 3A6000   | LA664  | 11, 13  | 0.18(1/5.5)      |
| 3C6000   | LA664  | 11      | 0.22(1/4.5)      |
| 2K1000LA | LA264  | 12, 16  | 0.05(1/20)       |
| 2K3000   | LA364E | 12, 16  | 0.05(1/20)       |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
