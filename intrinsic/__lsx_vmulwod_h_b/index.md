# \_\_lsx_vmulwod_h_b

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vmulwod_h_b (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmulwod.h.b vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply the odd-positioned signed 8-bit elements in `a` and the corresponding signed 8-bit elements in `b`, extending both to 16-bit, and save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (s16)(s8)a.byte[2 * i + 1] * (s16)(s8)b.byte[2 * i + 1];
}

// Expands to:

if (0) {
  dst.half[0] = ((s16)((s8)a.byte[1])) * ((s16)((s8)b.byte[1]));
  dst.half[1] = ((s16)((s8)a.byte[3])) * ((s16)((s8)b.byte[3]));
  dst.half[2] = ((s16)((s8)a.byte[5])) * ((s16)((s8)b.byte[5]));
  dst.half[3] = ((s16)((s8)a.byte[7])) * ((s16)((s8)b.byte[7]));
  dst.half[4] = ((s16)((s8)a.byte[9])) * ((s16)((s8)b.byte[9]));
  dst.half[5] = ((s16)((s8)a.byte[11])) * ((s16)((s8)b.byte[11]));
  dst.half[6] = ((s16)((s8)a.byte[13])) * ((s16)((s8)b.byte[13]));
  dst.half[7] = ((s16)((s8)a.byte[15])) * ((s16)((s8)b.byte[15]));
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
