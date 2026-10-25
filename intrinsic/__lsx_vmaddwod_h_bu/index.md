# \_\_lsx_vmaddwod_h_bu

LSX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lsx/integer_computation/index.md)

### Synopsis

```
__m128i __lsx_vmaddwod_h_bu (__m128i a, __m128i b, __m128i c)
#include <lsxintrin.h>
Instruction: vmaddwod.h.bu vr, vr, vr
CPU Flags: LSX
```

### Description

Multiply the odd-positioned unsigned 8-bit elements in `b` and the corresponding unsigned 8-bit elements in `c`, extending both to 16-bit, and add the product to the 16-bit elements in `a`.

### Operation

```
for (int i = 0; i < 8; i++) {
  dst.half[i] =
      (u16)(u8)b.byte[2 * i + 1] * (u16)(u8)c.byte[2 * i + 1] + (u16)a.half[i];
}

// Expands to:

if (0) {
  dst.half[0] =
      (((u16)((u8)b.byte[1])) * ((u16)((u8)c.byte[1]))) + ((u16)a.half[0]);
  dst.half[1] =
      (((u16)((u8)b.byte[3])) * ((u16)((u8)c.byte[3]))) + ((u16)a.half[1]);
  dst.half[2] =
      (((u16)((u8)b.byte[5])) * ((u16)((u8)c.byte[5]))) + ((u16)a.half[2]);
  dst.half[3] =
      (((u16)((u8)b.byte[7])) * ((u16)((u8)c.byte[7]))) + ((u16)a.half[3]);
  dst.half[4] =
      (((u16)((u8)b.byte[9])) * ((u16)((u8)c.byte[9]))) + ((u16)a.half[4]);
  dst.half[5] =
      (((u16)((u8)b.byte[11])) * ((u16)((u8)c.byte[11]))) + ((u16)a.half[5]);
  dst.half[6] =
      (((u16)((u8)b.byte[13])) * ((u16)((u8)c.byte[13]))) + ((u16)a.half[6]);
  dst.half[7] =
      (((u16)((u8)b.byte[15])) * ((u16)((u8)c.byte[15]))) + ((u16)a.half[7]);
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
