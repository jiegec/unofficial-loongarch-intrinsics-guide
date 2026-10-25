# \_\_lsx_vmini_b

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vmini_b (__m128i a, imm_n16_15 imm)
#include <lsxintrin.h>
Instruction: vmini.b vr, vr, imm
CPU Flags: LSX
```

### Description

Compute elementwise minimum for signed 8-bit elements in `a` and the sign-extended `imm`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = min((s8)a.byte[i], (s8)imm);
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
