# \_\_lsx_vsll_h

LSX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shift/index.md)

### Synopsis

```
__m128i __lsx_vsll_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsll.h vr, vr, vr
CPU Flags: LSX
```

### Description

Logical left shift each unsigned 16-bit element in `a` by the corresponding element of `b` modulo 16, store the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = a.half[i] << (b.half[i] & 0xf);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 1       | 2                |
| 3A6000   | LA664  | 1       | 4                |
| 3C6000   | LA664  | 1       | 4                |
| 2K1000LA | LA264  | 1       | 1                |
| 2K3000   | LA364E | 1       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
