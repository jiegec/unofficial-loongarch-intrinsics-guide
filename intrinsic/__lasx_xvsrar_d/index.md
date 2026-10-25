# \_\_lasx_xvsrar_d

LASX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/shift/index.md)

### Synopsis

```
__m256i __lasx_xvsrar_d (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsrar.d xr, xr, xr
CPU Flags: LASX
```

### Description

Arithmetic right shift with rounding each signed 64-bit element in `a` by the corresponding element of `b` modulo 64, store the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  if ((b.dword[i] & 0x3f) == 0) {
    dst.dword[i] = a.dword[i];
  } else {
    dst.dword[i] = ((s64)a.dword[i] >> (b.dword[i] & 0x3f)) +
                   (((s64)a.dword[i] >> ((b.dword[i] & 0x3f) - 1)) & 0x1);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 3       | 2                |
| 3A6000 | LA664 | 3       | 2                |
| 3C6000 | LA664 | 3       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
