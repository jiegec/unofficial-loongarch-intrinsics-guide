# \_\_lasx_xvrotri_b

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvrotri_b (__m256i a, imm0_7 imm)
#include <lasxintrin.h>
Instruction: xvrotri.b xr, xr, imm
CPU Flags: LASX
```

### Description

Rotate right the unsigned 8-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = (a.byte[i] >> imm) | (a.byte[i] << (8 - imm));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 1       | 4                |
| 3C6000 | LA664 | 1       | 4                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
