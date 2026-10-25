# \_\_lsx_vslei_wu

LSX intrinsic · group: [Integer Comparison](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_comparison/index.md)

### Synopsis

```
__m128i __lsx_vslei_wu (__m128i a, imm0_31 imm)
#include <lsxintrin.h>
Instruction: vslei.wu vr, vr, imm
CPU Flags: LSX
```

### Description

Compare the unsigned 32-bit elements in `a` and the zero-extended `imm`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = ((u32)a.word[i] <= imm) ? 0xFFFFFFFF : 0;
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
