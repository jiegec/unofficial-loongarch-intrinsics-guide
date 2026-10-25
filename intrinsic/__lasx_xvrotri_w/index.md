# \_\_lasx_xvrotri_w

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvrotri_w (__m256i a, imm0_31 imm)
#include <lasxintrin.h>
Instruction: xvrotri.w xr, xr, imm
CPU Flags: LASX
```

### Description

Rotate right the unsigned 32-bit elements in `a` by `imm`, store the result to `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.word[i] = (a.word[i] >> imm) | (a.word[i] << (32 - imm));
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
