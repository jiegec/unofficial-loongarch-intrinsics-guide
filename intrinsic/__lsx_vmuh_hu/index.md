# \_\_lsx_vmuh_hu

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vmuh_hu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmuh.hu vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply corresponding unsigned 16-bit elements in `a` and `b`, save the upper half of the double-width product in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (((u32)(u16)a.half[i] * (u32)(u16)b.half[i])) >> 16;
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 4       | 2                |
| 3A6000   | LA664  | 4       | 2                |
| 3C6000   | LA664  | 4       | 2                |
| 2K1000LA | LA264  | 4       | 1                |
| 2K3000   | LA364E | 4       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
