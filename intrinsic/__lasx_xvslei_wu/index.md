# \_\_lasx_xvslei_wu

LASX intrinsic · group: [Integer Comparison](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_comparison/index.md)

### Synopsis

```
__m256i __lasx_xvslei_wu (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvslei.wu xr, xr, imm
CPU Flags: LASX
```

### Description

Compare the unsigned 32-bit elements in `a` and the zero-extended `imm`, store all-ones to `dst` if corresponding element in `a` is less than or equal to `imm`, zero otherwise.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = ((u32)a.word[i] <= imm) ? 0xFFFFFFFF : 0;
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
