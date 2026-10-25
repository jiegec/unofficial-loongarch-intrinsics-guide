# \_\_lasx_xvreplve_b

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvreplve_b (__m256i a, int idx)
#include <lasxintrin.h>
Instruction: xvreplve.b xr, xr, r
CPU Flags: LASX
```

### Description

For each 128-bit lane, repeat that lane's element `idx` (index modulo 16) to fill the same lane.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = a.byte[idx % 16];
}
for (int i = 16; i < 32; i++) {
  dst.byte[i] = a.byte[(idx % 16) + 16];
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
