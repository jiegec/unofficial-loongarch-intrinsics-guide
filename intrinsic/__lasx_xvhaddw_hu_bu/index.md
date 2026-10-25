# \_\_lasx_xvhaddw_hu_bu

LASX intrinsic · group: [Integer Computation](https://jia.je/unofficial-loongarch-intrinsics-guide/lasx/integer_computation/index.md)

### Synopsis

```
__m256i __lasx_xvhaddw_hu_bu (__m256i a, __m256i b)
#include <lasxintrin.h>
Instruction: xvhaddw.hu.bu xr, xr, xr
CPU Flags: LASX
```

### Description

Add odd-positioned unsigned 8-bit elements in `a` to even-positioned unsigned 8-bit elements in `b`, zero-extending both to 16-bit, to get the 16-bit result.

### Operation

```
for (int i = 0; i < 16; i++) {
  dst.half[i] = (u16)(u8)a.byte[2 * i + 1] + (u16)(u8)b.byte[2 * i];
}

// Expands to:

if (0) {
  dst.half[0] = ((u16)((u8)a.byte[1])) + ((u16)((u8)b.byte[0]));
  dst.half[1] = ((u16)((u8)a.byte[3])) + ((u16)((u8)b.byte[2]));
  dst.half[2] = ((u16)((u8)a.byte[5])) + ((u16)((u8)b.byte[4]));
  dst.half[3] = ((u16)((u8)a.byte[7])) + ((u16)((u8)b.byte[6]));
  dst.half[4] = ((u16)((u8)a.byte[9])) + ((u16)((u8)b.byte[8]));
  dst.half[5] = ((u16)((u8)a.byte[11])) + ((u16)((u8)b.byte[10]));
  dst.half[6] = ((u16)((u8)a.byte[13])) + ((u16)((u8)b.byte[12]));
  dst.half[7] = ((u16)((u8)a.byte[15])) + ((u16)((u8)b.byte[14]));
  dst.half[8] = ((u16)((u8)a.byte[17])) + ((u16)((u8)b.byte[16]));
  dst.half[9] = ((u16)((u8)a.byte[19])) + ((u16)((u8)b.byte[18]));
  dst.half[10] = ((u16)((u8)a.byte[21])) + ((u16)((u8)b.byte[20]));
  dst.half[11] = ((u16)((u8)a.byte[23])) + ((u16)((u8)b.byte[22]));
  dst.half[12] = ((u16)((u8)a.byte[25])) + ((u16)((u8)b.byte[24]));
  dst.half[13] = ((u16)((u8)a.byte[27])) + ((u16)((u8)b.byte[26]));
  dst.half[14] = ((u16)((u8)a.byte[29])) + ((u16)((u8)b.byte[28]));
  dst.half[15] = ((u16)((u8)a.byte[31])) + ((u16)((u8)b.byte[30]));
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
