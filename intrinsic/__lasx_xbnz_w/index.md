# \_\_lasx_xbnz_w

LASX intrinsic · group: [Branch](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/branch/index.md)

### Synopsis

```
int __lasx_xbnz_w (__m256i a)
#include <lasxintrin.h>
Instruction: xvsetallnez.w fcc, xr; bcnez
CPU Flags: LASX
```

### Description

Expected to be used in branches: branch if all 32-bit elements in `a` are non-zero.

### Operation

```
dst = 1;
for (int i = 0; i < 8; i++) {
  if (a.word[i] == 0) {
    dst = 0;
  }
}
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
