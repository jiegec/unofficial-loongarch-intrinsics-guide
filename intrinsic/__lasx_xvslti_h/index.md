# \_\_lasx_xvslti_h

LASX intrinsic · group: [Integer Comparison](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_comparison/index.md)

### Synopsis

```
__m256i __lasx_xvslti_h (__m256i a, imm_n16_15 imm)
#include <lasxintrin.h>
Instruction: xvslti.h xr, xr, imm
CPU Flags: LASX
```

### Description

Compare the signed 16-bit elements in `a` and the sign-extended `imm`, store all-ones to `dst` if corresponding element in `a` is less than `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = ((s16)a.half[i] < imm) ? 0xFFFF : 0;
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
