# \_\_lsx_vsat_wu

LSX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/misc/index.md)

### Synopsis

```
__m128i __lsx_vsat_wu (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vsat.wu vr, vr, imm
CPU Flags: LSX
```

### Description

Saturate each unsigned 32-bit element in `a` to the number of significant bits specified by `imm`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = clamp<u32>(a.word[i], 0, (1 << (imm + 1)) - 1);
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
