# \_\_lasx_xvpickve_w_f

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256 __lasx_xvpickve_w_f (__m256 a, imm0_7 imm)
#include <lasxintrin.h>
Instruction: xvpickve.w xr, xr, imm
CPU Flags: LASX
```

### Description

Copy one 32-bit lane from `a` specified by `imm` to the first lane of `dst`, and set the other lanes to zero.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (i == 0) ? a.word[imm] : 0;
}

// Expands to:

if (0) {
  dst.word[0] = a.word[imm];
  dst.word[1] = 0;
  dst.word[2] = 0;
  dst.word[3] = 0;
  dst.word[4] = 0;
  dst.word[5] = 0;
  dst.word[6] = 0;
  dst.word[7] = 0;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 4                |
| 3C6000 | LA664 | 3       | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
