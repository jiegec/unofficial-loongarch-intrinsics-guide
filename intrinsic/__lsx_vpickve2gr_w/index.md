# \_\_lsx_vpickve2gr_w

LSX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/misc/index.md)

### Synopsis

```
int __lsx_vpickve2gr_w (__m128i a, imm0_3 idx)
#include <lsxintrin.h>
Instruction: vpickve2gr.w r, vr, imm
CPU Flags: LSX
```

### Description

Pick the 32-bit element at lane `idx` from `a`, sign-extend it to the GPR width, and store it in `dst`.

### Operation

```
dst = (s32)a.word[idx];
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 1                |
| 3A6000   | LA664  | 1       | 1                |
| 3C6000   | LA664  | 1       | 1                |
| 2K1000LA | LA264  | 2       | 0.5(1/2)         |
| 2K3000   | LA364E | 2       | 0.5(1/2)         |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
