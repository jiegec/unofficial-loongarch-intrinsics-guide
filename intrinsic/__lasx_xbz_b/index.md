# \_\_lasx_xbz_b

LASX intrinsic · group: [Branch](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/branch/index.md)

### Synopsis

```
int __lasx_xbz_b (__m256i a)
#include <lasxintrin.h>
Instruction: xvsetanyeqz.b fcc, xr; bcnez
CPU Flags: LASX
```

### Description

Expected to be used in branches: branch if any 8-bit element in `a` equals to zero.

### Operation

```
dst = 0;
for (int i = 0; i < 32; i++) {
  if (a.byte[i] == 0) {
    dst = 1;
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
