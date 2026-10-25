# \_\_lsx_vnori_b

LSX intrinsic · group: [Logical](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/logical/index.md)

### Synopsis

```
__m128i __lsx_vnori_b (__m128i a, imm0_255 imm)
#include <lsxintrin.h>
Instruction: vnori.b vr, vr, imm
CPU Flags: LSX
```

### Description

Compute bitwise NOR (~(a | imm)) of elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = ~(a.byte[i] | imm);
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
