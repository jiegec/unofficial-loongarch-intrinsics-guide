# \_\_lasx_xbnz_v

LASX intrinsic · group: [Branch](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/branch/index.md)

### Synopsis

```
int __lasx_xbnz_v (__m256i a)
#include <lasxintrin.h>
Instruction: xvsetnez.v fcc, xr; bcnez
CPU Flags: LASX
```

### Description

Expected to be used in branches: branch if the whole vector `a` is non-zero.

### Operation

```
dst = a.qword[0] != 0 || a.qword[1] != 0;
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | N/A     | 2                |
| 3A6000 | LA664 | N/A     | 2                |
| 3C6000 | LA664 | N/A     | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
