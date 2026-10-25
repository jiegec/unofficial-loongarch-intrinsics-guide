# \_\_lsx_vinsgr2vr_h

LSX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/misc/index.md)

### Synopsis

```
__m128i __lsx_vinsgr2vr_h (__m128i a, int b, imm0_7 imm)
#include <lsxintrin.h>
Instruction: vinsgr2vr.h vr, r, imm
CPU Flags: LSX
```

### Description

Insert the low 16 bits of `b` into lane `imm` of `a`; the other lanes are unchanged.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (i == imm) ? b : a.half[i];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 1                |
| 3A6000   | LA664  | 1       | 1                |
| 3C6000   | LA664  | 1       | 1                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
