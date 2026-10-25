# \_\_lsx_vsrar_h

LSX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shift/index.md)

### Synopsis

```
__m128i __lsx_vsrar_h (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsrar.h vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift with rounding each signed 16-bit element in `a` by the corresponding element of `b` modulo 16, store the result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  if ((b.half[i] & 0xf) == 0) {
    dst.half[i] = a.half[i];
  } else {
    dst.half[i] = ((s16)a.half[i] >> (b.half[i] & 0xf)) +
                  (((s16)a.half[i] >> ((b.half[i] & 0xf) - 1)) & 0x1);
  }
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
