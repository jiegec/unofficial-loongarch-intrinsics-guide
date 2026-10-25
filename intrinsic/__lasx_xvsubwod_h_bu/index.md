# \_\_lasx_xvsubwod_h_bu

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvsubwod_h_bu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvsubwod.h.bu xr, xr, xr
CPU Flags: LASX
```

### Description

Subtract the odd-positioned unsigned 8-bit elements in `b` from the corresponding elements in `a`, extending both to 16-bit, and save the 16-bit result in `dst`.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = (u16)(u8)a.byte[2 * i + 1] - (u16)(u8)b.byte[2 * i + 1];
}

// Expands to:

if (0) {
  dst.half[0] = ((u16)((u8)a.byte[1])) - ((u16)((u8)b.byte[1]));
  dst.half[1] = ((u16)((u8)a.byte[3])) - ((u16)((u8)b.byte[3]));
  dst.half[2] = ((u16)((u8)a.byte[5])) - ((u16)((u8)b.byte[5]));
  dst.half[3] = ((u16)((u8)a.byte[7])) - ((u16)((u8)b.byte[7]));
  dst.half[4] = ((u16)((u8)a.byte[9])) - ((u16)((u8)b.byte[9]));
  dst.half[5] = ((u16)((u8)a.byte[11])) - ((u16)((u8)b.byte[11]));
  dst.half[6] = ((u16)((u8)a.byte[13])) - ((u16)((u8)b.byte[13]));
  dst.half[7] = ((u16)((u8)a.byte[15])) - ((u16)((u8)b.byte[15]));
  dst.half[8] = ((u16)((u8)a.byte[17])) - ((u16)((u8)b.byte[17]));
  dst.half[9] = ((u16)((u8)a.byte[19])) - ((u16)((u8)b.byte[19]));
  dst.half[10] = ((u16)((u8)a.byte[21])) - ((u16)((u8)b.byte[21]));
  dst.half[11] = ((u16)((u8)a.byte[23])) - ((u16)((u8)b.byte[23]));
  dst.half[12] = ((u16)((u8)a.byte[25])) - ((u16)((u8)b.byte[25]));
  dst.half[13] = ((u16)((u8)a.byte[27])) - ((u16)((u8)b.byte[27]));
  dst.half[14] = ((u16)((u8)a.byte[29])) - ((u16)((u8)b.byte[29]));
  dst.half[15] = ((u16)((u8)a.byte[31])) - ((u16)((u8)b.byte[31]));
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
