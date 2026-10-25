# \_\_lasx_xvbsrl_v

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvbsrl_v (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvbsrl.v xr, xr, imm
CPU Flags: LASX
```

### Description

Shift each 128-bit lane of `a` right by `(imm % 16) * 8` bits, filling the vacated bits with zeros.

### Operation

```
int shift = (imm * 8) % 128;
dst.qword[0] = (u128)a.qword[0] >> shift;
dst.qword[1] = (u128)a.qword[1] >> shift;
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
