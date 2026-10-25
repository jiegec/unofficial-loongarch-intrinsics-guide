# \_\_lasx_xvreplve_w

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvreplve_w (__m256i a, int idx)
#include <lasxintrin.h>
Instruction: xvreplve.w xr, xr, r
CPU Flags: LASX
```

### Description

For each 128-bit lane, repeat that lane's element `idx` (index modulo 4) to fill the same lane.

### Operation

```
for (int i = 0; i < 4; i++) {
  dst.word[i] = a.word[idx % 4];
}
for (int i = 4; i < 8; i++) {
  dst.word[i] = a.word[(idx % 4) + 4];
}
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
