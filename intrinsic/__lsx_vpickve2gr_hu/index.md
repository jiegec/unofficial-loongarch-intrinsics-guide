# \_\_lsx_vpickve2gr_hu

LSX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/misc/index.md)

### Synopsis

```
unsigned int __lsx_vpickve2gr_hu (__m128i a, imm0_7 idx)
#include <lsxintrin.h>
Instruction: vpickve2gr.hu r, vr, imm
CPU Flags: LSX
```

### Description

Pick the 16-bit element at lane `idx` from `a`, zero-extend it to the GPR width, and store it in `dst`.

### Operation

```
dst = (u16)a.half[idx];
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
