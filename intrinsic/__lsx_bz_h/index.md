# \_\_lsx_bz_h

LSX intrinsic · group: [Branch](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/branch/index.md)

### Synopsis

```
int __lsx_bz_h (__m128i a)
#include <lsxintrin.h>
Instruction: vsetanyeqz.h fcc, vr; bcnez
CPU Flags: LSX
```

### Description

Expected to be used in branches: branch if any 16-bit element in `a` equals to zero.

### Operation

```
dst = 0;
for (int i = 0; i < 8; i++) {
  if (a.half[i] == 0) {
    dst = 1;
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | N/A     | 2                |
| 3A6000   | LA664  | N/A     | 2                |
| 3C6000   | LA664  | N/A     | 2                |
| 2K1000LA | LA264  | N/A     | 1                |
| 2K3000   | LA364E | N/A     | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
