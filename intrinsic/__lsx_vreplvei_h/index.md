# \_\_lsx_vreplvei_h

LSX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/misc/index.md)

### Synopsis

```
__m128i __lsx_vreplvei_h (__m128i a, imm0_7 idx)
#include <lsxintrin.h>
Instruction: vreplvei.h vr, vr, imm
CPU Flags: LSX
```

### Description

Repeat the element in lane `idx` of `a` to fill whole vector.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = a.half[idx];
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
