# \_\_lasx_xvrepl128vei_h

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvrepl128vei_h (__m256i a, imm0_7 idx)
#include <lasxintrin.h>
Instruction: xvrepl128vei.h xr, xr, imm
CPU Flags: LASX
```

### Description

For each 128-bit lane, repeat that lane's element `idx` to fill the same lane.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = a.half[idx];
}
for (int i = 8; i < 16; i++) {
  dst.half[i] = a.half[idx + 8];
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
