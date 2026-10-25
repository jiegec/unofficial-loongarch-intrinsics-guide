# \_\_lasx_xvpickve2gr_du

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
unsigned long int __lasx_xvpickve2gr_du (__m256i a, imm0_3 idx)
#include <lasxintrin.h>
Instruction: xvpickve2gr.du r, xr, imm
CPU Flags: LASX
```

### Description

Pick the 64-bit element at lane `idx` from `a`, zero-extend it to the GPR width, and store it in `dst`.

### Operation

```
dst = (u64)a.dword[idx];
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 1       | 1                |
| 3A6000 | LA664 | 1       | 1                |
| 3C6000 | LA664 | 1       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
