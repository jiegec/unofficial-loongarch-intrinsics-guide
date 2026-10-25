# \_\_lasx_xvreplgr2vr_h

LASX intrinsic · group: [Misc](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/misc/index.md)

### Synopsis

```
__m256i __lasx_xvreplgr2vr_h (int val)
#include <lasxintrin.h>
Instruction: xvreplgr2vr.h xr, r
CPU Flags: LASX
```

### Description

Broadcast the low 16 bits of `val` to all lanes of `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = val;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 1                |
| 3A6000 | LA664 | N/A     | 1                |
| 3C6000 | LA664 | N/A     | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
