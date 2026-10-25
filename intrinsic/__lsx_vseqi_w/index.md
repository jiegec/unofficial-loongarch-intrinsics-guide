# \_\_lsx_vseqi_w

LSX intrinsic · group: [Integer Comparison](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_comparison/index.md)

### Synopsis

```
__m128i __lsx_vseqi_w (__m128i a, imm_n16_15 imm)
#include <lsxintrin.h>
Instruction: vseqi.w vr, vr, imm
CPU Flags: LSX
```

### Description

Compare the 32-bit elements in `a` and the sign-extended `imm`, store all-ones to `dst` if equal, zero otherwise.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = ((s32)a.word[i] == imm) ? 0xFFFFFFFF : 0;
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
