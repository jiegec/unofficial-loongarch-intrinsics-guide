# \_\_lasx_xvmuh_bu

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvmuh_bu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvmuh.bu xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply corresponding unsigned 8-bit elements in `a` and `b`, save the upper half of the double-width product in `dst`.

### Operation

```
for (int i = 0; i < 32; i++) {
  dst.byte[i] = (((u16)(u8)a.byte[i] * (u16)(u8)b.byte[i])) >> 8;
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 4       | 2                |
| 3A6000 | LA664 | 4       | 2                |
| 3C6000 | LA664 | 4       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
