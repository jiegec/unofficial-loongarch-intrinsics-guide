# \_\_lasx_xvaddwev_h_bu

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvaddwev_h_bu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvaddwev.h.bu xr, xr, xr
CPU Flags: LASX
```

### Description

Add the even-positioned unsigned 8-bit elements in `a` and the corresponding unsigned 8-bit elements in `b`, extending both to 16-bit, and save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = (u16)(u8)a.byte[2 * i] + (u16)(u8)b.byte[2 * i];
}

// Expands to:

if (0) {
  dst.half[0] = ((u16)((u8)a.byte[0])) + ((u16)((u8)b.byte[0]));
  dst.half[1] = ((u16)((u8)a.byte[2])) + ((u16)((u8)b.byte[2]));
  dst.half[2] = ((u16)((u8)a.byte[4])) + ((u16)((u8)b.byte[4]));
  dst.half[3] = ((u16)((u8)a.byte[6])) + ((u16)((u8)b.byte[6]));
  dst.half[4] = ((u16)((u8)a.byte[8])) + ((u16)((u8)b.byte[8]));
  dst.half[5] = ((u16)((u8)a.byte[10])) + ((u16)((u8)b.byte[10]));
  dst.half[6] = ((u16)((u8)a.byte[12])) + ((u16)((u8)b.byte[12]));
  dst.half[7] = ((u16)((u8)a.byte[14])) + ((u16)((u8)b.byte[14]));
  dst.half[8] = ((u16)((u8)a.byte[16])) + ((u16)((u8)b.byte[16]));
  dst.half[9] = ((u16)((u8)a.byte[18])) + ((u16)((u8)b.byte[18]));
  dst.half[10] = ((u16)((u8)a.byte[20])) + ((u16)((u8)b.byte[20]));
  dst.half[11] = ((u16)((u8)a.byte[22])) + ((u16)((u8)b.byte[22]));
  dst.half[12] = ((u16)((u8)a.byte[24])) + ((u16)((u8)b.byte[24]));
  dst.half[13] = ((u16)((u8)a.byte[26])) + ((u16)((u8)b.byte[26]));
  dst.half[14] = ((u16)((u8)a.byte[28])) + ((u16)((u8)b.byte[28]));
  dst.half[15] = ((u16)((u8)a.byte[30])) + ((u16)((u8)b.byte[30]));
}
```

Tested on real machine.

### Latency and Throughput

| CPU    | µarch | Latency | Throughput (IPC) |
| ------ | ----- | ------- | ---------------- |
| 3C5000 | LA464 | 2       | 2                |
| 3A6000 | LA664 | 2       | 2                |
| 3C6000 | LA664 | 2       | 2                |

______________________________________________________________________

[All intrinsics in alphabetical order](https://jia.je/unofficial-loongarch-intrinsics-guide/intrinsic/index.md)
