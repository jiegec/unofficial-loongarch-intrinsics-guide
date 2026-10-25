# \_\_lsx_vsrlr_w

LSX intrinsic · group: [Shift](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/shift/index.md)

### Synopsis

```
__m128i __lsx_vsrlr_w (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vsrlr.w vr, vr, vr
CPU Flags: LSX
```

### Description

Logical right shift with rounding each unsigned 32-bit element in `a` by the corresponding element of `b` modulo 32, store the result in `dst`.

### Operation

```
for (int i = 0; i < 4; i++) {
  if ((b.word[i] & 0x1f) == 0) {
    dst.word[i] = a.word[i];
  } else {
    dst.word[i] = (a.word[i] >> (b.word[i] & 0x1f)) +
                  ((a.word[i] >> ((b.word[i] & 0x1f) - 1)) & 0x1);
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
