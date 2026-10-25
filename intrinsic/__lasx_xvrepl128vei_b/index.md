# \_\_lasx_xvrepl128vei_b

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvrepl128vei_b (__m256i a, imm0_15 idx)
#include <lasxintrin.h>
Instruction: xvrepl128vei.b xr, xr, imm
CPU Flags: LASX
```

### Description

For each 128-bit lane, repeat that lane's element `idx` to fill the same lane.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[idx];
}
for (int i = 16; i < 32; i++) {
  dst.byte[i] = a.byte[idx + 16];
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
