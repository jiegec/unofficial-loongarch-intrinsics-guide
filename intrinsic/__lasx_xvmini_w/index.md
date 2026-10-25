# \_\_lasx_xvmini_w

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvmini_w (__m256i a, imm_n16_15 imm)
#include <lasxintrin.h>
Instruction: xvmini.w xr, xr, imm
CPU Flags: LASX
```

### Description

Compute elementwise minimum for signed 32-bit elements in `a` and the sign-extended `imm`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = min((s32)a.word[i], (s32)imm);
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
