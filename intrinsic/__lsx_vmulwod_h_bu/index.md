# \_\_lsx_vmulwod_h_bu

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vmulwod_h_bu (__m128i a, __m128i b)
#include <lsxintrin.h>
Instruction: vmulwod.h.bu vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply the odd-positioned unsigned 8-bit elements in `a` and the corresponding unsigned 8-bit elements in `b`, extending both to 16-bit, and save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] = (u16)(u8)a.byte[2 * i + 1] * (u16)(u8)b.byte[2 * i + 1];
}

// Expands to:

if (0) {
  dst.half[0] = ((u16)((u8)a.byte[1])) * ((u16)((u8)b.byte[1]));
  dst.half[1] = ((u16)((u8)a.byte[3])) * ((u16)((u8)b.byte[3]));
  dst.half[2] = ((u16)((u8)a.byte[5])) * ((u16)((u8)b.byte[5]));
  dst.half[3] = ((u16)((u8)a.byte[7])) * ((u16)((u8)b.byte[7]));
  dst.half[4] = ((u16)((u8)a.byte[9])) * ((u16)((u8)b.byte[9]));
  dst.half[5] = ((u16)((u8)a.byte[11])) * ((u16)((u8)b.byte[11]));
  dst.half[6] = ((u16)((u8)a.byte[13])) * ((u16)((u8)b.byte[13]));
  dst.half[7] = ((u16)((u8)a.byte[15])) * ((u16)((u8)b.byte[15]));
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
