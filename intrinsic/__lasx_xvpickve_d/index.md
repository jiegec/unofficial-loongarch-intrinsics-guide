# \_\_lasx_xvpickve_d

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvpickve_d (__m256i a, imm0_3 imm)
#include <lasxintrin.h>
Instruction: xvpickve.d xr, xr, imm
CPU Flags: LASX
```

### Description

Copy one 64-bit lane from `a` specified by `imm` to the first lane of `dst`, and set the other lanes to zero.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.dword[i] = (i == 0) ? a.dword[imm] : 0;
}

// Expands to:

if (0) {
  dst.dword[0] = a.dword[imm];
  dst.dword[1] = 0;
  dst.dword[2] = 0;
  dst.dword[3] = 0;
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
