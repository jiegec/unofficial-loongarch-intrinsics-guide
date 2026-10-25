# \_\_lsx_vslti_b

LSX intrinsic · group: [Integer Comparison](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_comparison/index.md)

### Synopsis

```
__m128i __lsx_vslti_b (__m128i a, imm_n16_15 imm)
#include <lsxintrin.h>
Instruction: vslti.b vr, vr, imm
CPU Flags: LSX
```

### Description

Compare the signed 8-bit elements in `a` and the sign-extended `imm`, store all-ones to `dst` if corresponding element in `a` is less than `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = ((s8)a.byte[i] < imm) ? 0xFF : 0;
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
