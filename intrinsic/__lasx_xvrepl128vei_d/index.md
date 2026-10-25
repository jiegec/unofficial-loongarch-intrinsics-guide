# \_\_lasx_xvrepl128vei_d

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvrepl128vei_d (__m256i a, imm0_1 idx)
#include <lasxintrin.h>
Instruction: xvrepl128vei.d xr, xr, imm
CPU Flags: LASX
```

### Description

For each 128-bit lane, repeat that lane's element `idx` to fill the same lane.

### Operation

```
for (int i = 0; i < 2; i++) {
  dst.dword[i] = a.dword[idx];
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = a.dword[idx + 2];
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
