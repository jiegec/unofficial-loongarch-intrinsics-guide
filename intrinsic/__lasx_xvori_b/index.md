# \_\_lasx_xvori_b

LASX intrinsic · group: [Logical](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/logical/index.md)

### Synopsis

```
__m256i __lasx_xvori_b (__m256i a, imm0_255 imm)
#include <lasxintrin.h>
Instruction: xvori.b xr, xr, imm
CPU Flags: LASX
```

### Description

Compute bitwise OR of elements in `a` and `imm`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = a.byte[i] | imm;
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
