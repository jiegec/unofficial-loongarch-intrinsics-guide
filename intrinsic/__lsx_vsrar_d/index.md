# \_\_lsx_vsrar_d

LSX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shift/index.md)

### Synopsis

```
__m128i __lsx_vsrar_d (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsrar.d vr, vr, vr
CPU Flags: LSX
```

### Description

Arithmetic right shift with rounding each signed 64-bit element in `a` by the corresponding element of `b` modulo 64, store the result in `dst`.

### Operation

```
for (int i = 0; i < 2; i++) {
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

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 3       | 2                |
| 3A6000   | LA664  | 3       | 2                |
| 3C6000   | LA664  | 3       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
