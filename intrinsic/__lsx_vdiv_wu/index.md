# \_\_lsx_vdiv_wu

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vdiv_wu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vdiv.wu vr, vr, vr
CPU Flags: LSX
```

### Description

Divide each unsigned 32-bit element in `a` by the corresponding element in `b`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = (b.word[i] == 0) ? 0 : ((u32)a.word[i] / (u32)b.word[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency  | Throughput (IPC) |
| -------- | ------ | -------- | ---------------- |
| 3C5000   | LA464  | 11, 17.5 | 0.07(1/15)       |
| 3A6000   | LA664  | 11       | 0.18(1/5.5)      |
| 3C6000   | LA664  | 11, 17.5 | 0.22(1/4.5)      |
| 2K1000LA | LA264  | 12       | 0.06(1/18)       |
| 2K3000   | LA364E | 12, 18.5 | 0.06(1/18)       |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
