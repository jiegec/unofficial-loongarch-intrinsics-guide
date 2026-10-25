# \_\_lsx_vabsd_bu

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vabsd_bu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vabsd.bu vr, vr, vr
CPU Flags: LSX
```

### Description

Compute the absolute difference between corresponding unsigned 8-bit elements in `a` and `b`, save the result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.byte[i] = ((u8)a.byte[i] > (u8)b.byte[i]) ? (a.byte[i] - b.byte[i])
                                                : (b.byte[i] - a.byte[i]);
}
```

Tested on real machine.

### Latency and Throughput

| CPU      | µarch  | Latency | Throughput (IPC) |
| -------- | ------ | ------- | ---------------- |
| 3C5000   | LA464  | 2       | 2                |
| 3A6000   | LA664  | 2       | 2                |
| 3C6000   | LA664  | 2       | 2                |
| 2K1000LA | LA264  | 2       | 1                |
| 2K3000   | LA364E | 2       | 1                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
