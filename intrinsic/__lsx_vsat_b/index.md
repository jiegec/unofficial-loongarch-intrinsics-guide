# \_\_lsx_vsat_b

LSX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/misc/index.md)

### Synopsis

```
__m128i __lsx_vsat_b (__m128i a, imm0_7 imm)
#include <lsxintrin.h>
Instruction: vsat.b vr, vr, imm
CPU Flags: LSX
```

### Description

Saturate each signed 8-bit element in `a` to the number of significant bits specified by `imm`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = clamp<s8>(a.byte[i], -(1 << imm), (1 << imm) - 1);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
