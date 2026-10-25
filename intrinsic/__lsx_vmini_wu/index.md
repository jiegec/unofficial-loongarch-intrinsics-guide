# \_\_lsx_vmini_wu

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vmini_wu (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vmini.wu vr, vr, imm
CPU Flags: LSX
```

### Description

Compute elementwise minimum for unsigned 32-bit elements in `a` and the zero-extended `imm`.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = min((u32)a.word[i], (u32)imm);
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
