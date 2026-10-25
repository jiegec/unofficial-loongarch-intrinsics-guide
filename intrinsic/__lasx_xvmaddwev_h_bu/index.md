# \_\_lasx_xvmaddwev_h_bu

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvmaddwev_h_bu (__m256i a, __m256i b, __m256i c)
#include <lasxintrin.h>
Instruction: xvmaddwev.h.bu xr, xr, xr
CPU Flags: LASX
```

### Description

Multiply the even-positioned unsigned 8-bit elements in `b` and the corresponding unsigned 8-bit elements in `c`, extending both to 16-bit, and add the product to the 16-bit elements in `a`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] =
      (u16)(u8)b.byte[2 * i] * (u16)(u8)c.byte[2 * i] + (u16)a.half[i];
}

// Expands to:

if (0) {
  dst.half[0] =
      (((u16)((u8)b.byte[0])) * ((u16)((u8)c.byte[0]))) + ((u16)a.half[0]);
  dst.half[1] =
      (((u16)((u8)b.byte[2])) * ((u16)((u8)c.byte[2]))) + ((u16)a.half[1]);
  dst.half[2] =
      (((u16)((u8)b.byte[4])) * ((u16)((u8)c.byte[4]))) + ((u16)a.half[2]);
  dst.half[3] =
      (((u16)((u8)b.byte[6])) * ((u16)((u8)c.byte[6]))) + ((u16)a.half[3]);
  dst.half[4] =
      (((u16)((u8)b.byte[8])) * ((u16)((u8)c.byte[8]))) + ((u16)a.half[4]);
  dst.half[5] =
      (((u16)((u8)b.byte[10])) * ((u16)((u8)c.byte[10]))) + ((u16)a.half[5]);
  dst.half[6] =
      (((u16)((u8)b.byte[12])) * ((u16)((u8)c.byte[12]))) + ((u16)a.half[6]);
  dst.half[7] =
      (((u16)((u8)b.byte[14])) * ((u16)((u8)c.byte[14]))) + ((u16)a.half[7]);
  dst.half[8] =
      (((u16)((u8)b.byte[16])) * ((u16)((u8)c.byte[16]))) + ((u16)a.half[8]);
  dst.half[9] =
      (((u16)((u8)b.byte[18])) * ((u16)((u8)c.byte[18]))) + ((u16)a.half[9]);
  dst.half[10] =
      (((u16)((u8)b.byte[20])) * ((u16)((u8)c.byte[20]))) + ((u16)a.half[10]);
  dst.half[11] =
      (((u16)((u8)b.byte[22])) * ((u16)((u8)c.byte[22]))) + ((u16)a.half[11]);
  dst.half[12] =
      (((u16)((u8)b.byte[24])) * ((u16)((u8)c.byte[24]))) + ((u16)a.half[12]);
  dst.half[13] =
      (((u16)((u8)b.byte[26])) * ((u16)((u8)c.byte[26]))) + ((u16)a.half[13]);
  dst.half[14] =
      (((u16)((u8)b.byte[28])) * ((u16)((u8)c.byte[28]))) + ((u16)a.half[14]);
  dst.half[15] =
      (((u16)((u8)b.byte[30])) * ((u16)((u8)c.byte[30]))) + ((u16)a.half[15]);
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
