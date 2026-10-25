# \_\_lasx_xvmaxi_b

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvmaxi_b (__m256i a, imm_n16_15 imm)
#include <lasxintrin.h>
Instruction: xvmaxi.b xr, xr, imm
CPU Flags: LASX
```

### Description

Compute elementwise maximum for signed 8-bit elements in `a` and the sign-extended `imm`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = max((s8)a.byte[i], (s8)imm);
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
