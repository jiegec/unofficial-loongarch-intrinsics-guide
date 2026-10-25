# \_\_lsx_vmaxi_du

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vmaxi_du (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vmaxi.du vr, vr, imm
CPU Flags: LSX
```

### Description

Compute elementwise maximum for unsigned 64-bit elements in `a` and the zero-extended `imm`.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = max((u64)a.dword[i], (u64)imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 4                |
| 3C6000   | LA664  | 2       | 4                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
